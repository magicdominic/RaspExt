RASP: Raspberry Actuator & Sensor Platform
=======

How to compile RASP on the Raspberry Pi (similar on Desktop):

1. Copy Raspbian (Debian Wheezy for Raspberry Pi) to an SD card and configure Raspbian on the Raspberry Pi for your needs

2. Install dependencies:  
 (e.g. Debian) apt-get install qt-sdk git libbluetooth-dev i2c-tools libsdl-mixer1.2-dev bluez bluez-hcidump

3. Configure qtcreator:  
   Specify toolchain (gcc, gdb) and qmake if not auto detected.  
   Attention: You may have to disable the plugin RemoteLinux for the qtcreator, as it may not work while enabled.

4. Set up I2C:  
   Insert i2c-dev in /etc/modules  
   Remove i2c-bcm2708 in /etc/modprobe.d/raspi-blacklist  

   If you want to use RASP or any I2C command without root you have to execute the following command:  
	sudo adduser pi i2c  
   This adds the user pi (standard user on Raspbian) to the I2C group thus allowing access to the I2C devices from the user pi.  


   As there is a bug concerning clock stretching on the Raspberry Pi you may have to reduce the I2C clock speed.  
   You can use the program bcm_del under the folder tools which sets the clock speed of the I2C bus to approximately 66 kHz  
   You may want to execute this program during boot. You can add it to /etc/rc.local so that it is executed on every boot.

5. Now you are ready to compile RASP. First you have to copy it from some source you have available or clone it directly from Github:  
	git clone https://github.com/atraber/RaspExt.git  

   You can compile it either through the qtcreator or if you do not need the qtcreator or do not want to use it directly by executing the following commands in the RASP folder:  
	qmake  
	make  

6. Run RASP  

   In the working directory of RASP the following folders have to exist to which the user executing RASP must have read and write permissions:  
	config  
	scripts  
	resources  

   The folder config is used to store configurations for RASP, whereas the folder scripts is used to save scripts.  
   In the folder resources are some resources needed by RASP by runtime, so this folder has to be copied from the RASP source folder.
