# Set of register writes performed by the reference implementation to initialise RAM.

ACTIONS = [
	('write_register', 'CPM.DRCG', 0x00000003),
	('write_register', 'CPM.DRCG', 0x00000001),
	('write_register', 'DDR.DCTRL', 0x00f00000),
	('write_register', 'DDR.DCTRL', 0x00000000),
	('write_register', 'DDRP.DTAR', 0x00150000),
	('write_register', 'DDRP.DCR', 0x0000000b),
	('write_register', 'DDRP.MR0', 0x00000420),
	('write_register', 'DDRP.MR1', 0x00000002),
	('write_register', 'DDRP.ODTCR', 0x00000000),
	('write_register', 'DDRP.MR2', 0x00000000),
	('write_register', 'DDRP.PTR0', 0x002000d4),
	('write_register', 'DDRP.PTR1', 0x02d30d40),
	('write_register', 'DDRP.PTR2', 0x04013880),
	('write_register', 'DDRP.DTPR0', 0x2c906690),
	('write_register', 'DDRP.DTPR1', 0x005608a0),
	('write_register', 'DDRP.DTPR2', 0x10042a00),
	('write_register', 'DDRP.PGCR', 0x01842e02),
	('read_clear_write', 'DDRP.DXGCR', ('UNKNOWN',)), # This does the whole set
	('spin_until_contains', ),
	('write_register', 'DDRP.PIR', 0x00000061),
	('spin_until_contains', ),
	('write_register', 'DDRP.PIR', 0x00000081),
	('spin_until_contains', ),
	('read_update_write', 'DDRP.ZQXCR0', 0x100001ce),
	# Weird bit begin:
	('write_register', 'DDR.DCTRL', 0x0000000a),
	('write_register', 'DDR.DCTRL', 0x00000000),
	# Weird bit end
	('write_register', 'DDR.DCFG', 0x1aec9a73),
	('write_register', 'DDR.DTIMING1', 0x040d0605),
	('write_register', 'DDR.DTIMING2', 0x04100606),
	('write_register', 'DDR.DTIMING3', 0x27060416),
	('write_register', 'DDR.DTIMING4', 0x2a0c0733),
	('write_register', 'DDR.DTIMING5', 0x00070404),
	('write_register', 'DDR.DTIMING6', 0x19140202),
	('write_register', 'DDR.DMMAP0', 0x00000080),
	('write_register', 'DDR.DMMAP1', 0x0000ff00),
	('write_register', 'DDR.DCTRL', 0x0000000a),
	('write_register', 'DDR.DREFCNT', 0x00500003),
	('write_register', 'DDR.DCTRL', 0x0000c91e),
	('read_clear_write', 'DDR.DSTATUS', ('MISS',)),
]

class VerifyFailed(Exception):
	pass

class VerifyOutput:
	def __init__(self, registers):
		self.position = 0
		self.registers = registers

	def begin(self):
		pass

	def end(self):
		if self.position != len(ACTIONS):
			print("Warning: finished on position %d of %d" % (self.position, len(ACTIONS)))

		assert self.position == len(ACTIONS)
	
	def note(self, text):
		pass

	def write_register(self, name, **kw):
		verify = ACTIONS[self.position]
		if verify[0] != 'write_register':
			raise VerifyFailed(self.position, 'write_register', verify)

		if verify[1] != name:
			raise VerifyFailed(self.position, name, verify)

		reg = self.registers[verify[1]]
		expectparts = {}
		for partname in reg.keys():
			part = reg[partname]
			val = part.decode(verify[2])
			if not(partname not in kw and val==part.zero):
				expectparts[partname] = val

		if expectparts != kw:
			print("Got these for %s(%d):" % (name, self.position))
			for partname in sorted(kw.keys()):
				print("  %s=%s" % (partname, kw[partname]))

			print("\nExpected:")
			for partname in sorted(expectparts.keys()):
				print("  %s=%s" % (partname, expectparts[partname]))

			raise VerifyFailed(self.position, kw, name, expectparts)

		self.position += 1

	def delay_us(self, us):
		pass

	def write_register_raw(self, name, val):
		verify = ACTIONS[self.position]
		if verify[0] != 'write_register':
			raise VerifyFailed(self.position, 'write_register', verify)

		if verify[1] != name:
			raise VerifyFailed(self.position, name, verify)

		if verify[2] != val:
			raise VerifyFailed(self.position, val, verify)

		self.position += 1

	def read_clear_write(self, name, partnames):
		self.position += 1

	def spin_until_contains(self, name, partnames):
		self.position += 1

	def read_update_write(self, name, preserve=None, **kw):
		# TODO
		self.position += 1

