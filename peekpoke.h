static inline void poke16(unsigned int addr, unsigned short val)
{
	volatile unsigned short *ptr = (unsigned short *)(addr);

	*ptr = val;
}

static inline void poke32(unsigned int addr, unsigned int val)
{
	volatile unsigned int *ptr = (unsigned int *)(addr);

	*ptr = val;
}

static inline unsigned int peek32(unsigned int addr)
{
	volatile unsigned int *ptr = (unsigned int *)(addr);

	return *ptr;
}

static inline void update_l(unsigned int addr, unsigned int val)
{
	poke32(addr, read_l(addr) | val);
}
