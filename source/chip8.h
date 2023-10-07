#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "types.h"
#include "stack.h"

enum MemorySegments
{
	bInterpreter = 0x000,
	eInterpreter = 0x1FF,
	bFont = 0x050,
	eFont = 0x0A0,
	bROM = 0x200,
	eROM = 0xFFF
};

#define VideoMemWidth 64
#define VideoMemHeight 32

typedef struct Chip8
{
	byte memory[4096];
	uint32 videoMem[VideoMemWidth * VideoMemHeight];
	byte registers[16];
	Stack stack;
	uint16 pc;
	uint16 index;
	uint16 opcode;
	byte keypad[16];
	byte delayTimer;
	byte soundTimer;

	int screenUpdateRequested;
	int playSoundRequested;
} Chip8;


void chip8_Alloc(const Chip8 **const chip8);
void chip8_Dealloc(const Chip8 **const chip8);
void chip8_Init(Chip8 *const chip8);
void chip8_Reboot(Chip8 *const chip8);
void chip8_Cycle(Chip8 *const chip8);
byte chip8_GenerateRandomByte();
int chip8_LoadRomToMemory(const char *const file, Chip8 *const chip8);

#endif
