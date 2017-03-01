'''
Created on Nov 19, 2014

@author: Miguel Urco
'''

import os, time
import serial
import string
import copy

def readRCFile(filename):
    
    fp = open(filename, 'rU')
    lines = fp.readlines()
    
    frame = ""
    for line in lines:
        frame = frame + "%c" % int (line)
    
    return frame
    
    buff_array = []
    
    n = 4*1024
    nbytes = 0
    
    N = len(frame)
    q = N / n
    r = N % n
    
    if q < 1:
        buff_array.append(frame)
    elif r == 0:
        pieces = q
        for i in range(pieces):
            buff_array.append(frame[i*n:(i+1)*n])
    else:
        pieces = q + 1
        for i in range(pieces - 1):
            buff_array.append(frame[i*n:(i+1)*n])
        buff_array.append(frame[(pieces-1)*n:N])
        
    #Archivo es enviado en paquetes de n bytes
    for z in buff_array:
        nbytes = nbytes + h.write(z)
        
    h.close
    
    if N == nbytes:
        var2.set("**Envio exitoso**")
    else:
        var2.set("Error en el envio")
    
    return ddsRegisters

def readDDSFile(filename):
    
    fp = open(filename, 'rU')
    lines = fp.readlines()
    
    ddsRegisters = ""
    for thisLine in lines:
        
        if len(thisLine) == 9 or len(thisLine) == 8:
            x = "%c" %string.atoi(thisLine, 2)
            ddsRegisters = ddsRegisters + x
    
    return ddsRegisters

def sendSerialData(serial_dev, data, pack_size = 4*1024):
    
    ser = serial.Serial(serial_dev)
    ser.stopbits = serial.STOPBITS_ONE
    ser.parity = serial.PARITY_NONE
    ser.bytesize = serial.EIGHTBITS
    ser.baudrate = 1000000
    
    n_pack = int(len(data)/pack_size) + 1
    
    for i in range(n_pack):
        if (i+1)*pack_size > len(data):
            package = data[i*pack_size:]
        else:
            package = data[i*pack_size:(i+1)*pack_size]
        ser.write(package)
    
    time.sleep(1)
    rpta = ""
    
    while(ser.inWaiting()>0):
        rpta += ser.read()
        
    ser.close
    return rpta

if __name__ == '__main__':
    filename = '../configfiles/sousy-32MHz.dds'
    serial_dev = '/dev/tty.usbserial-DDS2-004'
    
    data = readDDSFile(filename)
    rpta = sendSerialData(serial_dev, data)
    print "Sent data"
    print rpta