#ifndef INTERUPT
#define INTERUPT


struct s_gb;
void doInterupt(struct s_gb * gb_s);
void timer(struct s_gb * gb_s);
void serial(struct s_gb * gb_s);
void joypad(struct s_gb * gb_s);
void lcd(struct s_gb * gb_s);
void vblanck(struct s_gb * gb_s);

#define INT_VBLANK (1 << 0)
#define INT_LCDSTAT (1 << 1)
#define INT_TIMER (1 << 2)
#define INT_SERIAL (1 << 3)
#define INT_JOYPAD (1 << 4)

struct				s_interupt 
{
	unsigned char	interMaster;
	unsigned char	interFlag;
	unsigned char	interEnable;
};

#endif
