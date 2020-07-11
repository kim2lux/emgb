#include "gpu.hpp"
#include <stdint.h>
#include "utils.h"


void Gpu::renderTile(int32_t idx, uint32_t startY, uint32_t startX)
{
	uint16_t posY = (startY / 8) * 32;
	uint16_t tileDataAddr_;
	if (unsign_ == true)
	{
		uint8_t u_tileIndex = cpu_.getMemory().read8bit(backgroudMap_ + posY + (startX / 8));
		tileDataAddr_ += u_tileIndex * 16;
		assert(tileDataAddr_ < 0x8FFF);
	}
	else
	{
		int8_t tileIndex = (int8_t)cpu_.getMemory().read8bit(backgroudMap_ + posY + (startX / 8));
		tileDataAddr_ += ((tileIndex + 0x80) * 16);
		assert(tileDataAddr_ < 0x97FF);
	}

	uint8_t positionInTile = (startY % 8) * 2;
	uint8_t upperByte = cpu_.getMemory().read8bit(tileDataAddr_ + positionInTile);
	uint8_t lowerByte = cpu_.getMemory().read8bit(tileDataAddr_ + positionInTile + 1);

	int colourBit = startX % 8;
	colourBit -= 7;
	colourBit = -colourBit;

	int colourNum = isBitSet(lowerByte, colourBit) ? 1 : 0;
	colourNum <<= 1;
	colourNum |= isBitSet(upperByte, colourBit) ? 1 : 0;

	if (colourNum != 0)
	{
		if (colourNum == 1)
			colourNum = 0x00444444;
		else if (colourNum == 2)
			colourNum = 0x00aaaaaa;
		else if (colourNum == 3)
			colourNum = 0x00000000;
		pixels_[(gameboy_width * scanline_) + idx] = colourNum;
	}
	else
		pixels_[(gameboy_width * scanline_) + idx] = 0x00FFFFFF;
}

void Gpu::updateGpuRegister()
{
	unsign_ = false;
	lcdCtrl_ = cpu_.getMemory().read8bit(LCD_DISPLAY_CTRL);
	backgroudMap_ = isBitSet(lcdCtrl_, 3) ? bg_tile_map_display_select_high : bg_tile_map_display_select_low;
	tileData_ = isBitSet(lcdCtrl_, 4) ? tile_data_addr_low : tile_data_addr_high;
	if (isBitSet(lcdCtrl_, 4))
	{
		unsign_ = true;
	}
	scanline_ = cpu_.getMemory().read8bit(LY_ADDR);

	scX_ = cpu_.getMemory().read8bit(SCX_ADDR);
	scY_ = cpu_.getMemory().read8bit(SCY_ADDR);
}

void Gpu::simpleRender()
{
	updateGpuRegister();
	uint16_t tileDataAddr_ = 0x8000; // to change
	uint16_t tileDataIndex = 0;
	unsign_ = true;// to change
	uint32_t bgmap = bg_tile_map_display_select_low;
	uint32_t color = 0x00ffffff;

	for (int yPos = 0; yPos < 144; yPos += 8)
	{
		bgmap = bg_tile_map_display_select_low + ((yPos / 8) * 0x20);
		for (int xPos = 0; xPos < 160; xPos += 8)
		{
			uint8_t u_tileIndex = cpu_.getMemory().read8bit(bgmap);
			bgmap += 1;
			tileDataIndex = tileDataAddr_ + (u_tileIndex * 16);
			for (int y = 0; y < 8; ++y)
			{
				uint8_t upperByte = cpu_.getMemory().read8bit(tileDataIndex);
				uint8_t lowerByte = cpu_.getMemory().read8bit(tileDataIndex + 1);
				color = 0x00;
				uint8_t dec = 7;
				for (int x = 0; x < 8; ++x)
				{
					color |= ((upperByte >> dec) & 0x01);
					color <<= 1;
					color |= ((lowerByte >> dec) & 0x01);
					dec--;

					if (color == 0)
						pixels_[((y + yPos) * gameboy_width) + (x + xPos)] = SDL_MapRGBA(window_surface_->format, 0xff, 0xff, 0xff, 0);
					else if (color == 1)
						pixels_[((y + yPos) * gameboy_width) + (x + xPos)] = SDL_MapRGBA(window_surface_->format, 0x44, 0x44, 0x44, 0);
					else if (color == 2)
						pixels_[((y + yPos) * gameboy_width) + (x + xPos)] = SDL_MapRGBA(window_surface_->format, 0xaa, 0xaa, 0xaa, 0);
					else if (color == 3)
						pixels_[((y + yPos) * gameboy_width) + (x + xPos)] = SDL_MapRGBA(window_surface_->format, 0, 0, 0, 0);
					
					color = 0x00;
				}
				tileDataIndex += 2;
			}
		}
		
	}
}

void Gpu::renderBackground()
{
	updateGpuRegister();
	uint32_t startY = scanline_ - scY_;
	uint32_t startX = 0;


	simpleRender();
	// for(int32_t idx = 0; idx < 160; ++idx) {
	// 	startX = idx + scX_;
	// 	//renderTile(idx, startY, startX);
	// }
}

void Gpu::rendering()
{
	uint8_t lcdCtrl = cpu_.getMemory().read8bit(LCD_DISPLAY_CTRL);
	// When Bit 0 is cleared, both background and window become blank (white),
	// and the Window Display Bit is ignored in that case. Only Sprites may still be displayed (if enabled in Bit 1).
	if (isLcdEnable())
	{
		renderBackground();
		// Window enable ?
		if (isBitSet(lcdCtrl, 5))
		{
		}
	}
	// renderSprite();
}

void Gpu::initDisplay()
{
	window_ = SDL_CreateWindow("2lux", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, gameboy_width, gameboy_height, 0);
	if (window_ == NULL)
	    LOG("SDL_CreateWindow Fail", LogStatus::CRITICAL);
	window_surface_ = SDL_GetWindowSurface(window_);
	pixels_ = (uint32_t*)window_surface_->pixels;
	std::cout << "bpp: " << (int32_t)window_surface_->format->BytesPerPixel << std::endl;
	std::cout << SDL_GetPixelFormatName(window_surface_->format->format) << std::endl;
}

void Gpu::render()
{
	SDL_UpdateWindowSurface(window_);
}

#ifdef DEAD_CODE

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
					pixels_d[(256 * (posy + y)) + posx + x] = color;
				}
				else
					pixels_d[(256 * (posy + y)) + posx + x] = 0x00ffffff;
				dec--;
			}
			tmpaddr += 2;
		}
		if ((posx + 8) >= (8 * 16))
		{
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
					pixels[(256 * (s_gb->gb_io.scrollY + posy + y)) + (s_gb->gb_io.scrollX + posx + x)] = color * 0xff;
				}
				dec--;
			}
			tmpaddr += 2;
		}
		if ((posx + 8) > 124)
		{
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
				if ((160 * (posy + y) + (posx + x)) < (160 * 144)) //check mem coruption error -> need to refactor this
					pixels[160 * (posy + y) + (posx + x)] = color;
				dec--;
			}
			tmpaddr += 2;
		}
	}
}

void initGpu(struct s_gb *s_gb)
{
	scanline = 0;
	tick = 0;
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
		scanline = 0;
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

	tick += (s_gb->gb_cpu.totalTick - lastTick); //FOR TEST !!! - lastTick;
	lastTick = s_gb->gb_cpu.totalTick;

	switch (gpuMode)
	{
	case HBLANK:
		if (tick >= 204)
		{
			scanline += 1;

			if (scanline >= 144)
			{
				//		if (gb_s->gb_interrupts.interEnable & INT_VBLANK)
				s_gb->gb_interrupts.interFlag |= INT_VBLANK;
				gpuMode = VBLANK;
			}
			else if (s_gb->gb_io.lcd.LcdIsOn == 1)
				rendering(s_gb);

			tick -= 204;
		}
		break;
	case VBLANK:
		////
		if (tick >= 456)
		{
			scanline += 1;

			if (scanline >= 153)
			{
				scanline = 0;
				gpuMode = OAM;
			}
			tick -= 456;
		}
		break;
	case OAM:
		////
		if (tick >= 80)
		{

			scanline = 0;
			gpuMode = VRAM;
			tick -= 80;
		}
		break;
	case VRAM:
		if (tick >= 172)
		{

			gpuMode = HBLANK;
			tick -= 172;
		}
		break;
	}
}
#endif