## misc

- ：：表示全局函数
- auto可以偷一点懒
- 类自带的sort会比全局的sort快
- hash_ set/map/nultiset/multimap

# sequence containers

##  vector

- 增长方式：两倍增长
- size()是元素个数，capacity()是容器的空间大小
- 大量拷贝，会有浪费 

## deque

- 分段连续
- ![](http://23.91.100.118:8080/images/2020/06/23/87fcb80da9232fb16c698aa54db0f4bc.png)
- maximum最大
- 没有sort
- deque->queue->stack，后两者没有自己的数据结构，被称为容器适配器，用的是deque的

# associated containers

## mutiset

- 查找很快，红黑树
- 重复可以放进去 

## multimap

## unordered_multiset

- 篮子比元素多
- hashtable

## unordered_multimap

- muti允许元素重合

## set

- key（value）不可以重复 

## map

- 允许用[]来insert，而且插入的是pair，不会重复，对比set，size实际是比非重复元素大的 















## allocator

- 模板的第二个参数是alocator，默认std::allocator
- 直接用，总要记得拿/还几个，不建议直接用
- VC allocate -> operator new -> malloc
  VC deallocate -> opratoe delete -> free
- GNU C的allocate模式，避免了多次malloc（内存cookies）造成的过多开销