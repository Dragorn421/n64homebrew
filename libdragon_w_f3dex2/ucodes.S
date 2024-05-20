.macro glabel label
    .global \label
    \label:
.endm

.balign 16

# dd skip=$((0xBCD0F0)) count=$((0x1630)) if=/home/dragorn421/Documents/oot/baseroms/gc-eu-mq-dbg/baserom-decompressed.z64 of=gspF3DZEX2_NoN_PosLight_fifoText.bin bs=1
glabel gspF3DZEX2_NoN_PosLight_fifoTextStart
    .incbin "/home/dragorn421/Documents/oot/baseroms/gc-eu-mq-dbg/baserom-decompressed.z64", 0xBCD0F0, 0x1630
glabel gspF3DZEX2_NoN_PosLight_fifoTextEnd

.balign 16

# dd skip=$((0xBCE720)) count=$((0x420)) if=/home/dragorn421/Documents/oot/baseroms/gc-eu-mq-dbg/baserom-decompressed.z64 of=gspF3DZEX2_NoN_PosLight_fifoData.bin bs=1
glabel gspF3DZEX2_NoN_PosLight_fifoDataStart
    .incbin "/home/dragorn421/Documents/oot/baseroms/gc-eu-mq-dbg/baserom-decompressed.z64", 0xBCE720, 0x420
glabel gspF3DZEX2_NoN_PosLight_fifoDataEnd
