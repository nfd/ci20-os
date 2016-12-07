#ifndef MIPS32_PEEKPOKE_H
#define MIPS32_PEEKPOKE_H
static inline void poke32(unsigned int addr, unsigned int val) {
	*((volatile unsigned int *)addr) = val;
}

static inline unsigned int peek32(unsigned int addr) {
	return *((volatile unsigned int *)addr);
}

#endif // MIPS32_PEEKPOKE_H
