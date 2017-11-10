#include "GB.h"
#include "cpu.h"


unsigned short read16(struct s_gb *s_gb)
{
  unsigned short value;
#ifdef GB_DEBUG
  //printf("READ 16 :: pc value %x %x\n", s_rom->rom[s_gb->gb_register.pc], s_rom->rom[s_gb->gb_register.pc + 1]);
#endif
  value = read8bit(s_gb->gb_register.pc + 1, s_gb) << 8;
  value |= read8bit(s_gb->gb_register.pc, s_gb);

  return value;
}

unsigned char read8(struct s_gb *s_gb)
{
	return read8bit(s_gb->gb_register.pc, s_gb);
}

void nop(struct s_gb *s_gb)
{
  s_gb->gb_cpu.totalTick += 4;
}

//jump
void jp_addr_16(struct s_gb *s_gb)
{
  s_gb->gb_cpu.totalTick += 16;
  s_gb->gb_register.pc = read16bit(s_gb->gb_register.pc, s_gb);
  s_gb->gb_cpu.jmpf = 1;
}

void jp_nz_16(struct s_gb *s_gb)
{
  if (ZERO_FLAG) {
    s_gb->gb_cpu.totalTick += 12;
  }
  else
    {
      s_gb->gb_cpu.totalTick += 16;
      s_gb->gb_cpu.jmpf = 1;
      s_gb->gb_register.pc = read16bit(s_gb->gb_register.pc, s_gb);
    }
  
}

void jp_nc_16(struct s_gb *s_gb)
{
	if (CARRY_FLAG) {
		s_gb->gb_cpu.totalTick += 12;
	}
	else
	{
		s_gb->gb_cpu.totalTick += 16;
		s_gb->gb_cpu.jmpf = 1;
		s_gb->gb_register.pc = read16bit(s_gb->gb_register.pc, s_gb);
	}

}

void jp_c_16(struct s_gb *s_gb)
{
	if (CARRY_FLAG == 0) {
		s_gb->gb_cpu.totalTick += 12;
	}
	else
	{
		s_gb->gb_cpu.totalTick += 16;
		s_gb->gb_cpu.jmpf = 1;
		s_gb->gb_register.pc = read16bit(s_gb->gb_register.pc, s_gb);
	}

}

void jp_addr_8(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 8;
	s_gb->gb_register.pc = read8(s_gb);
	s_gb->gb_cpu.jmpf = 1;
}

void jr_nz(struct s_gb *s_gb)
{

	if (ZERO_FLAG == 0)
	{
		s_gb->gb_cpu.totalTick += 12;

//		//prvoidf("\nbefore value = %x jump() %x\n", s_gb->gb_register.pc, (signed char)read8());
		s_gb->gb_register.pc = (s_gb->gb_register.pc + 1) + (signed char) read8bit(s_gb->gb_register.pc, s_gb);
		////prvoidf("\nafter value = %x\n", s_gb->gb_register.pc);
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 8;
//	//prvoidf("jump to %x\n", s_gb->gb_register.pc);
}


//load 



void ldi_a_hl_addr(struct s_gb *s_gb) { s_gb->gb_register.a = read8bit(s_gb->gb_register.hl, s_gb); s_gb->gb_register.hl += 1; }//2a
void ldd_a_hl_addr(struct s_gb *s_gb) { s_gb->gb_register.a = read8bit(s_gb->gb_register.hl, s_gb); s_gb->gb_register.hl -= 1; }//3a
void ld_a_de_addr(struct s_gb *s_gb) { s_gb->gb_register.a = read8bit(s_gb->gb_register.de, s_gb); }//1a
void ld_a_bc_addr(struct s_gb *s_gb) { s_gb->gb_register.a = read8bit(s_gb->gb_register.bc, s_gb); }//0a

void ld_addr_a_16(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 16; write8bit(read16bit(s_gb->gb_register.pc, s_gb), s_gb->gb_register.a, s_gb); }
void ld_hl_val_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 12; write8bit(s_gb->gb_register.hl, read8(s_gb), s_gb); }
void ld_b_val_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.b = read8(s_gb); }
void ld_d_val_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.d = read8(s_gb); }
void ld_c_val_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.c = read8(s_gb); }
void ld_a_val_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.a = read8(s_gb); }
void ld_l_val_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.l = read8(s_gb); }
//write a to addr
void ld_bc_addr_a(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; write8bit(s_gb->gb_register.bc, s_gb->gb_register.a, s_gb); }
void ld_de_addr_a(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; write8bit(s_gb->gb_register.de, s_gb->gb_register.a, s_gb); }
void ldd_hl_a_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; write8bit(s_gb->gb_register.hl, s_gb->gb_register.a, s_gb); s_gb->gb_register.hl -= 1; }
void ldi_hl_addr_a(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; write8bit(s_gb->gb_register.hl, s_gb->gb_register.a, s_gb); s_gb->gb_register.hl += 1; }
//e0
void ldd_addr_a_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 12; ; write8bit(0xFF00 + read8(s_gb), s_gb->gb_register.a, s_gb); }
void ldh_a_val_8(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 12; s_gb->gb_register.a = read8bit(0xff00 + read8bit(s_gb->gb_register.pc, s_gb), s_gb); }

//dec 
unsigned short dec16(unsigned short value, struct s_gb *s_gb)
{
	if (value & 0x0f) CLEAR_HALFC();
	else SET_HALFC();
	value -= 1;

	if (value) CLEAR_ZERO();
	else SET_ZERO();

	SET_NEG();
	return value;
}

unsigned char dec8(unsigned char value, struct s_gb *s_gb)
{
	if (value & 0x0f) CLEAR_HALFC();
	else SET_HALFC();
	value -= 1;

	if (value) CLEAR_ZERO();
	else SET_ZERO();

	SET_NEG();
	return value;
}

void dec_a(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.a = dec8(s_gb->gb_register.a, s_gb); }
void dec_b(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.b = dec8(s_gb->gb_register.b, s_gb); }
void dec_c(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.c = dec8(s_gb->gb_register.c, s_gb); }
void dec_d(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.d = dec8(s_gb->gb_register.d, s_gb); }
void dec_e(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.e = dec8(s_gb->gb_register.e, s_gb); }
void dec_h(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.h = dec8(s_gb->gb_register.h, s_gb); }
void dec_l(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.l = dec8(s_gb->gb_register.l, s_gb); }

void dec_sp(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.sp -= 1; }
void dec_de(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.de -= 1; }
void dec_bc(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.bc -= 1; }
void dec_hl(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.hl -= 1; }
void dec_hl_addr(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 12; dec8(memoperation(s_gb->gb_register.hl, -1, s_gb), s_gb); }
//voiderrupt

void	di(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	s_gb->gb_interrupts.interMaster = 0;
}

//cp

void cp(unsigned char reg, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	SET_NEG();
	if (s_gb->gb_register.a == reg)
		SET_ZERO();
	else
		CLEAR_ZERO();
	if (reg > s_gb->gb_register.a)
		SET_CARRY();
	else
		CLEAR_CARRY();
	if ((reg & 0x0f) > (s_gb->gb_register.a))
		SET_HALFC();
	else
		CLEAR_HALFC();
}

//0xfe CP
void cp_n(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	SET_NEG();
	unsigned char value = read8(s_gb);
//	//prvoidf("cp s_gb->gb_register.a %x value %x\n", s_gb->gb_register.a, value);
	if (s_gb->gb_register.a == value)
		SET_ZERO();
	else
		CLEAR_ZERO();
	if (value > s_gb->gb_register.a)
		SET_CARRY();
	else
		CLEAR_CARRY();
	if ((value & 0x0f) > (s_gb->gb_register.a & 0x0f))
		SET_HALFC();
	else
		CLEAR_HALFC();
}

void cp_a(struct s_gb *s_gb) 
{ 
	cp(s_gb->gb_register.a, s_gb);
}

void cp_b(struct s_gb *s_gb) 
{
	cp(s_gb->gb_register.b, s_gb);
}
void cp_c(struct s_gb *s_gb) { cp(s_gb->gb_register.c, s_gb); }
void cp_d(struct s_gb *s_gb) { cp(s_gb->gb_register.d, s_gb); }
void cp_e(struct s_gb *s_gb) { cp(s_gb->gb_register.e, s_gb); }
void cp_h(struct s_gb *s_gb) { cp(s_gb->gb_register.h, s_gb); }
void cp_l(struct s_gb *s_gb) { cp(s_gb->gb_register.l, s_gb); }
void cp_hl(struct s_gb *s_gb) { cp(read8bit(s_gb->gb_register.hl, s_gb), s_gb); }

//xor
void xor_a(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	s_gb->gb_register.a ^= s_gb->gb_register.a;
	check_zero(s_gb->gb_register.a, s_gb);
	CLEAR_CARRY();
	CLEAR_HALFC();
	CLEAR_NEG();
}

void xor(unsigned char reg, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	s_gb->gb_register.a ^= reg;
	check_zero(s_gb->gb_register.a, s_gb);
	CLEAR_CARRY();
	CLEAR_HALFC();
	CLEAR_NEG();
}

void xor_b(struct s_gb *s_gb) { xor (s_gb->gb_register.b, s_gb); }
void xor_c(struct s_gb *s_gb) { xor (s_gb->gb_register.c, s_gb); }
void xor_d(struct s_gb *s_gb) { xor (s_gb->gb_register.d, s_gb); }
void xor_e(struct s_gb *s_gb) { xor (s_gb->gb_register.e, s_gb); }
void xor_h(struct s_gb *s_gb) { xor (s_gb->gb_register.h, s_gb); }
void xor_l(struct s_gb *s_gb) { xor (s_gb->gb_register.l, s_gb); }
void xor_hl(struct s_gb *s_gb) { xor (read8bit(s_gb->gb_register.hl, s_gb), s_gb); }

void or (unsigned char reg, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	s_gb->gb_register.a |= reg;
	check_zero(s_gb->gb_register.a, s_gb);
	CLEAR_CARRY();
	CLEAR_HALFC();
	CLEAR_NEG();
}

void or_a(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; or (s_gb->gb_register.a, s_gb); }
void or_b(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; or (s_gb->gb_register.b, s_gb); }
void or_c(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; or (s_gb->gb_register.c, s_gb); }
void or_d(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; or (s_gb->gb_register.d, s_gb); }
void or_e(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; or (s_gb->gb_register.e, s_gb); }
void or_h(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; or (s_gb->gb_register.h, s_gb); }
void or_l(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; or (s_gb->gb_register.l, s_gb); }
void or_hl(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; or (read8bit(s_gb->gb_register.hl, s_gb), s_gb); }

void add16(unsigned short *res, unsigned short reg, struct s_gb *s_gb)
{
	unsigned int result = *res + reg;

	if (result & 0xffff00000) SET_CARRY();
	else CLEAR_CARRY();

	*res = (unsigned char)(result & 0xffff);
	check_zero(s_gb->gb_register.a, s_gb);

	if (((*res & 0x0f) + (reg & 0x0f)) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();

	CLEAR_NEG();
}
void add(unsigned char reg, struct s_gb *s_gb)
{
	int res = s_gb->gb_register.a + reg;
	
	if (res & 0xff00) SET_CARRY();
	else CLEAR_CARRY();

	s_gb->gb_register.a = (unsigned char)(res & 0xff);
	check_zero(s_gb->gb_register.a, s_gb);

	if (((s_gb->gb_register.a & 0x0f) + (reg & 0x0f)) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();

	CLEAR_NEG();
}

void add_a(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; add(s_gb->gb_register.a, s_gb); }
void add_b(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; add(s_gb->gb_register.b, s_gb); }
void add_c(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; add(s_gb->gb_register.c, s_gb); }
void add_d(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; add(s_gb->gb_register.d, s_gb); }
void add_e(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; add(s_gb->gb_register.e, s_gb); }
void add_h(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; add(s_gb->gb_register.h, s_gb); }
void add_l(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; add(s_gb->gb_register.l, s_gb); }
void add_hl(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; add(read8bit(s_gb->gb_register.hl, s_gb), s_gb); }

//call


void call_addr_16(struct s_gb *s_gb)
{
	unsigned short value;

	s_gb->gb_cpu.totalTick += 12;
	s_gb->gb_register.sp -= 2;
	value = read16bit(s_gb->gb_register.pc, s_gb);
	write16bitToAddr(s_gb->gb_register.sp, s_gb->gb_register.pc + 2, s_gb);
	s_gb->gb_register.pc = value;
	s_gb->gb_cpu.jmpf = 1;
}

void call_16_z(struct s_gb *s_gb)
{
	if (ZERO_FLAG)
	{
		unsigned short value;

		s_gb->gb_cpu.totalTick += 24;
		s_gb->gb_register.sp -= 2;
		value = read16bit(s_gb->gb_register.pc, s_gb);
		write16bitToAddr(s_gb->gb_register.sp, s_gb->gb_register.pc + 2, s_gb);
		s_gb->gb_register.pc = value;
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 12;
}

void call_16_nz(struct s_gb *s_gb)
{
	if (ZERO_FLAG == 0)
	{
		unsigned short value;

		s_gb->gb_cpu.totalTick += 24;
		s_gb->gb_register.sp -= 2;
		value = read16bit(s_gb->gb_register.pc, s_gb);
		write16bitToAddr(s_gb->gb_register.sp, s_gb->gb_register.pc + 2, s_gb);
		s_gb->gb_register.pc = value;
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 12;
}

void call_16_nc(struct s_gb *s_gb)
{
	if (CARRY_FLAG == 0)
	{
		unsigned short value;

		s_gb->gb_cpu.totalTick += 24;
		s_gb->gb_register.sp -= 2;
		value = read16bit(s_gb->gb_register.pc, s_gb);
		write16bitToAddr(s_gb->gb_register.sp, s_gb->gb_register.pc + 2, s_gb);
		s_gb->gb_register.pc = value;
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 12;
}

void call_16_c(struct s_gb *s_gb)
{
	if (CARRY_FLAG)
	{
		unsigned short value;

		s_gb->gb_cpu.totalTick += 24;
		s_gb->gb_register.sp -= 2;
		value = read16bit(s_gb->gb_register.pc, s_gb);
		write16bitToAddr(s_gb->gb_register.sp, s_gb->gb_register.pc + 2, s_gb);
		s_gb->gb_register.pc = value;
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 12;
}

//sbc
void sbc(unsigned char value, struct s_gb *s_gb)
{
	(CARRY_FLAG) > 1 ? value += 1 : value;

	SET_NEG();

	if (value > s_gb->gb_register.a) SET_CARRY();
	else CLEAR_CARRY();

	if (value == s_gb->gb_register.a) SET_ZERO();
	else CLEAR_ZERO();

	if ((value & 0x0f) > (s_gb->gb_register.a & 0x0f)) SET_HALFC();
	else CLEAR_HALFC();

	s_gb->gb_register.a -= value;
}
void sbc_n(struct s_gb *s_gb) { sbc(read8bit(s_gb->gb_register.pc, s_gb), s_gb); }

void sbc_b(struct s_gb *s_gb) { sbc(s_gb->gb_register.b, s_gb); }
void sbc_c(struct s_gb *s_gb) { sbc(s_gb->gb_register.c, s_gb); }
void sbc_d(struct s_gb *s_gb) { sbc(s_gb->gb_register.d, s_gb); }
void sbc_e(struct s_gb *s_gb) { sbc(s_gb->gb_register.e, s_gb); }
void sbc_h(struct s_gb *s_gb) { sbc(s_gb->gb_register.h, s_gb); }
void sbc_l(struct s_gb *s_gb) { sbc(s_gb->gb_register.l, s_gb); }
void sbc_hl(struct s_gb *s_gb) { sbc(read8bit(s_gb->gb_register.hl, s_gb), s_gb); }
void sbc_a(struct s_gb *s_gb) { sbc(s_gb->gb_register.a, s_gb); }

// ld val 16

void ld_bc_val_16(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 16; s_gb->gb_register.bc = read16bit(s_gb->gb_register.pc, s_gb); }
void ld_de_val_16(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 16; s_gb->gb_register.de = read16bit(s_gb->gb_register.pc, s_gb); }
void ld_hl_val_16(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 16; s_gb->gb_register.hl = read16bit(s_gb->gb_register.pc, s_gb); }
void ld_sp_val_16(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 16; s_gb->gb_register.sp = read16bit(s_gb->gb_register.pc, s_gb); }

//inc
unsigned short inc16(unsigned short value)
{
	//if (value & 0x0f) CLEAR_HALFC();
	//else SET_HALFC();
	value += 1;

	//if (value) CLEAR_ZERO();
	//else SET_ZERO();

	//CLEAR_NEG();
	return value;
}

unsigned char inc8(unsigned char value, struct s_gb *s_gb)
{
	if ((value & 0x0f) == 0x0f) SET_HALFC();
	else CLEAR_HALFC();
	value += 1;

	if (value) CLEAR_ZERO();
	else SET_ZERO();

	CLEAR_NEG();
	return value;
}

void inc_a(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.a = inc8(s_gb->gb_register.a, s_gb);  }
void inc_b(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.b = inc8(s_gb->gb_register.b, s_gb); }
void inc_c(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.c = inc8(s_gb->gb_register.c, s_gb); }
void inc_d(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.d = inc8(s_gb->gb_register.d, s_gb); }
void inc_e(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.e = inc8(s_gb->gb_register.e, s_gb); }
void inc_h(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.h = inc8(s_gb->gb_register.h, s_gb); }
void inc_l(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; s_gb->gb_register.l = inc8(s_gb->gb_register.l, s_gb); }

void inc_sp(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.sp = inc16(s_gb->gb_register.sp); }
void inc_bc(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.bc = inc16(s_gb->gb_register.bc); }
void inc_de(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.de = inc16(s_gb->gb_register.de); }
void inc_hl(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 8; s_gb->gb_register.hl = inc16(s_gb->gb_register.hl); }
void inc_hl_addr(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 12;  inc16(memoperation(s_gb->gb_register.hl, 1, s_gb)); }

void halt_proc(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4;  s_gb->gb_cpu.stopCpu = 1;  printf("halt proc\n"); }

//ld matrix
#include "ld.c"

void	ret_stack(struct s_gb *s_gb)
{
	s_gb->gb_register.pc = read16bit(s_gb->gb_register.sp, s_gb);
	s_gb->gb_register.sp += 2;
	s_gb->gb_cpu.jmpf = 1;
}

void ei(struct s_gb *s_gb)
{
	s_gb->gb_interrupts.interMaster = 1;
}
void cpl(struct s_gb *s_gb)
{
	s_gb->gb_register.a = ~s_gb->gb_register.a; SET_NEG(); SET_HALFC();
}

//and
void and_n(struct s_gb *s_gb)
{
	unsigned char val = read8(s_gb);

	s_gb->gb_register.a &= val;
	CLEAR_CARRY();
	CLEAR_NEG();
	if (s_gb->gb_register.a) CLEAR_ZERO();
	else SET_ZERO();
	SET_HALFC();
}

void and(unsigned char value, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	s_gb->gb_register.a &= value;
	CLEAR_CARRY();
	CLEAR_NEG();
	if (s_gb->gb_register.a) CLEAR_ZERO();
	else SET_ZERO();
	SET_HALFC();
}

void and_b(struct s_gb *s_gb) { and(s_gb->gb_register.b, s_gb); }
void and_c(struct s_gb *s_gb) { and (s_gb->gb_register.c, s_gb); }
void and_d(struct s_gb *s_gb) { and (s_gb->gb_register.d, s_gb); }
void and_e(struct s_gb *s_gb) { and (s_gb->gb_register.e, s_gb); }
void and_h(struct s_gb *s_gb) { and (s_gb->gb_register.h, s_gb); }
void and_l(struct s_gb *s_gb) { and (s_gb->gb_register.l, s_gb); }
void and_hl(struct s_gb *s_gb) { and (read8bit(s_gb->gb_register.hl, s_gb), s_gb); }
void and_a(struct s_gb *s_gb) { and (s_gb->gb_register.a, s_gb); }

//cb
void cb(struct s_gb *s_gb) { handleCB(read8(s_gb), s_gb); }

//pop
unsigned short pop16(struct s_gb *s_gb)
{
	unsigned short value;

	s_gb->gb_cpu.totalTick += 12;
	
	value = read16bit(s_gb->gb_register.sp, s_gb);
	s_gb->gb_register.sp += 2;
#ifdef GB_DEBUG
	//prvoidf("pop 16bit %x stack povoider %x\n", value, s_gb->gb_register.sp);
#endif

	return (value);
}

void	pop_hl(struct s_gb *s_gb) { s_gb->gb_register.hl = pop16(s_gb);}
void	pop_bc(struct s_gb *s_gb) { s_gb->gb_register.bc = pop16(s_gb);}
void	pop_de(struct s_gb *s_gb) { s_gb->gb_register.de = pop16(s_gb);}
void	pop_af(struct s_gb *s_gb) { s_gb->gb_register.af = pop16(s_gb);}

//push
void push16(unsigned short value, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 16;
	s_gb->gb_register.sp -= 2;
	write16bitToAddr(s_gb->gb_register.sp, value, s_gb);
}

void push_de(struct s_gb *s_gb) { push16(s_gb->gb_register.de, s_gb); }
void push_bc(struct s_gb *s_gb) { push16(s_gb->gb_register.bc, s_gb); }
void push_af(struct s_gb *s_gb) { push16(s_gb->gb_register.af, s_gb); }
void push_hl(struct s_gb *s_gb) { push16(s_gb->gb_register.hl, s_gb); }

//rst

void rst(unsigned short value, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 16;
#ifdef GB_DEBUG
	//prvoidf("push 16bit %x stack povoider %x\n", s_gb->gb_register.pc, s_gb->gb_register.sp);
#endif
	s_gb->gb_register.sp -= 2;
	write16bitToAddr(s_gb->gb_register.sp, s_gb->gb_register.pc, s_gb);

	s_gb->gb_register.pc = value;
	s_gb->gb_cpu.jmpf = 1;
	//getchar();
}
//rst
void rst_0(struct s_gb *s_gb) { rst(0, s_gb); }
void rst_8(struct s_gb *s_gb) { rst(0x0008, s_gb); }
void rst_10(struct s_gb *s_gb) { rst(0x0010, s_gb); }
void rst_18(struct s_gb *s_gb) { rst(0x0018, s_gb); }
void rst_20(struct s_gb *s_gb) { rst(0x0020, s_gb); }
void rst_28(struct s_gb *s_gb) { rst(0x0028, s_gb); }
void rst_30(struct s_gb *s_gb) { rst(0x0030, s_gb); }
void rst_38(struct s_gb *s_gb) { rst(0x0038, s_gb); }

void add_hl_de(struct s_gb *s_gb) { s_gb->gb_register.hl = s_gb->gb_register.de + s_gb->gb_register.hl; }

void jp_hl_addr(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	unsigned short value = s_gb->gb_register.hl;
	s_gb->gb_register.pc = value;
	s_gb->gb_cpu.jmpf = 1;
}

void jr_8(struct s_gb *s_gb)
{
	s_gb->gb_register.pc += ((signed char) read8(s_gb) + 1);
	s_gb->gb_cpu.jmpf = 1;
}

void jr_z_n8(struct s_gb *s_gb)
{
	if (ZERO_FLAG)
	{
		s_gb->gb_register.pc += ((signed char)read8(s_gb) + 1);
		s_gb->gb_cpu.jmpf = 1;
	}
}

void ld_a_add_16(struct s_gb *s_gb)
{
	s_gb->gb_register.a = read8bit(read16bit(s_gb->gb_register.pc, s_gb), s_gb);
}

void jp_nz_addr_16(struct s_gb *s_gb)
{
	if ((s_gb->gb_register.f & 0x80) == 0)
	{
		call_addr_16(s_gb);
		s_gb->gb_cpu.totalTick += 12;
	}
	s_gb->gb_cpu.totalTick += 16;
}

unsigned char add8(unsigned char reg, unsigned char value, struct s_gb *s_gb) {
	unsigned short res;
	
	res = reg + value;
	if (res & 0xff00) SET_CARRY();
	else CLEAR_CARRY();

	if ((res & 0xff) == 0) SET_ZERO();
	else CLEAR_ZERO();

	if ((reg & 0x0f) + (value & 0x0f) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();

	CLEAR_NEG();
	return (reg + value);
}

void add_a_n(struct s_gb *s_gb)
{
  s_gb->gb_register.a = add8(s_gb->gb_register.a, read8(s_gb), s_gb);
}

void sub(unsigned char value, struct s_gb *s_gb) {
	SET_NEG();

	if (value > s_gb->gb_register.a) SET_CARRY();
	else CLEAR_CARRY();

	if ((value & 0x0f) > (s_gb->gb_register.a & 0x0f)) SET_HALFC();
	else CLEAR_HALFC();

	s_gb->gb_register.a -= value;
	if (s_gb->gb_register.a) CLEAR_ZERO();
	else SET_ZERO();
}

//d6
void sub_a_8(struct s_gb *s_gb) { sub(read8bit(s_gb->gb_register.pc, s_gb), s_gb); }
// 0x90
void sub_b(struct s_gb *s_gb) { sub(s_gb->gb_register.b, s_gb); }

// 0x91
void sub_c(struct s_gb *s_gb) { sub(s_gb->gb_register.c, s_gb); }

// 0x92
void sub_d(struct s_gb *s_gb) { sub(s_gb->gb_register.d, s_gb); }

// 0x93
void sub_e(struct s_gb *s_gb) { sub(s_gb->gb_register.e, s_gb); }

// 0x94
void sub_h(struct s_gb *s_gb) { sub(s_gb->gb_register.h, s_gb); }

// 0x95
void sub_l(struct s_gb *s_gb) { sub(s_gb->gb_register.l, s_gb); }

// 0x96
void sub_hlp(struct s_gb *s_gb) { sub(read8bit(s_gb->gb_register.hl, s_gb), s_gb); }

// 0x97
void sub_a(struct s_gb *s_gb) { sub(s_gb->gb_register.a, s_gb); }


void ld_h_8(struct s_gb *s_gb) { s_gb->gb_register.h = read8bit(s_gb->gb_register.pc, s_gb); }

void ld_e_8(struct s_gb *s_gb) { s_gb->gb_register.e = read8bit(s_gb->gb_register.pc, s_gb); }

unsigned char rr(unsigned char value, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	value >>= 1;
	if (CARRY_FLAG)
		value |= 80;

	if ((value & 0x01) == 0x01) SET_CARRY();
	else CLEAR_CARRY();

	if (value) CLEAR_ZERO();
	else SET_ZERO();
	
	CLEAR_NEG();
	CLEAR_HALFC();
	
	return value;
}

void rrca(struct s_gb *s_gb)
{
	unsigned char carry = s_gb->gb_register.a & 0x01;
	if (carry) SET_CARRY();
	else CLEAR_CARRY();
	s_gb->gb_register.a >>= 1;

	if (carry) s_gb->gb_register.a |= 0x80;
	CLEAR_HALFC(); CLEAR_NEG(); CLEAR_ZERO();
}

void rra(struct s_gb *s_gb)
{
  	int carry = 0;
	
	s_gb->gb_cpu.totalTick += 4;
	if (CARRY_FLAG == 0)
		carry = 0 << 7;
	else
		carry = 1 << 7;


	if (s_gb->gb_register.a & 0x01) SET_CARRY();
	else CLEAR_CARRY();

	s_gb->gb_register.a >>= 1;
	s_gb->gb_register.a += carry;
	CLEAR_NEG(); CLEAR_HALFC(); CLEAR_ZERO();
}

void rrb(struct s_gb *s_gb) { s_gb->gb_register.b = rr(s_gb->gb_register.b, s_gb); }
void rrc(struct s_gb *s_gb) { s_gb->gb_register.c = rr(s_gb->gb_register.c, s_gb); }
void rrd(struct s_gb *s_gb) { s_gb->gb_register.d = rr(s_gb->gb_register.d, s_gb); }
void rre(struct s_gb *s_gb) { s_gb->gb_register.e = rr(s_gb->gb_register.e, s_gb); }
void rrh(struct s_gb *s_gb) { s_gb->gb_register.h = rr(s_gb->gb_register.h, s_gb); }
void rrl(struct s_gb *s_gb) { s_gb->gb_register.l = rr(s_gb->gb_register.l, s_gb); }

void jr_nc(struct s_gb *s_gb) {
	//prvoidf("JUMP IF CARRY FLAG\n");
	//prvoidf("CARRY FLAG VALUE %x\n", CARRY_FLAG);
	if ((CARRY_FLAG) == 0)
	{
		//prvoidf("JUMP CARRY FLAG NOT SET %x\n", read8());
		s_gb->gb_cpu.totalTick += 12;
		s_gb->gb_register.pc += (signed char)read8(s_gb) + 1;
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 8;
}

void jr_c(struct s_gb *s_gb) {
	//prvoidf("JUMP IF CARRY FLAG\n");
	//prvoidf("CARRY FLAG VALUE %x\n", CARRY_FLAG);
	if ((CARRY_FLAG) > 0)
	{
		//prvoidf("JUMP CARRY FLAG NOT SET %x\n", read8());
		s_gb->gb_cpu.totalTick += 12;
		s_gb->gb_register.pc += (signed char)read8(s_gb) + 1;
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 8;
}

void ret_nc(struct s_gb *s_gb)
{
	if (CARRY_FLAG) s_gb->gb_cpu.totalTick += 8;
	else {
		s_gb->gb_register.pc = pop16(s_gb);
		s_gb->gb_cpu.totalTick += 20;
		s_gb->gb_cpu.jmpf = 1;
	}
}

void ret_z(struct s_gb *s_gb)
{
	if (ZERO_FLAG) {
		s_gb->gb_cpu.totalTick += 8;
		s_gb->gb_register.pc = pop16(s_gb);
		s_gb->gb_cpu.jmpf = 1;
	}
	else
		s_gb->gb_cpu.totalTick += 8;
}

void or_a_n(struct s_gb *s_gb)
{
	s_gb->gb_register.a |= read8bit(s_gb->gb_register.pc, s_gb);
	if (s_gb->gb_register.a) CLEAR_ZERO();
	else SET_ZERO();
	CLEAR_CARRY();
	CLEAR_NEG();
	CLEAR_HALFC();

}

void xor_a_8(struct s_gb *s_gb)
{
	s_gb->gb_register.a ^= read8bit(s_gb->gb_register.pc, s_gb);
	check_zero(s_gb->gb_register.a, s_gb);
	CLEAR_NEG(); CLEAR_HALFC(); CLEAR_CARRY();
}

void adc_a_8(struct s_gb *s_gb)
{
  unsigned char value = 0;
	int result = s_gb->gb_register.a + value;

	value = read8bit(s_gb->gb_register.pc, s_gb);

	value += CARRY_FLAG ? 1 : 0;
	
	if (result & 0xff00) SET_CARRY();
	else CLEAR_CARRY();

	if (value == s_gb->gb_register.a) SET_NEG();
	else CLEAR_NEG();

	if (((value & 0x0f) + (s_gb->gb_register.a & 0x0f)) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();


	s_gb->gb_register.a = (unsigned char)(result & 0xff);

	if (s_gb->gb_register.a == 0) SET_ZERO();
	else CLEAR_ZERO();
}

void	add_hl_n(unsigned short value, struct s_gb *s_gb)
{
	unsigned long result = s_gb->gb_register.hl + value;

	if (result & 0xffff0000) SET_CARRY();
	else CLEAR_CARRY();

	s_gb->gb_register.hl = (unsigned short)(result & 0xffff);

	if (((s_gb->gb_register.hl & 0x0f) + (value & 0x0f)) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();

	//CLEAR_NEG();
}


void add_hl_hl(struct s_gb *s_gb) { add_hl_n(s_gb->gb_register.hl, s_gb); }
//0x09
void add_hl_bc(struct s_gb *s_gb) { add_hl_n(s_gb->gb_register.bc, s_gb); }
//0x29
//void add_hl_de(struct s_gb *s_gb) { add_hl_n(s_gb->gb_register.de); }
//0x39
void add_hl_sp(struct s_gb *s_gb) { add_hl_n(s_gb->gb_register.sp, s_gb); }


void ld_hl_sp_8(struct s_gb *s_gb)
{
	char value = read8bit(s_gb->gb_register.pc, s_gb);
	int res;

	res = (signed)value + s_gb->gb_register.sp;

	if (res & 0xffff0000) SET_CARRY();
	else CLEAR_CARRY();

	if (((s_gb->gb_register.sp & 0x0f) + (value & 0x0f)) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();

	CLEAR_NEG(); CLEAR_ZERO();

	s_gb->gb_register.hl = (unsigned short)(res & 0x0000ffff);
}

void daa(struct s_gb *s_gb)
{
	unsigned short s = s_gb->gb_register.a;
	s_gb->gb_cpu.totalTick += 4;
	if (NEG_FLAG) {
		if (HALFC_FLAG) s = (s - 0x06) & 0xFF;
		if (CARRY_FLAG) s -= 0x60;
	}
	else {
		if ( HALFC_FLAG || (s & 0xF) > 9) s += 0x06;
		if (CARRY_FLAG || s > 0x9F) s += 0x60;
	}

	s_gb->gb_register.a = (unsigned char)s;
	CLEAR_HALFC();

	if (s_gb->gb_register.a) CLEAR_ZERO();
	else SET_ZERO();

	if (s >= 0x100) SET_CARRY();
}

void ld_addr_sp(struct s_gb *s_gb)
{
	write16bitToAddr(read16bit(s_gb->gb_register.pc, s_gb), s_gb->gb_register.sp, s_gb);
}

void ret_c(struct s_gb *s_gb)
{
	if (ZERO_FLAG) {
		s_gb->gb_cpu.totalTick += 8;
		s_gb->gb_register.pc = pop16(s_gb);
		s_gb->gb_cpu.jmpf = 1;
	}
}

void ld_sp_hl(struct s_gb *s_gb)
{
	s_gb->gb_register.sp = s_gb->gb_register.hl;
	s_gb->gb_cpu.totalTick += 12;
}

void rlca(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	unsigned char carry = (s_gb->gb_register.a & 0x80) >> 7;
	if (carry) SET_CARRY();
	else CLEAR_CARRY();
	
	s_gb->gb_register.a <<= 1;
	s_gb->gb_register.a += carry;
	CLEAR_NEG(); CLEAR_HALFC(); CLEAR_ZERO();
}
void stop_cpu(struct s_gb *s_gb)
{
	printf("stop value %x\n", read8(s_gb));
	s_gb->gb_cpu.stopCpu = 1;
}

void op_undefined(struct s_gb *s_gb)
{
  printf("op undefined pc => %x\n", s_gb->gb_register.pc);
}

void jp_16_z(struct s_gb *s_gb)
{
	if (ZERO_FLAG) jp_addr_16(s_gb);
	else s_gb->gb_cpu.totalTick += 12;
}

void add_sp_8(struct s_gb *s_gb)
{
	unsigned long result = s_gb->gb_register.sp + read8bit(s_gb->gb_register.pc, s_gb);

	if (result & 0xffff0000) SET_CARRY();
	else CLEAR_CARRY();

	s_gb->gb_register.sp = (unsigned short)(result & 0xffff);

	if (((s_gb->gb_register.sp & 0x0f) + (read8bit(s_gb->gb_register.pc, s_gb) & 0x0f)) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();
}

void ret_nz(struct s_gb *s_gb)
{
	if (ZERO_FLAG) s_gb->gb_cpu.totalTick += 8;
	else {
		s_gb->gb_register.pc = pop16(s_gb);
		s_gb->gb_cpu.totalTick += 8;
	}
}

void rla(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;
	unsigned char carry = (s_gb->gb_register.a & 0x80) >> 7;
	if (carry) SET_CARRY();
	else CLEAR_CARRY();

	s_gb->gb_register.a <<= 1;
	CLEAR_NEG(); CLEAR_HALFC(); CLEAR_ZERO();
}

void set_carry(struct s_gb *s_gb) { CLEAR_NEG(); CLEAR_HALFC(); SET_CARRY(); s_gb->gb_cpu.totalTick += 4; } //37
void clear_carry(struct s_gb *s_gb) { CLEAR_NEG(); CLEAR_HALFC(); if (CARRY_FLAG) CLEAR_CARRY(); else SET_CARRY(); s_gb->gb_cpu.totalTick += 4; } //3f

//adc

void adc(unsigned char value, struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 4;

	value += CARRY_FLAG > 0 ? 1 : 0;

	int result = value + s_gb->gb_register.a;
	if (result & 0xff00) SET_CARRY();
	else CLEAR_CARRY();

	if (((s_gb->gb_register.a & 0x0f) + (value & 0x0f)) > 0x0f) SET_HALFC();
	else CLEAR_HALFC();

	SET_NEG();

	s_gb->gb_register.a = (unsigned char)(result & 0x00ff);
	if (s_gb->gb_register.a) CLEAR_ZERO();
	else SET_ZERO();
}

void reti(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 12;
	s_gb->gb_register.pc = read16bit(s_gb->gb_register.sp, s_gb);
	s_gb->gb_register.sp += 2;
	s_gb->gb_interrupts.interMaster = 1;
	s_gb->gb_cpu.jmpf = 1;
}

void adc_b(struct s_gb *s_gb) { adc(s_gb->gb_register.b, s_gb);  }
void adc_c(struct s_gb *s_gb) { adc(s_gb->gb_register.c, s_gb); }
void adc_d(struct s_gb *s_gb) { adc(s_gb->gb_register.d, s_gb); }
void adc_e(struct s_gb *s_gb) { adc(s_gb->gb_register.e, s_gb); }
void adc_h(struct s_gb *s_gb) { adc(s_gb->gb_register.h, s_gb); }
void adc_l(struct s_gb *s_gb) { adc(s_gb->gb_register.l, s_gb); }
void adc_hl(struct s_gb *s_gb) { s_gb->gb_cpu.totalTick += 4; adc(read8bit(s_gb->gb_register.hl, s_gb), s_gb); }
void adc_a(struct s_gb *s_gb) { adc(s_gb->gb_register.a, s_gb); }

void ld_ff_c_a(struct s_gb *s_gb)
{
	s_gb->gb_cpu.totalTick += 8;
	write8bit(0xff00 + s_gb->gb_register.c, s_gb->gb_register.a, s_gb);
}

void initCpu(struct s_gb * gb_s)
{
	struct s_cpu_z80 cpu_t[256] = {
		{ .opcode = 0x00, .value = "NOP", .func = nop, .size = 0 },
		{ 0x01, "ld bc value", ld_bc_val_16, 2 },
		{ 0x02, "save a to (bc) addr", ld_bc_addr_a, 0 },
		{ 0x03, "inc_bc", inc_bc, 0},
		{ 0x04, "inc_b", inc_b, 0 },
		{ 0x05, "dec b", dec_b, 0 },
		{ 0x06, "ld b value 8", ld_b_val_8, 1 },
		{ 0x07, "rlca", rlca, 0},
		{ 0x08, "load sp to addr", ld_addr_sp, 0},
		{ 0x09, "add hl bc", add_hl_bc, 0},
		{ 0x0a, "ld a bc addr", ld_a_bc_addr, 0 },
		{ 0x0b, "dec bc", dec_bc, 0},
		{ 0x0c, "inc_c", inc_c, 0 },
		{ 0x0d, "dec c", dec_c, 0 },
		{ 0x0e, "ld c value 8", ld_c_val_8, 1 },
		{ 0x0f, "rotate a carry", rrca, 0},
		{ 0x10, "stop cpu", stop_cpu, 1 },
		{ 0x11, "ld de value", ld_de_val_16, 2 },
		{ 0x12, "save a to (de) addr", ld_de_addr_a, 0 },
		{ 0x13, "inc_de", inc_de, 0 },
		{ 0x14, "inc_d", inc_d, 0 },
		{ 0x15, "dec b", dec_d, 0 },
		{ 0x16, "ld d value 8", ld_d_val_8, 1},
		{ 0x17, "rotate a left", rla, 0},
		{ 0x18, "jump to signed 8", jr_8, 1},
		{ 0x19, "add hl <- de", add_hl_de, 0},
		{ 0x1a, "ld a de addr", ld_a_de_addr, 0 },
		{ 0x1b, "dec_de", dec_de, 0},
		{ 0x1c, "inc_e", inc_e, 0 },
		{ 0x1d, "dec e", dec_e, 0 },
		{ 0x1e, "ld e 8", ld_e_8, 1},
		{ 0x1f, "rotate a right", rra, 0},
		{ 0x20, "JR NZ", jr_nz, 1 },
		{ 0x21, "ld hl value 16", ld_hl_val_16, 2 },
		{ 0x22, "save a to (hl) addr", ldi_hl_addr_a, 0 },
		{ 0x23, "inc_hl", inc_hl, 0 },
		{ 0x24, "inc_h", inc_h, 0 },
		{ 0x25, "dec b", dec_h, 0 },
		{ 0x26, "ld h 8", ld_h_8, 1},
		{ 0x27, "daa", daa, 0 },
		{ 0x28, "jump 8 if zero", jr_z_n8, 1},
		{ 0x29, "add hl hl", add_hl_hl, 0},
		{ 0x2a, "ldi a (hl)", ldi_a_hl_addr, 0},
		{ 0x2b, "dec hl", dec_hl, 1 },
		{ 0x2c, "inc_l", inc_l, 0 },
		{ 0x2d, "dec e", dec_l, 0 },
		{ 0x2e, "dec e", ld_l_val_8, 0 },
		{ 0x2f, "cpl", cpl, 0 },
		{ 0x30, "jmp if no carry", jr_nc, 1},
		{ 0x31, "ld value 16 voido sp", ld_sp_val_16, 2 },
		{ 0x32, "ldd a (hl)", ldd_hl_a_8, 0 },
		{ 0x33, "inc_sp", inc_sp, 0 },
		{ 0x34, "inc_hl_addr", inc_hl_addr, 0 },
		{ 0x35, "dec_hl_addr", dec_hl_addr, 0 },
		{ 0x36, "ld (hl) value 8", ld_hl_val_8, 1 },
		{ 0x37, "set carry", set_carry, 0 },
		{ 0x38, "jr n c", jr_c, 1},
		{ 0x39, "add hl <- sp", add_hl_sp, 0 },
		{ 0x3a, "ldd a (hl)", ldd_a_hl_addr, 0 },
		{ 0x3b, "dec sp", dec_sp, 0 },
		{ 0x3c, "inc_a", inc_a, 0 },
		{ 0x3d, "dec a", dec_a, 0 },
		{ 0x3e, "ld a value 8", ld_a_val_8, 1 },
		{ 0x3f, "clear carry", clear_carry, 0},

		//-----------------------------------------

		{ 0x40, "ld b <- b", ld_b_b, 0 }, //b
		{ 0x41, "ld b <- c", ld_b_c, 0 },
		{ 0x42, "ld b <- d", ld_b_d, 0 },
		{ 0x43, "ld b <- e", ld_b_e, 0 },
		{ 0x44, "ld b <- h", ld_b_h, 0 },
		{ 0x45, "ld b <- l", ld_b_l, 0 },
		{ 0x46, "ld b <- hl", ld_b_hl, 0 },
		{ 0x47, "ld b <- a", ld_b_a, 0 },

		{ 0x48, "ld c <- b", ld_c_b, 0 }, //c
		{ 0x49, "ld c <- c", ld_c_c, 0 },
		{ 0x4a, "ld c <- d", ld_c_d, 0 },
		{ 0x4b, "ld c <- e", ld_c_e, 0 },
		{ 0x4c, "ld c <- h", ld_c_h, 0 },
		{ 0x4d, "ld c <- l", ld_c_l, 0 },
		{ 0x4e, "ld c <- hl", ld_c_hl, 0 },
		{ 0x4f, "ld c <- a", ld_c_a, 0 },

		{ 0x50, "ld d <- b", ld_d_b, 0 }, //D
		{ 0x51, "ld d <- c", ld_d_c, 0 },
		{ 0x52, "ld d <- d", ld_d_d, 0 },
		{ 0x53, "ld d <- e", ld_d_e, 0 },
		{ 0x54, "ld d <- h", ld_d_h, 0 },
		{ 0x55, "ld d <- l", ld_d_l, 0 },
		{ 0x56, "ld d <- hl", ld_d_hl, 0 },
		{ 0x57, "ld d <- a", ld_d_a, 0 },

		{ 0x58, "ld e <- b", ld_e_b, 0 }, //E
		{ 0x59, "ld e <- c", ld_e_c, 0 },
		{ 0x5a, "ld e <- d", ld_e_d, 0 },
		{ 0x5b, "ld e <- e", ld_e_e, 0 },
		{ 0x5c, "ld e <- h", ld_e_h, 0 },
		{ 0x5d, "ld e <- l", ld_e_l, 0 },
		{ 0x5e, "ld e <- hl", ld_e_hl, 0 },
		{ 0x5f, "ld e <- a", ld_e_a, 0 },

		{ 0x60, "ld h <- b", ld_h_b, 0 }, //H
		{ 0x61, "ld h <- c", ld_h_c, 0 },
		{ 0x62, "ld h <- d", ld_h_d, 0 },
		{ 0x63, "ld h <- e", ld_h_e, 0 },
		{ 0x64, "ld h <- h", ld_h_h, 0 },
		{ 0x65, "ld h <- l", ld_h_l, 0 },
		{ 0x66, "ld h <- hl", ld_h_hl, 0 },
		{ 0x67, "ld h <- a", ld_h_a, 0 },

		{ 0x68, "ld l <- b", ld_l_b, 0 }, //L
		{ 0x69, "ld l <- c", ld_l_c, 0 },
		{ 0x6a, "ld l <- d", ld_l_d, 0 },
		{ 0x6b, "ld l <- e", ld_l_e, 0 },
		{ 0x6c, "ld l <- h", ld_l_h, 0 },
		{ 0x6d, "ld l <- l", ld_l_l, 0 },
		{ 0x6e, "ld l <- hl", ld_l_hl, 0 },
		{ 0x6f, "ld l <- a", ld_l_a, 0 },

		{ 0x70, "ld hl <- b", ld_hl_b, 0 }, //(HL)
		{ 0x71, "ld hl <- c", ld_hl_c, 0 },
		{ 0x72, "ld hl <- d", ld_hl_d, 0 },
		{ 0x73, "ld hl <- e", ld_hl_e, 0 },
		{ 0x74, "ld hl <- h", ld_hl_h, 0 },
		{ 0x75, "ld hl <- l", ld_hl_l, 0 },
		{ 0x76, "halt proc", halt_proc, 0 },
		{ 0x77, "ld hl <- a", ld_hl_a, 0 },

		{ 0x78, "ld a <- b", ld_a_b, 0 }, //A
		{ 0x79, "ld a <- c", ld_a_c, 0 },
		{ 0x7a, "ld a <- d", ld_a_d, 0 },
		{ 0x7b, "ld a <- e", ld_a_e, 0 },
		{ 0x7c, "ld a <- h", ld_a_h, 0 },
		{ 0x7d, "ld a <- l", ld_a_l, 0 },
		{ 0x7e, "ld a <- hl", ld_a_hl, 0 },
		{ 0x7f, "ld a <- a", ld_a_a, 0 },

		//-----------------------------------------

		{ 0x80, "add a <- b", add_b, 0},
		{ 0x81, "add a <- c", add_c, 0 },
		{ 0x82, "add a <- d", add_d, 0 },
		{ 0x83, "add a <- e", add_e, 0 },
		{ 0x84, "add a <- h", add_h, 0 },
		{ 0x85, "add a <- l", add_l, 0 },
		{ 0x86, "add a <- (hl)", add_hl, 0 },
		{ 0x87, "add a <- a", add_a, 0 },
		{ 0x88, "adc a b", adc_b,0 },
		{ 0x89, "adc a c", adc_c, 0 },
		{ 0x8a, "adc a d", adc_d, 0},
		{ 0x8b, "adc a e", adc_e, 0},
		{ 0x8c, "adc a h", adc_h, 0},
		{ 0x8d, "adc a l", adc_l, 0 },
		{ 0x8e, "adc a (hl)", adc_hl, 0},
		{ 0x8f, "adc a a", adc_a, 0},

		{ 0x90, "sub_b", sub_b, 0},
		{ 0x91, "sub_c", sub_c, 0 },
		{ 0x92, "sub_d", sub_d, 0 },
		{ 0x93, "sub_e", sub_e, 0 },
		{ 0x94, "sub_h", sub_h, 0 },
		{ 0x95, "sub_l", sub_l, 0 },
		{ 0x96, "sub_hl", sub_hlp, 0 },
		{ 0x97, "sub_a", sub_a, 0 },
		{ 0x98, "sbc", sbc_b, 0 },
		{ 0x99, "sbc", sbc_c, 0 },
		{ 0x9a, "sbc", sbc_d, 0 },
		{ 0x9b, "sbc", sbc_e, 0},
		{ 0x9c, "sbc", sbc_h, 0 },
		{ 0x9d, "sbc", sbc_l, 0 },
		{ 0x9e, "sbc", sbc_hl, 0 },
		{ 0x9f, "sbc_a_a", sbc_a, 0},
		{ 0xa0, "and b", and_b, 0},
		{ 0xa1, "and c", and_c, 0 },
		{ 0xa2, "and d", and_d, 0 },
		{ 0xa3, "and e", and_e, 0 },
		{ 0xa4, "and h", and_h, 0 },
		{ 0xa5, "and l", and_l, 0 },
		{ 0xa6, "and hl", and_hl, 0 },
		{ 0xa7, "and a", and_a, 0 },
		{ 0xa8, "xor b against a", xor_b, 0},
		{ 0xa9, "xor c against a", xor_c, 0 },
		{ 0xaa, "xor d against a", xor_d, 0 },
		{ 0xab, "xor e against a", xor_e, 0 },
		{ 0xac, "xor h against a", xor_h, 0 },
		{ 0xad, "xor l against a", xor_l, 0 },
		{ 0xae, "xor hl against a", xor_hl, 0 },
		{ 0xaf, "xor A against a", xor_a, 0 },
		{ 0xb0, "or_b", or_b, 0 },
		{ 0xb1, "or_c", or_c, 0 },
		{ 0xb2, "or_d", or_d, 0 },
		{ 0xb3, "or_e", or_e, 0 },
		{ 0xb4, "or_h", or_h, 0 },
		{ 0xb5, "or_l", or_l, 0 },
		{ 0xb6, "or_hl", or_hl, 0 },
		{ 0xb7, "or_a", or_a, 0 },
		{ 0xb8, "cp b", cp_b, 0 },
		{ 0xb9, "cp c", cp_c, 0 },
		{ 0xba, "cp d", cp_d, 0 },
		{ 0xbb, "cp e", cp_e, 0 },
		{ 0xbc, "cp h", cp_h, 0 },
		{ 0xbd, "cp l", cp_l, 0 },
		{ 0xbe, "cp (hl)", cp_hl, 0 },
		{ 0xbf, "cp a", cp_a, 0 },
		{ 0xc0, "ret nz", ret_nz, 0},
		{ 0xc1, "pop bc", pop_bc, 0},
		{ 0xc2, "jump nz 16", jp_nz_16, 2 },
		{ 0xc3, "JP addr 16", jp_addr_16, 0 },
		{ 0xc4, "CALL addr 16 if nz", call_16_nz, 2},
		{ 0xc5, "push bc", push_bc, 0 },
		{ 0xc6, "add a,n", add_a_n, 1},
		{ 0xc7, "rst 0", rst_0, 0},
		{ 0xc8, "ret z", ret_z, 0},
		{ 0xc9, "return", ret_stack, 0},
		{ 0xca, "jp 16 if Z", jp_16_z, 2},
		{ 0xcb, "additional operation => param opcode", cb, 1},
		{ 0xcc, "call 16 if z", call_16_z, 1 },
		{ 0xcd, "call addr 16", call_addr_16, 2},
		{ 0xce, "adc a 8", adc_a_8, 1},
		{ 0xcf, "rst 8", rst_8, 0},
		{ 0xd0, "ret nc", ret_nc, 0},
		{ 0xd1, "pop de", pop_de, 0 },
		{ 0xd2, "jp nc 16", jp_nc_16, 2},
		{ 0xd3, "no operation", op_undefined, 0},
		{ 0xd4, "call nc 16", call_16_nc, 2 },
		{ 0xd5, "push de", push_de, 0 },
		{ 0xd6, "sub a,n", sub_a_8, 1 },
		{ 0xd7, "rst 10", rst_10, 0},
		{ 0xd8, "ret c", ret_c, 0 },
		{ 0xd9, "reti", reti, 0},
		{ 0xda, "jp_c_16", jp_c_16, 2},
		{ 0xdb, "no operation", op_undefined, 0 },
		{ 0xdc, "call 16 if c", call_16_c, 1 },
		{ 0xdd, "no operation", op_undefined, 0 },
		{ 0xde, "sbc_n", sbc_n, 1},
		{ 0xdf, "rst 18", rst_18, 0 },
		{ 0xe0, "load a voido (0xff00+param)", ldd_addr_a_8, 1 }, //(ff00+addr) = a
		{ 0xe1, "pop 16 bit voido hl", pop_hl, 0},
		{ 0xe2, "load a voido (0xff00+c)", ld_ff_c_a, 0 }, //(ff00+c) =a
		{ 0xe3, "no operation", op_undefined, 0 },
		{ 0xe4, "no operation", op_undefined, 0 },
		{ 0xe5, "push hl", push_hl, 0 },
		{ 0xe6, "and n", and_n, 1},
		{ 0xe7, "rst 20", rst_20, 0},
		{ 0xe8, "add sp 8", add_sp_8, 0},
		{ 0xe9, "jmp to (hl)", jp_hl_addr, 0},
		{ 0xea, "load a void addr 16", ld_addr_a_16, 2},
		{ 0xeb, "no operation", op_undefined, 0 },
		{ 0xec, "no operation", op_undefined, 0 },
		{ 0xed, "no operation", op_undefined, 0 },
		{ 0xee, "xor a 8bit", xor_a_8, 1},
		{ 0xef, "rst 28", rst_28, 0 },
		{ 0xf0, "ldh a", ldh_a_val_8, 1 }, //f0
		{ 0xf1, "pop 16 bit voido af", pop_af, 0 },
		{ 0xf2, "undefined", op_undefined, 0 },
		{ 0xf3, "disable voiderrupt", di, 0},
		{ 0xf4, "undefined", op_undefined, 0},
		{ 0xf5, "rst 0", push_af, 0 },
		{ 0xf6, "or a n", or_a_n, 1},
		{ 0xf7, "rst 30", rst_30, 0 },
		{ 0xf8, "ld hl sp+r8", ld_hl_sp_8, 1 },
		{ 0xf9, "ld sp hl", ld_sp_hl, 0},
		{ 0xfa, "ld a (addr)", ld_a_add_16, 2},
		{ 0xfb, "enable voiderrupt", ei, 0},
		{ 0xfc, "no operation", op_undefined, 0 },
		{ 0xfd, "no operation", op_undefined, 0 },
		{ 0xfe, "cp n", cp_n, 1},
		{ 0xff, "rst 38", rst_38, 0 },
	};

	memcpy(&(gb_s->gb_cpu.gb_cpu_z80), &cpu_t, sizeof(cpu_t));
}
