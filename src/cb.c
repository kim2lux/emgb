#include "GB.h"
//https://cturt.github.io/cinoop.html

#define registers s_gb->gb_register 

#define FLAGS_ZERO (1 << 7)
#define FLAGS_NEGATIVE (1 << 6)
#define FLAGS_HALFCARRY (1 << 5)
#define FLAGS_CARRY (1 << 4)

#define FLAGS_ISZERO (registers.f & FLAGS_ZERO)
#define FLAGS_ISNEGATIVE (registers.f & FLAGS_NEGATIVE)
#define FLAGS_ISCARRY (registers.f & FLAGS_CARRY)
#define FLAGS_ISHALFCARRY (registers.f & FLAGS_HALFCARRY)

#define FLAGS_ISSET(x) (registers.f & (x))
#define FLAGS_SET(x) (registers.f |= (x))
#define FLAGS_CLEAR(x) (registers.f &= ~(x))

struct extendedInstruction {
	char *disassembly;
	void(*execute)(struct s_gb *);
	//unsigned char ticks;
};

extern const unsigned char extendedInstructionTicks[256];

static unsigned char rlc(unsigned char value, struct s_gb *s_gb) {
	int carry = (value & 0x80) >> 7;

	if (value & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	value <<= 1;
	value += carry;

	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);

	return value;
}

static unsigned char rrc(unsigned char value, struct s_gb *s_gb) {
	int carry = value & 0x01;

	value >>= 1;

	if (carry) {
		FLAGS_SET(FLAGS_CARRY);
		value |= 0x80;
	}
	else FLAGS_CLEAR(FLAGS_CARRY);

	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);

	return value;
}

static unsigned char rl(unsigned char value, struct s_gb *s_gb) {
	int carry = FLAGS_ISSET(FLAGS_CARRY) ? 1 : 0;

	if (value & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	value <<= 1;
	value += carry;

	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);

	return value;
}

static unsigned char rr(unsigned char value, struct s_gb *s_gb) {
	CLEAR_NEG(); CLEAR_HALFC();
	//printf("rr value %x\n", value);
	unsigned char tmp = 0;
	if ((s_gb->gb_register.f & 0x10) > 0)
	{
		tmp = 1;
	}
	if ((value & 0x01) > 0)
		FLAGS_SET(FLAGS_CARRY);
	else
		FLAGS_CLEAR(FLAGS_CARRY);
	value >>= 1;
	//printf("value before add carry %x\n", value);
	if (tmp > 0) 
		value |= 0x80;


	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);



	return value;
}

static unsigned char sla(unsigned char value, struct s_gb *s_gb) {
	if (value & 0x80) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	value <<= 1;

	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);

	return value;
}

static unsigned char sra(unsigned char value, struct s_gb *s_gb) {
	if (value & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	value = (value & 0x80) | (value >> 1);

	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);

	return value;
}

static unsigned char swap(unsigned char value, struct s_gb *s_gb) {
	value = ((value & 0xf) << 4) | ((value & 0xf0) >> 4);

	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY | FLAGS_CARRY);

	return value;
}

static unsigned char srl(unsigned char value, struct s_gb *s_gb) {
	if (value & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	value >>= 1;

	if (value) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);

	return value;
}

static void bit(unsigned char bit, unsigned char value, struct s_gb *s_gb) {
	if (value & bit) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE);
	FLAGS_SET(FLAGS_HALFCARRY);
}

static unsigned char set(unsigned char bit, unsigned char value) {
	value |= bit;
	return value;
}

// 0x00
void rlc_b(struct s_gb *s_gb) { registers.b = rlc(registers.b, s_gb); }

// 0x01
void rlc_c(struct s_gb *s_gb) { registers.c = rlc(registers.c, s_gb); }

// 0x02
void rlc_d(struct s_gb *s_gb) { registers.d = rlc(registers.d, s_gb); }

// 0x03
void rlc_e(struct s_gb *s_gb) { registers.e = rlc(registers.e, s_gb); }

// 0x04
void rlc_h(struct s_gb *s_gb) { registers.h = rlc(registers.h, s_gb); }

// 0x05
void rlc_l(struct s_gb *s_gb) { registers.l = rlc(registers.l, s_gb); }

// 0x06
void rlc_hlp(struct s_gb *s_gb) { write8bit(registers.hl, rlc(read8bit(registers.hl, s_gb), s_gb), s_gb); }

// 0x07
void rlc_a(struct s_gb *s_gb) { registers.a = rlc(registers.a, s_gb); }

// 0x08
void rrc_b(struct s_gb *s_gb) { registers.b = rrc(registers.b, s_gb); }

// 0x09
void rrc_c(struct s_gb *s_gb) { registers.c = rrc(registers.c, s_gb); }

// 0x0a
void rrc_d(struct s_gb *s_gb) { registers.d = rrc(registers.d, s_gb); }

// 0x0b
void rrc_e(struct s_gb *s_gb) { registers.e = rrc(registers.e, s_gb); }

// 0x0c
void rrc_h(struct s_gb *s_gb) { registers.h = rrc(registers.h, s_gb); }

// 0x0d
void rrc_l(struct s_gb *s_gb) { registers.l = rrc(registers.l, s_gb); }

// 0x0e
void rrc_hlp(struct s_gb *s_gb) { write8bit(registers.hl, rrc(read8bit(registers.hl, s_gb), s_gb), s_gb); }

// 0x0f
void rrc_a(struct s_gb *s_gb) { registers.a = rrc(registers.a, s_gb); }

// 0x10
void rl_b(struct s_gb *s_gb) { registers.b = rl(registers.b, s_gb); }

// 0x11
void rl_c(struct s_gb *s_gb) { registers.c = rl(registers.c, s_gb); }

// 0x12
void rl_d(struct s_gb *s_gb) { registers.d = rl(registers.d, s_gb); }

// 0x13
void rl_e(struct s_gb *s_gb) { registers.e = rl(registers.e, s_gb); }

// 0x14
void rl_h(struct s_gb *s_gb) { registers.h = rl(registers.h, s_gb); }

// 0x15
void rl_l(struct s_gb *s_gb) { registers.l = rl(registers.l, s_gb); }

// 0x16
void rl_hlp(struct s_gb *s_gb) { write8bit(registers.hl, rl(read8bit(registers.hl, s_gb), s_gb), s_gb); }

// 0x17
void rl_a(struct s_gb *s_gb) { registers.a = rl(registers.a, s_gb); }

// 0x18
void rr_b(struct s_gb *s_gb) { registers.b = rr(registers.b, s_gb); }

// 0x19
void rr_c(struct s_gb *s_gb) {  registers.c = rr(registers.c, s_gb); }

// 0x1a
void rr_d(struct s_gb *s_gb) { registers.d = rr(registers.d, s_gb); }

// 0x1b
void rr_e(struct s_gb *s_gb) { registers.e = rr(registers.e, s_gb); }

// 0x1c
void rr_h(struct s_gb *s_gb) { registers.h = rr(registers.h, s_gb); }

// 0x1d
void rr_l(struct s_gb *s_gb) { registers.l = rr(registers.l, s_gb); }

// 0x1e
void rr_hlp(struct s_gb *s_gb) { write8bit(registers.hl, rr(read8bit(registers.hl, s_gb), s_gb), s_gb); }

// 0x1f
void rr_a(struct s_gb *s_gb) { registers.a = rr(registers.a, s_gb); }

// 0x20
void sla_b(struct s_gb *s_gb) { registers.b = sla(registers.b, s_gb); }

// 0x21
void sla_c(struct s_gb *s_gb) { registers.c = sla(registers.c, s_gb); }

// 0x22
void sla_d(struct s_gb *s_gb) { registers.d = sla(registers.d, s_gb); }

// 0x23
void sla_e(struct s_gb *s_gb) { registers.e = sla(registers.e, s_gb); }

// 0x24
void sla_h(struct s_gb *s_gb) { registers.h = sla(registers.h, s_gb); }

// 0x25
void sla_l(struct s_gb *s_gb) { registers.l = sla(registers.l, s_gb); }

// 0x26
void sla_hlp(struct s_gb *s_gb) { write8bit(registers.hl, sla(read8bit(registers.hl, s_gb), s_gb), s_gb); }

// 0x27
void sla_a(struct s_gb *s_gb) { registers.a = sla(registers.a, s_gb); }

// 0x28
void sra_b(struct s_gb *s_gb) { registers.b = sra(registers.b, s_gb); }

// 0x29
void sra_c(struct s_gb *s_gb) { registers.c = sra(registers.c, s_gb); }

// 0x2a
void sra_d(struct s_gb *s_gb) { registers.d = sra(registers.d, s_gb); }

// 0x2b
void sra_e(struct s_gb *s_gb) { registers.e = sra(registers.e, s_gb); }

// 0x2c
void sra_h(struct s_gb *s_gb) { registers.h = sra(registers.h, s_gb); }

// 0x2d
void sra_l(struct s_gb *s_gb) { registers.l = sra(registers.l, s_gb); }

// 0x2e
void sra_hlp(struct s_gb *s_gb) { write8bit(registers.hl,  sra((unsigned char) registers.hl, s_gb), s_gb); }

// 0x2f
void sra_a(struct s_gb *s_gb) { registers.a = sra(registers.a, s_gb); }

// 0x30
void swap_b(struct s_gb *s_gb) { registers.b = swap(registers.b, s_gb); }

// 0x31
void swap_c(struct s_gb *s_gb) { registers.c = swap(registers.c, s_gb); }

// 0x32
void swap_d(struct s_gb *s_gb) { registers.d = swap(registers.d, s_gb); }

// 0x33
void swap_e(struct s_gb *s_gb) { registers.e = swap(registers.e, s_gb); }

// 0x34
void swap_h(struct s_gb *s_gb) { registers.h = swap(registers.h, s_gb); }

// 0x35
void swap_l(struct s_gb *s_gb) { registers.l = swap(registers.l, s_gb); }

// 0x36
void swap_hlp(struct s_gb *s_gb) { write8bit(registers.hl, swap(read8bit(registers.hl, s_gb), s_gb), s_gb); }

// 0x37
void swap_a(struct s_gb *s_gb) { registers.a = swap(registers.a, s_gb); }

// 0x38
void srl_b(struct s_gb *s_gb) { registers.b = srl(registers.b, s_gb); }

// 0x39
void srl_c(struct s_gb *s_gb) { registers.c = srl(registers.c, s_gb); }

// 0x3a
void srl_d(struct s_gb *s_gb) { registers.d = srl(registers.d, s_gb); }

// 0x3b
void srl_e(struct s_gb *s_gb) { registers.e = srl(registers.e, s_gb); }

// 0x3c
void srl_h(struct s_gb *s_gb) { registers.h = srl(registers.h, s_gb); }

// 0x3d
void srl_l(struct s_gb *s_gb) { registers.l = srl(registers.l, s_gb); }

// 0x3e
void srl_hlp(struct s_gb *s_gb) { write8bit(registers.hl, srl(read8bit(registers.hl, s_gb), s_gb), s_gb); }

// 0x3f
void srl_a(struct s_gb *s_gb) {
	if (registers.a & 0x01) FLAGS_SET(FLAGS_CARRY);
	else FLAGS_CLEAR(FLAGS_CARRY);

	registers.a >>= 1;

	if (registers.a) FLAGS_CLEAR(FLAGS_ZERO);
	else FLAGS_SET(FLAGS_ZERO);

	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_HALFCARRY);
}

// 0x40
void bit_0_b(struct s_gb *s_gb) { bit(1 << 0, registers.b, s_gb); }

// 0x41
void bit_0_c(struct s_gb *s_gb) { bit(1 << 0, registers.c, s_gb); }

// 0x42
void bit_0_d(struct s_gb *s_gb) { bit(1 << 0, registers.d, s_gb); }

// 0x43
void bit_0_e(struct s_gb *s_gb) { bit(1 << 0, registers.e, s_gb); }

// 0x44
void bit_0_h(struct s_gb *s_gb) { bit(1 << 0, registers.h, s_gb); }

// 0x45
void bit_0_l(struct s_gb *s_gb) { bit(1 << 0, registers.l, s_gb); }

// 0x46
void bit_0_hlp(struct s_gb *s_gb) { bit(1 << 0, read8bit(registers.hl, s_gb), s_gb); }

// 0x47
void bit_0_a(struct s_gb *s_gb) { bit(1 << 0, registers.a, s_gb); }

// 0x48
void bit_1_b(struct s_gb *s_gb) { bit(1 << 1, registers.b, s_gb); }

// 0x49
void bit_1_c(struct s_gb *s_gb) { bit(1 << 1, registers.c, s_gb); }

// 0x4a
void bit_1_d(struct s_gb *s_gb) { bit(1 << 1, registers.d, s_gb); }

// 0x4b
void bit_1_e(struct s_gb *s_gb) { bit(1 << 1, registers.e, s_gb); }

// 0x4c
void bit_1_h(struct s_gb *s_gb) { bit(1 << 1, registers.h, s_gb); }

// 0x4d
void bit_1_l(struct s_gb *s_gb) { bit(1 << 1, registers.l, s_gb); }

// 0x4e
void bit_1_hlp(struct s_gb *s_gb) { bit(1 << 1, read8bit(registers.hl, s_gb), s_gb); }

// 0x4f
void bit_1_a(struct s_gb *s_gb) { bit(1 << 1, registers.a, s_gb); }

// 0x50
void bit_2_b(struct s_gb *s_gb) { bit(1 << 2, registers.b, s_gb); }

// 0x51
void bit_2_c(struct s_gb *s_gb) { bit(1 << 2, registers.c, s_gb); }

// 0x52
void bit_2_d(struct s_gb *s_gb) { bit(1 << 2, registers.d, s_gb); }

// 0x53
void bit_2_e(struct s_gb *s_gb) { bit(1 << 2, registers.e, s_gb); }

// 0x54
void bit_2_h(struct s_gb *s_gb) { bit(1 << 2, registers.h, s_gb); }

// 0x55
void bit_2_l(struct s_gb *s_gb) { bit(1 << 2, registers.l, s_gb); }

// 0x56
void bit_2_hlp(struct s_gb *s_gb) { bit(1 << 2, read8bit(registers.hl, s_gb), s_gb); }

// 0x57
void bit_2_a(struct s_gb *s_gb) { bit(1 << 2, registers.a, s_gb); }

// 0x58
void bit_3_b(struct s_gb *s_gb) { bit(1 << 3, registers.b, s_gb); }

// 0x59
void bit_3_c(struct s_gb *s_gb) { bit(1 << 3, registers.c, s_gb); }

// 0x5a
void bit_3_d(struct s_gb *s_gb) { bit(1 << 3, registers.d, s_gb); }

// 0x5b
void bit_3_e(struct s_gb *s_gb) { bit(1 << 3, registers.e, s_gb); }

// 0x5c
void bit_3_h(struct s_gb *s_gb) { bit(1 << 3, registers.h, s_gb); }

// 0x5d
void bit_3_l(struct s_gb *s_gb) { bit(1 << 3, registers.l, s_gb); }

// 0x5e
void bit_3_hlp(struct s_gb *s_gb) { bit(1 << 3, read8bit(registers.hl, s_gb), s_gb); }

// 0x5f
void bit_3_a(struct s_gb *s_gb) { bit(1 << 3, registers.a, s_gb); }

// 0x60
void bit_4_b(struct s_gb *s_gb) { bit(1 << 4, registers.b, s_gb); }

// 0x61
void bit_4_c(struct s_gb *s_gb) { bit(1 << 4, registers.c, s_gb); }

// 0x62
void bit_4_d(struct s_gb *s_gb) { bit(1 << 4, registers.d, s_gb); }

// 0x63
void bit_4_e(struct s_gb *s_gb) { bit(1 << 4, registers.e, s_gb); }

// 0x64
void bit_4_h(struct s_gb *s_gb) { bit(1 << 4, registers.h, s_gb); }

// 0x65
void bit_4_l(struct s_gb *s_gb) { bit(1 << 4, registers.l, s_gb); }

// 0x66
void bit_4_hlp(struct s_gb *s_gb) { bit(1 << 4, read8bit(registers.hl, s_gb), s_gb); }

// 0x67
void bit_4_a(struct s_gb *s_gb) { bit(1 << 4, registers.a, s_gb); }

// 0x68
void bit_5_b(struct s_gb *s_gb) { bit(1 << 5, registers.b, s_gb); }

// 0x69
void bit_5_c(struct s_gb *s_gb) { bit(1 << 5, registers.c, s_gb); }

// 0x6a
void bit_5_d(struct s_gb *s_gb) { bit(1 << 5, registers.d, s_gb); }

// 0x6b
void bit_5_e(struct s_gb *s_gb) { bit(1 << 5, registers.e, s_gb); }

// 0x6c
void bit_5_h(struct s_gb *s_gb) { bit(1 << 5, registers.h, s_gb); }

// 0x6d
void bit_5_l(struct s_gb *s_gb) { bit(1 << 5, registers.l, s_gb); }

// 0x6e
void bit_5_hlp(struct s_gb *s_gb) { bit(1 << 5, read8bit(registers.hl, s_gb), s_gb); }

// 0x6f
void bit_5_a(struct s_gb *s_gb) { bit(1 << 5, registers.a, s_gb); }

// 0x70
void bit_6_b(struct s_gb *s_gb) { bit(1 << 6, registers.b, s_gb); }

// 0x71
void bit_6_c(struct s_gb *s_gb) { bit(1 << 6, registers.c, s_gb); }

// 0x72
void bit_6_d(struct s_gb *s_gb) { bit(1 << 6, registers.d, s_gb); }

// 0x73
void bit_6_e(struct s_gb *s_gb) { bit(1 << 6, registers.e, s_gb); }

// 0x74
void bit_6_h(struct s_gb *s_gb) { bit(1 << 6, registers.h, s_gb); }

// 0x75
void bit_6_l(struct s_gb *s_gb) { bit(1 << 6, registers.l, s_gb); }

// 0x76
void bit_6_hlp(struct s_gb *s_gb) { bit(1 << 6, read8bit(registers.hl, s_gb), s_gb); }

// 0x77
void bit_6_a(struct s_gb *s_gb) { bit(1 << 6, registers.a, s_gb); }

// 0x78
void bit_7_b(struct s_gb *s_gb) { bit(1 << 7, registers.b, s_gb); }

// 0x79
void bit_7_c(struct s_gb *s_gb) { bit(1 << 7, registers.c, s_gb); }

// 0x7a
void bit_7_d(struct s_gb *s_gb) { bit(1 << 7, registers.d, s_gb); }

// 0x7b
void bit_7_e(struct s_gb *s_gb) { bit(1 << 7, registers.e, s_gb); }

// 0x7c
void bit_7_h(struct s_gb *s_gb) { bit(1 << 7, registers.h, s_gb); }

// 0x7d
void bit_7_l(struct s_gb *s_gb) { bit(1 << 7, registers.l, s_gb); }

// 0x7e
void bit_7_hlp(struct s_gb *s_gb) { bit(1 << 7, read8bit(registers.hl, s_gb), s_gb); }

// 0x7f
void bit_7_a(struct s_gb *s_gb) { bit(1 << 7, registers.a, s_gb); }

// 0x80
void res_0_b(struct s_gb *s_gb) { registers.b &= ~(1 << 0); }

// 0x81
void res_0_c(struct s_gb *s_gb) { registers.c &= ~(1 << 0); }

// 0x82
void res_0_d(struct s_gb *s_gb) { registers.d &= ~(1 << 0); }

// 0x83
void res_0_e(struct s_gb *s_gb) { registers.e &= ~(1 << 0); }

// 0x84
void res_0_h(struct s_gb *s_gb) { registers.h &= ~(1 << 0); }

// 0x85
void res_0_l(struct s_gb *s_gb) { registers.l &= ~(1 << 0); }

// 0x86
void res_0_hlp(struct s_gb *s_gb) { write8bit(registers.hl, read8bit(registers.hl, s_gb) & ~(1 << 0), s_gb); }

// 0x87
void res_0_a(struct s_gb *s_gb) { registers.a &= ~(1 << 0); }

// 0x88
void res_1_b(struct s_gb *s_gb) { registers.b &= ~(1 << 1); }

// 0x89
void res_1_c(struct s_gb *s_gb) { registers.c &= ~(1 << 1); }

// 0x8a
void res_1_d(struct s_gb *s_gb) { registers.d &= ~(1 << 1); }

// 0x8b
void res_1_e(struct s_gb *s_gb) { registers.e &= ~(1 << 1); }

// 0x8c
void res_1_h(struct s_gb *s_gb) { registers.h &= ~(1 << 1); }

// 0x8d
void res_1_l(struct s_gb *s_gb) { registers.l &= ~(1 << 1); }

// 0x8e
void res_1_hlp(struct s_gb *s_gb) { write8bit(registers.hl, registers.hl & ~(1 << 1), s_gb); }

// 0x8f
void res_1_a(struct s_gb *s_gb) { registers.a &= ~(1 << 1); }

// 0x90
void res_2_b(struct s_gb *s_gb) { registers.b &= ~(1 << 2); }

// 0x91
void res_2_c(struct s_gb *s_gb) { registers.c &= ~(1 << 2); }

// 0x92
void res_2_d(struct s_gb *s_gb) { registers.d &= ~(1 << 2); }

// 0x93
void res_2_e(struct s_gb *s_gb) { registers.e &= ~(1 << 2); }

// 0x94
void res_2_h(struct s_gb *s_gb) { registers.h &= ~(1 << 2); }

// 0x95
void res_2_l(struct s_gb *s_gb) { registers.l &= ~(1 << 2); }

// 0x96
void res_2_hlp(struct s_gb *s_gb) { write8bit(registers.hl, registers.hl & ~(1 << 2), s_gb); }

// 0x97
void res_2_a(struct s_gb *s_gb) { registers.a &= ~(1 << 2); }

// 0x98
void res_3_b(struct s_gb *s_gb) { registers.b &= ~(1 << 3); }

// 0x99
void res_3_c(struct s_gb *s_gb) { registers.c &= ~(1 << 3); }

// 0x9a
void res_3_d(struct s_gb *s_gb) { registers.d &= ~(1 << 3); }

// 0x9b
void res_3_e(struct s_gb *s_gb) { registers.e &= ~(1 << 3); }

// 0x9c
void res_3_h(struct s_gb *s_gb) { registers.h &= ~(1 << 3); }

// 0x9d
void res_3_l(struct s_gb *s_gb) { registers.l &= ~(1 << 3); }

// 0x9e
void res_3_hlp(struct s_gb *s_gb) { write8bit(registers.hl, registers.hl & ~(1 << 3), s_gb); }

// 0x9f
void res_3_a(struct s_gb *s_gb) { registers.a &= ~(1 << 3); }

// 0xa0
void res_4_b(struct s_gb *s_gb) { registers.b &= ~(1 << 4); }

// 0xa1
void res_4_c(struct s_gb *s_gb) { registers.c &= ~(1 << 4); }

// 0xa2
void res_4_d(struct s_gb *s_gb) { registers.d &= ~(1 << 4); }

// 0xa3
void res_4_e(struct s_gb *s_gb) { registers.e &= ~(1 << 4); }

// 0xa4
void res_4_h(struct s_gb *s_gb) { registers.h &= ~(1 << 4); }

// 0xa5
void res_4_l(struct s_gb *s_gb) { registers.l &= ~(1 << 4); }

// 0xa6
void res_4_hlp(struct s_gb *s_gb) { write8bit(registers.hl, registers.hl & ~(1 << 4), s_gb); }

// 0xa7
void res_4_a(struct s_gb *s_gb) { registers.a &= ~(1 << 4); }

// 0xa8
void res_5_b(struct s_gb *s_gb) { registers.b &= ~(1 << 5); }

// 0xa9
void res_5_c(struct s_gb *s_gb) { registers.c &= ~(1 << 5); }

// 0xaa
void res_5_d(struct s_gb *s_gb) { registers.d &= ~(1 << 5); }

// 0xab
void res_5_e(struct s_gb *s_gb) { registers.e &= ~(1 << 5); }

// 0xac
void res_5_h(struct s_gb *s_gb) { registers.h &= ~(1 << 5); }

// 0xad
void res_5_l(struct s_gb *s_gb) { registers.l &= ~(1 << 5); }

// 0xae
void res_5_hlp(struct s_gb *s_gb) { write8bit(registers.hl, registers.hl & ~(1 << 5), s_gb); }

// 0xaf
void res_5_a(struct s_gb *s_gb) { registers.a &= ~(1 << 5); }

// 0xb0
void res_6_b(struct s_gb *s_gb) { registers.b &= ~(1 << 6); }

// 0xb1
void res_6_c(struct s_gb *s_gb) { registers.c &= ~(1 << 6); }

// 0xb2
void res_6_d(struct s_gb *s_gb) { registers.d &= ~(1 << 6); }

// 0xb3
void res_6_e(struct s_gb *s_gb) { registers.e &= ~(1 << 6); }

// 0xb4
void res_6_h(struct s_gb *s_gb) { registers.h &= ~(1 << 6); }

// 0xb5
void res_6_l(struct s_gb *s_gb) { registers.l &= ~(1 << 6); }

// 0xb6
void res_6_hlp(struct s_gb *s_gb) { write8bit(registers.hl, registers.hl & ~(1 << 6), s_gb); }

// 0xb7
void res_6_a(struct s_gb *s_gb) { registers.a &= ~(1 << 6); }

// 0xb8
void res_7_b(struct s_gb *s_gb) { registers.b &= ~(1 << 7); }

// 0xb9
void res_7_c(struct s_gb *s_gb) { registers.c &= ~(1 << 7); }

// 0xba
void res_7_d(struct s_gb *s_gb) { registers.d &= ~(1 << 7); }

// 0xbb
void res_7_e(struct s_gb *s_gb) { registers.e &= ~(1 << 7); }

// 0xbc
void res_7_h(struct s_gb *s_gb) { registers.h &= ~(1 << 7); }

// 0xbd
void res_7_l(struct s_gb *s_gb) { registers.l &= ~(1 << 7); }

// 0xbe
void res_7_hlp(struct s_gb *s_gb) { write8bit(registers.hl, read8bit(registers.hl, s_gb) & ~(1 << 7), s_gb); }

// 0xbf
void res_7_a(struct s_gb *s_gb) { registers.a &= ~(1 << 7); }

// 0xc0
void set_0_b(struct s_gb *s_gb) { registers.b = set(1 << 0, registers.b); }

// 0xc1
void set_0_c(struct s_gb *s_gb) { registers.c = set(1 << 0, registers.c); }

// 0xc2
void set_0_d(struct s_gb *s_gb) { registers.d = set(1 << 0, registers.d); }

// 0xc3
void set_0_e(struct s_gb *s_gb) { registers.e = set(1 << 0, registers.e); }

// 0xc4
void set_0_h(struct s_gb *s_gb) { registers.h = set(1 << 0, registers.h); }

// 0xc5
void set_0_l(struct s_gb *s_gb) { registers.l = set(1 << 0, registers.l); }

// 0xc6
void set_0_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 0, read8bit(registers.hl, s_gb)), s_gb); }

// 0xc7
void set_0_a(struct s_gb *s_gb) { registers.a = set(1 << 0, registers.a); }

// 0xc8
void set_1_b(struct s_gb *s_gb) { registers.b = set(1 << 1, registers.b); }

// 0xc9
void set_1_c(struct s_gb *s_gb) { registers.c = set(1 << 1, registers.c); }

// 0xca
void set_1_d(struct s_gb *s_gb) { registers.d = set(1 << 1, registers.d); }

// 0xcb
void set_1_e(struct s_gb *s_gb) { registers.e = set(1 << 1, registers.e); }

// 0xcc
void set_1_h(struct s_gb *s_gb) { registers.h = set(1 << 1, registers.h); }

// 0xcd
void set_1_l(struct s_gb *s_gb) { registers.l = set(1 << 1, registers.l); }

// 0xce
void set_1_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 1, read8bit(registers.hl, s_gb)), s_gb); }

// 0xcf
void set_1_a(struct s_gb *s_gb) { registers.a = set(1 << 1, registers.a); }

// 0xd0
void set_2_b(struct s_gb *s_gb) { registers.b = set(1 << 2, registers.b); }

// 0xd1
void set_2_c(struct s_gb *s_gb) { registers.c = set(1 << 2, registers.c); }

// 0xd2
void set_2_d(struct s_gb *s_gb) { registers.d = set(1 << 2, registers.d); }

// 0xd3
void set_2_e(struct s_gb *s_gb) { registers.e = set(1 << 2, registers.e); }

// 0xd4
void set_2_h(struct s_gb *s_gb) { registers.h = set(1 << 2, registers.h); }

// 0xd5
void set_2_l(struct s_gb *s_gb) { registers.l = set(1 << 2, registers.l); }

// 0xd6
void set_2_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 2, read8bit(registers.hl, s_gb)), s_gb); }

// 0xd7
void set_2_a(struct s_gb *s_gb) { registers.a = set(1 << 2, registers.a); }

// 0xd8
void set_3_b(struct s_gb *s_gb) { registers.b = set(1 << 3, registers.b); }

// 0xd9
void set_3_c(struct s_gb *s_gb) { registers.c = set(1 << 3, registers.c); }

// 0xda
void set_3_d(struct s_gb *s_gb) { registers.d = set(1 << 3, registers.d); }

// 0xdb
void set_3_e(struct s_gb *s_gb) { registers.e = set(1 << 3, registers.e); }

// 0xdc
void set_3_h(struct s_gb *s_gb) { registers.h = set(1 << 3, registers.h); }

// 0xdd
void set_3_l(struct s_gb *s_gb) { registers.l = set(1 << 3, registers.l); }

// 0xde
void set_3_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 3, read8bit(registers.hl, s_gb)), s_gb); }

// 0xdf
void set_3_a(struct s_gb *s_gb) { registers.a = set(1 << 3, registers.a); }

// 0xe0
void set_4_b(struct s_gb *s_gb) { registers.b = set(1 << 4, registers.b); }

// 0xe1
void set_4_c(struct s_gb *s_gb) { registers.c = set(1 << 4, registers.c); }

// 0xe2
void set_4_d(struct s_gb *s_gb) { registers.d = set(1 << 4, registers.d); }

// 0xe3
void set_4_e(struct s_gb *s_gb) { registers.e = set(1 << 4, registers.e); }

// 0xe4
void set_4_h(struct s_gb *s_gb) { registers.h = set(1 << 4, registers.h); }

// 0xe5
void set_4_l(struct s_gb *s_gb) { registers.l = set(1 << 4, registers.l); }

// 
void set_4_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 4, read8bit(registers.hl, s_gb)), s_gb); }

// 0xe7
void set_4_a(struct s_gb *s_gb) { registers.a = set(1 << 4, registers.a); }

// 0xe8
void set_5_b(struct s_gb *s_gb) { registers.b = set(1 << 5, registers.b); }

// 0xe9
void set_5_c(struct s_gb *s_gb) { registers.c = set(1 << 5, registers.c); }

// 0xea
void set_5_d(struct s_gb *s_gb) { registers.d = set(1 << 5, registers.d); }

// 0xeb
void set_5_e(struct s_gb *s_gb) { registers.e = set(1 << 5, registers.e); }

// 0xec
void set_5_h(struct s_gb *s_gb) { registers.h = set(1 << 5, registers.h); }

// 0xed
void set_5_l(struct s_gb *s_gb) { registers.l = set(1 << 5, registers.l); }

// 0xee
void set_5_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 5, read8bit(registers.hl, s_gb)), s_gb); }

// 0xef
void set_5_a(struct s_gb *s_gb) { registers.a = set(1 << 5, registers.a); }

// 0xf0
void set_6_b(struct s_gb *s_gb) { registers.b = set(1 << 6, registers.b); }

// 0xf1
void set_6_c(struct s_gb *s_gb) { registers.c = set(1 << 6, registers.c); }

// 0xf2
void set_6_d(struct s_gb *s_gb) { registers.d = set(1 << 6, registers.d); }

// 0xf3
void set_6_e(struct s_gb *s_gb) { registers.e = set(1 << 6, registers.e); }

// 0xf4
void set_6_h(struct s_gb *s_gb) { registers.h = set(1 << 6, registers.h); }

// 0xf5
void set_6_l(struct s_gb *s_gb) { registers.l = set(1 << 6, registers.l); }

// 0xf6
void set_6_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 6, read8bit(registers.hl, s_gb)), s_gb); }

// 0xf7
void set_6_a(struct s_gb *s_gb) { registers.a = set(1 << 6, registers.a); }

// 0xf8
void set_7_b(struct s_gb *s_gb) { registers.b = set(1 << 7, registers.b); }

// 0xf9
void set_7_c(struct s_gb *s_gb) { registers.c = set(1 << 7, registers.c); }

// 0xfa
void set_7_d(struct s_gb *s_gb) { registers.d = set(1 << 7, registers.d); }

// 0xfb
void set_7_e(struct s_gb *s_gb) { registers.e = set(1 << 7, registers.e); }

// 0xfc
void set_7_h(struct s_gb *s_gb) { registers.h = set(1 << 7, registers.h); }

// 0xfd
void set_7_l(struct s_gb *s_gb) { registers.l = set(1 << 7, registers.l); }

// 0xfe
void set_7_hlp(struct s_gb *s_gb) { write8bit(registers.hl, set(1 << 7, read8bit(registers.hl, s_gb)), s_gb); }

// 0xff
void set_7_a(struct s_gb *s_gb) { registers.a = set(1 << 7, registers.a); }




const struct extendedInstruction extendedInstructions[256] = {
	{ "RLC B", rlc_b },           // 0x00
	{ "RLC C", rlc_c },           // 0x01
	{ "RLC D", rlc_d },           // 0x02
	{ "RLC E", rlc_e },           // 0x03
	{ "RLC H", rlc_h },           // 0x04
	{ "RLC L", rlc_l },           // 0x05
	{ "RLC (HL)", rlc_hlp },      // 0x06
	{ "RLC A", rlc_a },           // 0x07
	{ "RRC B", rrc_b },           // 0x08
	{ "RRC C", rrc_c },           // 0x09
	{ "RRC D", rrc_d },           // 0x0a
	{ "RRC E", rrc_e },           // 0x0b
	{ "RRC H", rrc_h },           // 0x0c
	{ "RRC L", rrc_l },           // 0x0d
	{ "RRC (HL)", rrc_hlp },      // 0x0e
	{ "RRC A", rrc_a },           // 0x0f
	{ "RL B", rl_b },             // 0x10
	{ "RL C", rl_c },             // 0x11
	{ "RL D", rl_d },             // 0x12
	{ "RL E", rl_e },             // 0x13
	{ "RL H", rl_h },             // 0x14
	{ "RL L", rl_l },             // 0x15
	{ "RL (HL)", rl_hlp },        // 0x16
	{ "RL A", rl_a },             // 0x17
	{ "RR B", rr_b },             // 0x18
	{ "RR C", rr_c },             // 0x19
	{ "RR D", rr_d },             // 0x1a
	{ "RR E", rr_e },             // 0x1b
	{ "RR H", rr_h },             // 0x1c
	{ "RR L", rr_l },             // 0x1d
	{ "RR (HL)", rr_hlp },        // 0x1e
	{ "RR A", rr_a },             // 0x1f
	{ "SLA B", sla_b },           // 0x20
	{ "SLA C", sla_c },           // 0x21
	{ "SLA D", sla_d },           // 0x22
	{ "SLA E", sla_e },           // 0x23
	{ "SLA H", sla_h },           // 0x24
	{ "SLA L", sla_l },           // 0x25
	{ "SLA (HL)", sla_hlp },      // 0x26
	{ "SLA A", sla_a },           // 0x27
	{ "SRA B", sra_b },           // 0x28
	{ "SRA C", sra_c },           // 0x29
	{ "SRA D", sra_d },           // 0x2a
	{ "SRA E", sra_e },           // 0x2b
	{ "SRA H", sra_h },           // 0x2c
	{ "SRA L", sra_l },           // 0x2d
	{ "SRA (HL)", sra_hlp },      // 0x2e
	{ "SRA A", sra_a },           // 0x2f
	{ "SWAP B", swap_b },         // 0x30
	{ "SWAP C", swap_c },         // 0x31
	{ "SWAP D", swap_d },         // 0x32
	{ "SWAP E", swap_e },         // 0x33
	{ "SWAP H", swap_h },         // 0x34
	{ "SWAP L", swap_l },         // 0x35
	{ "SWAP (HL)", swap_hlp },    // 0x36
	{ "SWAP A", swap_a },         // 0x37
	{ "SRL B", srl_b },           // 0x38
	{ "SRL C", srl_c },           // 0x39
	{ "SRL D", srl_d },           // 0x3a
	{ "SRL E", srl_e },           // 0x3b
	{ "SRL H", srl_h },           // 0x3c
	{ "SRL L", srl_l },           // 0x3d
	{ "SRL (HL)", srl_hlp },      // 0x3e
	{ "SRL A", srl_a },           // 0x3f
	{ "BIT 0, B", bit_0_b },      // 0x40
	{ "BIT 0, C", bit_0_c },      // 0x41
	{ "BIT 0, D", bit_0_d },      // 0x42
	{ "BIT 0, E", bit_0_e },      // 0x43
	{ "BIT 0, H", bit_0_h },      // 0x44
	{ "BIT 0, L", bit_0_l },      // 0x45
	{ "BIT 0, (HL)", bit_0_hlp }, // 0x46
	{ "BIT 0, A", bit_0_a },      // 0x47
	{ "BIT 1, B", bit_1_b },      // 0x48
	{ "BIT 1, C", bit_1_c },      // 0x49
	{ "BIT 1, D", bit_1_d },      // 0x4a
	{ "BIT 1, E", bit_1_e },      // 0x4b
	{ "BIT 1, H", bit_1_h },      // 0x4c
	{ "BIT 1, L", bit_1_l },      // 0x4d
	{ "BIT 1, (HL)", bit_1_hlp }, // 0x4e
	{ "BIT 1, A", bit_1_a },      // 0x4f
	{ "BIT 2, B", bit_2_b },      // 0x50
	{ "BIT 2, C", bit_2_c },      // 0x51
	{ "BIT 2, D", bit_2_d },      // 0x52
	{ "BIT 2, E", bit_2_e },      // 0x53
	{ "BIT 2, H", bit_2_h },      // 0x54
	{ "BIT 2, L", bit_2_l },      // 0x55
	{ "BIT 2, (HL)", bit_2_hlp }, // 0x56
	{ "BIT 2, A", bit_2_a },      // 0x57
	{ "BIT 3, B", bit_3_b },      // 0x58
	{ "BIT 3, C", bit_3_c },      // 0x59
	{ "BIT 3, D", bit_3_d },      // 0x5a
	{ "BIT 3, E", bit_3_e },      // 0x5b
	{ "BIT 3, H", bit_3_h },      // 0x5c
	{ "BIT 3, L", bit_3_l },      // 0x5d
	{ "BIT 3, (HL)", bit_3_hlp }, // 0x5e
	{ "BIT 3, A", bit_3_a },      // 0x5f
	{ "BIT 4, B", bit_4_b },      // 0x60
	{ "BIT 4, C", bit_4_c },      // 0x61
	{ "BIT 4, D", bit_4_d },      // 0x62
	{ "BIT 4, E", bit_4_e },      // 0x63
	{ "BIT 4, H", bit_4_h },      // 0x64
	{ "BIT 4, L", bit_4_l },      // 0x65
	{ "BIT 4, (HL)", bit_4_hlp }, // 0x66
	{ "BIT 4, A", bit_4_a },      // 0x67
	{ "BIT 5, B", bit_5_b },      // 0x68
	{ "BIT 5, C", bit_5_c },      // 0x69
	{ "BIT 5, D", bit_5_d },      // 0x6a
	{ "BIT 5, E", bit_5_e },      // 0x6b
	{ "BIT 6, H", bit_5_h },      // 0x6c
	{ "BIT 6, L", bit_5_l },      // 0x6d
	{ "BIT 5, (HL)", bit_5_hlp }, // 0x6e
	{ "BIT 5, A", bit_5_a },      // 0x6f
	{ "BIT 6, B", bit_6_b },      // 0x70
	{ "BIT 6, C", bit_6_c },      // 0x71
	{ "BIT 6, D", bit_6_d },      // 0x72
	{ "BIT 6, E", bit_6_e },      // 0x73
	{ "BIT 6, H", bit_6_h },      // 0x74
	{ "BIT 6, L", bit_6_l },      // 0x75
	{ "BIT 6, (HL)", bit_6_hlp }, // 0x76
	{ "BIT 6, A", bit_6_a },      // 0x77
	{ "BIT 7, B", bit_7_b },      // 0x78
	{ "BIT 7, C", bit_7_c },      // 0x79
	{ "BIT 7, D", bit_7_d },      // 0x7a
	{ "BIT 7, E", bit_7_e },      // 0x7b
	{ "BIT 7, H", bit_7_h },      // 0x7c
	{ "BIT 7, L", bit_7_l },      // 0x7d
	{ "BIT 7, (HL)", bit_7_hlp }, // 0x7e
	{ "BIT 7, A", bit_7_a },      // 0x7f
	{ "RES 0, B", res_0_b },      // 0x80
	{ "RES 0, C", res_0_c },      // 0x81
	{ "RES 0, D", res_0_d },      // 0x82
	{ "RES 0, E", res_0_e },      // 0x83
	{ "RES 0, H", res_0_h },      // 0x84
	{ "RES 0, L", res_0_l },      // 0x85
	{ "RES 0, (HL)", res_0_hlp }, // 0x86
	{ "RES 0, A", res_0_a },      // 0x87
	{ "RES 1, B", res_1_b },      // 0x88
	{ "RES 1, C", res_1_c },      // 0x89
	{ "RES 1, D", res_1_d },      // 0x8a
	{ "RES 1, E", res_1_e },      // 0x8b
	{ "RES 1, H", res_1_h },      // 0x8c
	{ "RES 1, L", res_1_l },      // 0x8d
	{ "RES 1, (HL)", res_1_hlp }, // 0x8e
	{ "RES 1, A", res_1_a },      // 0x8f
	{ "RES 2, B", res_2_b },      // 0x90
	{ "RES 2, C", res_2_c },      // 0x91
	{ "RES 2, D", res_2_d },      // 0x92
	{ "RES 2, E", res_2_e },      // 0x93
	{ "RES 2, H", res_2_h },      // 0x94
	{ "RES 2, L", res_2_l },      // 0x95
	{ "RES 2, (HL)", res_2_hlp }, // 0x96
	{ "RES 2, A", res_2_a },      // 0x97
	{ "RES 3, B", res_3_b },      // 0x98
	{ "RES 3, C", res_3_c },      // 0x99
	{ "RES 3, D", res_3_d },      // 0x9a
	{ "RES 3, E", res_3_e },      // 0x9b
	{ "RES 3, H", res_3_h },      // 0x9c
	{ "RES 3, L", res_3_l },      // 0x9d
	{ "RES 3, (HL)", res_3_hlp }, // 0x9e
	{ "RES 3, A", res_3_a },      // 0x9f
	{ "RES 4, B", res_4_b },      // 0xa0
	{ "RES 4, C", res_4_c },      // 0xa1
	{ "RES 4, D", res_4_d },      // 0xa2
	{ "RES 4, E", res_4_e },      // 0xa3
	{ "RES 4, H", res_4_h },      // 0xa4
	{ "RES 4, L", res_4_l },      // 0xa5
	{ "RES 4, (HL)", res_4_hlp }, // 0xa6
	{ "RES 4, A", res_4_a },      // 0xa7
	{ "RES 5, B", res_5_b },      // 0xa8
	{ "RES 5, C", res_5_c },      // 0xa9
	{ "RES 5, D", res_5_d },      // 0xaa
	{ "RES 5, E", res_5_e },      // 0xab
	{ "RES 5, H", res_5_h },      // 0xac
	{ "RES 5, L", res_5_l },      // 0xad
	{ "RES 5, (HL)", res_5_hlp }, // 0xae
	{ "RES 5, A", res_5_a },      // 0xaf
	{ "RES 6, B", res_6_b },      // 0xb0
	{ "RES 6, C", res_6_c },      // 0xb1
	{ "RES 6, D", res_6_d },      // 0xb2
	{ "RES 6, E", res_6_e },      // 0xb3
	{ "RES 6, H", res_6_h },      // 0xb4
	{ "RES 6, L", res_6_l },      // 0xb5
	{ "RES 6, (HL)", res_6_hlp }, // 0xb6
	{ "RES 6, A", res_6_a },      // 0xb7
	{ "RES 7, B", res_7_b },      // 0xb8
	{ "RES 7, C", res_7_c },      // 0xb9
	{ "RES 7, D", res_7_d },      // 0xba
	{ "RES 7, E", res_7_e },      // 0xbb
	{ "RES 7, H", res_7_h },      // 0xbc
	{ "RES 7, L", res_7_l },      // 0xbd
	{ "RES 7, (HL)", res_7_hlp }, // 0xbe
	{ "RES 7, A", res_7_a },      // 0xbf
	{ "SET 0, B", set_0_b },      // 0xc0
	{ "SET 0, C", set_0_c },      // 0xc1
	{ "SET 0, D", set_0_d },      // 0xc2
	{ "SET 0, E", set_0_e },      // 0xc3
	{ "SET 0, H", set_0_h },      // 0xc4
	{ "SET 0, L", set_0_l },      // 0xc5
	{ "SET 0, (HL)", set_0_hlp }, // 0xc6
	{ "SET 0, A", set_0_a },      // 0xc7
	{ "SET 1, B", set_1_b },      // 0xc8
	{ "SET 1, C", set_1_c },      // 0xc9
	{ "SET 1, D", set_1_d },      // 0xca
	{ "SET 1, E", set_1_e },      // 0xcb
	{ "SET 1, H", set_1_h },      // 0xcc
	{ "SET 1, L", set_1_l },      // 0xcd
	{ "SET 1, (HL)", set_1_hlp }, // 0xce
	{ "SET 1, A", set_1_a },      // 0xcf
	{ "SET 2, B", set_2_b },      // 0xd0
	{ "SET 2, C", set_2_c },      // 0xd1
	{ "SET 2, D", set_2_d },      // 0xd2
	{ "SET 2, E", set_2_e },      // 0xd3
	{ "SET 2, H", set_2_h },      // 0xd4
	{ "SET 2, L", set_2_l },      // 0xd5
	{ "SET 2, (HL)", set_2_hlp }, // 0xd6
	{ "SET 2, A", set_2_a },      // 0xd7
	{ "SET 3, B", set_3_b },      // 0xd8
	{ "SET 3, C", set_3_c },      // 0xd9
	{ "SET 3, D", set_3_d },      // 0xda
	{ "SET 3, E", set_3_e },      // 0xdb
	{ "SET 3, H", set_3_h },      // 0xdc
	{ "SET 3, L", set_3_l },      // 0xdd
	{ "SET 3, (HL)", set_3_hlp }, // 0xde
	{ "SET 3, A", set_3_a },      // 0xdf
	{ "SET 4, B", set_4_b },      // 0xe0
	{ "SET 4, C", set_4_c },      // 0xe1
	{ "SET 4, D", set_4_d },      // 0xe2
	{ "SET 4, E", set_4_e },      // 0xe3
	{ "SET 4, H", set_4_h },      // 0xe4
	{ "SET 4, L", set_4_l },      // 0xe5
	{ "SET 4, (HL)", set_4_hlp }, // 0xe6
	{ "SET 4, A", set_4_a },      // 0xe7
	{ "SET 5, B", set_5_b },      // 0xe8
	{ "SET 5, C", set_5_c },      // 0xe9
	{ "SET 5, D", set_5_d },      // 0xea
	{ "SET 5, E", set_5_e },      // 0xeb
	{ "SET 5, H", set_5_h },      // 0xec
	{ "SET 5, L", set_5_l },      // 0xed
	{ "SET 5, (HL)", set_5_hlp }, // 0xee
	{ "SET 5, A", set_5_a },      // 0xef
	{ "SET 6, B", set_6_b },      // 0xf0
	{ "SET 6, C", set_6_c },      // 0xf1
	{ "SET 6, D", set_6_d },      // 0xf2
	{ "SET 6, E", set_6_e },      // 0xf3
	{ "SET 6, H", set_6_h },      // 0xf4
	{ "SET 6, L", set_6_l },      // 0xf5
	{ "SET 6, (HL)", set_6_hlp }, // 0xf6
	{ "SET 6, A", set_6_a },      // 0xf7
	{ "SET 7, B", set_7_b },      // 0xf8
	{ "SET 7, C", set_7_c },      // 0xf9
	{ "SET 7, D", set_7_d },      // 0xfa
	{ "SET 7, E", set_7_e },      // 0xfb
	{ "SET 7, H", set_7_h },      // 0xfc
	{ "SET 7, L", set_7_l },      // 0xfd
	{ "SET 7, (HL)", set_7_hlp }, // 0xfe
	{ "SET 7, A", set_7_a },      // 0xff
};

const unsigned char extendedInstructionTicks[256] = {
	8, 8, 8, 8, 8,  8, 16, 8,  8, 8, 8, 8, 8, 8, 16, 8, // 0x0_
	8, 8, 8, 8, 8,  8, 16, 8,  8, 8, 8, 8, 8, 8, 16, 8, // 0x1_
	8, 8, 8, 8, 8,  8, 16, 8,  8, 8, 8, 8, 8, 8, 16, 8, // 0x2_
	8, 8, 8, 8, 8,  8, 16, 8,  8, 8, 8, 8, 8, 8, 16, 8, // 0x3_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x4_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x5_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x6_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x7_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x8_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0x9_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0xa_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0xb_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0xc_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0xd_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8, // 0xe_
	8, 8, 8, 8, 8,  8, 12, 8,  8, 8, 8, 8, 8, 8, 12, 8  // 0xf_
};


void	handleCB(unsigned char cbopcode, struct s_gb *s_gb)
{
	extendedInstructions[cbopcode].execute(s_gb);
	s_gb->gb_cpu.totalTick += extendedInstructionTicks[cbopcode];
}
