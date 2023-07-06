from pathlib import Path
import struct


model = Path("../brew3d/brewmodel.model64")


ROMHACKED_OFFSET = 0x104000

rom_base = Path("model64viewer64.z64")
rom_romhacked = Path("model64viewer64_romhacked.z64")


model_bytes = model.read_bytes()
print("len(model_bytes) =", len(model_bytes))
print("model_bytes[:16] =", model_bytes[:16])

rom_bytes = bytearray(rom_base.read_bytes())
print("len(rom_bytes) =", len(rom_bytes))

assert len(rom_bytes) <= ROMHACKED_OFFSET

rom_bytes += b"\x42" * (ROMHACKED_OFFSET - len(rom_bytes))
assert len(rom_bytes) == ROMHACKED_OFFSET

rom_bytes += b"xxxxyyyy"

# 16-align
rom_bytes += b"\x42" * (16 - len(rom_bytes) % 16)

offset = len(rom_bytes)
size = len(model_bytes)

rom_bytes += model_bytes

print(f"offset = 0x{offset:08X}")
print(f"size = 0x{size:X}")

rom_bytes[ROMHACKED_OFFSET : ROMHACKED_OFFSET + 8] = struct.pack(">II", offset, size)

rom_romhacked.write_bytes(rom_bytes)
