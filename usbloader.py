# Put stuff onto the CI20 (or any jz4780 -- or other?) board using USB.
import optparse
import time

import usb.core
import usb.util

IDVENDOR  = 0xA108
IDPRODUCT = 0x4780

JZ4780_TCSM_START = 0xf4000800
JZ4780_TCSM_END   = 0xf4004000

BM_REQUEST_DEVICE_TO_HOST      = 1 << 7
BM_REQUEST_TYPE_CLASS          = 1 << 5
BM_REQUEST_TYPE_VENDOR         = 2 << 5
BM_REQUEST_RECIPIENT_INTERFACE = 1 << 0
BM_REQUEST_RECIPIENT_ENDPOINT  = 2 << 0
BM_REQUEST_RECIPIENT_OTHER     = 3 << 0

TIMEOUT = 1000

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

	def boot_stage1(self, data, load_address, entrypoint):
		assert isinstance(data, bytes)

		self.set_data_address(load_address)
		self.send_data(data)
		self.flush_caches()
		self.start1(entrypoint)

def main():
	parser = optparse.OptionParser()
	parser.add_option('--load-address', type="int", default=JZ4780_TCSM_START)
	parser.add_option('--entrypoint', type="int", default=JZ4780_TCSM_START)
	opts, args = parser.parse_args()

	stage1 = args[0]
	# TODO stage 2

	with open(stage1, 'rb') as h:
		data = h.read()

	loader = UsbLoader()
	loader.boot_stage1(data, opts.load_address, opts.entrypoint)

if __name__ == '__main__':
	main()

