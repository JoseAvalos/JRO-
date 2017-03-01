'''
Created on Dec 2, 2014

@author: Miguel Urco
'''
import sys
import struct
import socket

DEBUG = False
CMD_RESET           =0X01
CMD_ENABLE          =0X02
CMD_CHANGEIP        =0X03

def ascii2hex(cadena):
        
    hex_cad = ''
    for c in cadena:
        hex_cad += hex(ord(c))[2:].rjust(2,'0') + ' '
        
    return hex_cad

class IPData(object):
    
    '''
    Clase para manejar la trama de datos provenientes del/hacia el DDS, la cual tiene
    el siguiente formato:
    
    **********************************
    ** FORMATO GENERAL DE UNA TRAMA **
    **********************************
    
    1. Cabecera (5 bytes):     Secuencia Fija que deber ser "$JRO$"
    2. Longitud (3 bytes):     Cantidad de bytes de la Data
    3. Id class (1 byte):
    4. Id device (1 byte):
    5. Cmd (2 bytes):
    3. Payload (n bytes):     Carga Util conteniendo secuencia,comandos y parametros
    4. Xor (1 byte):          Byte de revision de consistencia de la data al aplicar Xor a todos los bytes,
                              desde la longitud hasta el payload.

    '''
    __HEADER = "$JRO$"
    
    def __init__(self, id_class, id_dev, ip, port):
        '''
        '''
        self.id_class = id_class
        self.id_dev = id_dev
        
        self.address = (ip, port)
        
        self.__iniVariables()
        
    def __iniVariables(self):
        
        self.tx_buffer = None
        self.rx_buffer = None
        
        #self.header = None
        self.len = None
        self.cmd = None
        self.payload = None
        
        self.invalid = True
        self.errormsg = ''
        self.hasPayload = False
        
    def __getXor(self, cadena):
        '''
        '''
        #trama = '%03d' %lenght + ipPayload
        xor = 0
        for character in cadena:
            xor = xor ^ ord(character)
        
#         xor_hex = hex(xor)
#         xor_hex = xor_hex[2:]
#         xor_hex = xor_hex.rjust(2,'0')
        
        return xor
    
    def __verifyXor(self, cadena):
        
        xor = self.__getXor(cadena)
        
        if xor != 0:
            return 0
        
        return 1
    
    def __encoder(self, cmd, payload):
        '''
        Inputs:
            cmd            :    Entero que indica el tipo de comando
            payload        :    Cadena de caracteres con informacion, depende del comando
            
        '''
        
        #seq = '%04d' %(sequence)
        #conf = '%04d' %(confcode)
        
        #Number to Cad: 2 Bytes <> H, 4 Bytes <> I
        cmd_cad = struct.pack(">H", cmd)
        
        data = chr(self.id_class) + chr(self.id_dev) + cmd_cad + payload
        
        len_data = len(data) + 1 # + xor
        len_cad = struct.pack('>I', len_data)
        
        lenAndData = len_cad + chr(self.id_class) + chr(self.id_dev) + cmd_cad + payload
        
        xor = self.__getXor(lenAndData)
        
        trama = self.__HEADER + lenAndData + chr(xor)
        
        self.tx_buffer = trama
        
        return trama
    
    def __decoder(self, rx_buffer):
        '''
        Evalua la trama y la separa en los campos correspondientes
        
        4Bytes    |    4Bytes    | 1Byte    | 1 Byte |  2 Bytes |    n Bytes          |    1 Byte
        Header    |    Len       | Id class | Id dev |   Cmd    |   Payload           |    Xor

        '''
        self.invalid = True
        self.hasPayload = False
        self.rx_buffer = rx_buffer
        
        try:
            index = rx_buffer.find(self.__HEADER)
        except:
            self.errormsg = "No string"
            return 0
            
        if index == -1:
            self.errormsg = "No header found: %s" %ascii2hex(rx_buffer)
            return 0
        
        rx_buffer = rx_buffer[index + len(self.__HEADER):]
        
        len_cad = rx_buffer[0:4]
        
        len_data = struct.unpack('>I',len_cad)[0]
        
        lenAndDataAndXor = rx_buffer[0:len_data + 4]    #Incluye los 4 bytes de la longitud
        
        dataAndXor = lenAndDataAndXor[4:]
        
        if len(dataAndXor) < len_data:
            self.errormsg = "Data length is lower than %s" %(len_data)
            return 0
        
#         print self.header, ", ", ascii2hex(lenCad), ", ", ascii2hex(ipDataAndXor), ", ", hex(self.xor)
        
        if not self.__verifyXor(lenAndDataAndXor):
            self.errormsg = "Invalid xor: %s" %lenAndDataAndXor[-1]
            return 0
        
        self.invalid = False
        
        len_payload = len_data - 5  #Decrementar 1B (id_class), 1B (id_dev), 2B (cmd) y 1B (xor)
        
        id_class = ord(dataAndXor[0])
        id_dev = ord(dataAndXor[1])
        cmd_cad = dataAndXor[2:4]
        payload = dataAndXor[4:4+len_payload]
        
        cmd = struct.unpack('>H',cmd_cad)[0]
        
        self.id_class = id_class
        self.id_dev = id_dev
        self.cmd = cmd
        
        if len(payload) < 1:
            self.errormsg = "IP data is valid but it hasn't payload"
            return 1
        
        self.hasPayload = True
        self.payload = payload
        
        self.errormsg = "Successful"
        
        return 1

    def __decoder_api(self, rx_buffer, debug = DEBUG):
        """
        Input:
            rx_buffer    :    Trama recibida como respuesta a un comando enviada a un dispositivo.
        
        Return:
            0    :    Trama recibida incorrecta. La cadena "rx_buffer" no ha sido decodificada correctamente.
            -1    :    Dispositivo no inicializado. El dispositivo, dds o rc, no ha sido inicializado
                        correctamente.
            -2    :    Trama enviada no reconocida. La cadena recibida es correcta y el dispositivo ha sido
                        inicializaado correctamente pero la trama enviada no ha sido reconocido por el
                        dispositivo o el comando enviado no ha sido implementado.
            >0    :    Trama enviada y recibida correcta
        """
        
        if not self.__decoder(rx_buffer):
            print "Error decoding eth data: ", ascii2hex(self.rx_buffer)
            return 0
        
        if self.getPayload() == "NI":
            return -1
        
        if self.getPayload() == "KO":
            return -2
        
        if debug:
            print ascii2hex(self.rx_buffer)
            
        return self.getPayload()
     
    def getRxBufferHex(self):
        
        if self.rx_buffer == None:
            return ''
        
        cad = ascii2hex(self.rx_buffer)
            
        return cad

    def getTxBufferHex(self):
        
        if self.tx_buffer == None:
            return ''
        
        cad = ascii2hex(self.tx_buffer)
            
        return cad
       
    def isInvalid(self):
        
        return self.invalid
    
    def getCmd(self):
        return self.cmd
        
    def getPayload(self):
        return self.payload
    
    def getErrorMessage(self):
        
        return self.errormsg
    
    def getTxBuffer(self):
        
        return self.tx_buffer

    def getRxBuffer(self):
        
        return self.rx_buffer
    
    def __encodeIpCmd(self, ip, mask, gateway):
        
        payload = ip + '/' + mask + '/' + gateway
        return self.__encoder(CMD_CHANGEIP, payload)
    
    def __encodeResetCmd(self):
        
        payload = ""
        return self.__encoder(CMD_RESET, payload)
    
    def __sendTCPData(self, cadena):

        sck = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sck.settimeout(10)
        
        try:
            sck.connect(self.address)
        except:
            print "Error"
            sys.exit()
        
        sck.send(cadena)
        
        rx_buffer  = sck.recv(255);
        
        sck.close()
        
        return rx_buffer
    
    def changeIP(self, ip, mask, gateway):
        
        tx_buffer = self.__encodeIpCmd(ip, mask, gateway)
        rx_buffer = self.__sendTCPData(tx_buffer)
        
        sts = self.__decoder_api(rx_buffer)
        
        if sts > 0:
            self.address = (ip, self.address[1])
            
        return sts
    
    def reset(self):
        
        tx_buffer = self.__encodeResetCmd()
        rx_buffer = self.__sendTCPData(tx_buffer)
        
        return self.__decoder_api(rx_buffer)
    
    def sendData(self, cmd, payload):
        
        tx_buffer = self.__encoder(cmd, payload)
        
        print "TX: ", ascii2hex(tx_buffer)
        
        rx_buffer = self.__sendTCPData(tx_buffer)
        
        print "RX: ", ascii2hex(rx_buffer)
        
        return self.__decoder_api(rx_buffer)
    