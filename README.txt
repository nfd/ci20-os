This is a MIPS CI20 experimental operating system.

Commands to load it from serial port:

make && cp os.bin /private/tftpboot/
setenv serverip 192.168.1.12
setenv ipaddr 192.168.1.7
tftpboot 0x80000000 192.168.1.12:os.bin
go 0x80000000


