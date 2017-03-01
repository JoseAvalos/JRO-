'''
Created on Nov 19, 2014

@author: Miguel Urco
'''

usage='''
This script was written to test the changeIP module.

Host Ip address and mask should be in the same network as DDS device before
execute any command.

After execute this script a text similar to the next one
should be seen on the screen.

Example:
    user $ python test_change_ip.py --current-ip192.68.7.2 --new-ip=10.10.20.63 --new-mask=255.255.255.0 --new-gateway=0.0.0.0

Output:

    Configuring DDS on 192.168.7.2:2000
    
    Reading current DDS parameters ...
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 12 94 
    RX:  24 4a 52 4f 24 00 00 00 06 02 01 80 12 01 96 
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 14 92 
    RX:  24 4a 52 4f 24 00 00 00 0b 02 01 80 14 00 00 00 00 00 00 9c 
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 15 93 
    RX:  24 4a 52 4f 24 00 00 00 0b 02 01 80 15 00 00 00 00 00 00 9d 
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 13 95 
    RX:  24 4a 52 4f 24 00 00 00 06 02 01 80 13 04 92 
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 16 90 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 16 00 00 92 
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 17 91 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 17 20 00 b3 
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 19 9f 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 19 00 00 9d 
    TX:  24 4a 52 4f 24 00 00 00 05 02 01 80 18 9e 
    RX:  24 4a 52 4f 24 00 00 00 07 02 01 80 18 00 00 9c 
    mult=1 freqA=0 freqB=0 mode=4 phaseA=0 phaseB=8192 ampl1=0 ampl2=0
    
    Updating to new IP address 10.10.20.68
    1

'''
import sys

from DDSDevice import DDSDevice
from optparse import OptParseError
import optparse

parser = optparse.OptionParser(usage=usage)
parser.add_option("-a", "--current-ip", dest="current_ip", type="string", default="192.168.7.2")
parser.add_option("-b", "--current-port", dest="current_port", type="int", default=2000)
parser.add_option("-i", "--new-ip", dest="ip", type="string", default="192.168.7.2")
parser.add_option("-m", "--new-mask", dest="mask", type="string", default="255.255.255.0")
parser.add_option("-g", "--new-gateway", dest="gateway", type="string", default="0.0.0.0")

if __name__ == '__main__':
    
    (options, args) = parser.parse_args()
    
    dds_ip = options.current_ip
    dds_port = options.current_port
    
    new_dds_ip = options.ip
    new_dds_mask = options.mask
    new_dds_gateway = options.gateway
    
    print "Configuring DDS on %s:%d" %(dds_ip, dds_port)
    dds_dev = DDSDevice(dds_ip, dds_port)
     
    print
    print "Reading current DDS parameters ..."
    print dds_dev.readDDSConfig()
    
    print
    print "Updating to new IP address %s" %new_dds_ip
    print dds_dev.changeIP(new_dds_ip, new_dds_mask, new_dds_gateway)
    