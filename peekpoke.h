static inline void write_l(unsigned int addr, unsigned int val)
{
	volatile unsigned int *ptr = (unsigned int *)(addr);

	*ptr = val;
}

static inline unsigned int read_l(unsigned int addr)
{
	volatile unsigned int *ptr = (unsigned int *)(addr);

	return *ptr;
}

