import d2xx
import sys
import os
import string
import tkMessageBox
from Tkinter import *
from tkMessageBox import *
from tkMessageBox import *
from tkColorChooser import askcolor              
from tkFileDialog   import askopenfilename    
from tkSimpleDialog   import askstring

def EliminaSaltoDeLinea(cadena):
    i=0
    for elemento in cadena:
        if elemento =='\n' or elemento =='\r':
            pass
        else:
            i=i+1
    return cadena [:i]

def buscar():
    global d
    try:
        d = d2xx.listDevices()
        cadena = ""
        if len(d)>0:
            for i in range(len(d)):
                cadena = cadena +"[%d]"%i+" : "+ d[i] + "\n"
            var.set("Equipos encontrados: " +"\n" + cadena)
            var2.set("")
        else:
            var.set("No hay equipos conectados")
    except:
        tkMessageBox.showinfo( "INFO", "Espere, reconociendo equipo")
    
def buscar_archivo():
    global d
    m=E1.get()
    dispo = int (m)
    if dispo <= len(d)-1:
        h = d2xx.openEx(d[dispo]) 
        h.setDivisor(3)   #aqui configuramos los baudios.
        h.setDataCharacteristics(8,2,0)   
        h.setTimeouts(1000,1000)
        #leemos la meoria EEPROM
        equipo=h.eeRead()
        num_serie = equipo['serialNumber']
    
        letra = num_serie[:2]
        if letra == "DD":
            print "Grabar un equipo DDS"
            #leer archivos de extension .dds
            archivo = askopenfilename(filetypes = [('Archivos dds', '*.dds')])
            inList = open(archivo, 'rU').readlines()    
            buffer = ""
            for line in inList:
                if len(line) == 9 or len(line) == 8:
                    buffer += "%c" % string.atoi(line,2)
                    
            h.write(buffer)
            h.close   
            var2.set("**Envio exitoso**")
            
        elif letra == "CR":
            print "Grabar un equipo CR"
            #leer archivos de extension .dat
            archivo = askopenfilename(filetypes = [('Archivos dat', '*.dat')])
            inList = open(archivo, 'rU').readlines()

            n = 4*1024
            buff_array = []
            frame = ""
            nbytes = 0
            for line in inList:
                frame = frame + "%c" % int (line)
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
            
        elif letra=="RE":
            print "Grabar un equipo de REX"
            #------------------------------------------------------
            #Generar archivo de configuracion para REX:
            archivo = askopenfilename(filetypes = [('Archivos rxd', '*.rxd*')])
            fichero1 = open(archivo, 'rU').readlines()
            data1=[]
            for line in fichero1:
                cadena=EliminaSaltoDeLinea(line)
                data1.append(cadena)
            #Extraccion de datos:
            NumberChannel = archivo[-1] #Dato de numero de canales
            if NumberChannel == "d":
                NumberChannel = "2"
                print "El archivo configura 2 canal"
            elif NumberChannel == "1":
                print "El archivo configura 1 canal"
            else:
                print "La informacion de canales es desconocido"
                quit()
            File = data1[1]
            for i in range(len(File)):
                if File[i]==chr(92):
                    l=i
            NameFile = File[l+1:]   # dato del nombre del archivo
            bloque1 = data1[5:19]
            datosA = []
            for letras in bloque1:
                for i in range(len(letras)):
                    if letras[i]==' ':
                        l=i
                datosA.append(letras[l+1:])
            if datosA[2] == "11111111111111111111111111111111":
                datosA[2] = "4294967295"
            else:
                print "Error en archivo"
                quit()
            #datosA = primer bloque de datos
            bloque2 = data1[19:-4]
            datosB = []
            for letras in bloque2:
                for i in range(len(letras)):
                    if letras[i]==' ':
                        l=i
                datosB.append(letras[l+1:])
            #datosB = segundo bloque de datos
            #-------Proceso de escritura de datos ----------
            fichero2 = open('C:/files_rex_out/'+NameFile[:-4]+'.rex','w')
            fichero2.write(NumberChannel+'\n')
            conta = 0
            for i in datosA:
                num = int(i)
                num_hex = "%x" %num
                nbytes = len(num_hex)
                ceros="00000000"
                valor_hex = ceros[0:(8-nbytes)]+num_hex
                data_byte_0 = valor_hex[0:2]
                data_byte_1 = valor_hex[2:4]
                data_byte_2 = valor_hex[4:6]
                data_byte_3 = valor_hex[6:8]
                fichero2.write('3'+'\n'+'7'+'\n')
                fichero2.write(str(conta)+'\n'+ '6'+'\n')
                conta = conta + 1
                fichero2.write("%s" % string.atoi(data_byte_0,16)+'\n'+'3'+'\n')
                fichero2.write("%s" % string.atoi(data_byte_1,16)+'\n'+'2'+'\n')
                fichero2.write("%s" % string.atoi(data_byte_2,16)+'\n'+'1'+'\n')
                fichero2.write("%s" % string.atoi(data_byte_3,16)+'\n'+'0'+'\n')
            
            conta = 0
            for i in datosB:
                num = int(i)
                if num<0:
                    num =  4294967296+num
                num_hex = "%x" %num
                nbytes = len(num_hex)
                ceros="00000000"
                valor_hex = ceros[0:(8-nbytes)]+num_hex
                data_byte_0 = valor_hex[0:2]
                data_byte_1 = valor_hex[2:4]
                data_byte_2 = valor_hex[4:6]
                data_byte_3 = valor_hex[6:8]
                fichero2.write('0'+'\n'+'7'+'\n')
                fichero2.write(str(conta)+'\n'+ '6'+'\n')
                conta = conta + 1
                fichero2.write("%s" % string.atoi(data_byte_0,16)+'\n'+'3'+'\n')
                fichero2.write("%s" % string.atoi(data_byte_1,16)+'\n'+'2'+'\n')
                fichero2.write("%s" % string.atoi(data_byte_2,16)+'\n'+'1'+'\n')
                fichero2.write("%s" % string.atoi(data_byte_3,16)+'\n'+'0'+'\n')

            for i in data_final:
                fichero2.write(i)
            fichero2.close()
            #------------------------------------------------------
            #leer archivos de extension .rex
            inList = open('C:/files_rex_out/'+NameFile[:-4]+'.rex', 'rU').readlines()
            buffer = ""
            for line in inList:
                buffer =buffer + "%c" %int(line)
            h.write(buffer)   
            #-------------------------------
            #segmeto de codigo para depurar:
            #dato_rex = h.read(len(buffer))
            #print "datos grabados:"
            #for i in dato_rex:
            #    r=ord(i)
            #    print r
            #print "***FIN***"
            #-------------------------------
            h.close   
            var2.set("**Envio exitoso**")
#
        elif letra=="DI":
            print "Grabar un equipo DIVISOR"
            #leer archivos de extension .div
            archivo = askopenfilename(filetypes = [('Archivos div', '*.div')])
            inList = open(archivo, 'rU').readlines()
            buffer = ""
            for line in inList:
                #---
                num=int(line)
                num_hex= "%x"%num
                nbytes=len(num_hex)
                if nbytes==1:
                    num_hex="00000"+num_hex
                elif nbytes==2:
                    num_hex="0000"+num_hex
                elif nbytes==3:
                    num_hex="000"+num_hex
                elif nbytes==4:
                    num_hex="00"+num_hex
                elif nbytes==5:
                    num_hex="0"+num_hex
                
                data_byte1=num_hex[0:2]
                data_byte2=num_hex[2:4]
                data_byte3=num_hex[4:6]
                buffer += "%c" % string.atoi(data_byte1,16)
                buffer += "%c" % string.atoi(data_byte2,16)
                buffer += "%c" % string.atoi(data_byte3,16)
                print data_byte1
                print data_byte2
                print data_byte3
                #---
            h.write(buffer)
            h.close
            var2.set("**Envio exitoso**")
			
        else:
            print "equipo desconocido"
    else:
            tkMessageBox.showinfo( "ERROR", "no se encuentra equipo")

def programar_cr(ruta_file):
    global g
    #Rutina de programacion de CR
    inList = open(ruta_file, 'rU').readlines()
    n = 4096
    buff_array = []
    frame = ""
    for line in inList:
        frame = frame + "%c" % int (line)
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
        g.write(z)
    g.close

def programar_rex(ruta_file):
    global g
    #rutina de programacion de REX
    #Generar archivo de configuracion para REX:
    fichero1 = open(ruta_file, 'rU').readlines()
    data1=[]
    for line in fichero1:
        cadena=EliminaSaltoDeLinea(line)
        data1.append(cadena)
    #Extraccion de datos:
    NumberChannel = ruta_file[-1] #Dato de numero de canales
    if NumberChannel == "d":
        NumberChannel = "2"
        print "El archivo configura 2 canal"
    elif NumberChannel == "1":
        print "El archivo configura 1 canal"
    else:
        print "La informacion de canales es desconocido"
        quit()
    File = data1[1]
    for i in range(len(File)):
        if File[i]==chr(92):
            l=i
    NameFile = File[l+1:]   # dato del nombre del archivo
    bloque1 = data1[5:19]
    datosA = []
    for letras in bloque1:
        for i in range(len(letras)):
            if letras[i]==' ':
                l=i
        datosA.append(letras[l+1:])
    if datosA[2] == "11111111111111111111111111111111":
        datosA[2] = "4294967295"
    else:
        print "Error en archivo"
        quit()
    #datosA = primer bloque de datos
    bloque2 = data1[19:-4]
    datosB = []
    for letras in bloque2:
        for i in range(len(letras)):
            if letras[i]==' ':
                l=i
        datosB.append(letras[l+1:])
    #datosB = segundo bloque de datos
    #-------Proceso de escritura de datos ----------
    fichero2 = open('C:/files_rex_out/'+NameFile[:-4]+'.rex','w')
    fichero2.write(NumberChannel+'\n')
    conta = 0
    for i in datosA:
        num = int(i)
        num_hex = "%x" %num
        nbytes = len(num_hex)
        ceros="00000000"
        valor_hex = ceros[0:(8-nbytes)]+num_hex
        data_byte_0 = valor_hex[0:2]
        data_byte_1 = valor_hex[2:4]
        data_byte_2 = valor_hex[4:6]
        data_byte_3 = valor_hex[6:8]
        fichero2.write('3'+'\n'+'7'+'\n')
        fichero2.write(str(conta)+'\n'+ '6'+'\n')
        conta = conta + 1
        fichero2.write("%s" % string.atoi(data_byte_0,16)+'\n'+'3'+'\n')
        fichero2.write("%s" % string.atoi(data_byte_1,16)+'\n'+'2'+'\n')
        fichero2.write("%s" % string.atoi(data_byte_2,16)+'\n'+'1'+'\n')
        fichero2.write("%s" % string.atoi(data_byte_3,16)+'\n'+'0'+'\n')
    
    conta = 0
    for i in datosB:
        num = int(i)
        if num<0:
            num =  4294967296+num
        num_hex = "%x" %num
        nbytes = len(num_hex)
        ceros="00000000"
        valor_hex = ceros[0:(8-nbytes)]+num_hex
        data_byte_0 = valor_hex[0:2]
        data_byte_1 = valor_hex[2:4]
        data_byte_2 = valor_hex[4:6]
        data_byte_3 = valor_hex[6:8]
        fichero2.write('0'+'\n'+'7'+'\n')
        fichero2.write(str(conta)+'\n'+ '6'+'\n')
        conta = conta + 1
        fichero2.write("%s" % string.atoi(data_byte_0,16)+'\n'+'3'+'\n')
        fichero2.write("%s" % string.atoi(data_byte_1,16)+'\n'+'2'+'\n')
        fichero2.write("%s" % string.atoi(data_byte_2,16)+'\n'+'1'+'\n')
        fichero2.write("%s" % string.atoi(data_byte_3,16)+'\n'+'0'+'\n')

    for i in data_final:
        fichero2.write(i)
    fichero2.close()
    #------------------------------------------------------
    #leer archivos de extension .rex
    inList = open('C:/files_rex_out/'+NameFile[:-4]+'.rex', 'rU').readlines()
    buffer = ""
    for line in inList:
        buffer =buffer + "%c" %int(line)
    g.write(buffer)   
    #-------------------------------
    #segmeto de codigo para depurar:
    #dato_rex = h.read(len(buffer))
    #print "datos grabados:"
    #for i in dato_rex:
    #    r=ord(i)
    #    print r
    #print "***FIN***"
    #-------------------------------
    g.close   

def programar_dds(ruta_file):
    global g
    inList = open(ruta_file, 'rU').readlines()
    buffer = ""
    for line in inList:
        if len(line) == 9 or len(line) == 8:
            buffer += "%c" % string.atoi(line,2)
    
    g.write(buffer)   
    g.close   

def programar_divisor(ruta_file):
    global g
    inList = open(ruta_file, 'rU').readlines()
    buffer = ""
    for line in inList:
        num=int(line)
        num_hex= "%x"%num
        nbytes=len(num_hex)
        if nbytes==1:
            num_hex="00000"+num_hex
        elif nbytes==2:
            num_hex="0000"+num_hex
        elif nbytes==3:
            num_hex="000"+num_hex
        elif nbytes==4:
            num_hex="00"+num_hex
        elif nbytes==5:
            num_hex="0"+num_hex
            
        data_byte1=num_hex[0:2]
        data_byte2=num_hex[2:4]
        data_byte3=num_hex[4:6]
        buffer += "%c" % string.atoi(data_byte1,16)
        buffer += "%c" % string.atoi(data_byte2,16)
        buffer += "%c" % string.atoi(data_byte3,16)
    
    g.write(buffer)   
    g.close 

data_final=['3'+'\n','7'+'\n','0'+'\n','6'+'\n','1'+'\n','0'+'\n','3'+'\n','7'+'\n','0'+'\n','6'+'\n','0'+'\n','0'+'\n','170']    

if len(sys.argv) == 1:
    
    root = Tk()
    root.title('Jro-USB_V1.2')
    #boton para identificar equipo:
    root.button = Button(root, text="Buscar equipo", fg="black", command=buscar)
    root.button.pack(padx=30, pady=10)
    
    #etiqueta donde se muestra el equipo:
    var = StringVar()
    msg = Label(root, textvariable=var)
    msg.pack(expand=YES, fill=BOTH)
    msg.config(width=60, height=7, bg='beige')
    
    #texbox de entrada:
    L1 = Label(root, text="Numero de acceso:")
    L1.pack( side = LEFT)
    E1 = Entry(root, bd =5)
    E1.pack(side = LEFT)
    
    root.button = Button(root, text="abrir y grabar", fg="black", command=buscar_archivo)
    root.button.pack(padx=25, pady=10)
    #etiqueta donde se muestra el equipo
    
    var2 = StringVar()
    msg = Label(root, textvariable=var2)
    msg.pack()
    msg.config(width=20, height=1, bg='beige')
    
    root.mainloop()
    
elif len(sys.argv) == 2:
    try:
        q = d2xx.listDevices()
        cadena = ""
        if len(q)>0:
            print "Equipos conectados:"
            for i in range(len(q)):
                print "[%d]"%i +" : "+ q[i]
        else:
            print "No hay equipos conectados"
    except:
        print "Espere, reconociendo equipo"
    
elif len(sys.argv) == 3:
    equipo = sys.argv[1]
    ruta = sys.argv[2]
    
    g = d2xx.openEx(equipo)
    g.setDivisor(3)   #aqui configuramos los baudios.    
    g.setDataCharacteristics(8,2,0)   
    g.setTimeouts(1000,1000)
    
    if equipo[:2] == "CR":
        programar_cr(ruta)
        print "programar CR"
    elif equipo[:2] == "RE":
        programar_rex(ruta)
        print "programar REX"
    elif equipo[:2] == "DD":
        programar_dds(ruta)
        print ("programar DDS")
    elif equipo[:2] == "DI":
        programar_divisor(ruta)
        print ("programar DIVISOR")
    else:
        print "parametros incorrectos"

else:
    print "Error en argumentos" + str(len(sys.argv))
