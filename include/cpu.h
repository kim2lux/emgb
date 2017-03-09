#ifndef __CPU__
#define __CPU__

struct s_gb;
void initCpu(struct s_gb * s_gb);
typedef void (*cpufunc)(struct s_gb*);

struct		s_cpu_z80
{
  unsigned char	opcode;
  char	       	*value;
  cpufunc       func;
  unsigned char	size;
};

#pragma pack(push, 1)

struct s_register
{
  struct {
    union  {
      struct {
	unsigned char f;
	unsigned char a;
      };
			unsigned short af;
    };
  };
  struct {
    union {
      struct {
	unsigned char c;
	unsigned char b;
      };
      unsigned short bc;
    };
  };
  
  struct {
    union {
      struct {
	unsigned char e;
	unsigned char d;
      };
      unsigned short de;
    };
  };
  
  struct {
    union {
      struct {
	unsigned char l;
	unsigned char h;
      };
      unsigned short hl;
    };
  };

  unsigned short	pc;
  unsigned short	sp;
  //unsigned char	flags;
};

struct	       			s_cpu {
	struct	s_cpu_z80	gb_cpu_z80[256];
	unsigned int		totalTick;
	unsigned char		stopCpu;
	unsigned char		jmpf;
};

#pragma pack(pop)


#define SET_ZERO() s_gb->gb_register.f |= 0x80
#define CLEAR_ZERO() s_gb->gb_register.f &= ~(0x80)


#define SET_NEG() s_gb->gb_register.f |= 0x40
#define CLEAR_NEG() s_gb->gb_register.f &= ~(0x40)


#define SET_HALFC() s_gb->gb_register.f |= 0x20
#define CLEAR_HALFC() s_gb->gb_register.f &= ~(0x20)


#define SET_CARRY() s_gb->gb_register.f |= 0x10
#define CLEAR_CARRY() s_gb->gb_register.f &= ~(0x10)


#define ZERO_FLAG (s_gb->gb_register.f & 0x80)
#define CARRY_FLAG (s_gb->gb_register.f & 0x10)
#define NEG_FLAG (s_gb->gb_register.f & 0x40)
#define HALFC_FLAG (s_gb->gb_register.f & 0x20)

#endif
