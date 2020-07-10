# 保护模式

~~这部分第一次接触实在过于杂乱，根据Thunder J的blog进行的学习以及总结，添加了自己的一些想法，等以后学得熟悉了再回来重构~~

## 保护模式初探

32位CPU在16位模式下运行的状态为实模式，当CPU发展到32位的时候出现保护模式，保护模式下CPU变成了32根地址总线，32根地址总线足够访问4GB的空间，为了满足4GB空间寻址，寄存器宽度也增加了一倍，从原来的2字节变为4字节32位。除了段寄存器仍然使用16位，其余通用寄存器都提升到32位。
寄存器要保持向下兼容，不会重新构造原来的基础设备而是在原有的寄存器基础上进行了拓展。经过拓展后的寄存器在原有名字上加了个e，如图所示

![image-20200711010518407](img/image-20200711010518407.png)

### 段描述符

保护模式中的段基址不再是像实模式那样直接存放物理地址，段寄存器中要记录32位地址的数据段基址，16位肯定是装不下的，所以段基址都存储在一个数据结构中——全局描述符表。其中每个表项称为段描述符，其大小为64字节，用来描述各个内存段的起始地址、大小、权限等信息。而这里段寄存器中存放的是段选择子 selector 。如果把全局描述符表当作数组来看的话，段选择子就是数组的下标，用来索引段描述符。该全局描述符表很大，所以放在内存中，由GDTR寄存器指向它。

Tip：因为段描述符是在内存中，CPU访问较慢，效率不高，故在80286的保护模式中增加了一个段描述符缓冲寄存器用来提高效率。CPU每次将获取到的内存信息整理之后存入此寄存器，之后每次访问相同的段时，直接读取对应的段描述符缓冲寄存器即可。

实模式 VS 保护模式寻址模式

![image-20200711011318970](img/image-20200711011318970.png)

32位CPU既支持实模式有支持保护模式，为了区分当前指令到底是哪个模式下运行的，编译器提供了伪指令`bits`

> 指令格式：[bits 16]或[bits 32]，分别对应16位和32位
> [bits 16］是告诉编译器，下面的代码帮我编译成16 位的机器码。
> [bits 32］是告诉编译器，下面的代码帮我编译成32 位的机器码。

举一个例子：

操作数通过反转前缀0x66实现模式转换

![image-20200711012301849](img/image-20200711012301849.png)

同样寻址通过反转前缀0x67实现模式转换

![image-20200711012205917](img/image-20200711012205917.png)

下面总结一下，保护模式首先是必须向前兼容的，故其访问内存依然是`段基址:段内偏移`的方式，结合前面总结过实模式的一些安全问题，想要解决这些问题就得既保证向前兼容，又保证安全性。CPU工程师想到的方法就是增加更多的安全属性位，下图即是段描述符格式：

![image-20200711012421367](img/image-20200711012421367.png)

访问内存的形式如下图所示

![image-20200711012715273](img/image-20200711012715273.png)

### 全局描述符

全局描述符表GDT相当于是一个描述符的数组，数组每一个元素都是8个字节的描述符，而选择子则是提供下标在GDT中索引描述符。假设 A[10] 数组即为GDT表，则

- GDT表相当于数组A
- 数组中每个数据A[0]~A[10]相当于描述符
- A[0]~A[10]中的0~10索引下标则是选择子

全局描述符表是公用的，GDTR这个专门的寄存器则存放GDT表的内存地址和大小，是一个48位的寄存器，对这个寄存器操作无法用mov等指令，这里用的是`lgdt`指令初始化，指令格式是：`lgdt 48位内存数据`

![image-20200711014920544](img/image-20200711014920544.png)

举个访问内存的例子，例如选择子是 0x8，将其加载到 ds 寄存器后，访问 ds: 0x9 这样的内存，其过程是首先拆分 0x8 为二进制 `0000 0000 0000 1000` 然后得到 0x8 的低 2 位是RPL，其值为 00。第 2 是 TI ，其值 0，表示是在 GOT 中索引段描述符。用 0x8 的高 13 位 0x1 在 GOT 中索引，也就是 GOT 中的第 1 个段描述符(GDT 中第 0 个段描述符不可用)。假设第 1 个段描述符中的 3个段基址部分，其值为 0xl234oCPU 将 0xl234 作为段基址，与段内偏移地址 0x9 相加， `0x1234 + 0x9 = 0x123d`。用所得的和 0x123d 作为访存地址。

Tip：GDT中第0个段描述符不可用是为了防止未初始化段选择子，如果未初始化段选择子就会访问到第0个段描述符从而抛出异常。

为了让`段基址:段内偏移`策略继续可用，CPU采取的做法是将超过1MB的部分自动绕回到0地址，继续从0地址开始映射。相当于把地址对1MB求模。超过1MB多余出来的内存被称为高端内存区HMA。

这种地址绕回的做法需要通过两种情况分别讨论：

- 对于只有20位地址线的CPU，不需要任何操作便能自动实现地址绕回
- 当其他有更多地址总线的时候，因为CPU可以访问更多的内存，所以不会产生地址回滚。这种情况下的解决方案就是对第21根地址线进行操作。开启A20则直接访问物理地址即可，关闭A20则使用回绕方式访问。

打开A20的操作方法有以下三个步骤，主要是将0x92端口第一位置一即可

```asm
in al, 0x92
or al, 0000_0010B
out 0x92, al
```

### CR0寄存器

CRx系列寄存器属于控制寄存器一类，这里主要介绍CR0寄存器，这个寄存器如下图所示，其中第0位PE位表示是否开启保护模式。

![image-20200711015050571](img/image-20200711015050571.png)

对CR0的PE位操作如下所示

```asm
mov eax,cr0
or eax,0x00000001
mov cr0,eax
```

现在基础知识总结的差不多了，进入下一个实验阶段，更新我们的mbr和loader，因为我们的loader.bin会超过512字节，所以要把mbr.S中加载loader.bin的读入扇区数增大，目前是1扇区，这里直接改为4扇区

```asm
...
52 mov cx, 4         ; 带读入的扇区数
53 call rd_disk_m_16 ; 以下读取程序的起始部分(一个扇区)
...
```

## 进入保护模式

现在基础知识总结的差不多了，进入下一个实验阶段，更新我们的mbr和loader，因为我们的loader.bin会超过512字节，所以要把mbr.S中加载loader.bin的读入扇区数增大，目前是1扇区，这里直接改为4扇区

```asm
...
52 mov cx, 4         ; 带读入的扇区数
53 call rd_disk_m_16 ; 以下读取程序的起始部分(一个扇区)
...
```

如下图所示，cx 寄存器中存放的这个参数非常重要，代表读入扇区数，如果`loader.bin`的大小超过mbr读入的扇区数，就需要对这个参数进行修改

![image-20200711021807326](img/image-20200711021807326.png)

接下来就是更新`boot.inc`，里面存放的是`loader.S`的一些符号信息，相当于头文件，比之前主要多定义了GDT描述符的属性和选择子的属性。Linux使用的是平坦模型，整个内存都在一个段里，这里平坦模型在我们定义的描述符中，段基址是0，`段界限 * 粒度 = 4G` 粒度选的是4k，故段界限是 0xFFFFF

```asm
;boot.inc
;--------------------- loader 和 kernel---------------------

LOADER_BASE_ADDR equ 0x900
LOADER_START_SECTOR equ 0x2

;--------------------  gdt 描述符属性  ----------------------
DESC_G_4K         equ 1_00000000000000000000000b        ;描述符的G位为4k粒度，以二进制表示，下划线可去掉
DESC_D_32         equ  1_0000000000000000000000b
DESC_L            equ   0_000000000000000000000b        ;64位代码标记，此处标记为0便可
DESC_AVL          equ    0_00000000000000000000b        ;CPU不用此位，暂置为0
DESC_LIMIT_CODE2  equ     1111_0000000000000000b        ;段界限，需要设置为0xFFFFF
DESC_LIMIT_DATA2  equ     DESC_LIMIT_CODE2
DESC_LIMIT_VIDEO2 equ      0000_000000000000000b
DESC_P			  equ         1_000000000000000b
DESC_DPL_0        equ          00_0000000000000b
DESC_DPL_1		  equ          01_0000000000000b
DESC_DPL_2        equ		   10_0000000000000b
DESC_DPL_3        equ          11_0000000000000b
DESC_S_CODE		  equ            1_000000000000b
DESC_S_DATA       equ            DESC_S_CODE
DESC_S_sys        equ            0_000000000000b
DESC_TYPE_CODE    equ             1000_00000000b		;x=1,c=0,r=0,a=0 代码段是可执行的,非一致性,不可读,已访问位a清0.  
DESC_TYPE_DATA    equ             0010_00000000b		;x=0,e=0,w=1,a=0 数据段是不可执行的,向上扩展的,可写,已访问位a清0.

DESC_CODE_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_CODE2 + DESC_P + DESC_DPL_0 + DESC_S_CODE + DESC_TYPE_CODE + 0x00 ;定义代码段的高四字节，(0x00 << 24)表示"段基址的24~31"字段，该字段位于段描述符高四字节24~31位，平坦模式段基址为0，所以这里用0填充，最后的0x00也是
DESC_DATA_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_DATA2 + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x00
DESC_VIDEO_HIGH4 equ (0x00 << 24) + DESC_G_4K + DESC_D_32 + DESC_L + DESC_AVL + DESC_LIMIT_VIDEO2 + DESC_P + DESC_DPL_0 + DESC_S_DATA + DESC_TYPE_DATA + 0x0b

;--------------   选择子属性  ---------------
RPL0  equ   00b
RPL1  equ   01b
RPL2  equ   10b
RPL3  equ   11b
TI_GDT	 equ   000b
TI_LDT	 equ   100b
```

下面修改 `loader.S`

```asm
;loader.S
%include "boot.inc"
   section loader vstart=LOADER_BASE_ADDR
   LOADER_STACK_TOP equ LOADER_BASE_ADDR
   jmp loader_start                    ; 此处的物理地址是:

;构建gdt及其内部的描述符
   GDT_BASE:   dd    0x00000000
           dd    0x00000000

   CODE_DESC:  dd    0x0000FFFF
           dd    DESC_CODE_HIGH4

   DATA_STACK_DESC:  dd    0x0000FFFF
             dd    DESC_DATA_HIGH4

   VIDEO_DESC: dd    0x80000007           ;limit=(0xbffff-0xb8000)/4k=0x7
           dd    DESC_VIDEO_HIGH4  ; 此时dpl已改为0

   GDT_SIZE   equ   $ - GDT_BASE
   GDT_LIMIT   equ   GDT_SIZE -    1
   times 60 dq 0                     ; 此处预留60个描述符的slot
   SELECTOR_CODE equ (0x0001<<3) + TI_GDT + RPL0         ; 相当于(CODE_DESC - GDT_BASE)/8 + TI_GDT + RPL0
   SELECTOR_DATA equ (0x0002<<3) + TI_GDT + RPL0     ; 同上
   SELECTOR_VIDEO equ (0x0003<<3) + TI_GDT + RPL0     ; 同上

   ;以下是定义gdt的指针，前2字节是gdt界限，后4字节是gdt起始地址

   gdt_ptr  dw  GDT_LIMIT
        dd  GDT_BASE
   loadermsg db '2 loader in real.'

   loader_start:

;------------------------------------------------------------
;INT 0x10    功能号:0x13    功能描述:打印字符串
;------------------------------------------------------------
;输入:
;AH 子功能号=13H
;BH = 页码
;BL = 属性(若AL=00H或01H)
;CX＝字符串长度
;(DH、DL)＝坐标(行、列)
;ES:BP＝字符串地址
;AL＝显示输出方式
;   0——字符串中只含显示字符，其显示属性在BL中。显示后，光标位置不变
;   1——字符串中只含显示字符，其显示属性在BL中。显示后，光标位置改变
;   2——字符串中含显示字符和显示属性。显示后，光标位置不变
;   3——字符串中含显示字符和显示属性。显示后，光标位置改变
;无返回值
   mov     sp, LOADER_BASE_ADDR
   mov     bp, loadermsg           ; ES:BP = 字符串地址
   mov     cx, 17             ; CX = 字符串长度
   mov     ax, 0x1301         ; AH = 13,  AL = 01h
   mov     bx, 0x001f         ; 页号为0(BH = 0) 蓝底粉红字(BL = 1fh)
   mov     dx, 0x1800         ;
   int     0x10                    ; 10h 号中断

;----------------------------------------   准备进入保护模式   ------------------------------------------
                                    ;1 打开A20
                                    ;2 加载gdt
                                    ;3 将cr0的pe位置1


   ;-----------------  打开A20  ----------------
   in al,0x92
   or al,0000_0010B
   out 0x92,al

   ;-----------------  加载GDT  ----------------
   lgdt [gdt_ptr]


   ;-----------------  cr0第0位置1  ----------------
   mov eax, cr0
   or eax, 0x00000001
   mov cr0, eax

   ;jmp dword SELECTOR_CODE:p_mode_start         ; 刷新流水线，避免分支预测的影响,这种cpu优化策略，最怕jmp跳转，
   jmp  SELECTOR_CODE:p_mode_start         ; 刷新流水线，避免分支预测的影响,这种cpu优化策略，最怕jmp跳转，
                         ; 这将导致之前做的预测失效，从而起到了刷新的作用。

[bits 32]
p_mode_start:
   mov ax, SELECTOR_DATA
   mov ds, ax
   mov es, ax
   mov ss, ax
   mov esp,LOADER_STACK_TOP
   mov ax, SELECTOR_VIDEO
   mov gs, ax

   mov byte [gs:160], 'P'

   jmp $
```

同之前的方法编译，注意这里loader.bin编译后为615个字节，需要2个扇区大小，写入磁盘时要给count赋值为2

![image-20200711022300308](img/image-20200711022300308.png)

> MBR :		sudo dd if=mbr.bin of=./hd60M.img bs=512 count=1 conv=notrunc
>
> Loader :	sudo dd if=loader.bin of=./hd60M.img bs=512 count=2 seek=2 conv=notrunc

运行结果如下，其中`1 MBR`来自实模式下的mbr.S，`2 loader in real`来自实模式下用BIOS中断0x10实现的，左上角第二行的`P`是在保护模式下输出的。

![image-20200711022423029](img/image-20200711022423029.png)

查看GDT表中的内容和我们设置的相符，其中第0个不可用。查看寄存器信息PE位设置为1表示已经进入保护模式。

![image-20200711022609514](image-20200711022609514.png)

## 保护模式进阶

