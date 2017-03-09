#ifndef __JOYPAD__
#define __JOYPAD__
struct s_gb;
void handleEvent(struct s_gb * gb_s);
void keyUp(struct s_gb * gb_s);
void keyDown(struct s_gb * gb_s);

struct				s_joypad
{
	unsigned char	key; // = 0xff;
	unsigned char	button_key; // = 0x0f;
	unsigned char	button_dir; // = 0x0f;
};
#endif
