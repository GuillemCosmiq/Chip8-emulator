#include "opcodes.h"
#include "types.h"
#include "chip8.h"

void chip8_op_00e0(Chip8 *const chip8)
{
	memset(chip8->videoMem, 0, sizeof(chip8->videoMem));
}

void chip8_op_00ee(Chip8 *const chip8)
{
	chip8->pc = stack_Pop(&chip8->stack);
}

void chip8_op_1nnn(Chip8 *const chip8)
{
	chip8->pc = chip8->opcode & 0x0fffu;
}

void chip8_op_2nnn(Chip8 *const chip8)
{
	stack_Push(&chip8->stack, chip8->pc);
	chip8->pc = chip8->opcode & 0x0fffu;
}

void chip8_op_3xkk(Chip8 *const chip8)
{
	if (chip8->registers[((chip8->opcode & 0x0f00u) >> 8)]
		== (byte)(chip8->opcode & 0x00ffu))
	{
		chip8->pc += 2;
	}
}

void chip8_op_4xkk(Chip8 *const chip8)
{
	if (chip8->registers[(chip8->opcode & 0x0f00u) >> 8]
		!= (byte)(chip8->opcode & 0x00ffu))
	{
		chip8->pc += 2;
	}
}

void chip8_op_5xy0(Chip8 *const chip8)
{
	if (chip8->registers[(chip8->opcode & 0x0f00u) >> 8]
		== chip8->registers[(chip8->opcode & 0x00f0u) >> 4])
	{
		chip8->pc += 2;
	}
}

void chip8_op_6xkk(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] = (byte)(chip8->opcode & 0x00ffu);
}

void chip8_op_7xkk(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] += (byte)(chip8->opcode & 0x00ffu);
}

void chip8_op_8xy0(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] = chip8->registers[(chip8->opcode & 0x00f0u) >> 4];
}

void chip8_op_8xy1(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] |= chip8->registers[(chip8->opcode & 0x00f0u) >> 4];
}

void chip8_op_8xy2(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] &= chip8->registers[(chip8->opcode & 0x00f0u) >> 4];
}

void chip8_op_8xy3(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] ^= chip8->registers[(chip8->opcode & 0x00f0u) >> 4];
}

void chip8_op_8xy4(Chip8 *const chip8)
{
	byte *vx = &chip8->registers[(chip8->opcode & 0x0f00u) >> 8];
	byte *vy = &chip8->registers[(chip8->opcode & 0x00f0u) >> 4];
	uint16 result = *vx + *vy;
	chip8->registers[0xf] = result > 0x00ffu ? 1 : 0;
	*vx = (byte)(result & 0x00ffu);
}

void chip8_op_8xy5(Chip8 *const chip8)
{
	byte *vx = &chip8->registers[(chip8->opcode & 0x0f00u) >> 8];
	byte *vy = &chip8->registers[(chip8->opcode & 0x00f0u) >> 4];
	chip8->registers[0xf] = *vx > * vy ? 1 : 0;
	*vx -= *vy;
}

void chip8_op_8xy6(Chip8 *const chip8)
{
	byte *vx = &chip8->registers[(chip8->opcode & 0x0f00u) >> 8u];
	chip8->registers[0xf] = *vx & 0x0001u;
	*vx /= 2;
}

void chip8_op_8xy7(Chip8 *const chip8)
{
	byte *vx = &chip8->registers[(chip8->opcode & 0x0f00u) >> 8];
	byte *vy = &chip8->registers[(chip8->opcode & 0x00f0u) >> 4];
	chip8->registers[0xf] = *vy > * vx ? 1 : 0;
	*vx = *vy - *vx;
}

void chip8_op_8xye(Chip8 *const chip8)
{
	byte *vx = &chip8->registers[(chip8->opcode & 0x0f00u) >> 8u];
	chip8->registers[0xf] = (*vx & 0x80u) >> 7u;
	*vx *= 2;
}

void chip8_op_9xy0(Chip8 *const chip8)
{
	if (chip8->registers[(chip8->opcode & 0x0f00u) >> 8]
		!= chip8->registers[(chip8->opcode & 0x00f0u) >> 4])
	{
		chip8->pc += 2;
	}
}

void chip8_op_annn(Chip8 *const chip8)
{
	chip8->index = chip8->opcode & 0x0fffu;
}

void chip8_op_bnnn(Chip8 *const chip8)
{
	chip8->pc = chip8->registers[0] + (chip8->opcode & 0x0fffu);
}

void chip8_op_cxkk(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] = chip8_GenerateRandomByte() & (chip8->opcode & 0x00ffu);
}

void chip8_op_dxyn(Chip8 *const chip8)
{
	byte spriteHeight = chip8->opcode & 0x000fu;
	byte vPixelPosX = chip8->registers[(chip8->opcode & 0x0f00) >> 8] % VideoMemWidth;
	byte vPixelPosY = chip8->registers[(chip8->opcode & 0x00f0) >> 4] % VideoMemHeight;

	chip8->registers[0xf] = 0;

	for (int row = 0; row < spriteHeight; ++row)
	{
		for (int col = 0; col < 8; ++col)
		{
			byte memPixel = chip8->memory[chip8->index + row] & (0x80 >> col);
			uint32 *vMemPixel = &chip8->videoMem[(vPixelPosX + col) + VideoMemWidth * (vPixelPosY + row)];

			if (memPixel)
			{
				if (*vMemPixel == 0xffffffff)
				{
					chip8->registers[0xf] = 1;
				}
				*vMemPixel ^= 0xffffffff;
			}
		}
	}

	chip8->screenUpdateRequested = 1;
}

void chip8_op_ex9e(Chip8 *const chip8)
{
	byte vx = (chip8->opcode & 0x0f00u) >> 8;
	if (chip8->keypad[chip8->registers[vx]])
	{
		chip8->pc += 2;
	}
}

void chip8_op_exa1(Chip8 *const chip8)
{
	byte vx = (chip8->opcode & 0x0f00u) >> 8;
	if (!chip8->keypad[chip8->registers[vx]])
	{
		chip8->pc += 2;
	}
}

void chip8_op_fx07(Chip8 *const chip8)
{
	chip8->registers[(chip8->opcode & 0x0f00u) >> 8] = chip8->delayTimer;
}

void chip8_op_fx0a(Chip8 *const chip8)
{
	byte vx = (chip8->opcode & 0x0f00u) >> 8;
	int keyPressed = 0;
	for (int i = 0; i < 0xf; ++i)
	{
		if (chip8->keypad[i])
		{
			chip8->registers[vx] = i;
			keyPressed = 1;
		}
	}
	if (!keyPressed)
		chip8->pc -= 2;
}

void chip8_op_fx15(Chip8 *const chip8)
{
	chip8->delayTimer = chip8->registers[(chip8->opcode & 0x0f00u) >> 8];
}

void chip8_op_fx18(Chip8 *const chip8)
{
	chip8->soundTimer = chip8->registers[(chip8->opcode & 0x0f00u) >> 8];
}

void chip8_op_fx1e(Chip8 *const chip8)
{
	chip8->index += chip8->registers[(chip8->opcode & 0x0f00u) >> 8];
}

void chip8_op_fx29(Chip8 *const chip8)
{
	chip8->index = bFont + (5 * chip8->registers[(chip8->opcode & 0x0f00u) >> 8]);
}

void chip8_op_fx33(Chip8 *const chip8)
{
	byte memByte = chip8->registers[(chip8->opcode & 0x0f00u) >> 8];

	for (int i = 2; i >= 0; --i)
	{
		chip8->memory[chip8->index + i] = memByte % 10;
		memByte /= 10;
	}
}

void chip8_op_fx55(Chip8 *const chip8)
{
	byte vx = (chip8->opcode & 0x0f00u) >> 8;
	for (int i = 0; i <= vx; ++i)
	{
		chip8->memory[chip8->index + i] = chip8->registers[i];
	}
}

void chip8_op_fx65(Chip8 *const chip8)
{
	byte vx = (chip8->opcode & 0x0f00u) >> 8;
	for (int i = 0; i <= vx; ++i)
	{
		chip8->registers[i] = chip8->memory[chip8->index + i];
	}
}