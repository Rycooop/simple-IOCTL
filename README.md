# simple-IOCTL
IOCTL proof of concept with UM. Easily detected by kernel level anticheats since you need a DeviceObject to setup communication if manually mapped. Driver will need to be manually mapped to work without test signing. Good intro to kernel level cheats.

driver.h is part of the usermode portion. Compile both for x64 release

Didnt comment a ton because pretty self explanatory

Make sure test signing is on, in administrator CMD create the driver.

sc create testioctl type="kernel" binpath="Full path to driver here"
sc start testioctl

To unload the driver:
sc stop testioctl
