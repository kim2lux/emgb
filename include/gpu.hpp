#pragma once
#include <SDL.h>
#include "cpu.hpp"
#include "utils.h"

static constexpr uint16_t LCD_DISPLAY_CTRL = 0xff40;
static constexpr uint16_t LCDC_STATUS_ADDR = 0xff41;

static constexpr uint16_t SCY_ADDR = 0xff42;
static constexpr uint16_t SCX_ADDR = 0xff43;

static constexpr uint16_t WINY_ADDR = 0xff4a;
static constexpr uint16_t WINX_ADDR = 0xff4b;

static constexpr uint16_t LY_ADDR = 0xff44;
static constexpr uint16_t LYC_ADDR = 0xff45;

const int MAX_VIDEO_CYCLES = 456;
const int MIN_HBLANK_MODE_CYCLES = 204;
const int OAM_MODE_CYCLES = 80;
const int MIN_LCD_TRANSFER_CYCLES = 172;
const int VBLANK_CYCLES = 4560;
static constexpr uint8_t VERTICAL_BLANK_SCAN_LINE = 144;
static constexpr uint8_t VERTICAL_BLANK_SCAN_LINE_MAX = 153;

enum GpuMode
{
	H_BLANK = 0,
	V_BLANK = 1,
	OAM = 2,
	LCD_TX = 3,
};

enum LcdCTrl : uint8_t
{
	LCD_DISPLAY = 7,
	WIN_TILE_MAP_DISPLAY = 6,
	WIN_DISPLAY = 5,
	BG_WIN_TILE_DATA_SELECT = 4,
	BG_TILE_MAP_DISPLAY = 3,
	OBJ_SIZE = 2,
	OBJ_DISPLAY = 1,
	BG_WIN_PRIORITY = 0,
};

class Gpu
{
public:
	Gpu(Z80Cpu &cpu) : cpu_(cpu)
	{
		gpuMode_ = GpuMode::H_BLANK;
		lcdEnable_ = true;
		gpuCycle_ = 0;
		scanline_ = 0;
	}

	void isLcdEnable() {
        uint8_t lcdCtrl = cpu_.getMemory().read8bit(LCD_DISPLAY_CTRL);
		if (isBitSet(lcdCtrl, LcdCTrl::LCD_DISPLAY)) {
			lcdEnable_ = true;
		}
	}

	void updateGpu() {
		//std::cout << "scanline: " << std::dec << scanline_ <<  std::endl;
		if (lcdEnable_ == true) {
			gpuCycle_ += cpu_.tickCount_;
			switch (gpuMode_) {
				case GpuMode::H_BLANK:
				hblank(); break;
				case GpuMode::V_BLANK:
				vblank(); break;
				case GpuMode::OAM:
				oam(); break;
				case GpuMode::LCD_TX:
				lcdTransfert(); break;
			}
		}
	}

	void compareScanline() {
		scanline_ = cpu_.getMemory().read8bit(LY_ADDR);
		uint8_t scanlineCompare = cpu_.getMemory().read8bit(LYC_ADDR);
		uint8_t lcdStatus = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);

		if (scanline_ == scanlineCompare) {
			setBit(lcdStatus, 2); //set coincidence flag
			if (isBitSet(lcdStatus, 6)) {  // request interrupt on coincidence flag ?
			    cpu_.requestInterrupt(InterruptType::LCDC);
			}
		}
		else {
			clearBit(lcdStatus, 2);
		}
		cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcdStatus);
	}

	void hblank() {
		if (gpuCycle_ >= MIN_HBLANK_MODE_CYCLES) {
			gpuCycle_ -= MIN_HBLANK_MODE_CYCLES;

			scanline_ = cpu_.getMemory().read8bit(LY_ADDR);
			cpu_.getMemory().write8bit(LY_ADDR, ++scanline_);
			compareScanline();
			if (scanline_ >= VERTICAL_BLANK_SCAN_LINE) {
				gpuMode_ = GpuMode::V_BLANK;

				//set lcd status to vblank
				uint8_t lcd = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
				setBit(lcd, 0);
				cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcd);

                //request vblank interrupt
				cpu_.requestInterrupt(InterruptType::VBLANCK);
				// request interrupt on hblank flag ?
				if (isBitSet(lcd, 4)) { 
					cpu_.requestInterrupt(InterruptType::LCDC);
				}
			}
			else {
				gpuMode_ = GpuMode::OAM;
				uint8_t lcd = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
				setBit(lcd, 1);
				clearBit(lcd, 0);
				cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcd);
                // request interrupt on OAM flag ?
				if (isBitSet(lcd, 5)) { 
					cpu_.requestInterrupt(InterruptType::LCDC);
				}
			}
		}
	}

	void vblank() {
		vblanckCycle_ += cpu_.tickCount_;

        scanline_ = cpu_.getMemory().read8bit(LY_ADDR);
        // keep inc scanline and call LCD interrupt
		if (vblanckCycle_ >= MAX_VIDEO_CYCLES) {
			vblanckCycle_ = 0;
			if (scanline_ < VERTICAL_BLANK_SCAN_LINE_MAX) {
				cpu_.getMemory().write8bit(LY_ADDR, ++scanline_);
			}
			compareScanline();
		}

        if (gpuCycle_ >= VBLANK_CYCLES) {
			vblanckCycle_ = 0;
		    gpuCycle_ = 0;
			gpuMode_ = GpuMode::OAM;
			uint8_t lcd = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);

			cpu_.getMemory().write8bit(LY_ADDR, 0);
			compareScanline();
			setBit(lcd, 1); //set OAM mode
			clearBit(lcd, 0); //clear vblank
			// request interrupt on OAM flag ?
			if (isBitSet(lcd, 5)) {
				cpu_.requestInterrupt(InterruptType::LCDC);
			}
			cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcd);

		}
	}

	void oam()
	{
		if (gpuCycle_ >= OAM_MODE_CYCLES) {
			gpuCycle_ -= OAM_MODE_CYCLES;

			gpuMode_ =  GpuMode::LCD_TX;
			uint8_t lcd = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
			setBit(lcd, 0);
			setBit(lcd, 1);
			cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcd);
		}
	}

	void lcdTransfert()
	{
		if (gpuCycle_ >= MIN_LCD_TRANSFER_CYCLES) {
			gpuCycle_ -= MIN_LCD_TRANSFER_CYCLES;

			gpuMode_ = GpuMode::H_BLANK;

			//display rendering

			uint8_t lcd = cpu_.getMemory().read8bit(LCDC_STATUS_ADDR);
			clearBit(lcd, 0);
			clearBit(lcd, 1);
			cpu_.getMemory().write8bit(LCDC_STATUS_ADDR, lcd);
			if (isBitSet(lcd, 3)) {
				cpu_.requestInterrupt(InterruptType::LCDC);
			}
		}
	}

	private:
		Z80Cpu &cpu_;
		SDL_Window *window;
		SDL_Surface *screenSurface;
		SDL_Texture *texture;
		SDL_Renderer *renderer;
		unsigned int *pixels;

		SDL_Window *window_d;
		SDL_Surface *screenSurface_d;
		SDL_Texture *texture_d;
		SDL_Renderer *renderer_d;
		unsigned int *pixels_d;

		bool lcdEnable_ = true;
		uint32_t gpuCycle_ = 0;
		enum GpuMode gpuMode_;
		uint16_t scanline_ = 0;
		uint32_t vblanckCycle_ = 0;
	};
