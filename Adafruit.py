'''  * Archivo:   Adafruit.py
     * Dispositivo: PIC16F887
     * Autor: Margareth Vela 
     * Programa: Adafruit
     * Creado: Agosto 16, 2021
     * Última modificación: Agosto, 2021'''

'''------------------------------------------------------------------------------
                            Importación de librerías
-------------------------------------------------------------------------------- '''
from Adafruit_IO import Client, Feed
import serial
import time

'''------------------------------------------------------------------------------
                            Definición del puerto serial
------------------------------------------------------------------------------'''
com_serial = serial.Serial(port='COM3', baudrate = 9600, bytesize = 8, parity = 'N', stopbits = 1)

'''------------------------------------------------------------------------------
                            Conexión con Adafruit
------------------------------------------------------------------------------'''
ADAFRUIT_IO_USERNAME = "Margareth_Vela"
ADAFRUIT_IO_KEY = "aio_xGpp92aDBu2UOOdODCtUNKew9D6H"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
bef_val = ''

while(1):
    
    n = 0 #Valores iniciales
    contador = []
    puerto = []
        
    #Recibir valores del contador

    var = com_serial.read().decode('utf-8')
    if var == '\r':
        try:
            bef_val = valor
        except:
            continue
        while(n<3):
            var = com_serial.read().decode('utf-8')
            var_temp = var
            contador.append(var_temp)
            n = n+1
                   
    #Contador
    valor = ''.join(contador)
    try:
        if (bef_val == valor):
            continue
        else:
            val = valor
    except:
        continue
    
    try:
        contador_feed = aio.feeds('contador') #Se coloca llave del feed
        aio.send_data(contador_feed.key, val) #Se manda el valor despues de la coma
        contador_data = aio.receive(contador_feed.key)
        print(f'Valor del contador: {contador_data.value}')
    except:
        continue
    
    #Valor del puerto
    puerto_feed = aio.feeds('porta') #Se coloca llave del feed
    puerto_data = aio.receive(puerto_feed.key) #Se recibe el dato
    print(f'Valor del puerto: {puerto_data.value}')
    for i in puerto_data.value:
        puerto.append(i)
        
    #Mandar valores del puerto al pic
    puerto.append('\r')
    for i in puerto:
        unidad = i
        com_serial.write(unidad.encode('utf-8'))


