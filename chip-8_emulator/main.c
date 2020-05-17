#include "device.h"
#include "chip8.h"
#include "../extern/SDL/include/SDL.h"
#include <stdio.h>
#include <stdlib.h>

void ParseCommandArguments(Device* const, Chip8* const, int, char*[]);

int main(int argc, char* argv[])
{
	Device *device = NULL;
	Chip8 *chip8 = NULL;

	device_Alloc(&device);
	chip8_Alloc(&chip8);
	device_Init(device);
	chip8_Init(chip8);

	ParseCommandArguments(device, chip8, argc, argv);

	for (;!device->stop;)
	{
		uint32 startFrameTime = SDL_GetTicks();

		device_HandleInput(device, chip8);
		chip8_Cycle(chip8);

		uint32 delta = SDL_GetTicks() - startFrameTime;
		if (delta < device->capFrameMs)
			SDL_Delay(device->capFrameMs - delta);

		if (chip8->playSoundRequested)
			device_PlaySound(device, chip8);

		if (chip8->screenUpdateRequested)
			device_Draw(device, chip8);
	}

	chip8_Dealloc(&chip8);
	device_Destroy(device);
	device_Dealloc(&device);

	return 0;
}

void ParseCommandArguments(Device *const device, Chip8 *const chip8, int argc, char* argv[])
{
	if (argc < 2 && !chip8_LoadRomToMemory(argv[1], chip8) && !chip8_LoadRomToMemory("../games/Pong [Paul Vervalin, 1990].ch8", chip8))
	{
		printf("Cannot load any rom...\n Exiting emulator.");
		device->stop = 1;
	}
	if (argc >= 3)
	{
		int argvAsInt = atoi(argv[2]);
		if (argvAsInt)
			device->scale = atoi(argv[2]);
		else
			device->scale = 3;
	}
	else
		device->scale = 10;
	if (argc >= 4)
	{
		int argvAsInt = atoi(argv[3]);
		if (argvAsInt)
			device->capFrameMs = atoi(argv[3]);
		else
			device->capFrameMs = 3;
	}
	else
		device->capFrameMs = 3;
}