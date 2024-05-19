.macro glabel label
    .global \label
    \label:
.endm

.balign 16

glabel rspbootTextStart
    .incbin "/home/dragorn421/Documents/oot/baseroms/gc-eu-mq-dbg/baserom-decompressed.z64", 0x9F20, 0xD0
glabel rspbootTextEnd

.balign 16

glabel gspF3DZEX2_NoN_PosLight_fifoTextStart
    .incbin "/home/dragorn421/Documents/oot/baseroms/gc-eu-mq-dbg/baserom-decompressed.z64", 0xBCD0F0, 0x1630
glabel gspF3DZEX2_NoN_PosLight_fifoTextEnd

.balign 16

glabel gspF3DZEX2_NoN_PosLight_fifoDataStart
    .incbin "/home/dragorn421/Documents/oot/baseroms/gc-eu-mq-dbg/baserom-decompressed.z64", 0xBCE720, 0x420
glabel gspF3DZEX2_NoN_PosLight_fifoDataEnd
