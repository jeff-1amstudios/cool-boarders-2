"""Emit short, byte-exact data fragments that the word disassembler cannot cover."""
from splat.segtypes.common.data import CommonSegData


class PSXSegBytes(CommonSegData):
    def scan(self, rom_bytes):
        pass

    def split(self, rom_bytes):
        data = rom_bytes[self.rom_start:self.rom_end]
        path = self.asm_out_path()
        path.parent.mkdir(parents=True, exist_ok=True)
        lines = ['.section .data, "wa"', ""]
        for offset in range(0, len(data), 16):
            lines.append(".byte " + ", ".join(f"0x{value:02X}" for value in data[offset:offset + 16]))
        path.write_text("\n".join(lines) + "\n")
