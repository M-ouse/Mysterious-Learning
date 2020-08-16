# Bomb Lab

任务总评：认真做完之后，你的读汇编能力会极大提升。从事逆向的人员可以做一下练练手，体验没有IDA等工具辅助的感觉。

共有6个phase，每个phase需要输入正确的字符串保证程序不会执行`bomb`函数~~（这不就是CTF题嘛）~~。

实验推荐的看汇编方式：

> objdump -d > obj.txt

以及配合gdb调试。

个人实验环境：gdb(pwndbg插件) + objdump

因为本身是做逆向方向的，详细的讲解文章通过谷歌又俯拾即是，所以这里仅仅用自己最精简的语言过一遍。

## phase1

判断输入字串长度+和Dst字符串比较

## phase2

动态调试可以发现判断`2 * [i]`是否等于`[i+1]`

## phase3

读入的第一个参数被作为switch的opt，第二个参数和选中的case相比。

## phase4

就是正常分析，我的答案是(7,0)

## phase5

就是输入被mod掉当作另一个表的index，最后根据Dst字串逆向分析下标，加上n倍模数16即可。

## phase6

看一会就能看出来是个链表的数据结构

```c
struct {
    int value;
    int order;
    node* next;
} node;
```

## secret

一开始没注意，，，打开IDA发现还有个secret，进入secret的方法很简单，phase4的ans加上`DrEvil`，就可以进入Secret Phase，分析一下其中的算法和树的形状就出来了，还可以根据变量命名的规则确定树的形状。我的lab要求fun7返回2，所以我的答案是20（估计每个lab要求的返回值不同，有好多抄袭0分的23333）。

树：

```
              36
        /           \
      8               50
    /    \          /    \
   /      \        /      \
  6       22      45      107
 / \     /  \    /  \    /   \
1   7   20  35  40  47  99  1001
```

算法：

```c
struct treeNode
{
    int data;
    struct treeNode* leftChild;
    struct treeNode* rightChild;
};

int fun7(struct treeNode* p, int v)
{
    if (p == NULL)
        return -1;
    else if (v < p->data)
        return 2 * fun7(p->leftChild, v);
    else if (v == p->data)
        return 0;
    else 
        return 2 * fun7(p->rightChild, v) + 1;
}
```

