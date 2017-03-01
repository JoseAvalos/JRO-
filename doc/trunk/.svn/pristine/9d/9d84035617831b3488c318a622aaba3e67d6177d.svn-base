'''
Created on Nov 19, 2014

@author: Miguel Urco

This script was written to test DDSDevice module.

Host Ip address and mask should be in the same network as DDS device before
execute any command.

After execute this script a text similar to the next one
should be seen on the screen.

IMPORTANT: A clock of 10MHz should be used on this test.

Example:
    user $ python test_dds_eth.py

Output:

    Enabling RF ...
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 11 4f 4b 11 
    
    Reading current DDS parameters ...
    RX:  24 4a 52 4f 24 00 00 00 06 02 01 80 12 01 96 
    RX:  24 4a 52 4f 24 00 00 00 0b 02 01 80 14 00 00 00 00 00 00 9c 
    RX:  24 4a 52 4f 24 00 00 00 0b 02 01 80 15 00 00 00 00 00 00 9d 
    RX:  24 4a 52 4f 24 00 00 00 06 02 01 80 13 04 92 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 16 00 00 92 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 17 20 00 b3 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 19 0f c0 52 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 18 0f c0 53 
    (1, 0, 0, 4, 0, 8192, 4032, 4032)
    
    Writing new DDS parameters ...
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 12 4f 4b 12 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 14 4f 4b 14 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 15 4f 4b 15 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 13 4f 4b 13 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 16 4f 4b 16 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 17 4f 4b 17 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 19 4f 4b 19 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 00 18 4f 4b 18 
    
    Reading DDS parameters again ...
    RX:  24 4a 52 4f 24 00 00 00 06 02 01 80 12 14 83 
    RX:  24 4a 52 4f 24 00 00 00 0b 02 01 80 14 0f ff ff ff ff ff 6c 
    RX:  24 4a 52 4f 24 00 00 00 0b 02 01 80 15 00 00 00 00 00 00 9d 
    RX:  24 4a 52 4f 24 00 00 00 06 02 01 80 13 04 92 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 16 00 00 92 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 17 30 5b f8 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 19 0f c0 52 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 18 0f c0 53 
    (20, 17592186044415, 0, 4, 0, 12379, 4032, 4032)

'''
import sys

from DDSDevice import DDSDevice

ddsIp = "192.168.7.2"
ddsPort = 2000

if __name__ == '__main__':
    
    dds_dev = DDSDevice(ddsIp, ddsPort)
    
    print
    print "Enabling RF ..."
    dds_dev.enableRF()
     
    print
    print "Reading current DDS parameters ..."
    print dds_dev.readDDSConfig()
    
    print
    print "Writing new DDS parameters ..."
#     dds_dev.writeDDSConfig1(multiplier=20, freq_regA = 0x1FFFFFFFFFFF)
    dds_dev.writeDDSConfig(clock=10, multiplier=20, freqA = 49.92)
    
    print
    print "Reading DDS parameters again ..."
    print dds_dev.readDDSConfig()