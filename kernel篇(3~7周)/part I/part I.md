# 部署工作环境

因为是第一次接触linux kernel，加上选择参考的书籍并不是很旧，所以选择书中软件版本搭建环境。

准备如下：

`虚拟机：Vmware Workstation`

`系统：ubuntu 16.04 x64`

`虚拟环境（加载在虚拟机中）:bochs-2.6.2`

安装依赖：

> sudo apt install build-essential xorg-dev bison libgtk2.0-dev

选择bochs-2.6.2版本，解压

设置环境

```
./configure \
--prefix=YOUR_WORKING_ENV \
--enable-debugger \
--enable-disasm \
--enable-iodebug \
--enable-x86-debugger \
--with-x \
--with-x11
```

找到MakeFile中`LIBS =`在最后加上`-lpthread`

> LIBS =  -lm -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig     -lfreetype -lpthread

然后`sudo make`编译，`sudo make install`安装，cd进入bin目录，创建一个`bochsrc.disk`的配置文件

```
# Configuration file for Bochs
# 设置Bochs在运行过程中能够使用的内存: 32 MB
megs: 32

# 设置真实机器的BIOS和VGA BIOS
# 修改成你们对应的地址

romimage: file=YOUR_WORKING_ENV/bochs-2.6.2/share/bochs/BIOS-bochs-latest
vgaromimage: file=YOUR_WORKING_ENV/bochs-2.6.2/share/bochs/VGABIOS-lgpl-latest

# 设置Bochs所使用的磁盘
# 设置启动盘符
boot: disk

# 设置日志文件的输出
log: bochs.out

# 开启或关闭某些功能，修改成你们对应的地址
mouse: enabled=0
keyboard:keymap=YOUR_WORKING_ENV/bochs-2.6.2/share/bochs/keymaps/x11-pc-us.map

# 硬盘设置
ata0: enabled=1, ioaddr1=0x1f0, ioaddr2=0x3f0, irq=14

# 增加gdb支持，这里添加会报错，暂时不需要
# gdbstub: enabled=1, port=1234, text_base=0, data_base=0, bss_base=0
```

接着在bin下运行bochs即可，为了避免不必要的错误，建议命令前加上`sudo`

![](http://23.91.100.118:8080/images/2020/07/07/6ecb54ba98cb939c48cae6d7b1653aaf.png)

第一次输入直接敲回车，第二次输入文件名`bochsrc.disk`完成初始化文件

```
You can also start bochs with the -q option to skip these menus.

1. Restore factory default configuration
2. Read options from...
3. Edit options
4. Save options to...
5. Restore the Bochs state from...
6. Begin simulation
7. Quit now

Please choose one: [2] // 直接回车

What is the configuration file name?
To cancel, type 'none'. [none] bochsrc.disk // 输入我们刚才配置的文件即可
00000000000i[     ] reading configuration from bochsrc.disk
```

运行之后会中断提示`Mouse capture off`，输入`c`继续运行，运行成功后需要继续设置设备，用`bximage`进行模拟

```
Usage: bximage [options] [filename]

Supported options:
  -fd              create floppy image                           // 创建软盘
  -hd              create hard disk image                        // 创建硬盘
  -mode=...        image mode (hard disks only)                  // 创建硬盘类型
  -size=...        image size in megabytes                       // 创建大小
  -q               quiet mode (don't prompt for user input)      // 以静默模式创建,不和用户交互
  --help           display this help and exit
```

用如下语句创建一个名为hd60M.img的虚拟镜像

> sudo ./bximage -hd -mode="flat" -size=60 -q hd60M.img

在之前的`bochsrc.disk`配置文件中添加一行`ata0-master: type=disk, path="hd60M.img", mode=flat, cylinders=121, heads=16, spt=63`，重新指定配置文件运行

```
sudo ./bochs -f bochsrc.disk
```

再次报错，意思是当前磁盘不是一个有效的启动盘，接下来我们需要编写BIOS、MBR等，环境搭建至此结束。

![](http://23.91.100.118:8080/images/2020/07/07/637823d26b81538b087d825f1b06f6a8.png)