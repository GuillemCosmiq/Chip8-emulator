#include "chip8.h"
#include "opcodes.h"
#include <stdio.h>

void chip8_Alloc(const Chip8 **const chip8)
{
	*chip8 = (Chip8 *const)malloc(sizeof(Chip8));
}

void chip8_Dealloc(const Chip8 **const chip8)
{
	free(*chip8);
}

void chip8_Init(Chip8 *const chip8)
{
	memset(chip8->memory, 0, sizeof(chip8->memory));
	chip8_Reboot(chip8);

	byte font[80] =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0,
		0x20, 0x60, 0x20, 0x20, 0x70,
		0xF0, 0x10, 0xF0, 0x80, 0xF0,
		0xF0, 0x10, 0xF0, 0x10, 0xF0,
		0x90, 0x90, 0xF0, 0x10, 0x10,
		0xF0, 0x80, 0xF0, 0x10, 0xF0,
		0xF0, 0x80, 0xF0, 0x90, 0xF0,
		0xF0, 0x10, 0x20, 0x40, 0x40,
		0xF0, 0x90, 0xF0, 0x90, 0xF0,
		0xF0, 0x90, 0xF0, 0x10, 0xF0,
		0xF0, 0x90, 0xF0, 0x90, 0x90,
		0xE0, 0x90, 0xE0, 0x90, 0xE0,
		0xF0, 0x80, 0x80, 0x80, 0xF0,
		0xE0, 0x90, 0x90, 0x90, 0xE0,
		0xF0, 0x80, 0xF0, 0x80, 0xF0,
		0xF0, 0x80, 0xF0, 0x80, 0x80 
	};

	for (int i = 0; i < 80; ++i)
	{
		chip8->memory[bFont + i] = font[i];
	}

	srand(time(NULL));
}

void chip8_Reboot(Chip8 *const chip8)
{
	chip8->pc = bROM;
	stack_Init(&chip8->stack);
	memset(chip8->videoMem, 0, sizeof(chip8->videoMem));
	memset(chip8->keypad, 0, sizeof(chip8->keypad));
	chip8->screenUpdateRequested = 0;
	chip8->playSoundRequested = 0;
}

void chip8_Cycle(Chip8 *const chip8)
{
	// fetch
	byte *pcMem = &chip8->memory[chip8->pc];
	chip8->opcode = (uint16)((*pcMem << 8) | *(pcMem + 1));
	chip8->pc += 2;

	// decode && execute

	switch (chip8->opcode & 0xf000)
	{
	case 0x1000:
		chip8_op_1nnn(chip8); break;
	case 0x2000:
		chip8_op_2nnn(chip8); break;
	case 0x3000:
		chip8_op_3xkk(chip8); break;
	case 0x4000:
		chip8_op_4xkk(chip8); break;
	case 0x5000:
		chip8_op_5xy0(chip8); break;
	case 0x6000:
		chip8_op_6xkk(chip8); break;
	case 0x7000:
		chip8_op_7xkk(chip8); break;
	case 0x9000:
		chip8_op_9xy0(chip8); break;
	case 0xA000:
		chip8_op_annn(chip8); break;
	case 0xB000:
		chip8_op_bnnn(chip8); break;
	case 0xC000:
		chip8_op_cxkk(chip8); break;
	case 0xD000:
		chip8_op_dxyn(chip8); break;
	case 0x8000:
	{
		switch (chip8->opcode & 0xF00F)
		{
		case 0x8000:
			chip8_op_8xy0(chip8); break;
		case 0x8001:
			chip8_op_8xy1(chip8); break;
		case 0x8002:
			chip8_op_8xy2(chip8); break;
		case 0x8003:
			chip8_op_8xy3(chip8); break;
		case 0x8004:
			chip8_op_8xy4(chip8); break;
		case 0x8005:
			chip8_op_8xy5(chip8); break;
		case 0x8006:
			chip8_op_8xy6(chip8); break;
		case 0x8007:
			chip8_op_8xy7(chip8); break;
		case 0x800E:
			chip8_op_8xye(chip8); break;
		default: emu_assert(1); break;
		} break;
	}

	case 0x0000:
	{
		switch (chip8->opcode & 0xF0FF)
		{
		case 0x00E0:
			chip8_op_00e0(chip8); break;
		case 0x00EE:
			chip8_op_00ee(chip8); break;
		default: emu_assert(1); break;
		} break;
	}

	case 0xE000:
	{
		switch (chip8->opcode & 0xF0FF)
		{
		case 0xE0A1:
			chip8_op_exa1(chip8); break;
		case 0xE09E:
			chip8_op_ex9e(chip8); break;
		default: emu_assert(1); break;
		} break;
	}

	case 0xF000:
	{
		switch (chip8->opcode & 0xF0FF)
		{
		case 0xF007:
			chip8_op_fx07(chip8); break;
		case 0xF00A:
			chip8_op_fx0a(chip8); break;
		case 0xF015:
			chip8_op_fx15(chip8); break;
		case 0xF018:
			chip8_op_fx18(chip8); break;
		case 0xF01E:
			chip8_op_fx1e(chip8); break;
		case 0xF029:
			chip8_op_fx29(chip8); break;
		case 0xF033:
			chip8_op_fx33(chip8); break;
		case 0xF055:
			chip8_op_fx55(chip8); break;
		case 0xF065:
			chip8_op_fx65(chip8); break;
		default: emu_assert(1); break;
		} break;
	}
	default: emu_assert(1); break;
	}

	if (chip8->delayTimer > 0)
		chip8->delayTimer -= 1;

	if (chip8->soundTimer > 0)
		chip8->soundTimer -= 1;
	else
		chip8->playSoundRequested = 1;
}

byte chip8_GenerateRandomByte()
{
	return rand() % 0xff;
}

int chip8_LoadRomToMemory(const char *const file, Chip8 *const chip8)
{
	if (!file)
		return 0;

	FILE *fp = fopen(file, "rb");
	if (!fp)
		return 0;

	fseek(fp, 0L, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	fread(&chip8->memory[bROM], size, 1, fp);
	fclose(fp);

	return 1;
}
