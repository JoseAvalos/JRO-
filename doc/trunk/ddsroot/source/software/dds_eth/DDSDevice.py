'''
Created on Dec 2, 2014

@author: Miguel Urco
'''
import sys
import struct

from IPData import IPData, ascii2hex

CMD_RESET           =0X01
CMD_ENABLE          =0X02
CMD_CHANGEIP        =0X03

DDS_CMD_RESET       =0X10
DDS_CMD_ENABLE_RF   =0x11
DDS_CMD_MULTIPLIER  =0X12
DDS_CMD_MODE        =0x13
DDS_CMD_FREQUENCY_A  =0X14
DDS_CMD_FREQUENCY_B  =0x15
DDS_CMD_PHASE_A      =0X16
DDS_CMD_PHASE_B      =0x17
DDS_CMD_AMPLITUDE_1  =0X19      #Se han invertido la posicion de los canales
DDS_CMD_AMPLITUDE_2  =0x18      #en el PCB
DDS_CMD_READ        =0x8000

class DDSDevice(IPData):
    
    def __init__(self, dds_ip, dds_port=2000, id_dev=0x01, id_class=0x02):
        
        IPData.__init__(self, id_class=id_class, id_dev=id_dev, ip=dds_ip, port=dds_port)
        
        self.__multiplier = None
        self.__master_clock = None
    
    def enableRF(self):
        
        payload = chr(0x01)
        return self.sendData(DDS_CMD_ENABLE_RF, payload)

    def disableRF(self):
        
        payload = chr(0x00)
        return self.sendData(DDS_CMD_ENABLE_RF, payload)
    
    def wrMultiplier(self, multiplier, clock=None):

        self.__multiplier = multiplier
        
        payload = chr(multiplier & 0x1F)
        
        if clock != None:
            self.__master_clock = clock*multiplier
            clock_cad = struct.pack(">H", int(self.__master_clock))
        
            payload += clock_cad
        
        return self.sendData(DDS_CMD_MULTIPLIER, payload)
    
    def wrMode(self, mode):
        
        payload = chr(mode)
        
        return self.sendData(DDS_CMD_MODE, payload)

    def wrFreqRegisterA(self, freq_register):
        '''
        Inputs:
            freq_register    :    Entero, valor del registro de 48 bits (6 Bytes)
        '''
        payload = struct.pack(">Q", freq_register)
        
        return self.sendData(DDS_CMD_FREQUENCY_A, payload[2:])
        
    def wrFreqRegisterB(self, freq_register):
        '''
        Inputs:
            freq_register    :    Entero, valor del registro de 48 bits (6 Bytes)
        '''
        
        payload = struct.pack(">Q", freq_register)
        
        return self.sendData(DDS_CMD_FREQUENCY_B, payload[2:])
    
    def wrFrequencyA(self, frequency):
        '''
        Inputs:
            frequency    :    Valor real de la frequencia, desde 0 hasta (Master Clock/2)
        '''
        if not self.__master_clock:
            raise ValueError, "Master clock was not defined"
            
        freq_register = int(1.0*frequency*2**48/self.__master_clock)
        
        return self.wrFreqRegisterA(freq_register)
 
    def wrFrequencyB(self, frequency):
        '''
        Inputs:
            frequency    :    Valor real de la frequencia, desde 0 hasta (Master Clock/2)
        '''
        if not self.__master_clock:
            raise ValueError, "Master clock was not defined"
            
        freq_register = int(1.0*frequency*2**48/self.__master_clock)
        
        return self.wrFreqRegisterB(freq_register)
           
    def wrPhaseA(self, phase):
        '''
        Inputs:
            phase        :    Valor de la phase1 de 0 - 360 grados
        '''
        phase_register = int(phase*2**14/360.0) & 0x3FFF
        
        payload = struct.pack(">H", phase_register)
        
        return self.sendData(DDS_CMD_PHASE_A, payload)
    
    def wrPhaseB(self, phase):
        '''
        Inputs:
            phase    :    Valor de la phase2 de 0 - 360 grados
        '''  
        
        phase_register = int(phase*2**14/360.0) & 0x3FFF
        
        payload = struct.pack(">H", phase_register)
        
        return self.sendData(DDS_CMD_PHASE_B, payload)       

    def wrAmplitude1(self, amplitude):
        '''
        Inputs:
            amplitude        :    Valor de la amplitude del canal I de 0 - 4095 (0x0FFF)
        '''
        payload = struct.pack(">H", amplitude & 0x0FFF)
        
        return self.sendData(DDS_CMD_AMPLITUDE_1, payload)
    
    def wrAmplitude2(self, amplitude):
        '''
        Inputs:
            amplitude        :    Valor de la amplitude del canal I de 0 - 4095 (0x0FFF)
        '''
        payload = struct.pack(">H", amplitude & 0x0FFF)
        
        return self.sendData(DDS_CMD_AMPLITUDE_2, payload)
           
    def rdMultiplier(self):
        
        payload = ""
        
        sts = self.sendData(DDS_CMD_READ | DDS_CMD_MULTIPLIER, payload)
        
        if sts > 0:
            return ord(self.getPayload())
        
        return sts
    
    def rdMode(self):
        
        payload = ""
        
        sts =  self.sendData(DDS_CMD_READ | DDS_CMD_MODE, payload)

        if sts > 0:
            return ord(self.getPayload())
        
        return sts
    
    def rdFreqRegisterA(self):
        
        payload = ""
        
        sts =  self.sendData(DDS_CMD_READ | DDS_CMD_FREQUENCY_A, payload)
        
        if sts > 0:
            return struct.unpack('>Q', '\x00\x00' + self.getPayload())[0]
        
        return sts
    
    def rdFreqRegisterB(self):
        
        payload = ""
        
        sts =  self.sendData(DDS_CMD_READ | DDS_CMD_FREQUENCY_B, payload)

        if sts > 0:
            return struct.unpack('>Q', '\x00\x00' + self.getPayload())[0]
        
        return sts

    def rdFrequencyA(self):
        
        freq_reg = self.rdFreqRegisterA()
        
        if freq_reg < 1:
            return freq_reg
        
        return self.__master_clock*freq_reg/(2**48)

    def rdFrequencyB(self):
        
        freq_reg = self.rdFreqRegisterB()
        
        if freq_reg < 1:
            return freq_reg
        
        return self.__master_clock*freq_reg/(2**48)
    
    def rdPhaseA(self):
        
        payload = ""
        
        sts =  self.sendData(DDS_CMD_READ | DDS_CMD_PHASE_A, payload)

        if sts > 0:
            return struct.unpack('>H', self.getPayload())[0]
        
        return sts
    
    def rdPhaseB(self):
        
        payload = ""
        
        sts =  self.sendData(DDS_CMD_READ | DDS_CMD_PHASE_B, payload)
        
        if sts > 0:
            return struct.unpack('>H', self.getPayload())[0]
        
        return sts  
    
    def rdAmplitude1(self):
        
        payload = ""
        
        sts =  self.sendData(DDS_CMD_READ | DDS_CMD_AMPLITUDE_1, payload)

        if sts > 0:
            return struct.unpack('>H', self.getPayload())[0]
        
        return sts
    
    def rdAmplitude2(self):
        
        payload = ""
        
        sts =  self.sendData(DDS_CMD_READ | DDS_CMD_AMPLITUDE_2, payload)
    
        if sts > 0:
            return struct.unpack('>H', self.getPayload())[0]
        
        return sts

    def readDDSConfig(self):
        """
        Output:
        
        multiplier        :
        frequencyA        :
        frequencyB        :
        mode              :
        phaseA            :
        phaseB            :
        amplitude1        :
        amplitude2        :
        
        """
        mult = self.rdMultiplier()
        freqA = self.rdFreqRegisterA()
        freqB = self.rdFreqRegisterB()
        mode = self.rdMode()
        phaseA = self.rdPhaseA()
        phaseB = self.rdPhaseB()
        amp1 = self.rdAmplitude1()
        amp2 = self.rdAmplitude2()
        
        return "mult=%d freqA=%d freqB=%d mode=%d phaseA=%d phaseB=%d ampl1=%d ampl2=%d" %(mult,
                                                                                         freqA,
                                                                                         freqB,
                                                                                         mode,
                                                                                         phaseA,
                                                                                         phaseB,
                                                                                         amp1,
                                                                                         amp2)
        
    def writeDDSConfig1(self,
                  multiplier,
                  freq_regA,
                  freq_regB=0x000000000000,
                  mode=0x04,
                  phaseA=0x0000,
                  phaseB=0x2000,
                  amplitude1=0x0FC0,
                  amplitude2=0x0FC0,
                  clock=None):
        
        print self.wrMultiplier(multiplier, clock)
        print self.wrFreqRegisterA(freq_regA)
        print self.wrFreqRegisterB(freq_regB)
        print self.wrMode(mode)
        print self.wrPhaseA(phaseA)
        print self.wrPhaseB(phaseB)
        print self.wrAmplitude1(amplitude1)
        
        return self.wrAmplitude2(amplitude2)
       
    def writeDDSConfig(self,
                  multiplier,
                  clock,
                  freqA,
                  freqB=0,
                  mode=0x04,
                  phaseA=0x0000,
                  phaseB=0x2000,
                  amplitude1=0x0FC0,
                  amplitude2=0x0FC0):
        
        print self.wrMultiplier(multiplier, clock)
        print self.wrFrequencyA(freqA)
        print self.wrFrequencyB(freqB)
        print self.wrMode(mode)
        print self.wrPhaseA(phaseA)
        print self.wrPhaseB(phaseB)
        print self.wrAmplitude1(amplitude1)
        
        return self.wrAmplitude2(amplitude2)