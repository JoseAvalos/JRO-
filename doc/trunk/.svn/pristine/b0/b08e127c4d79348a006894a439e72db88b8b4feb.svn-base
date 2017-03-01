'''
Created on Nov 19, 2014

@author: Miguel Urco

This script was written to test RCDevice serial module.

After execute this script a text similar to the next one
should be seen on the screen.

Example:
    user $ python test_rc_serial.py
    
Output:

    Sent data
    RC Successful

'''

from SerialDevice import *

if __name__ == '__main__':
    filename = '/Users/miguel/Desktop/testdemo/TestPF100K.dat'
    serial_dev = '/dev/tty.usbserial-CRV2_1B'
    
    data = readRCFile(filename)
    rpta = sendSerialData(serial_dev, data)
    print "Sent data"
    print rpta