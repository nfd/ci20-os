import math
import argparse
from collections import namedtuple

import referenceddr

NS = namedtuple('NS', ['val'])
TCK = namedtuple('TCK', ['val'])

class Span:
	def __init__(self, expr, ram, is_ticks=False):
		self.expr = expr # The span in NS, as a string requiring eval() or as a float or int.
		self.ram = ram
		self.is_ticks = is_ticks

	@property
	def val(self):
		if isinstance(self.expr, (float, int)):
			return self.expr
		else:
			env = {'ram': self.ram,
					'nCK': self.ram.nCK,
					'math': math}
			return eval(self.expr, env)

	@property
	def ns(self):
		" The time span in nanoseconds "
		return self.ram.ticks_to_ns(self.val) if self.is_ticks else self.val
	
	@property
	def ticks(self):
		" The time span in units of tCK "
		return self.ram.ns_to_ticks(self.ns) if not self.is_ticks else self.val

class RAMInfo:
	def __init__(self, name, ddr_speed_hz, follow_reference_code=True):
		self.name = name
		self.ddr_speed_hz = ddr_speed_hz
		self.nCK = 1000000000/ddr_speed_hz # nanoseconds per DDR clock cycle.
		self.follow_reference_code = follow_reference_code

	def ns_to_ticks(self, ns):
		return int(math.ceil(ns / self.nCK))

	def ticks_to_ns(self, ticks):
		return ticks * self.nCK

	def __setattr__(self, name, val):
		if isinstance(val, NS):
			val = Span(val.val, self)
		elif isinstance(val, TCK):
			val = Span(val.val, self, is_ticks=True)

		super().__setattr__(name, val)

	def size(self, rank=1):
		return (2**self.rows) * (2**self.columns) * self.banks * (self.bus_width_bits // 8)

	def size_mask(self, rank=1):
		size = self.size(rank=rank)
		if self.follow_reference_code:
			size *= 2 # See comments around DMMAP0 register configuration

		return 0x100000000 - size


class RegPart:
	" Represents a contiguous named portion of a memory-mapped device register "

	def __init__(self, shift=0, lim=0, choices=None):
		self.shift = shift
		self.lim = lim
		self.choices = choices
		self.mask = ((1 << (lim - shift + 1)) - 1) << shift

	def get_val(self, val):
		if isinstance(val, str):
			assert isinstance(self.choices, dict), (val, self.choices)
			val = self.choices[val]

		maxval  = (1 << (self.lim - self.shift + 1)) - 1
		assert val <= maxval, (val, maxval)
		return val << self.shift

	def decode(self, val):
		val &= self.mask
		val >>= self.shift
		if isinstance(self.choices, dict):
			inverse = {value: key for key, value in self.choices.items()}
			try:
				return inverse[val]
			except KeyError:
				return val
		else:
			return val
	
	@property
	def zero(self):
		if not self.choices:
			return 0
		else:
			for key, val in self.choices.items():
				if val == 0:
					return key
			else:
				raise ValueError()

class SparseRegPart:
	" Represents a discontiguous named portion of a memory-mapped device register "
	def __init__(self, bits=None, choices=None):
		self.bits = list(sorted(bits)) # sorted low to high
		self.choices = choices
		self.maxval = (1 << len(self.bits)) - 1

	def get_val(self, val):
		if isinstance(val, str):
			assert isinstance(self.choices, dict), val
			val = self.choices[val]

		assert val <= self.maxval, (val, maxval)

		bitvals = []
		while val:
			if val & 1:
				bitvals.append(1)
			else:
				bitvals.append(0)
			val >>= 2

		regval = 0
		for bit, val in zip(self.bits, bitvals):
			if val:
				regval |= (1 << bit)

		return regval

	def decode(self, sparseval):
		val = 0
		for bit in reversed(self.bits): # high to low
			val <<= 1
			if sparseval & (1 << bit):
				val |= 1


		# TODO clag
		if isinstance(self.choices, dict):
			inverse = {value: key for key, value in self.choices.items()}
			try:
				return inverse[val]
			except KeyError:
				return val
		else:
			return val
		# end clag


def RegFlag(bit=0):
	return RegPart(shift=bit, lim=bit)

REGISTERS = {'CPM.DRCG': {
				'DLLRESET': RegFlag(bit=1),
				'CFGCLKEN': RegFlag(bit=0) },
			'DDR.DCTRL': {
				'DFI_RST': RegFlag(bit=23),
				'DLL_RST': RegFlag(bit=22),
				'CTL_RST': RegFlag(bit=21),
				'CFG_RST': RegFlag(bit=20),
				'KEEPSR': RegFlag(bit=17),
				'ACTPD': RegFlag(bit=15),
				'PDT': RegPart(shift=12, lim=14, choices={'NEVER': 0, 'TCK8': 1,
					'TCK16': 2, 'TCK32': 3, 'TCK64': 4, 'TCK128': 5}),
				'ACTSTP': RegFlag(bit=11),
				'RESERVED1': RegFlag(bit=8),
				'DPD': RegFlag(bit=6),
				'SR': RegFlag(bit=5),
				'UNALIGN': RegFlag(bit=4),
				'ALH': RegFlag(bit=3),
				'RESERVED2': RegFlag(bit=2),
				'CKE': RegFlag(bit=1),
				'RESET': RegFlag(bit=0) },
			'DDR.DTIMING1': {
				'tRTP': RegPart(shift=24, lim=29),
				'tWTR': RegPart(shift=16, lim=21),
				'tWR': RegPart(shift=8, lim=13),
				'tWL': RegPart(shift=0, lim=5) },
			'DDR.DTIMING2': {
				'tCCD': RegPart(shift=24, lim=29),
				'tRAS': RegPart(shift=16, lim=21),
				'tRCD': RegPart(shift=8, lim=13),
				'tRL': RegPart(shift=0, lim=5) },
			'DDR.DTIMING3': {
				'ONUM': RegPart(shift=27, lim=30),
				'tCKSRE': RegPart(shift=24, lim=26),
				'tRP': RegPart(shift=16, lim=21),
				'tRRD': RegPart(shift=8, lim=13),
				'tRC': RegPart(shift=0, lim=5) },
			'DDR.DTIMING4': {
				'tRFC': RegPart(shift=24, lim=29),
				'tEXTRW': RegPart(shift=21, lim=23),
				'tRWCOV': RegPart(shift=19, lim=20),
				'tCKE': RegPart(shift=16, lim=18),
				'tMINSR': RegPart(shift=8, lim=11),
				'tXP': RegPart(shift=4, lim=6),
				'tMRD': RegPart(shift=0, lim=1) },
			'DDR.DTIMING5': {
				'tCTLUPD': RegPart(shift=24, lim=31),
				'tRTW': RegPart(shift=16, lim=21),
				'tRDLAT': RegPart(shift=8, lim=13),
				'tWDLAT': RegPart(shift=0, lim=5) },
			'DDR.DTIMING6': {
				'tXSRD': RegPart(shift=24, lim=31),
				'tFAW': RegPart(shift=16, lim=21),
				'tCFGW': RegPart(shift=8, lim=13),
				'tCFGR': RegPart(shift=0, lim=5) },
			'DDR.DCFG': { # DDR control config
				'ROW1':	RegPart(shift=27, lim=29),
				'COL1':	RegPart(shift=24, lim=26),
				'BA1':	RegPart(shift=23, lim=23, choices={'BANKS4': 0, 'BANKS8': 1}),
				'IMBA':	RegFlag(bit=22),
				'BSL':	RegPart(shift=21, lim=21, choices={'BURST4': 0, 'BURST8': 1}),
				'TYPE':	RegPart(shift=17, lim=19, choices={'LPDDR': 3, 'DDR2': 4, 'LPDDR2': 5, 'DDR3': 6}),
				'ODTEN': RegFlag(bit=16),
				'MISPE': RegFlag(bit=15),
				'ROW0':	RegPart(shift=11, lim=13),
				'COL0':	RegPart(shift=8, lim=10),
				'CS1EN': RegFlag(bit=7),
				'CS0EN': RegFlag(bit=6),
				'CL': RegPart(shift=2, lim=5),
				'BA0': RegPart(shift=1, lim=1, choices={'BANKS4': 0, 'BANKS8': 1}),
				'DW': RegPart(shift=0, lim=0, choices={'WIDTH16': 0, 'WIDTH32': 1})},
			'DDR.DMMAP0': {
				'BASE': RegPart(shift=8, lim=15),
				'MASK': RegPart(shift=0, lim=7)},
			'DDR.DMMAP1': {
				'BASE': RegPart(shift=8, lim=15),
				'MASK': RegPart(shift=0, lim=7)},
			'DDR.DREFCNT': {
				'CON': RegPart(shift=16, lim=23),
				'CNT': RegPart(shift=8, lim=15),
				'CLK_DIV': RegPart(shift=1, lim=3, choices={'DIV16': 0, 'DIV32': 1, 'DIV64': 2, 'DIV128': 3,
					'DIV256': 4, 'DIV512': 5, 'DIV1024': 6}),
				'REF_EN': RegFlag(bit=0)},
			'DDR.DSTATUS': {
				'ENDIAN': RegFlag(bit=7),
				'MISS': RegFlag(bit=6),
				'DPDN': RegFlag(bit=5),
				'PDN': RegFlag(bit=4),
				'AREF': RegFlag(bit=3),
				'SREF': RegFlag(bit=2),
				'CKE0': RegFlag(bit=0)},
			'DDR.DREMAP1': {
				'BIT3MP': RegPart(shift=24, lim=28),
				'BIT2MP': RegPart(shift=16, lim=20),
				'BIT1MP': RegPart(shift=8, lim=12),
				'BIT0MP': RegPart(shift=0, lim=4)},
			'DDR.DREMAP2': {
				'BIT7MP': RegPart(shift=24, lim=28),
				'BIT6MP': RegPart(shift=16, lim=20),
				'BIT5MP': RegPart(shift=8, lim=12),
				'BIT4MP': RegPart(shift=0, lim=4)},
			'DDR.DREMAP3': {
				'BIT11MP': RegPart(shift=24, lim=28),
				'BIT10MP': RegPart(shift=16, lim=20),
				'BIT9MP': RegPart(shift=8, lim=12),
				'BIT8MP': RegPart(shift=0, lim=4)},
			'DDR.DREMAP4': {
				'BIT15MP': RegPart(shift=24, lim=28),
				'BIT14MP': RegPart(shift=16, lim=20),
				'BIT13MP': RegPart(shift=8, lim=12),
				'BIT12MP': RegPart(shift=0, lim=4)},
			'DDR.DREMAP5': {
				'BIT19MP': RegPart(shift=24, lim=28),
				'BIT18MP': RegPart(shift=16, lim=20),
				'BIT17MP': RegPart(shift=8, lim=12),
				'BIT16MP': RegPart(shift=0, lim=4)},
			'DDRP.DTAR': { # Parts are made up, could be wrong
				'BANK': RegPart(shift=28, lim=31),
				'ROW': RegPart(shift=12, lim=27),
				'COL': RegPart(shift=0, lim=11)},
			'DDRP.DCR': { # PHY configuration register
				'TYPE': RegPart(shift=0, lim=2, choices={'MDDR': 0, 'DDR': 1, 'DDR2': 2, 'DDR3': 3, 'LPDDR2': 4}),
				'DDR8BNK': RegFlag(bit=3)}, # Other parts unknown
			'DDRP.MR0': { # PHY mode register 0
				'WR': RegPart(shift=9, lim=11),
				'CL': RegPart(shift=4, lim=6),
				'BL': RegPart(shift=0, lim=1, choices={'BL8': 0, 'BLDYNAMIC': 1, 'BL4': 2})},
			'DDRP.MR1': { # PHY mode register 1
				'RTT': SparseRegPart(bits=(9, 6, 2), choices={'DIS': 0, 'RZQ4': 1, 'RZQ2': 2,
					'RZQ6': 3, 'RZQ12': 4, 'RZQ8': 5}),
				'DIC': SparseRegPart(bits=(5, 1), choices={'RZQ6': 0, 'RZQ7': 1})},
			'DDRP.MR2': { # PHY mode register 2
				'tCWL': RegPart(shift=3, lim=5) },
			'DDRP.ODTCR': { # PHY on-die termination control register
				'UNKNOWN': RegPart(shift=0, lim=31)},
			'DDRP.PTR0': { # PHY timing register 0
				'ITMSRST': RegPart(shift=18, lim=21),
				'DLLLOCK': RegPart(shift=6, lim=17),
				'PLLRST': RegPart(shift=0, lim=5)},
			'DDRP.PTR1': { # PHY timing register 1
				'DINIT0': RegPart(shift=0, lim=18),
				'DINIT1': RegPart(shift=19, lim=26)},
			'DDRP.PTR2': { # PHY timing register 2
				'DINIT2': RegPart(shift=0, lim=16),
				'DINIT3': RegPart(shift=17, lim=27)},
			'DDRP.DTPR0': { # PHY DRAM timing parameters register 0
				'tCCD_ABOVE_4': RegFlag(bit=31),
				'tRC': RegPart(shift=25, lim=30),
				'tRRD': RegPart(shift=21, lim=24),
				'tRAS': RegPart(shift=16, lim=20),
				'tRCD': RegPart(shift=12, lim=15),
				'tRP': RegPart(shift=8, lim=11),
				'tWTR': RegPart(shift=5, lim=7),
				'tRTP': RegPart(shift=2, lim=4),
				'tMRD': RegPart(shift=0, lim=1)},
			'DDRP.DTPR1': { # PHY DRAM timing parameters register 1
				'tRFC': RegPart(shift=16, lim=23),
				'POSTAMBLE_ODT_DELAY': RegPart(shift=11, lim=15),
				'tMOD': RegPart(shift=9, lim=10),
				'tFAW': RegPart(shift=3, lim=7)},
			'DDRP.DTPR2': { # PHY DRAM timing parameters register 2
				'tDLLLOCK': RegPart(shift=19, lim=28),
				'tCKE': RegPart(shift=15, lim=18),
				'tXP': RegPart(shift=10, lim=14),
				'tXS': RegPart(shift=0, lim=9)},
			'DDRP.PGCR': { # PHY general configuration register
				'ITMDMD': RegFlag(bit=0),
				'DQSCFG': RegFlag(bit=1),
				'DFTCMP': RegFlag(bit=2),
				'CKEN': RegPart(shift=9, lim=11),
				'CKDV': RegPart(shift=12, lim=13),
				'RANKEN': RegPart(shift=18, lim=19),
				'ZCKSEL': RegPart(shift=22, lim=23, choices={'DIV_2': 0, 'DIV_8': 1, 'DIV_32': 2, 'DIV_64': 3}),
				'PDDISDX': RegFlag(bit=24)},
			'DDRP.PGSR': { # PHY general status register
				'IDONE': RegFlag(bit=0),
				'DLDONE': RegFlag(bit=1),
				'ZCDONE': RegFlag(bit=2),
				'DIDONE': RegFlag(bit=3),
				'DTDONE': RegFlag(bit=4),
				'DTERR': RegFlag(bit=5),
				'DTIERR': RegFlag(bit=6),
				'DFTEERR': RegFlag(bit=7)},
			'DDRP.DXGCR': { # PHY DATAX configuration register (x8)
				'instances': 8,
				'UNKNOWN': RegPart(shift=9, lim=10),
				'DXEN': RegFlag(bit=0), },
			'DDRP.PIR': { # PHY initialisation register
				'INIT': RegFlag(bit=0),
				'DLLSRST': RegFlag(bit=1),
				'DLLLOCK': RegFlag(bit=2),
				'ZCAL': RegFlag(bit=3),
				'ITMSRST': RegFlag(bit=4),
				'DRAMRST': RegFlag(bit=5),
				'DRAMINT': RegFlag(bit=6),
				'QSTRN': RegFlag(bit=7),
				'EYETRN': RegFlag(bit=8),
				'DLLBYP': RegFlag(bit=17)},
			'DDRP.ZQXCR0': { # PHY impedance control register 0
				'PULLDOWN_IMPEDANCE': RegPart(shift=0, lim=4),
				'PULLUP_IMPEDANCE': RegPart(shift=5, lim=9),
				'UNKNOWN_1': RegPart(shift=10, lim=27),
				'ZDEN': RegFlag(bit=28),
				'UNKNOWN_2': RegPart(shift=29, lim=31)},
}

AUTOGEN_HEADER = """\
/* Do not edit -- auto-generated file.
 * See ddr.py for details.
*/

#include "libci20/peekpoke.h"
#include "ci20board.h"
#include "libci20/timer.h"

static void read_clear_write(uint32_t addr, uint32_t mask)
{
	uint32_t val = peek32(addr);
	val &= ~mask;
	poke32(addr, val);
}

static void spin_until(uint32_t addr, uint32_t val)
{
	while((peek32(addr) & val) != val)
		;
}

static void read_mask_update(uint32_t addr, uint32_t mask, uint32_t update)
{
	uint32_t val = peek32(addr);
	val &= mask;
	val |= update;
	poke32(addr, val);
}

void ddr_init(void)
{
"""

AUTOGEN_FOOTER = "}"

class AutogenOutput:
	def begin(self):
		print(AUTOGEN_HEADER)

	def end(self):
		print(AUTOGEN_FOOTER)

	def note(self, text):
		print('\t/* %s */' % (text))

	def _reg_names(self, name):
		reg = REGISTERS[name]
		if 'instances' in reg:
			for instance in range(reg['instances']):
				yield reg, name.replace('.', '_') + str(instance)
		else:
			yield reg, name.replace('.', '_')

	def _calculate_write_val(self, reg, parts):
		regval = 0

		for partname, partval in parts.items():
			regpart = reg[partname]
			partval = regpart.get_val(partval)
			regval |= partval

		return regval

	def write_register(self, name, **kw):
		comment = '\t/* %s <- %s */' % (name, ', '.join('%s:%s' % (key, val) for key, val in sorted(kw.items())))
		print(comment)

		for reg, name in self._reg_names(name):
			regval = self._calculate_write_val(reg, kw)

			print("\tpoke32(%s, 0x%08x);" % (name, regval))
		print()
	
	def write_register_raw(self, name, val):
		for reg, name in self._reg_names(name):
			print("\tpoke32(%s, 0x%08x);" % (name, val))

	def read_clear_write(self, name, partnames):
		for reg, name in self._reg_names(name):
			mask = 0x0

			for partname in partnames:
				regpart = reg[partname]
				mask |= regpart.mask

			print("\tread_clear_write(%s, 0x%08x);" % (name, mask))
	
	def delay_us(self, usec):
		print('\tspintimer_usleep(%d);' % (usec))

	def spin_until_contains(self, name, partnames):
		reg = REGISTERS[name]

		mask = 0
		for partname in partnames:
			regpart = reg[partname]
			mask |= regpart.mask

		print('\tspin_until(%s, 0x%08x);' % (name.replace('.', '_'), mask))
	
	def read_update_write(self, name, preserve=None, **kw):
		mask = 0
		reg = REGISTERS[name]
		for partname in preserve:
			mask |= reg[partname].mask

		for reg, name in self._reg_names(name):
			regval = self._calculate_write_val(reg, kw)
			#print("/*%-12s <= (val & 0x%08x) | 0x%08x*/" % (name, mask, regval))
			print("\tread_mask_update(%s, 0x%08x, 0x%08x);" % (name, mask, regval))

def init_phy(hardware, ram):
	assert ram.burst_length in (4, 8)
	assert ram.ranks in (1, 2)

	# Bank, row, and column to use for training.
	hardware.write_register('DDRP.DTAR', BANK=0, ROW=0x150, COL=0)

	# Physical attributes of the RAM 
	phy_wr = ram.tWR.ticks -4 if ram.tWR.ticks <= 8 else ram.tWR.ticks / 2
	phy_bl = 'BL8' if ram.burst_length == 8 else 'BL4'

	hardware.write_register('DDRP.DCR', TYPE='DDR3', DDR8BNK=1)
	hardware.write_register('DDRP.MR0', WR=phy_wr, CL=ram.tCL.ticks - 4, BL=phy_bl)

	# Electrical characteristics of the board / RAM
	hardware.write_register('DDRP.MR1', DIC='RZQ7', RTT='DIS')
	hardware.write_register_raw('DDRP.ODTCR', 0) # No on-die termination

	# One more RAM timing attribute.
	hardware.write_register('DDRP.MR2', tCWL=ram.tCWL.ticks - 5)

	# Timing attributes related to the PHY itself.
	hardware.write_register('DDRP.PTR0', ITMSRST=8, DLLLOCK=ram.phy_dlllock.ticks,
			PLLRST=ram.ns_to_ticks(50))

	ptr1_unknown_delay_ns = max(ram.ticks_to_ns(5), (ram.tRFC.ns + 10))
	hardware.write_register('DDRP.PTR1', DINIT0=ram.ns_to_ticks(500000),
			DINIT1=ram.ns_to_ticks(ptr1_unknown_delay_ns))

	hardware.write_register('DDRP.PTR2', DINIT2=ram.ns_to_ticks(200000),
			DINIT3=512)

	# DDR timing attributes replicated in the PHY.
	hardware.write_register('DDRP.DTPR0', 
			tCCD_ABOVE_4=1 if ram.tCCD.ticks > 4 else 0,
			tRC=ram.tRC.ticks,
			tRRD=ram.tRRD.ticks,
			tRAS=ram.tRAS.ticks,
			tRCD=ram.tRCD.ticks,
			tRP=ram.tRP.ticks,
			tWTR=ram.tWTR.ticks,
			tRTP=ram.tRTP.ticks,
			tMRD=ram.tMRD.ticks - 4)

	hardware.write_register('DDRP.DTPR1',
			tRFC=ram.tRFC.ticks,
			POSTAMBLE_ODT_DELAY=1, # Number of clocks to wait after postamble before enabling ODT?
			tMOD=ram.tMOD.ticks - 12,
			tFAW=ram.tFAW.ticks)

	hardware.write_register('DDRP.DTPR2',
			tDLLLOCK=ram.tDLLLOCK.ticks, # Ticks to wait for DLL to stabilise?
			tCKE=ram.phy_dtpr2_tCKE.ticks,
			tXP=max(ram.tXP.ticks, ram.tXPDLL.ticks),
			tXS=max(ram.tXS.ticks, ram.tXSDLL.ticks))

	# General configuration
	hardware.write_register('DDRP.PGCR',
			DQSCFG=1, # Enable DQS (data strobe) configuration?
			CKEN=7, # Enable CK line (clock) for SDRAM (but why=7?)
			CKDV=2, # No idea
			RANKEN=0b01 if ram.ranks == 1 else 0b11,
			ZCKSEL='DIV_32',
			PDDISDX=1, # Enable power-down of individual DATX8 bytes when disabled
	)

	hardware.note('Begin training')

	# Clear an unknown two bits from each DXGCR (enable DATAX?)
	# NB these registers are instances so this will clear all of them.
	hardware.read_clear_write('DDRP.DXGCR', ('UNKNOWN',))

	# Wait until initialised
	hardware.spin_until_contains('DDRP.PGSR', ('IDONE', 'DLDONE', 'ZCDONE'))

	# Reset the controller.
	hardware.write_register('DDRP.PIR', INIT=1, DRAMINT=1, DRAMRST=1)
	hardware.spin_until_contains('DDRP.PGSR', ('IDONE', 'DLDONE', 'ZCDONE', 'DIDONE'))

	# Begin QS training.
	hardware.write_register('DDRP.PIR', INIT=1, QSTRN=1)
	hardware.spin_until_contains('DDRP.PGSR', ('IDONE', 'DLDONE', 'ZCDONE', 'DIDONE', 'DTDONE'))

	# Write impedance control (not sure what the values mean).
	hardware.read_update_write('DDRP.ZQXCR0',
			preserve=('UNKNOWN_1', 'UNKNOWN_2'),
			PULLUP_IMPEDANCE=0xe,
			PULLDOWN_IMPEDANCE=0xe,
			ZDEN=1)

def remap_memory(hardware, ram):
	# We want to ensure that multiple banks are used frequently.  If the
	# current memory access layout looks like this:
	#
	# (highest bit) (banks) (rows) (columns - protected) (lowest bit)
	#
	# ... we want to remap it so it looks like this:
	#
	# (highest bit) (rows) (banks) (columns - protected) (lowest bit)
	#
	# ... the idea being that memory access is faster when accesses are spread
	# out across all the banks, and moving them to lower bits will help that
	# process thanks to the principle of locality.

	assert ram.ranks == 1 # Behaviour is slightly different for two ranks

	ilog2 = lambda val: int(math.log2(val))

	# The normal organisation, where each bit is remapped to itself.  Note that
	# the lower 12 bits are protected, so we can only change the upper 20.
	bits_mapped = {num: num for num in range(20)} 

	# Find the highest bit of address space which will be used. This is the
	# number of bits which we need to represent the highest addressable value
	# in the RAM, minus the number of bits we need to represent the highest
	# addressable bank...
	first_bank_bit = ilog2(ram.size()) - ilog2(ram.banks)
	
	#... except that the jz4780 doesn't let us mess with the lower 12 bits,
	# so we have to pretend they don't exist.
	first_bank_bit -= 12 # Lower 12 bits are protected.

	# Now we can actually do the work: swap the bank bits with the lowest
	# possible bits.
	for new_bank_bit in range(ilog2(ram.banks)):
		old_bank_bit = first_bank_bit + new_bank_bit

		bits_mapped[old_bank_bit], bits_mapped[new_bank_bit] = \
				bits_mapped[new_bank_bit], bits_mapped[old_bank_bit]
	
	# TODO we actually end up only changing three registers with the above
	# arrangement, so this is a little wasteful.
	hardware.write_register('DDR.DREMAP1',
			BIT3MP=bits_mapped[3], BIT2MP=bits_mapped[2],
			BIT1MP=bits_mapped[1], BIT0MP=bits_mapped[0])

	hardware.write_register('DDR.DREMAP2',
			BIT7MP=bits_mapped[7], BIT6MP=bits_mapped[6],
			BIT5MP=bits_mapped[5], BIT4MP=bits_mapped[4])

	hardware.write_register('DDR.DREMAP3',
			BIT11MP=bits_mapped[11], BIT10MP=bits_mapped[10],
			BIT9MP=bits_mapped[9],   BIT8MP=bits_mapped[8])

	hardware.write_register('DDR.DREMAP4',
			BIT15MP=bits_mapped[15], BIT14MP=bits_mapped[14],
			BIT13MP=bits_mapped[13], BIT12MP=bits_mapped[12])

	hardware.write_register('DDR.DREMAP5',
			BIT19MP=bits_mapped[19], BIT18MP=bits_mapped[18],
			BIT17MP=bits_mapped[17], BIT16MP=bits_mapped[16])

def init_ram(hardware, ram):
	assert ram.bus_width_bits in (16, 32)
	assert ram.banks in (4, 8)
	assert ram.ranks in (1, 2)
	assert ram.burst_length in (4, 8)

	hardware.note('reset PHY PLL')
	hardware.write_register('CPM.DRCG', DLLRESET=1, CFGCLKEN=1)
	hardware.delay_us(400000)
	hardware.write_register('CPM.DRCG', DLLRESET=0, CFGCLKEN=1)
	hardware.delay_us(400000)

	hardware.note('reset DDRC')
	hardware.write_register('DDR.DCTRL', DFI_RST=1, DLL_RST=1, CTL_RST=1, CFG_RST=1)
	hardware.write_register_raw('DDR.DCTRL', 0)

	hardware.note('init PHY')
	init_phy(hardware, ram)

	hardware.note('configure DDRC')

	if ram.follow_reference_code:
		# This reinitialisation of the DDRC seems useless?
		hardware.write_register('DDR.DCTRL', ALH=1, CKE=1) # Turn on the clock for less than 1 ns
		hardware.write_register_raw('DDR.DCTRL', 0)        # Turn it off again

	hardware.write_register('DDR.DCFG',
			IMBA=1,
			MISPE=1,
			TYPE=ram.ram_type,
			DW='WIDTH32' if ram.bus_width_bits == 32 else 'WIDTH16',
			BA0='BANKS8' if ram.banks == 8 else 'BANKS4',
			BA1='BANKS8' if ram.banks == 8 else 'BANKS4',
			CL=ram.ddr_dcfg_cl.ticks, 
			CS1EN=1 if ram.ranks == 2 else 0,
			CS0EN=1,
			COL1=ram.columns - 8,
			ROW1=ram.rows - 12,
			COL0=ram.columns - 8,
			ROW0=ram.rows - 12,
			BSL='BURST8' if ram.burst_length == 8 else 'BURST4')


	hardware.note('write DDR timing')
	hardware.write_register('DDR.DTIMING1',
			tRTP=ram.tRTP.ticks,
			tWTR=int(ram.tWL.ticks + ram.bl.ticks / 2 + ram.tWTR.ticks), # TODO: Why?
			tWR=ram.tWR.ticks,
			tWL=ram.tWL.ticks)

	hardware.write_register('DDR.DTIMING2',
			tCCD=ram.tCCD.ticks,
			tRAS=ram.tRAS.ticks,
			tRCD=ram.tRCD.ticks,
			tRL=ram.tRL.ticks)

	hardware.write_register('DDR.DTIMING3',
			ONUM=4, # Value required by jz4780 programmer's manual
			tCKSRE=ram.tCKSRE.ticks,
			tRP=ram.tRP.ticks,
			tRRD=ram.tRRD.ticks,
			tRC=ram.tRC.ticks)

	hardware.write_register('DDR.DTIMING4',
			tRFC=int(ram.tRFC.ticks / 2) - 1, # Reg is div 2 (and starts at 1)
			tEXTRW=ram.ddr_dtiming4_tEXTRW,
			tRWCOV=ram.ddr_dtiming4_tRWCOV,
			tCKE=ram.ddr_dtiming4_tCKE.ticks, # TODO: DS implies subtract one tick but reference implementation does not
			tMINSR=(ram.tMINSR.ticks - 1) // 8,
			tXP=ram.tXP.ticks,
			tMRD=ram.tMRD.ticks - 1 # 00 = 1tCK, 01 = 2tCK etc
	)

	hardware.write_register('DDR.DTIMING5',
			tCTLUPD=ram.ddr_dtiming5_tCTLUPD,
			tRTW=ram.tRTW.ticks,
			tRDLAT=ram.tRL.ticks - 2,
			tWDLAT=ram.tWL.ticks - 1)

	hardware.write_register('DDR.DTIMING6',
			tXSRD=ram.tXSRD.ticks // 4,
			tFAW=ram.tFAW.ticks,
			tCFGW=ram.ddr_dtiming6_tCFGW.ticks,  
			tCFGR=ram.ddr_dtiming6_tCFGR.ticks)

	hardware.note('map memory')
	# Mapping memory is strange: the reference code attempts to ensure that there
	# is always memory available at 0x20000000 (=512MB) physical. We just assume
	# that we always have just the one 1GB rank, however.
	assert ram.ranks == 1 and ram.size(rank=1) > 512 * 1024 * 1024
	# Map rank 0 at paddr 0
	# Note: This is very weird: the reference code multiplies size by 2 here when
	# calculating the mask. Seems wrong!
	hardware.write_register('DDR.DMMAP0', BASE=0, MASK=ram.size_mask(rank=1) >> 24)
	hardware.write_register('DDR.DMMAP1', BASE=0xFF, MASK=0) # Default no-RAM-here value

	hardware.note('enable DDRC')
	hardware.write_register('DDR.DCTRL', CKE=1, ALH=1) # Note: Shouldn't need ALH=1.

	hardware.note('enable auto-refresh')
	# We set up the refresh counter to use a clock divder of 32, so this will
	# give us a 12.5MHz clock.
	hardware.write_register('DDR.DREFCNT',
			REF_EN=1,
			CLK_DIV=ram.refresh_clock_divider,
			CON=ram.ddr_drefcnt_con.ticks)

	hardware.note('enable power saving features and start DDR clock')
	hardware.write_register('DDR.DCTRL',
			ACTPD=1,
			PDT='TCK64', # Power down after 16 tCKs
			ACTSTP=1,
			RESERVED1=1, # TODO why?
			ALH=1,
			RESERVED2=1, # TODO why?
			UNALIGN=1, # TODO why?
			CKE=1)

	hardware.note('remap memory')
	remap_memory(hardware, ram)

	hardware.note('clear status')
	hardware.read_clear_write('DDR.DSTATUS', ('MISS',))

def generate(output):
	# Samsung 256Mx8 DDR3L-1600, 400MHz clock
	ram = RAMInfo('K4B2G0846Q-BYK0', 400000000, follow_reference_code=True) 
	ram.tRTP = NS('max(4 * nCK, 7.5)')
	ram.tWTR = NS('max(4 * nCK, 7.5)')
	ram.tWR = NS(15)
	ram.tCL = TCK(6) # In ticks
	ram.tCWL = TCK('ram.tCL.ticks - 1') # In ticks
	ram.tRL = TCK('ram.tCL.ticks')
	ram.tWL = TCK('ram.tCWL.ticks')
	ram.tCCD = TCK(4)
	# ram.tRAS = NS(35) # DS value
	ram.tRAS = NS(38) # Reference code
	ram.tRCD = NS(13.75)
	#ram.tCKSRE = NS('max(5 * nCK, 10)')  # DS value
	ram.tCKSRE = TCK(7) # reference code value (TODO)
	ram.tRP = NS(13.75)
	ram.tRRD = NS('max(4 * nCK, 6)')
	#ram.tRC = NS(48.75) # DS value
	ram.tRC = NS(53) # reference code value (TODO)
	ram.tRFC = NS(215) # TODO data sheet recommends NS(160); NS(215) is reference code.
	ram.tCKE = NS('max(3 * nCK, 5.625)')
	ram.tMINSR = TCK(60) # reference implementation. TODO there is no justification for this in RAM DS
	ram.tXP = NS('max(3 * nCK, 6)')
	ram.tMRD = NS('4 * nCK')
	ram.tRTW = TCK('ram.tRL.ticks + ram.tCCD.ticks + 2 - ram.tWL.ticks')
	#ram.tXSRD = TCK(160) # DS value
	ram.tXSRD = TCK(100) # reference code value
	#ram.tFAW = NS(30) # DS value
	ram.tFAW = NS(50) # reference code value (TODO)
	ram.tMOD = NS('max(12 * nCK, 15)')
	ram.tXPDLL = NS('max(10 * nCK, 24)')
	ram.tXS = NS('max(5 * nCK, ram.tRFC.ns + 10)')
	ram.tXSDLL = NS('ram.tDLLLOCK.ns')
	ram.tDLLLOCK = TCK(512)
	ram.phy_dlllock = NS(5.12) # TODO
	ram.ranks = 1 # or 2
	ram.burst_length = 8
	ram.ram_type = 'DDR3'
	ram.bus_width_bits = 32
	ram.banks = 8
	ram.rows = 15
	ram.columns = 10
	ram.bl = TCK(8) # TODO: check this
	ram.tREFI = NS(7800) # Average refresh interval
	ram.refresh_clock_divider = 'DIV32'

	if ram.follow_reference_code:
		# Prerequisites
		assert ram.refresh_clock_divider == 'DIV32' 

		# length of self-refresh delay period: this seems miscalculated due to rounding errors arising from
		# two integer divisions.
		ram.ddr_drefcnt_con = TCK('ram.tREFI.ns // math.ceil(1000000000 / ram.ddr_speed_hz) // 32 - 1')

		# tCKE for DTIMING4: this rounds twice (once here and once when
		# converting to ticks), introducing errors in the conversion.
		ram.ddr_dtiming4_tCKE = NS('math.ceil(ram.tCKE.ticks * ram.nCK * 1000 / 1000)') # Gives us 8 (but we want 7.5)

		# tCKE for PHY: this stores a nanosecond value directly -- of all the
		# weird things, this seems most likely to be wrong. (Note use of TCK here)
		ram.phy_dtpr2_tCKE = TCK('math.ceil(ram.tCKE.ns)')
		
		# Frankly baffling -- also should be irrelevant: "not used in this version", say docs.
		ram.ddr_dcfg_cl = TCK('8 | min(ram.tCL.ticks - 1, 4)')

		# JZ4780 values which deviate from their default values apparently for no good reason.
		ram.ddr_dtiming4_tEXTRW = 0
		ram.ddr_dtiming4_tRWCOV = 1
		ram.ddr_dtiming5_tCTLUPD = 0
		ram.ddr_dtiming6_tCFGW = TCK('2')
		ram.ddr_dtiming6_tCFGR = TCK('2')
	else:
		assert ram.refresh_clock_divider == 'DIV32' 

		# Self-refresh delay: average refresh interval (ns) / length of a refresh interval tick (ns)
		ram.ddr_drefcnt_con = TCK('int(ram.tREFI.ns // ram.ticks_to_ns(32))')

		# Use tCKE as intended for DDR controller and PHY.
		ram.ddr_dtiming4_tCKE = ram.tCKE
		ram.phy_dtpr2_tCKE = ram.tCKE

		# Don't set DCFG.CL as the docs claim it's not used in this version
		ram.ddr_dcfg_cl = TCK('0')

		# Default values of these registers.
		ram.ddr_dtiming4_tEXTRW = 1
		ram.ddr_dtiming4_tRWCOV = 0
		ram.ddr_dtiming5_tCTLUPD = 255
		ram.ddr_dtiming6_tCFGW = TCK('5')
		ram.ddr_dtiming6_tCFGR = TCK('5')

	output.begin()
	init_ram(output, ram)
	output.end()

def decode(name, value):
	reg = REGISTERS[name]
	partnames = sorted(reg.keys())
	for partname in partnames:
		part = reg[partname]
		print(partname, part.decode(value))

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('--decode', nargs='?', help='Decode a register in the form <name>=<value')
	parser.add_argument('--verify', action='store_true')
	args = parser.parse_args()

	if args.decode:
		name, value = args.decode.split(',')
		name = name.upper()
		name = name.replace('_', '.')
		value = int(value, 16)
		decode(name, value)
	elif args.verify:
		output = referenceddr.VerifyOutput(REGISTERS)
		generate(output)
	else:
		output = AutogenOutput()
		generate(output)

