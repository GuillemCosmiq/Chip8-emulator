#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "types.h"

typedef struct Chip8 Chip8;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Texture SDL_Texture;

typedef struct Device
{
	SDL_Renderer *renderer;
	SDL_Window *window ;
	SDL_Texture *texture;
	uint16 capFrameMs;
	int scale;
	int stop;
} Device;

void device_Alloc(const Device **const device);
void device_Dealloc(const Device **const device);
void device_Init(Device *const device);
void device_Destroy(Device* const device);
void device_HandleInput(Device *const device, Chip8 *const chip8);
void device_Draw(Device *const device, Chip8 *const chip8);
void device_PlaySound(Device* const device, Chip8* const chip8);

#endif
