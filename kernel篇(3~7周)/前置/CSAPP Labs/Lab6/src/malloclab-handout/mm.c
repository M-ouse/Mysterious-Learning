/* 
* mm.c 
* 
* Name: LegendaryPaladin 
* 姓名：轩辕奇侠 
* 
* Brief introduction: 
*       This solution used an algorithm with segregated free list 
*   and a binary search tree combined to maximize both space utilization 
*   and throughput. Please note that each free list is a unique size class, 
*   which means it only contains blocks with EXACTLY THE SAME SIZE. 
*  
* Details of binary search tree: 
*       The BST is used to store free blocks sized larger than a specific 
*   threshold (I set it to 40 bytes), of which each node is a header of a 
*   free list of a same size, When searching for a free block in BST, 
*   I applied the best-fit policy which selects the free block with 
*   approximately minimum size among all the blocks sized larger than  
*   requested. 
* 
* Details of segregated free list: 
*       I set up a small array to store the headers of segregated free lists. 
*   Each free list, as described above, has blocks with a same size. Blocks 
*   which is not large enough to be stored in BST will be stored here. 
* 
* Block layout: 
*       Allocated block: 
* 
*           [Header: <size><prev_alloc><1>] 
*           [Payload...] 
* 
*       Large free block: 
* 
*           [Header: <size><prev_alloc><0>] 
*           [4-byte pointer to next block with same size] 
*           [4-byte pointer to previous block with same size] 
*           [Pointer to its left child in BST] 
*           [Pointer to its right child in BST] 
*           [Pointer to the pointer to itself in its parent block in BST] 
*           [...] 
*           [Footer: <size><0>] 
* 
*       Small free block: 
* 
*           [Header: <size><prev_alloc><0>] 
*           [4-byte pointer to next block with same size] 
*           [4-byte pointer to previous block with same size] 
*           [...] 
*           [Footer: <size><0>] 
* 
*           Note that I omitted the footer of allocated block, instead, I 
*       stored its allocation info in the header of next block. 
* 
* 
* 概述: 
*       本次作业，我通过分离空闲链表 + 二叉搜索树来实现空间利用率和效率的 
*   最大化。注意我的每个链表都只存储一种大小的空闲块。 
* 
* 二叉搜索树: 
*       二叉搜索树用于存大于某个特定阈值的块（我设为40字节），每个节点是 
*   具有该大小的空闲块链表的表头。在树中查找结点的时候，我会使用“最优适配” 
*   策略，选择树中大于请求大小的块中近似最小的那个。 
* 
* 分离空闲链表: 
*       我给分离空闲链表的表头开了个小数组，每个空闲链表同样也只存一种大小的块。 
*   大小不够放到树里的块会放到这里。 
* 
* 块布局: 
*       已分配块: 
* 
*           [头部: <大小><上一块分配状况><1>] 
*           [有效载荷...] 
* 
*       大的空闲块: 
* 
*           [头部: <大小><上一块分配状况><0>] 
*           [指向下一个相同大小块的4字节指针] 
*           [指向上一个相同大小块的4字节指针] 
*           [指向左儿子的指针] 
*           [指向右儿子的指针] 
*           [指向它父亲指向它的指针的指针……] 
*           [...] 
*           [脚部: <大小><0>] 
* 
*       小的空闲块: 
* 
*           [头部: <大小><上一块分配状况><0>] 
*           [指向下一个相同大小块的4字节指针] 
*           [指向上一个相同大小块的4字节指针] 
*           [...] 
*           [脚部: <大小><0>] 
* 
*           注意我把已分配块的脚部省略了，把它的分配状态信息 
*       存到了下一个块的头部。 
* 
*/  
#include <assert.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
  
/* Other helper headers */  
#include <linux/kernel.h>  
#include <linux/stddef.h>  
  
#include "mm.h"  
#include "memlib.h"  
  
/* do not change the following! */  
#ifdef DRIVER  
/* create aliases for driver tests */  
#define malloc mm_malloc  
#define free mm_free  
#define realloc mm_realloc  
#define calloc mm_calloc  
#endif /* def DRIVER */  
  
typedef void *block_ptr;  
  
/* Global var and data structure pointers */  
static char *heap_listp = 0;        /* Pointer to first block */  
static block_ptr larger_bin_root;   /* Root of the BST which contains larger blocks */  
static block_ptr *bins;             /* Array of the headers of segregated free lists */  
static const unsigned int fixed_bin_count = 5;  /* Number of segregated free lists */  
#ifdef DEBUG  
static int operid;  
#endif  
  
/* Function prototypes */  
static block_ptr coalesce(block_ptr bp);  
static block_ptr extend_heap(size_t words);  
static void place(block_ptr bp, size_t asize);  
static void insert_free_block(block_ptr bp, size_t blocksize);  
static void printblock(block_ptr bp);  
static void checkblock(block_ptr bp);  
static block_ptr find_fit(size_t asize);  
#ifdef DEBUG  
static void printtree(block_ptr node, int depth);  
#endif  
  
/* Macros and utility inline functions */  
  
/* Single word (4) or double word (8) alignment */  
#define ALIGNMENT   8  
  
/* Round size up to ALIGNMENT */  
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)  
  
/* Basic constants */  
#define WSIZE       4           /* Word and header/footer size (bytes) */  
#define DSIZE       8           /* Doubleword size (bytes) */  
#define BLOCKSIZE   (1 << 6)  /* Extend heap by this amount (bytes) */  
  
#define MAX(x, y) ((x) > (y)? (x) : (y))    
  
/* Pack a size and allocated bit into a word */  
#define PACK(size, alloc)   ((size) | (alloc))  
  
/* Read and write a word at address p */  
#define GET(p)              (*(unsigned int *)(p))  
#define PUTTRUNC(p, val)    (GET(p) = (val))  
  
/* Write header info at address p without modifying prev_alloc */  
#define PUT(p, val)         (GET(p) = (GET(p) & 0x2) | (val))  
  
/* Read the size and allocated fields from address p */  
#define GET_SIZE(p)         (GET(p) & ~0x7)  
#define GET_ALLOC(p)        (GET(p) & 0x1)  
  
/* Given block ptr bp, compute address of its header and footer */  
#define HDRP(bp)            ((char *)(bp) - WSIZE)  
#define FTRP(bp)            ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)  
  
/* Given block ptr bp, compute address of next and previous blocks */  
#define NEXT_BLKP(bp)       ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))  
#define PREV_BLKP(bp)       ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))  
  
/* Given block ptr bp, set next block's prev_alloc */  
#define SET_ALLOC(bp)       (GET(HDRP(NEXT_BLKP(bp))) |= 0x2)  
#define SET_UNALLOC(bp)     (GET(HDRP(NEXT_BLKP(bp))) &= ~0x2)  
  
/* Given block ptr bp, get prev_alloc */  
#define GET_PREV_ALLOC(bp)  (GET(HDRP(bp)) & 0x2)  
  
/* Given a freed block ptr bp, compute 'address' of next and previous blocks of same size */  
#define NEXT_SAMESZ_BLKP(bp)    (*(unsigned int *)(bp))  
#define PREV_SAMESZ_BLKP(bp)    (*(unsigned int *)((char *)(bp) + WSIZE))  
  
#define WNULL 0U  
  
/* Convert 4-byte address to 8-byte address */  
static inline block_ptr word_to_ptr(unsigned int w)  
{  
    return ((w) == WNULL ? NULL : (block_ptr)((unsigned int)(w) + 0x800000000UL));  
}  
  
/* Convert 8-byte address to 4-byte address */  
static inline unsigned int ptr_to_word(block_ptr p)  
{  
    return ((p) == NULL ? WNULL : (unsigned int)((unsigned long)(p) - 0x800000000UL));  
}  
  
/* The following are macros for BST node blocks */  
  
/* Check if this block is large enough to be a BST node */  
#define IS_OVER_BST_SIZE(size)  ((size) > DSIZE * fixed_bin_count)  
#define IS_BST_NODE(bp)         (IS_OVER_BST_SIZE(GET_SIZE(HDRP(bp))))  
  
/* Given BST block ptr bp, compute address of its left child or right child */  
#define LCHLD_BLKPREF(bp)       ((block_ptr *)((char *)(bp) + DSIZE))  
#define RCHLD_BLKPREF(bp)       ((block_ptr *)((char *)(bp) + DSIZE * 2))  
  
/* Crap-like triple pointer > < */  
#define PARENT_CHLDSLOTPREF(bp) ((block_ptr **)((char *)(bp) + DSIZE * 3))  
  
#define LCHLD_BLKP(bp) (*LCHLD_BLKPREF(bp))  
#define RCHLD_BLKP(bp) (*RCHLD_BLKPREF(bp))  
#define PARENT_CHLDSLOTP(bp) (*PARENT_CHLDSLOTPREF(bp))  
  
/* Reset the fields of a free block bp */  
#define reset_block(bp)                                                     \  
{                                                                           \  
    NEXT_SAMESZ_BLKP(bp) = WNULL;                                           \  
    PREV_SAMESZ_BLKP(bp) = WNULL;                                           \  
    if (IS_BST_NODE(bp))                                                    \  
    {                                                                       \  
        LCHLD_BLKP(bp) = NULL;                                              \  
        RCHLD_BLKP(bp) = NULL;                                              \  
        PARENT_CHLDSLOTP(bp) = NULL;                                        \  
    }                                                                       \  
}  
  
/* Remove bp from its free list */  
#define remove_linked_freed_block(bp)                                               \  
{                                                                                   \  
    if (PREV_SAMESZ_BLKP(bp))                                                       \  
        NEXT_SAMESZ_BLKP(word_to_ptr(PREV_SAMESZ_BLKP(bp))) = NEXT_SAMESZ_BLKP(bp); \  
    if (NEXT_SAMESZ_BLKP(bp))                                                       \  
        PREV_SAMESZ_BLKP(word_to_ptr(NEXT_SAMESZ_BLKP(bp))) = PREV_SAMESZ_BLKP(bp); \  
}  
  
static inline void remove_freed_block(block_ptr bp)  
{  
    if (IS_BST_NODE(bp) && PARENT_CHLDSLOTP(bp))  
    {  
        /* I hate deleting node. */  
        block_ptr l = LCHLD_BLKP(bp), r = RCHLD_BLKP(bp), cur;  
        if ((cur = word_to_ptr(NEXT_SAMESZ_BLKP(bp))))  
        {  
            PARENT_CHLDSLOTP(cur) = PARENT_CHLDSLOTP(bp);  
            *PARENT_CHLDSLOTP(bp) = cur;  
            LCHLD_BLKP(cur) = l;  
            if (l)  
                PARENT_CHLDSLOTP(l) = LCHLD_BLKPREF(cur);  
            RCHLD_BLKP(cur) = r;  
            if (r)  
                PARENT_CHLDSLOTP(r) = RCHLD_BLKPREF(cur);  
        }  
        else if (l && r)  
        {  
            /* Find left-most node in right branch to replace curr */  
            if (!(cur = LCHLD_BLKP(r)))  
            {  
                /* Right child doesn't have lchild */  
                LCHLD_BLKP(r) = l;  
                PARENT_CHLDSLOTP(r) = PARENT_CHLDSLOTP(bp);  
                *PARENT_CHLDSLOTP(bp) = r;  
                PARENT_CHLDSLOTP(l) = LCHLD_BLKPREF(r);  
            }  
            else  
            {  
                while (LCHLD_BLKP(cur))  
                    cur = LCHLD_BLKP(cur);  
                *PARENT_CHLDSLOTP(bp) = cur;  
                *PARENT_CHLDSLOTP(cur) = RCHLD_BLKP(cur);  
                if (RCHLD_BLKP(cur))  
                    PARENT_CHLDSLOTP(RCHLD_BLKP(cur)) = PARENT_CHLDSLOTP(cur);  
                PARENT_CHLDSLOTP(cur) = PARENT_CHLDSLOTP(bp);  
                LCHLD_BLKP(cur) = l;  
                RCHLD_BLKP(cur) = r;  
                PARENT_CHLDSLOTP(l) = LCHLD_BLKPREF(cur);  
                PARENT_CHLDSLOTP(r) = RCHLD_BLKPREF(cur);  
            }  
        }  
        else if (r)  
        {  
            *PARENT_CHLDSLOTP(bp) = r;  
            PARENT_CHLDSLOTP(r) = PARENT_CHLDSLOTP(bp);  
        }  
        else if (l)  
        {  
            *PARENT_CHLDSLOTP(bp) = l;  
            PARENT_CHLDSLOTP(l) = PARENT_CHLDSLOTP(bp);  
        }  
        else  
            *PARENT_CHLDSLOTP(bp) = NULL;  
    }  
    else if (!PREV_SAMESZ_BLKP(bp))  
        /* Remove a free list header from the array of headers */  
        bins[GET_SIZE(HDRP(bp)) / DSIZE - 1] = word_to_ptr(NEXT_SAMESZ_BLKP(bp));  
    remove_linked_freed_block(bp);  
}  
  
/* Function definitions */  
  
/* 
 * bestfit_search - search for a block with requested size or larger in BST. 
 */  
block_ptr *bestfit_search(block_ptr *node, size_t size, int specific)  
{  
    block_ptr *candidate, curr = *node;  
    size_t curr_size;  
  
    if (curr == NULL)  
        return NULL;  
  
    curr_size = GET_SIZE(HDRP(curr));  
  
    if (size < curr_size)  
    {  
        if (specific)  
            return bestfit_search(LCHLD_BLKPREF(curr), size, specific);  
        if ((candidate = bestfit_search(LCHLD_BLKPREF(curr), size, specific)))  
            return candidate;  
        return node;  
    }  
    else if (size > curr_size)  
        return bestfit_search(RCHLD_BLKPREF(curr), size, specific);  
    else  
        return node;  
}  
  
/* 
 * Return whether the pointer is in the heap. 
 * May be useful for debugging. 
 */  
static inline int in_heap(const block_ptr p)  
{  
    return p <= mem_heap_hi() && p >= mem_heap_lo();  
}  
  
/* 
 * Return whether the pointer is aligned. 
 * May be useful for debugging. 
 */  
static inline int aligned(const block_ptr p)  
{  
    return ((unsigned long)p % ALIGNMENT) == 0;  
}  
  
/* 
 * Initialize: return -1 on error, 0 on success. 
 */  
int mm_init(void)  
{  
    /* Create the initial empty heap */  
    if ((bins = mem_sbrk(  
        ALIGN(fixed_bin_count * sizeof(block_ptr)) +  
        4 * WSIZE)) == (block_ptr)-1)  
        return -1;  
    memset(bins, 0, fixed_bin_count * sizeof(block_ptr));  
    heap_listp = (char *)ALIGN((unsigned long)(bins + fixed_bin_count));  
    larger_bin_root = NULL;  
    PUTTRUNC(heap_listp, 0);                            /* Alignment padding */  
    PUTTRUNC(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); /* Prologue header */  
    PUTTRUNC(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); /* Prologue footer */  
    PUTTRUNC(heap_listp + (3 * WSIZE), PACK(0, 1));     /* Epilogue header */  
    heap_listp += (2 * WSIZE);  
    SET_ALLOC(heap_listp);  
#ifdef DEBUG  
    {  
        printblock(heap_listp);  
        checkblock(heap_listp);  
    }  
    operid = 0;  
#endif  
    return 0;  
}  
  
/* 
 * malloc 
 */  
block_ptr malloc(size_t size)  
{  
    size_t asize;      /* Adjusted block size */  
    size_t extendsize; /* Amount to extend heap if no fit */  
    char *bp;  
  
    if (heap_listp == 0)  
    {  
        mm_init();  
    }  
  
    /* Ignore spurious requests */  
    if (size == 0)  
        return NULL;  
  
  
    /* Adjust block size to include overhead and alignment reqs. */  
    if (size <= DSIZE)  
        asize = 2 * DSIZE;  
    else  
        asize = DSIZE * ((size + (WSIZE) + (DSIZE - 1)) / DSIZE);  
  
#ifdef DEBUG  
    printf("\nMalloc request: size = %zu, rounded to %zu \033[41;37m[ID:%d]\033[0m\n", size, asize, operid++);  
#endif  
  
    /* Search the free list for a fit */  
    if ((bp = find_fit(asize)) != NULL)  
    {  
#ifdef DEBUG  
        {  
            puts("Found fit!");  
            checkblock(bp);  
            printblock(bp);  
        }  
#endif  
        place(bp, asize);  
        return bp;  
    }  
  
    /* No fit found. Get more memory and place the block */  
    extendsize = MAX(asize, BLOCKSIZE);  
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)  
        return NULL;  
    place(bp, asize);  
    return bp;  
}  
  
/* 
 * free 
 */  
void free(block_ptr ptr)  
{  
    block_ptr tmp;  
    size_t size;  
    if (!ptr || !in_heap(ptr) || !aligned(ptr))  
        return;  
  
#ifdef DEBUG  
    {  
        printf("\nFree request: ptr = %p \033[41;37m[ID:%d]\033[0m\n", ptr, operid++);  
        printblock(ptr);  
    }  
#endif  
  
    size = GET_SIZE(HDRP(ptr));  
    PUT(HDRP(ptr), PACK(size, 0));  
    PUT(FTRP(ptr), PACK(size, 0));  
    SET_UNALLOC(ptr);  
    reset_block(ptr);  
    tmp = coalesce(ptr);  
    insert_free_block(tmp, GET_SIZE(HDRP(tmp)));  
}  
  
/* 
 * realloc - I don't want to look at mm-naive.c 
 */  
block_ptr realloc(block_ptr oldptr, size_t size)  
{  
    size_t oldsize;  
    block_ptr newptr;  
  
    /* If size == 0 then this is just free, and we return NULL. */  
    if (size == 0)  
    {  
        free(oldptr);  
        return 0;  
    }  
  
    /* If oldptr is NULL, then this is just malloc. */  
    if (oldptr == NULL)  
    {  
        return malloc(size);  
    }  
  
    newptr = malloc(size);  
  
    /* If realloc() fails the original block is left untouched  */  
    if (!newptr)  
    {  
        return 0;  
    }  
  
    /* Copy the old data. */  
    oldsize = GET_SIZE(HDRP(oldptr));  
    if (size < oldsize)  
        oldsize = size;  
    memcpy(newptr, oldptr, oldsize);  
  
    /* Free the old block. */  
    free(oldptr);  
  
    return newptr;  
}  
  
/* 
 * calloc - I don't want to look at mm-naive.c 
 * This function is not tested by mdriver, but it is 
 * needed to run the traces. 
 */  
block_ptr calloc(size_t nmemb, size_t size)  
{  
    size_t bytes = nmemb * size;  
    block_ptr newptr;  
  
    newptr = malloc(bytes);  
    memset(newptr, 0, bytes);  
  
    return newptr;  
}  
  
/* 
 * coalesce - Boundary tag coalescing. Return ptr to coalesced block 
 */  
static block_ptr coalesce(block_ptr bp)  
{  
    /*  
     * TODO Here is the bug: Do update the bins while doing this. 
     * Tried to fix. Not sure what will happen. 
     */  
    block_ptr prev, next = NEXT_BLKP(bp);  
  
    /* Use GET_PREV_ALLOC to judge if prev block is allocated */  
    size_t prev_alloc = GET_PREV_ALLOC(bp);  
    size_t next_alloc = GET_ALLOC(HDRP(next));  
    size_t size = GET_SIZE(HDRP(bp));  
  
    if (prev_alloc && next_alloc)            /* Case 1 */  
    {  
        return bp;  
    }  
  
    else if (prev_alloc && !next_alloc)      /* Case 2 */  
    {  
        remove_freed_block(next);  
        size += GET_SIZE(HDRP(next));  
        PUT(HDRP(bp), PACK(size, 0));  
        PUT(FTRP(bp), PACK(size, 0));  
    }  
  
    else if (!prev_alloc && next_alloc)       /* Case 3 */  
    {  
        prev = PREV_BLKP(bp);  
        remove_freed_block(prev);  
        size += GET_SIZE(HDRP(prev));  
        PUT(FTRP(bp), PACK(size, 0));  
        PUT(HDRP(prev), PACK(size, 0));  
        bp = prev;  
    }  
  
    else                                      /* Case 4 */  
    {  
        prev = PREV_BLKP(bp);  
        remove_freed_block(next);  
        remove_freed_block(prev);  
        size += GET_SIZE(HDRP(prev)) + GET_SIZE(FTRP(next));  
        PUT(HDRP(prev), PACK(size, 0));  
        PUT(FTRP(next), PACK(size, 0));  
        bp = prev;  
    }  
    reset_block(bp);  
    // insert_free_block(bp, size);  
    return bp;  
}  
  
/* 
 * extend_heap - Extend heap with free block and return its block pointer 
 */  
static block_ptr extend_heap(size_t words)  
{  
    char *bp;  
    size_t size;  
  
    /* Allocate an even number of words to maintain alignment */  
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;  
    if ((long)(bp = mem_sbrk(size)) == -1)  
        return NULL;  
  
#ifdef DEBUG  
    printf("\nExtended the heap by %zu words.\n", words);  
#endif  
  
    /* Initialize free block header/footer and the epilogue header */  
    PUT(HDRP(bp), PACK(size, 0));           /* Free block header */  
    PUT(FTRP(bp), PACK(size, 0));           /* Free block footer */  
    reset_block(bp);  
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));   /* New epilogue header */  
  
    /* Coalesce if the previous block was free */  
    return coalesce(bp);  
}  
  
/* 
 * place - Place block of asize bytes at start of free block bp 
 *       and split if remainder would be at least minimum block size 
 */  
static void place(block_ptr bp, size_t asize)  
{  
    size_t csize = GET_SIZE(HDRP(bp)), delta = csize - asize;  
  
    if (delta >= (2 * DSIZE))  
    {  
        PUT(HDRP(bp), PACK(asize, 1));  
        PUT(FTRP(bp), PACK(asize, 1));  
        SET_ALLOC(bp);  
        bp = NEXT_BLKP(bp);  
        PUT(HDRP(bp), PACK(delta, 0));  
        PUT(FTRP(bp), PACK(delta, 0));  
        SET_UNALLOC(bp);  
        reset_block(bp);  
        insert_free_block(bp, delta);  
#ifdef DEBUG  
        {  
            printf("Block with size %zu remains a block:\n", asize);  
            printblock(bp);  
        }  
#endif  
    }  
    else  
    {  
        PUT(HDRP(bp), PACK(csize, 1));  
        PUT(FTRP(bp), PACK(csize, 1));  
        SET_ALLOC(bp);  
    }  
}  
  
/* 
 * insert_free_block - insert a block into BST or segregated free list 
 * BLOCKSIZE should be duplicate of double word 
 */  
static void insert_free_block(block_ptr bp, size_t blocksize)  
{  
    block_ptr *new = &larger_bin_root, parent = NULL;  
  
    if (!IS_OVER_BST_SIZE(blocksize))  
    {  
        /* Insert into segregated free list */  
        size_t dcount = blocksize / DSIZE;  
        if (bins[dcount - 1])  
        {  
            /* Connect it before the existing block as a new header */  
            NEXT_SAMESZ_BLKP(bp) = ptr_to_word(bins[dcount - 1]);  
            PREV_SAMESZ_BLKP(bins[dcount - 1]) = ptr_to_word(bp);  
        }  
        PREV_SAMESZ_BLKP(bp) = WNULL;  
        bins[dcount - 1] = bp;  
        return;  
    }  
  
    /* Figure out where to put the new node in BST */  
    while (*new)  
    {  
        size_t curr_size = GET_SIZE(HDRP(parent = *new));  
  
        if (blocksize < curr_size)  
            new = LCHLD_BLKPREF(parent);  
        else if (blocksize > curr_size)  
            new = RCHLD_BLKPREF(parent);  
        else  
        {  
            /* MWHAHAHAHAHA */  
            block_ptr next = word_to_ptr(NEXT_SAMESZ_BLKP(bp) = NEXT_SAMESZ_BLKP(parent));  
            if (next)  
                /* Connect it before the existing block as a new header */  
                PREV_SAMESZ_BLKP(next) = ptr_to_word(bp);  
            NEXT_SAMESZ_BLKP(parent) = ptr_to_word(bp);  
            PREV_SAMESZ_BLKP(bp) = ptr_to_word(parent);  
            return;  
        }  
    }  
  
    /* Connect this node as a child */  
    *new = bp;  
    PARENT_CHLDSLOTP(bp) = new;  
#ifdef DEBUG  
    {  
        printf("Inserting a block: ");  
        printblock(bp);  
    }  
#endif  
}  
  
/* 
 * find_fit - Find a fit for a block with asize bytes 
 * asize should be duplicate of double word 
 */  
static block_ptr find_fit(size_t asize)  
{  
    block_ptr curr, *blocks;  
    size_t dcount = asize / DSIZE;  
  
    if (!IS_OVER_BST_SIZE(asize))  
    {  
        if (bins[dcount - 1])  
        {  
            /* Found a free list of this size! */  
            curr = bins[dcount - 1];  
            bins[dcount - 1] = word_to_ptr(NEXT_SAMESZ_BLKP(curr));  
            remove_freed_block(curr);  
            return curr;  
        }  
        /* ...not found? Proceed to BST */  
    }  
  
    if ((blocks = bestfit_search(&larger_bin_root, asize, 0)) == NULL)  
        /* No best-fit found...T T */  
        return NULL;  
  
    curr = *blocks;  
  
    /* Found a best-fit block! LOL */  
#ifdef DEBUG  
    if ((*blocks = word_to_ptr(NEXT_SAMESZ_BLKP(curr))) == NULL)  
    {  
        printf("** All blocks with size %u (request: %zu) deleted.\n", GET_SIZE(HDRP(curr)), asize);  
    }  
#else  
    /* Set the node to the next same size block if it has */  
    *blocks = word_to_ptr(NEXT_SAMESZ_BLKP(curr));  
#endif  
    remove_freed_block(curr);  
    return curr;  
}  
  
/* 
 * printblock - print a block for debugging 
 */  
static inline void printblock(block_ptr bp)  
{  
    size_t hsize, halloc, fsize, falloc;  
  
    hsize = GET_SIZE(HDRP(bp));  
    halloc = GET_ALLOC(HDRP(bp));  
    fsize = GET_SIZE(FTRP(bp));  
    falloc = GET_ALLOC(FTRP(bp));  
  
    if (hsize == 0)  
    {  
        printf("%p: EOL\n", bp);  
        return;  
    }  
    if (halloc)  
        printf("\033[43;37m%p: header: [%zu:%c:%c] footer: -\033[0m\n", bp,  
        hsize, (GET_PREV_ALLOC(bp) ? 'a' : 'f'), (halloc ? 'a' : 'f'));  
    else  
    {  
        printf("\033[42;30m%p: header: [%zu:%c:%c] footer: [%zu:%c]\033[0m", bp,  
            hsize, (GET_PREV_ALLOC(bp) ? 'a' : 'f'), (halloc ? 'a' : 'f'),  
            fsize, (falloc ? 'a' : 'f'));  
        if (IS_BST_NODE(bp))  
            printf("\033[1;44;33m[BST Node| parent slotp: %p, l: %p, r: %p]\033[0m",  
            PARENT_CHLDSLOTP(bp), LCHLD_BLKP(bp), RCHLD_BLKP(bp));  
        if (PREV_SAMESZ_BLKP(bp))  
            printf("\033[1;33m[PREV] %p\033[0m", word_to_ptr(PREV_SAMESZ_BLKP(bp)));  
        putchar('\n');  
    }  
}  
  
/* 
 * checkblock - as the name goes 
 */  
static inline void checkblock(block_ptr bp)  
{  
    if (!aligned(bp))  
        printf("\n\033[1;47;31m## Error: %p is not doubleword aligned\033[0m\n", bp);  
    if (!GET_ALLOC(HDRP(bp)) && (GET(HDRP(bp)) & ~0x2) != (GET(FTRP(bp)) & ~0x2))  
        printf("\n\033[1;47;31m## Error: header does not match footer, header: %u, footer: %u \033[0m\n",  
        GET(HDRP(bp)), GET(FTRP(bp)));  
    if (GET_ALLOC(HDRP(bp)) != (GET_PREV_ALLOC(NEXT_BLKP(bp)) >> 1))  
        printf("\n\033[1;47;31m## Error: %p allocation does not match next block's prev_alloc\033[0m\n", bp);  
}  
  
static void printchain(block_ptr node)  
{  
    while (node)  
    {  
        printblock(node);  
        printf("->");  
        node = word_to_ptr(NEXT_SAMESZ_BLKP(node));  
    }  
}  
  
static void printtree(block_ptr node, int depth)  
{  
    int i;  
    if (node == NULL)  
        return;  
    printf("BST: ");  
    for (i = 0; i < depth; i++)  
        putchar('-');  
    printchain(node);  
    putchar('\n');  
    printtree(LCHLD_BLKP(node), depth + 1);  
    printtree(RCHLD_BLKP(node), depth + 1);  
}  
  
static void checklist(block_ptr node)  
{  
    if (node == NULL)  
        return;  
    if (PREV_SAMESZ_BLKP(node) &&  
        word_to_ptr(NEXT_SAMESZ_BLKP(word_to_ptr(PREV_SAMESZ_BLKP(node)))) != node)  
        printf("\n\033[1;47;31m## Bad neighbor pointer: %p\033[0m\n", node);  
    checklist(word_to_ptr(NEXT_SAMESZ_BLKP(node)));  
}  
  
static void checktree(block_ptr node)  
{  
    if (node == NULL)  
        return;  
    if (*PARENT_CHLDSLOTP(node) != node)  
        printf("\n\033[1;47;31m## Bad parent pointer: %p\033[0m\n", node);  
    checklist(node);  
    checktree(LCHLD_BLKP(node));  
    checktree(RCHLD_BLKP(node));  
}  
  
/* 
 * checkheap - check the heap for consistency 
 */  
void mm_checkheap(int verbose)  
{  
    char *bp = heap_listp;  
  
    if (verbose)  
        printf("Heap (%p):\n", heap_listp);  
  
    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || !GET_ALLOC(HDRP(heap_listp)))  
        printf("\n\033[1;47;31m## Bad prologue header\033[0m\n");  
    checkblock(heap_listp);  
  
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))  
    {  
        if (verbose)  
            printblock(bp);  
        checkblock(bp);  
    }  
  
    if (verbose)  
    {  
        printblock(bp);  
        {  
            unsigned int i;  
            for (i = 1; i <= fixed_bin_count; i++)  
            if (bins[i - 1])  
            {  
                printf("BIN #%d: size = %d ", i, i * DSIZE);  
                checklist(bins[i - 1]);  
                printchain(bins[i - 1]);  
            }  
            putchar('\n');  
            checktree(larger_bin_root);  
            printtree(larger_bin_root, 0);  
        }  
    }  
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))  
        printf("\n\033[1;47;31m## Bad epilogue header\033[0m\n");  
} 