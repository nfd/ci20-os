# Put stuff onto the CI20 (or any jz4780 -- or other?) board using USB.
import optparse
import time
from collections import namedtuple

import usb.core
import usb.util

from thirdparty.pyelftools.elftools.elf import elffile

IDVENDOR  = 0xA108
IDPRODUCT = 0x4780

JZ4780_TCSM_START = 0xf4000800
JZ4780_TCSM_END   = 0xf4004000

TCSM_BANK_SIZE = 0x800

BM_REQUEST_DEVICE_TO_HOST      = 1 << 7
BM_REQUEST_TYPE_CLASS          = 1 << 5
BM_REQUEST_TYPE_VENDOR         = 2 << 5
BM_REQUEST_RECIPIENT_INTERFACE = 1 << 0
BM_REQUEST_RECIPIENT_ENDPOINT  = 2 << 0
BM_REQUEST_RECIPIENT_OTHER     = 3 << 0

TIMEOUT = 1000

def pad_bytes(data, pad_length):
	if len(data) % pad_length != 0:
		data += b'0' * (pad_length - (len(data) % pad_length))
	return data

def address_in_tcsm(address):
	return address >= JZ4780_TCSM_START and address < JZ4780_TCSM_END

LoadSegment = namedtuple('LoadSegment', ('data', 'address'))
class LoadSegments:
	def __init__(self, elf_filename):
		self.load = [] # List of LoadSegments

		with open(elf_filename, 'rb') as h:
			elf = elffile.ELFFile(h)

			for segment in elf.iter_segments():
				if segment['p_type'] == 'PT_LOAD':
					self.load.append(LoadSegment(data=segment.data(), address=segment['p_vaddr']))

			self.entrypoint = elf['e_entry']
	
	def __getitem__(self, idx):
		return self.load[idx]

	def __len__(self):
		return len(self.load)

	def __repr__(self):
		return '<LoadSegments; num=%d entry=%x>' % (len(self.load), self.entrypoint)

class UsbLoader:
	VR_GET_CPU_INFO     = 0
	VR_SET_DATA_ADDRESS = 1
	VR_SET_DATA_LENGTH  = 2
	VR_FLUSH_CACHES     = 3
	VR_PROGRAM_START1   = 4
	VR_PROGRAM_START2   = 5

	ENDPOINT_HOST_TO_DEVICE = 0x1
	ENDPOINT_DEVICE_TO_HOST = 0x81

	KNOWN_CPUS = {'JZ4780V1'}

	def __init__(self):
		self.dev = self._initusb()

		cpuinfo = self.cpuinfo()
		if cpuinfo in UsbLoader.KNOWN_CPUS:
			print("Found CPU: %s" % (cpuinfo))
		else:
			print("Unknown CPU with info %s" % (cpuinfo))

	def _initusb(self):
		dev = usb.core.find(idVendor=IDVENDOR, idProduct=IDPRODUCT)
		if dev is None:
			raise ValueError("Can't find device")

		# Choose the first (and only) configuration.
		dev.set_configuration()

		return dev

	def cpuinfo(self):
		result = self.dev.ctrl_transfer(BM_REQUEST_DEVICE_TO_HOST | BM_REQUEST_TYPE_VENDOR,
				UsbLoader.VR_GET_CPU_INFO, 0, 0, 8, TIMEOUT)

		return ''.join(chr(x) for x in result)

	def set_data_address(self, address):
		self.dev.ctrl_transfer(BM_REQUEST_TYPE_VENDOR,
				UsbLoader.VR_SET_DATA_ADDRESS, address >> 16, address & 0xFFFF, 0, TIMEOUT)
	
	def set_data_length(self, length):
		self.dev.ctrl_transfer(BM_REQUEST_TYPE_VENDOR,
				UsbLoader.VR_SET_DATA_LENGTH, length >> 16, length & 0xFFFF, 0, TIMEOUT)
	
	def flush_caches(self):
		self.dev.ctrl_transfer(BM_REQUEST_TYPE_VENDOR,
				UsbLoader.VR_FLUSH_CACHES, 0, 0, None, TIMEOUT)

	def start1(self, entrypoint):
		self.dev.ctrl_transfer(BM_REQUEST_TYPE_VENDOR,
				UsbLoader.VR_PROGRAM_START1, entrypoint >> 16, entrypoint & 0xFFFF, None, TIMEOUT)

	def start2(self, entrypoint):
		self.dev.ctrl_transfer(BM_REQUEST_TYPE_VENDOR,
				UsbLoader.VR_PROGRAM_START2, entrypoint >> 16, entrypoint & 0xFFFF, None, TIMEOUT)
	
	def send_data(self, data):
		self.dev.write(UsbLoader.ENDPOINT_HOST_TO_DEVICE, data)

	def read_data(self, amount):
		return self.dev.read(UsbLoader.ENDPOINT_DEVICE_TO_HOST, amount, TIMEOUT)

	def boot_stage1(self, loadsegments):
		for segment in loadsegments:
			data = segment.data

			if address_in_tcsm(segment.address):
				# Apparently the boot loader likes its data padded.
				data = pad_bytes(data, TCSM_BANK_SIZE)

			print("Writing from %08x to %08x" % (segment.address, segment.address + len(data)))
			self.set_data_address(segment.address)
			self.send_data(data)

		self.flush_caches()
		print("Executing at %08x" % (loadsegments.entrypoint))
		self.start2(loadsegments.entrypoint)

def main():
	parser = optparse.OptionParser()
	opts, args = parser.parse_args()

	elf_filename = args[0]
	segments = LoadSegments(elf_filename)

	loader = UsbLoader()
	loader.boot_stage1(segments)

if __name__ == '__main__':
	main()

