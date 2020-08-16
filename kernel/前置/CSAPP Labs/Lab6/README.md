# Malloc Lab

## 1. 实验目标：

完成动态内存分配器，模拟malloc，free，realloc等函数功能，本次实验仅需要在`mm.c`中完成修改。

需要填写的函数：

> int mm_init(void)
> void *mm_malloc(size_t size)
> void mm_free(void *ptr);
> void *mm_realloc(void *ptr, size_t size);

tips：

> 1. 合理地使用宏可以减少DEBUG开销
> 2. 实验要求8 bytes对齐，即mm_malloc返回的指针需要为8的倍数
> 3. 建议将trace文件夹放在mdriver所在文件夹中，并且更改config.h文件当中TRACEDIR的宏定义为
>    \#define TRACEDIR “traces/“。
>    make之后就可以直接运行./mdriver进行全部测试
>    也可以使用./mdriver -f 命令对某个文件进行测试

## 2. 本次实验涉及的概念

### 1. 数据结构

- 三种适配方式
  - first fit：扫描所有的块，只要当前块的大小满足要求就使用，速度较快。但容易导致空闲列表中前面的块被不断地细分，而后面的一些块却一直迟迟得不到利用
  - second fit: 扫描的时候，每次从上一次扫描的下一个块开始，这样可以使得整个列表的块都可以被使用，这使得效率更高。然而，实际应用中，作用也很有限，容易产生很大的空间浪费，造成大量碎片
  - best fit：这种方式最大的好处是可以充分地利用空间。找到所有满足要求的块中最小的那一个，这样可以很大程度上避免浪费。当然，这也使得时间成本较高，尤其是如果空间链表的组织方式不太恰当的话，容易导致每次都要遍历一整个列表
- 三种空闲列表的组织方式
  - implicit free list：直接将所有的块(不管是否有分配)串在一起，然后遍历。这种方式可也使得块最小可以达到8 bytes。当然，这种方式效率很低，尤其是当块的数量较多的时候
  - explicit free list：在每一个free 块中保存两个指针，将所有空闲的块组成一个双向链表。和隐式相比，这种方式最大的好处在于我们不需要遍历已经分配的块，速度上快了很多，当然，由于需要保存指针，所以每一个块最小为16 bytes
  - segregated free list：这种方式的特点在于，根据块的不同大小，分成k组，组织成k条双向链表。分类的方式有很多，比如可以采用2的倍数的分类方式，{1},{2},{3~4},{5~8}……大小为6的块放在第四条链中，大小为3的块则放在第三条链中等等。
- 两种合并的方式
  - immediate coalescing
  - deferred coalescing
- 两种内存碎片
  - internal fragmentation：内部碎片，即是已经被分配出去（能明确指出属于哪个进程）却不能被利用的内存空间。比如当前我需要44 bytes的内存，然后malloc的时候分配到了一个48 bytes的块，这样的话，剩下的4 bytes的内存尽管我不需要用到，但是其他程序也无法使用，这就是内部碎片
  - external fragmentation：外部碎片，即还没有被分配出去（不属于任何进程），但由于太小了无法分配给申请内存空间的新进程的内存空闲区域
- 空间列表的排序方式
  - size order
  - address order

### 2. 实验所需要的宏：

```c
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define PACK(size, alloc) ((size) | (alloc))

#define GET(p)  (*(unsigned int*)(p))
#define PUT(p, val) (*(unsigned int*)(p) = (unsigned int)(val))

#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_PREV_ALLOC(p) (GET(p) & 0x2)

#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define LAST_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

#define NEXT_PTR(bp) ((char*)(bp))
#define LAST_PTR(bp) ((char*)(bp) + WSIZE)

#define LINK_NEXT(bp) ((char *)GET(bp))
#define LINK_LAST(bp) ((char *)GET(bp + WSIZE))
```

全局变量

```c
static void* start_pos; // 永远指向当前堆的最顶部
static char* end_link_list; // 链表数组后的下一个块
static char* start_link_list; // 链表数组的表头
```

### 3. API

#### extend_heap

　　关于堆的拓展，我们需要注意到，每次拓展的大小都应该为8的倍数，这样才能保证8字节对齐。
　　其次，在拓展的时候，可以有一个小优化。假设我们需要拓展的大小为size。拓展时，我们先查看位于堆顶端的块，如果堆顶端是一个空闲的块，并且大小为msize的话，我们可以只拓展size - msize即可。这样的话可以在一定程度上提高空间利用率(针对某些比较特殊的数据效果很明显)。当然，这样的话也会使得整个程序效率降低(频繁使用mem_sbrk的话对程序性能的影响是很大的，这是一个系统调用)。

#### link_list

#### find_fit

　　在寻找能放得下size个字节的最小的块的时候，我们有两种处理策略。一种是遍历当前的链表，寻找是否有满足要求的块，有的话就返回。另一种方法是只遍历size所在的链表，如果没有，直接返回NULL，交由后面堆去拓展。
　　第一种方法的话很明显时间上效率较低，但是能够保证较大的块能够被使用。后一种方法的话时间上效率较高，但是可能导致较大的某个块一直无法被利用，从而导致了空间的浪费。

#### malloc & free

　　如下。有几个要注意的点。首先，分配内存的时候，我们需要一个大小至少为WSIZE(allocated block的头部) + size的块，且最小为16。如果小于16的话，会导致free的时候放不下，从而出现问题。其次，注意好标志位。free的时候下一个块的第二个标志位应该清零。以及free的时候，要顺便看下前后能不能合并，可以合并的话应该合并完后再插入到链表当中。

#### place

具体在算法中会描述

#### realloc

考虑到有块间重叠的因素，用memove代替memcpy实际效果会更好

#### 4.Tips：

在网上学到的满分策略：[](https://blog.csdn.net/huang1024rui/article/details/50526998)

使用空闲链表+BST

- 使用一小撮空闲链表来维护较小的空闲块
- 使用二叉搜索树来维护大的空闲块链表
- 所有的链表只存一种大小的空闲块
- 具体如图所示，二叉搜索树的每个节点都是一个链表的头部



优化策略：

1、已分配的块可以省略脚部（Footer）。

那么相邻空闲块合并时从何得知前一块是不是空闲呢？

我们已经知道，块是按8字节对齐的，也就是说块头部中存储的块的大小是8的倍数，二进制表示中最后三位都是0。而在头部中，最低一位是存储当前块的分配状况的，那么就拿第二位来存储前一个块的分配情况好了。

这样我们可以在合并时检查自己头部来得到前一块是不是被分配了，如果没有被分配，就与之合并。

2、块指针可以用4字节保存。

虽然在64位机器中，指针是64位的，不过由于本次Lab的writeup中指出

Because we are running on 64-bit machines, your allocator must be coded accordingly, with one exception: the size of the heap will never be greater than or equal to 2^32 bytes. This does not imply anything about the location of the heap, but there is a neat optimization that can be done using this information. However, be very, very careful if you decide to take advantage of this fact.

## 3. ptmalloc

（没时间整理了）

https://blog.csdn.net/z_ryan/article/details/79950737