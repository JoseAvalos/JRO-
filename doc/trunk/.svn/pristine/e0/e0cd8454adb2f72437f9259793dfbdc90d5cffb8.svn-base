'''
Created on Nov 19, 2014

@author: Miguel Urco

This script was written to test DDSDevice serial module.

After execute this script a text similar to the next one
should be seen on the screen.

Example:
    user $ python test_dds_serial.py
    
Output:

    Sent data
    DDS Successful

'''

from SerialDevice import *

if __name__ == '__main__':
    filename = '../configfiles/sousy-32MHz.dds'
    serial_dev = '/dev/tty.usbserial-DDS2-004'
    
    data = readDDSFile(filename)
    rpta = sendSerialData(serial_dev, data)
    print "Sent data"
    print rpta