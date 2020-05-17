#include "device.h"
#include "chip8.h"
#include "../extern/SDL/include/SDL.h"

void device_Alloc(const Device **const device)
{
	*device = (Device *const)malloc(sizeof(Device));
}

void device_Dealloc(const Device **const device)
{
	free(*device);
}

void device_Init(Device *const device)
{
	device->stop = 0;
	device->scale = 10;
	device->capFrameMs = 3;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
	SDL_CreateWindowAndRenderer(VideoMemWidth * device->scale, VideoMemHeight * device->scale, 0, &device->window, &device->renderer);
	SDL_SetWindowTitle(device->window, "Chip8 emulator by Guillem Costa");
	SDL_SetWindowSize(device->window, VideoMemWidth * device->scale, VideoMemHeight * device->scale);
	SDL_SetWindowResizable(device->window, SDL_TRUE);
	device->texture = SDL_CreateTexture(device->renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, VideoMemWidth, VideoMemHeight);
}

void device_Destroy(Device *const device)
{
	SDL_DestroyRenderer(device->renderer);
	SDL_DestroyWindow(device->window);
	SDL_DestroyTexture(device->texture);
	SDL_Quit();
}

void SetKeyState(Chip8 *const chip8, SDL_Keycode key, int keyState)
{
	switch (key)
	{
		// row 1:
	case SDLK_4: chip8->keypad[1] = keyState; break;
	case SDLK_5: chip8->keypad[2] = keyState; break;
	case SDLK_6: chip8->keypad[3] = keyState; break;
	case SDLK_7: chip8->keypad[0xc] = keyState; break;
		// row 2:
	case SDLK_r: chip8->keypad[4] = keyState; break;
	case SDLK_t: chip8->keypad[5] = keyState; break;
	case SDLK_y: chip8->keypad[6] = keyState; break;
	case SDLK_u: chip8->keypad[0xd] = keyState; break;
		// row 3:
	case SDLK_f: chip8->keypad[7] = keyState; break;
	case SDLK_g: chip8->keypad[8] = keyState; break;
	case SDLK_h: chip8->keypad[9] = keyState; break;
	case SDLK_j: chip8->keypad[0xe] = keyState; break;
		// row 4:
	case SDLK_v: chip8->keypad[0xa] = keyState; break;
	case SDLK_b: chip8->keypad[0] = keyState; break;
	case SDLK_n: chip8->keypad[0xb] = keyState; break;
	case SDLK_m: chip8->keypad[0xf] = keyState; break;
	}
}

void device_HandleInput(Device *const device, Chip8 *const chip8)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			device->stop = 1;
			break;
		case (SDL_DROPFILE):
		{
			const char* const droppedFileDir = event.drop.file;
			chip8_Reboot(chip8);
			chip8_LoadRomToMemory(droppedFileDir, chip8);
			SDL_free(droppedFileDir);
			break;
		}
		case SDL_KEYDOWN:
		{
			SetKeyState(chip8, event.key.keysym.sym, 1);
			break;
		}
		case SDL_KEYUP:
		{
			SetKeyState(chip8, event.key.keysym.sym, 0);
			break;
		}
		}
	}
}

void device_Draw(Device *const device, Chip8 *const chip8)
{
	SDL_UpdateTexture(device->texture, NULL, chip8->videoMem, sizeof(chip8->videoMem[0]) * VideoMemWidth);
	SDL_RenderClear(device->renderer);
	SDL_RenderCopy(device->renderer, device->texture, NULL, NULL);
	SDL_RenderPresent(device->renderer);
	chip8->screenUpdateRequested = 0;
}

void device_PlaySound(Device* const device, Chip8* const chip8)
{
	chip8->playSoundRequested = 0;
	// TODO: Add audio support.
}
