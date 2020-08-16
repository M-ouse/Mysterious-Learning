# 初见实模式

## BIOS

- 作用：
  - 自检及初始化
  - 程序服务
  - 设定中断

实模式下初始化BIOS时候最大寻址范围为2^20（1MB），并在上电的时候强制设置`CS:IP`为`F000:FFF0`，寻址时访问的地址为`0xFFFF0 = (0xF000<<4)+0xFFF0`

在实模式下BIOS 1MB的内存布局：

![](http://23.91.100.118:8080/images/2020/07/06/53830fbbe5699da77e4e3ee25b4cc545.png)

来看一下0xFFFF0距离1MB只有16个byte大小，显然是作为一个跳转表将ip跳转到合适的区域上。

BIOS最后的工作就是校验启动盘中位于0盘0道1扇区的内容，里面的数据主要是MBR（主引导记录446字节+硬盘分区表64字节），如果末尾两字节为0x55，0xaa（在内存中是0xaa55），那么就会把这块数据加载到0x7c00处，跳转到这个位置继续执行。把数据写到0x7c00是为了防止过早覆盖中断向量表，栈空间等其他数据结构预留空间。

简言之，BIOS将对硬件做一系列检测，然后将控制权给MBR

## MBR

### BIOS加载MBR

BIOS的最后一项工作是检测硬盘中位于0磁盘0磁道1扇区大小为512字节的最后2个字节是否为0x55和0xaa,如果是的话便认为该扇区中存在可执行程序，并将该扇区(512字节大小)内容加载到内存物理地址0x7C00处，然后执行jmp 0:0x7c00指令，CPU跳转到地址(0x0左移四位+0x7c00=)0x7c00处执行MBR程序

### MBR easy

按照书里的代码仔细阅读&调试了一次，最好是可以照着抄一遍

```asm
;主引导程序 
;------------------------------------------------------------
SECTION MBR vstart=0x7c00         
   mov ax,cs      
   mov ds,ax
   mov es,ax
   mov ss,ax
   mov fs,ax
   mov sp,0x7c00
   mov ax,0xb800 ;参考上表的基址
   mov gs,ax

; 清屏
;利用0x06号功能，上卷全部行，则可清屏。
; -----------------------------------------------------------
;INT 0x10   功能号:0x06	   功能描述:上卷窗口
;------------------------------------------------------
;输入：
;AH 功能号= 0x06
;AL = 上卷的行数(如果为0,表示全部)
;BH = 上卷行属性
;(CL,CH) = 窗口左上角的(X,Y)位置
;(DL,DH) = 窗口右下角的(X,Y)位置
;无返回值：
   mov     ax, 0600h
   mov     bx, 0700h
   mov     cx, 0               ; 左上角: (0, 0)
   mov     dx, 184fh	       ; 右下角: (80,25),
			       ; 因为VGA文本模式中，一行只能容纳80个字符,共25行。
			       ; 下标从0开始，所以0x18=24,0x4f=79
   int     10h                 ; int 10h

   ; 输出背景色绿色，前景色红色，并且跳动的字符串"1 MBR"
   mov byte [gs:0x00],'1'      ; 一字节为数据,一字节为属性
   mov byte [gs:0x01],0xA4     ; A表示绿色背景闪烁，4表示前景色为红色

   mov byte [gs:0x02],' '
   mov byte [gs:0x03],0xA4

   mov byte [gs:0x04],'M'
   mov byte [gs:0x05],0xA4

   mov byte [gs:0x06],'B'
   mov byte [gs:0x07],0xA4

   mov byte [gs:0x08],'R'
   mov byte [gs:0x09],0xA4

   jmp $		       ; 通过死循环使程序悬停在此

   times 510-($-$$) db 0
   db 0x55,0xaa			;标识
```

这里我在0x7c00下了个断点，如图：

![](http://23.91.100.118:8080/images/2020/07/10/7650ed54650494615f93ff4fe88321eb.png)

### MBR medium

mbr的功能远非如此，从0x7c00进入mbr之后，我们可以让mbr在0-1MB这片区域读取磁盘内容，并把这份数据写到一个特定的位置，让mbr把控制权交给它，实际上这就是后面要学习的loader。

```asm
;boot.inc
;-------------	 loader和kernel   ----------

LOADER_BASE_ADDR equ 0x900 
LOADER_START_SECTOR equ 0x2

```



```asm
;主引导程序 
;------------------------------------------------------------
%include "boot.inc"
SECTION MBR vstart=0x7c00         
   mov ax,cs      
   mov ds,ax
   mov es,ax
   mov ss,ax
   mov fs,ax
   mov sp,0x7c00
   mov ax,0xb800
   mov gs,ax

;清屏
;利用0x06号功能，上卷全部行，则可清屏。
; -----------------------------------------------------------
;INT 0x10   功能号:0x06	   功能描述:上卷窗口
;------------------------------------------------------
;输入：
;AH 功能号= 0x06
;AL = 上卷的行数(如果为0,表示全部)
;BH = 上卷行属性
;(CL,CH) = 窗口左上角的(X,Y)位置
;(DL,DH) = 窗口右下角的(X,Y)位置
;无返回值：
   mov     ax, 0600h
   mov     bx, 0700h
   mov     cx, 0                   ; 左上角: (0, 0)
   mov     dx, 184fh		      ; 右下角: (80,25),
				   ; 因为VGA文本模式中，一行只能容纳80个字符,共25行。
				   ; 下标从0开始，所以0x18=24,0x4f=79
   int     10h                     ; int 10h

   ; 输出字符串:MBR
   mov byte [gs:0x00],'1'
   mov byte [gs:0x01],0xA4

   mov byte [gs:0x02],' '
   mov byte [gs:0x03],0xA4

   mov byte [gs:0x04],'M'
   mov byte [gs:0x05],0xA4	   ;A表示绿色背景闪烁，4表示前景色为红色

   mov byte [gs:0x06],'B'
   mov byte [gs:0x07],0xA4

   mov byte [gs:0x08],'R'
   mov byte [gs:0x09],0xA4
   ; 寄存器传三个参数
   mov eax,LOADER_START_SECTOR	 ; 起始扇区LBA地址
   mov bx,LOADER_BASE_ADDR       ; 写入的地址
   mov cx,1			            ; 待读入的扇区数,这里是简单的loader故一个扇区足够
   call rd_disk_m_16		    ; 以下读取程序的起始部分（一个扇区）
  
   jmp LOADER_BASE_ADDR
       
;-------------------------------------------------------------------------------
;功能:读取硬盘n个扇区
rd_disk_m_16:	   
;-------------------------------------------------------------------------------
				       ; eax=LBA扇区号
				       ; ebx=将数据写入的内存地址
				       ; ecx=读入的扇区数
      mov esi,eax	  ;备份eax
      mov di,cx		  ;备份cx
;读写硬盘:
;第1步：选择通道，往该通道的sector count寄存器中写入待操作的扇区数
;因为bochs配置文件中虚拟硬盘属于ata0,是Primary通道,所以sector count寄存器由0x1f2端口访问
      mov dx,0x1f2
      mov al,cl
      out dx,al            ;读取的扇区数
      ;out 往端口中写数据
      ;in  从端口中读数据

      mov eax,esi	   ;恢复ax

;第2步：将LBA地址写入三个LBA寄存器和device寄存器的低4位

      ;LBA地址7~0位写入端口0x1f3
      mov dx,0x1f3                       
      out dx,al                          

      ;LBA地址15~8位写入端口0x1f4
      mov cl,8
      shr eax,cl
      mov dx,0x1f4
      out dx,al

      ;LBA地址23~16位写入端口0x1f5
      shr eax,cl
      mov dx,0x1f5
      out dx,al

      shr eax,cl
      and al,0x0f	   ; lba第24~27位
      or al,0xe0	   ; 设置7～4位为1110,表示lba模式
      mov dx,0x1f6
      out dx,al

;第3步：向command寄存器写入读命令，0x20 
      mov dx,0x1f7 ;要写入的端口
      mov al,0x20  ;要写入的数据          
      out dx,al

;第4步：检测硬盘状态，读取该通道上的status寄存器，判断硬盘工作是否完成
  .not_ready:
      ;同一端口，写时表示写入命令字，读时表示读入硬盘状态
      nop
      in al,dx
      and al,0x88	       ;第4位为1表示硬盘控制器已准备好数据传输，第7位为1表示硬盘忙
      cmp al,0x08
      jnz .not_ready	   ;若未准备好，继续等。

;第5步：从0x1f0端口读数据
      mov ax, di
      mov dx, 256
      mul dx
      mov cx, ax	   ; di为要读取的扇区数，一个扇区有512字节，每次读入一个字，
			          ; 共需di*512/2次，所以di*256
      mov dx, 0x1f0
  .go_on_read: ; 循环写入bx指向的内存
      in ax,dx
      mov [bx],ax
      add bx,2		  
      loop .go_on_read
      ret

   times 510-($-$$) db 0
   db 0x55,0xaa
```

mbr的编译命令以及加载位置：

```bash
sudo nasm -I include/ -o mbr.bin mbr.S
sudo dd if=./mbr.bin of=YOUR_ENV/bochs-2.6.2/bin/hd60M.img bs=512 count=1 conv=notrunc
```

loader的作用是加载一些描述表，为从实模式向保护模式以及加载内核做准备，该数据结构是向上增长的，同样出于安全考虑要把loader放在一个安全的位置，这里选择0x900。

从实模式下1MB内存布局我们可以看出两个地址空间0x500~0x7BFF和0x7E00~0x9FBFF都是可用的内存区域，因此loader可以加载到这两个内存区域中的一个。但考虑如下两个因素：

首先， loader 中要定义一些数据结构(如 GDT 全局描述符表，不懂没关系，以后会说)，这些数据结 构将来的内核还是要用的，所以 loader加载到内存后不能被覆盖。
其次，随着咱们不断添加功能，内核必然越来越大，其所在的内存地址也会向越来越高的地方发展， 难免会超过可用区域的上限，咱们尽量把 loader放在低处，多留出一些空间给内核。

```asm
;loader.S
%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR

mov byte [gs:0x00],'2'
mov byte [gs:0x01],0xA4

mov byte [gs:0x02],' '
mov byte [gs:0x03],0xA4

mov byte [gs:0x04],'L'
mov byte [gs:0x05],0xA4

mov byte [gs:0x06],'O'
mov byte [gs:0x07],0xA4

mov byte [gs:0x08],'A'
mov byte [gs:0x09],0xA4

mov byte [gs:0x0a],'D'
mov byte [gs:0x0b],0xA4

mov byte [gs:0x0c],'E'
mov byte [gs:0x0d],0xA4

mov byte [gs:0x0e],'R'
mov byte [gs:0x0f],0xA4

jmp $
```

loader的编译命令以及加载位置：

```bash
sudo nasm -I include/ -o loader.bin loader.S
sudo dd if=./loader.bin of=YOUR_ENV/bochs-2.6.2/bin/hd60M.img bs=512 count=1 seek=2 conv=notrunc
```

![](http://23.91.100.118:8080/images/2020/07/10/a78bd41e116fc152cd4d155c0d56a0f1.png)

总结：

实模式下，用户程序可以随意根据段基址+段内偏移地址访问任意的内容单元，甚至进行篡改，这可能会危害计算机的安全，因此，保护模式下会对各种内存段的描述符信息加以限制，来确保计算机的安全性，将所有和段描述符有关的信息存储在**全局描述符表**中，其中每一个表项64字节，称为段描述符，用于描述各个内存段的起始地址，大小，权限等信息。全局描述符表存在内存中，由GDTR寄存器指向。
 此时的段寄存器不再保存段基址，而是保存“段选择子”，用于索引全局描述符表中的内存段，但是为了减少对内存的访问（全局描述符表在内存中），又给段寄存器使用了缓冲技术，即**段描述符缓冲寄存器**。
 为了将保护模式和实模式加以区别，我们需要告诉编译器代码的格式，即将代码编译成16位的机器码还是32位的机器码，通过[bits 16] [bits 32]来指定。如果不指定，默认是[bits 16]。因此，我们需要清楚自己的代码是运行在什么模式下。

32位和16位相互兼容，如果需要在实模式下操作16位，或者保护模式下操作32位，编译器会自动加上反转前缀0x66或者0x67
