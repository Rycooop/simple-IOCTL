# simple-IOCTL
IOCTL proof of concept with UM

Didnt comment a ton because pretty self explanatory

Make sure test signing is on, in administrator CMD create the driver.

sc create testioctl type="kernel" binpath="Full path to driver here"
sc start testioctl

To unload the driver:
sc stop testioctl
