# CANbus_Israel and abshick EV


#GITHUB: https://github.com/autowp/arduino-mcp2515  

#MCP2515 CAN Bus Module TJA1050 Receiver SPI Module for Arduino: https://www.ebay.com/itm/MCP2515-CAN-Bus-Module-TJA1050-Receiver-SPI-Module-for-Arduino/292587667149?

#RS232 driver: https://www.ftdichip.com/Drivers/VCP.htm

Serial communication 
Bud rate: 115200
raspberrypi.local


sudo /sbin/ip link set can0 up type can bitrate 125000

chmod u+x cansend

chmod u+x candump

./candump can0


Download and copy the CAN test programs to the Pi: http://www.skpang.co.uk/dl/can-test_pi2.zip
Pi CAN 2 website: https://copperhilltech.com/pican-2-can-interface-for-raspberry-pi-2-3/
