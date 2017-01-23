#!/usr/bin/env python3
import os
import sys
import subprocess

# args: <kernel> <sigma0> <output>

SARUMAN = 'saruman'
OBJINFO = os.path.join(SARUMAN, 'objinfo')
OBJPATCH = os.path.join(SARUMAN, 'objpatch')
OBJCAT = os.path.join(SARUMAN, 'objcat')

debug = False
if sys.argv[1] == '--debug':
	debug = True
	del sys.argv[1]

KERNEL, SIGMA0, OUTPUT = sys.argv[1], sys.argv[2], sys.argv[3]

def hex_objinfo(filename, *args):
	result = subprocess.run((OBJINFO,) + args + (filename,), stdout=subprocess.PIPE)
	return int(result.stdout, 16)

# Find out some details
KERNEL_MEMORY_LAYOUT_STRUCT = hex_objinfo(KERNEL, '-S', 'kernel_memory_layout')
KERNEL_LOWEST_VADDR = hex_objinfo(KERNEL, '--lowest-vaddr')
SIGMA0_HIGHEST_VADDR = hex_objinfo(SIGMA0, '--highest-vaddr')
SIGMA0_ENTRYPOINT = hex_objinfo(SIGMA0, '--entrypoint')

# TODO: mips-specific portion; sigma0 vaddrs end up in kernel space.
SIGMA0_HIGHEST_VADDR |= 0x80000000 # convert to kseg0
# end mips-specific portion.

if debug:
	print('%08x: Kernel memory layout' % (KERNEL_MEMORY_LAYOUT_STRUCT,))
	print('%08x: Lowest vaddr' % (KERNEL_LOWEST_VADDR,))
	print('%08x: Highest vaddr' % (SIGMA0_HIGHEST_VADDR,))
	print('%08x: Sigma0 entrypoint' % (SIGMA0_ENTRYPOINT,))

# create the main executable
concatenator = subprocess.Popen([OBJCAT, '--to-kseg0', KERNEL, SIGMA0], stdout=subprocess.PIPE)
patcher = subprocess.Popen([OBJPATCH,
	'-V', '0x%x=u32:0x%x' % (KERNEL_MEMORY_LAYOUT_STRUCT, SIGMA0_ENTRYPOINT),
	'-V', '0x%x=u32:0x%x' % (KERNEL_MEMORY_LAYOUT_STRUCT + 4, KERNEL_LOWEST_VADDR),
	'-V', '0x%x=u32:0x%x' % (KERNEL_MEMORY_LAYOUT_STRUCT + 8, SIGMA0_HIGHEST_VADDR)],
	stdin=concatenator.stdout, stdout=subprocess.PIPE)
concatenator.stdout.close()

stdout, stderr = patcher.communicate()
with open(OUTPUT, 'wb') as h:
	h.write(stdout)


