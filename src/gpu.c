#include "io.h"
#include "GB.h"

#define GB_W 160
#define GB_H 144

void initDisplay(struct s_gb *s_gb)
{
	SDL_Init(SDL_INIT_VIDEO);
	
	s_gb->gb_gpu.window = SDL_CreateWindow("GB",
		300, 300, GB_W, GB_H, 0);
	if (s_gb->gb_gpu.window == NULL)
		ERR("cannot create SDL windows");
	s_gb->gb_gpu.renderer = SDL_CreateRenderer(s_gb->gb_gpu.window, -1, SDL_RENDERER_TARGETTEXTURE);
	if (s_gb->gb_gpu.renderer == NULL)
		ERR("cannot create SDL renderer");
	s_gb->gb_gpu.texture = SDL_CreateTexture(s_gb->gb_gpu.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, GB_W, GB_H);
	if (s_gb->gb_gpu.texture == NULL)
		ERR("cannot create SDL texture");
	s_gb->gb_gpu.pixels = malloc(sizeof(Uint32) * GB_W * GB_H);
	if (s_gb->gb_gpu.pixels == NULL)
		ERR("cannot alloc pixels");
	
	s_gb->gb_gpu.window_d = SDL_CreateWindow("DEBUG",
		300, 600, 256, 256, 0);
	if (s_gb->gb_gpu.window_d == NULL)
		ERR("cannot create SDL windows");
	s_gb->gb_gpu.renderer_d = SDL_CreateRenderer(s_gb->gb_gpu.window_d, -1, SDL_RENDERER_TARGETTEXTURE);
	if (s_gb->gb_gpu.renderer_d == NULL)
		ERR("cannot create SDL renderer");
	s_gb->gb_gpu.texture_d = SDL_CreateTexture(s_gb->gb_gpu.renderer_d, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 256);
	if (s_gb->gb_gpu.texture_d == NULL)
		ERR("cannot create SDL texture");
	s_gb->gb_gpu.pixels_d = malloc(sizeof(Uint32) * 256 * 256);
	if (s_gb->gb_gpu.pixels_d == NULL)
		ERR("cannot alloc pixels");
		
}

void displayAll(struct s_gb *s_gb)
{
	int y, x = 0;
	unsigned short line;
	int color = 0;
	int dec;
	int posx = 0;
	int posy = 0;

	for (int index = 0x8000; index < 0x9800; index += 0x10)
	{
	
		int tmpaddr = index;
		for (y = 0; y < 8; y++)
		{
			dec = 15;
			line = read16bit(tmpaddr, s_gb);

			for (x = 0; x < 8; x++)
			{
				color = ((line >> dec) & 0x01);
				if ((line >> (dec - 8)) & 0x01)
					color += 2;
				if (color != 0)
				{
					if (color == 1)
						color = 0x00444444;
					else if (color == 2)
						color = 0x00aaaaaa;
					else if (color == 3)
						color = 0x00000000;
					s_gb->gb_gpu.pixels_d[(256 * (posy + y)) + posx + x] = color;
				}
				else
					s_gb->gb_gpu.pixels_d[(256 * (posy + y)) + posx + x] = 0x00ffffff;
				dec--;
			}
			tmpaddr += 2;
		}
		if ((posx + 8) >= (8*16)) {
			posy += 8;
			posx = 0;
		}
		else
			posx += 8;
	}
}

void renderingWindow(struct s_gb *s_gb)
{
	int y, x = 0;
	unsigned short line;
	int color = 0;
	int dec;
	int posx = 0;
	int posy = 0;

	unsigned short limit = s_gb->gb_io.lcd.WindowTileMapSelect + 0x400;

	int baseaddr = s_gb->gb_io.lcd.BgWindowTileData;

	for (int index = s_gb->gb_io.lcd.WindowTileMapSelect; index < limit; index += 0x1)
	{
		unsigned char tileindex = read8bit(index, s_gb);
		int tmpaddr = baseaddr + (tileindex * 16);
		for (y = 0; y < 8; y++)
		{
			dec = 15;
			line = read16bit(tmpaddr, s_gb);

			for (x = 0; x < 8; x++)
			{
				color = ((line >> dec) & 0x01) + ((line >> (dec - 8)) & 0x01);
				if (color != 0)
				{
					s_gb->gb_gpu.pixels[(256 * (s_gb->gb_io.scrollY + posy + y)) + (s_gb->gb_io.scrollX + posx + x)] = color * 0xff;
				}
				dec--;

			}
			tmpaddr += 2;
		}
		if ((posx + 8) > 124) {
			posy += 8;
			posx = 0;
		}
		else
			posx += 8;
	}
}

void renderingSprite(struct s_gb *s_gb)
{
	int y, x = 0;
	unsigned short line;
	int color = 0;
	int dec;

	unsigned short limit = 0xFE9F;
	
	int baseaddr = 0x8000;


	for (int index = 0xFE00; index < limit; index += 4)
	{

		unsigned char posy = s_gb->gb_mem.oam[index - 0xFE00] - 16;
		unsigned char posx = s_gb->gb_mem.oam[index + 1 - 0xFE00] - 8;
		unsigned char tileindex = s_gb->gb_mem.oam[index + 2 - 0xFE00];


		int tmpaddr = baseaddr + (tileindex * 16);

		for (y = 0; (tileindex && y < s_gb->gb_io.lcd.SpriteSize); y++)
		{
			dec = 15;
			line = read16bit(tmpaddr, s_gb);

			for (x = 0; x < 8; x++)
			{
				
				color = ((line >> dec) & 0x01);
				if ((line >> (dec - 8)) & 0x01)
					color += 2;
				if (color == 0)
					color = 0x00ffffff;
				else if (color == 1)
					color = 0x00444444;
				else if (color == 2)
					color = 0x00aaaaaa;
				else if (color == 3)
					color = 0x00000000;
				if ((160 * (posy + y) + (posx + x)) < (160 *  144))  //check mem coruption error -> need to refactor this
					s_gb->gb_gpu.pixels[160 * (posy + y ) + (posx + x)] = color;
				dec--;
	

			}
			tmpaddr += 2;
		}
	}
}

void rendering(struct s_gb *s_gb)
{
	int pitch = 0;
	void *pixels;

	//memset(s_gb->gb_gpu.pixels, 0, 160 * 144 * sizeof(Uint32));
		SDL_RenderClear(s_gb->gb_gpu.renderer);
	SDL_LockTexture(s_gb->gb_gpu.texture, NULL, &pixels, &pitch);
	memcpy(s_gb->gb_gpu.pixels, pixels , 160 * 144 * 4);

	
	if (s_gb->gb_io.lcd.BgWindowDisplay == 1)
		renderingBg(s_gb);
	/*if (s_gb->gb_io.lcd.WindowIsOn == 1)
		renderingWindow(s_gb);*/
	if (s_gb->gb_io.lcd.SpriteIsOn == 1)
		renderingSprite(s_gb);
	memcpy(pixels, s_gb->gb_gpu.pixels, 160 * 144 * 4);
	SDL_UnlockTexture(s_gb->gb_gpu.texture);
	
	SDL_RenderCopy(s_gb->gb_gpu.renderer, s_gb->gb_gpu.texture, NULL, NULL);
	SDL_RenderPresent(s_gb->gb_gpu.renderer);

	// step 2 debug

	memset(s_gb->gb_gpu.pixels_d, 0x00ff0000, 256 * 256 * sizeof(Uint32));
	SDL_RenderClear(s_gb->gb_gpu.renderer_d);
	SDL_LockTexture(s_gb->gb_gpu.texture_d, NULL, &pixels, &pitch);
	memcpy(s_gb->gb_gpu.pixels_d, pixels, 256 * 256 * 4);

	displayAll(s_gb);
	memcpy(pixels, s_gb->gb_gpu.pixels_d, 256 * 256 * 4);
	SDL_UnlockTexture(s_gb->gb_gpu.texture_d);

	SDL_RenderCopy(s_gb->gb_gpu.renderer_d, s_gb->gb_gpu.texture_d, NULL, NULL);
	SDL_RenderPresent(s_gb->gb_gpu.renderer_d);
	
}

void initGpu(struct s_gb *s_gb)
{
	s_gb->gb_gpu.scanline = 0;
	
	s_gb->gb_gpu.tick = 0;
}

char lcdIsEnable(unsigned char lcdc)
{
	return ((lcdc & 0x01) == 0) ? 0 : 1;
}

void setLcdStatus(struct s_gb *s_gb)
{
	if (lcdIsEnable(read8bit(0xff40, s_gb) >> 7) == 0)
	{
		//s_gb->gb_cpu.totalTick = 0;
		s_gb->gb_gpu.scanline = 0;
		printf("reset scanline & s_gb->gb_cpu.totalTick\n");
		
		write8bit(0xff41, 253, s_gb);
		return;
	}
	/*
	curmode = status & 0x03;
	*/
}
void updateGpu(struct s_gb *s_gb)
{
	static unsigned int lastTick = 0;

	s_gb->gb_gpu.tick += (s_gb->gb_cpu.totalTick - lastTick); //FOR TEST !!! - lastTick;
	lastTick = s_gb->gb_cpu.totalTick;

	switch (s_gb->gb_gpu.gpuMode)
	{
	case HBLANK:
		if (s_gb->gb_gpu.tick >= 204)
		{
			s_gb->gb_gpu.scanline += 1;
			
			if (s_gb->gb_gpu.scanline >= 144)
			{
		//		if (gb_s->gb_interrupts.interEnable & INT_VBLANK)
				s_gb->gb_interrupts.interFlag |= INT_VBLANK;
				s_gb->gb_gpu.gpuMode = VBLANK;
			}
			else
				if (s_gb->gb_io.lcd.LcdIsOn == 1)
					rendering(s_gb);

			s_gb->gb_gpu.tick -= 204;
		}
		break;
	case VBLANK:
		////
		if (s_gb->gb_gpu.tick >= 456)
		{
			s_gb->gb_gpu.scanline += 1;
			
			if (s_gb->gb_gpu.scanline >= 153)
			{
				s_gb->gb_gpu.scanline = 0;
				s_gb->gb_gpu.gpuMode = OAM;
			}
			s_gb->gb_gpu.tick -= 456;
		}
		break;
	case OAM:
		////
		if (s_gb->gb_gpu.tick >= 80)
		{
		
			s_gb->gb_gpu.scanline = 0;
			s_gb->gb_gpu.gpuMode = VRAM;
			s_gb->gb_gpu.tick -= 80;
		}
		break;
	case VRAM:
		if (s_gb->gb_gpu.tick >= 172)
		{
			
			s_gb->gb_gpu.gpuMode = HBLANK;
			s_gb->gb_gpu.tick -= 172;
		}
		break;
	}
}
