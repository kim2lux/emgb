#ifndef __CPU__
#define __CPU__

struct s_gb;
void initCpu(struct s_gb *s_gb);
typedef void (*cpufunc)(struct s_gb *);

struct s_cpu_z80
{
  unsigned char opcode;
  char *value;
  cpufunc func;
  unsigned char size;
};

#pragma pack(push, 1)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

struct s_register
{
  struct
  {
    union
    {
      struct
      {
        unsigned char f;
        unsigned char a;
      };
      unsigned short af;
    };
  };
  struct
  {
    union
    {
      struct
      {
        unsigned char c;
        unsigned char b;
      };
      unsigned short bc;
    };
  };

  struct
  {
    union
    {
      struct
      {
        unsigned char e;
        unsigned char d;
      };
      unsigned short de;
    };
  };

  struct
  {
    union
    {
      struct
      {
        unsigned char l;
        unsigned char h;
      };
      unsigned short hl;
    };
  };

  unsigned short pc;
  unsigned short sp;
};
#pragma GCC diagnostic pop
struct s_cpu
{
  struct s_cpu_z80 gb_cpu_z80[256];
  unsigned int totalTick;
  unsigned char stopCpu;
  unsigned char jmpf;
};

#pragma pack(pop)

#define SET_ZERO() cpu.regs_.f |= 0x80
#define CLEAR_ZERO() cpu.regs_.f &= ~(0x80)

#define SET_NEG() cpu.regs_.f |= 0x40
#define CLEAR_NEG() cpu.regs_.f &= ~(0x40)

#define SET_HALFC() cpu.regs_.f |= 0x20
#define CLEAR_HALFC() cpu.regs_.f &= ~(0x20)

#define SET_CARRY() cpu.regs_.f |= 0x10
#define CLEAR_CARRY() cpu.regs_.f &= ~(0x10)

#endif
