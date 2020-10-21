;--------------------- loader 和 kernel---------------------

; loader 和 kernel
PAGE_DIR_TABLE_POS equ 0x100000
; 页表相关属性
PG_P equ 1b
PG_RW_R equ 00b
PG_RW_W equ 10b
PG_US_S equ 000b
PG_US_U equ 100b

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