#ifndef __TIMER__
#define __TIMER__


struct s_gb;
void initTimer(struct s_gb *s_gb);
void updateTimer(struct s_gb *s_gb);

struct				s_timer
{
  unsigned int	freq;
  unsigned char	ctrl;
  unsigned char	modulator;
  int				timerCount;
};

#endif
