/*
 * Archivo:   Lab05.c
 * Dispositivo: PIC16F887
 * Autor: Margareth Vela 
 * 
 * Programa: PIC y ADAFRUIT IO
 * Hardware: Push buttons en PORTB y FTDI en PORTC.
 * 
 * Creado: Agosto 15, 2021
 * Última modificación: Agosto, 2021
 */

//------------------------------------------------------------------------------
//                          Importación de librerías
//------------------------------------------------------------------------------    
 #include <xc.h>
 #include <stdint.h>

//------------------------------------------------------------------------------
//                          Directivas del compilador
//------------------------------------------------------------------------------
#define _XTAL_FREQ 8000000 //Para delay

//------------------------------------------------------------------------------
//                          Variables
//------------------------------------------------------------------------------
uint8_t contador; //Variables para el contador 
uint8_t unidades = 0;
uint8_t decenas = 0;
uint8_t centenas = 0;
uint8_t decenas_temp = 0;

uint8_t u_flag = 1; //Variables para el valor que recibe el pic 
uint8_t d_flag = 0;
uint8_t c_flag = 0;
uint8_t unidad = 0;
uint8_t decena = 0;
uint8_t valor = 0;
uint8_t var_temp = 0; //Variables temporales
uint8_t cont_temp = 0;

//------------------------------------------------------------------------------
//                          Configuración
//------------------------------------------------------------------------------
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

//------------------------------------------------------------------------------
//                          Prototipos
//------------------------------------------------------------------------------
void setup(void);  //Configuración

void main(void) {
    setup();
    while(1){
        contador = PORTD;
        centenas = contador/100; //Se divide por 100 para obtener las centenas
        decenas_temp = contador%100;//El residuo se almacena en la variable temporal de decenas
        decenas = decenas_temp/10;//Se divide en 10 el valor de decenas_temp para 
                                  //obtener el valor a desplegar en el display   
        unidades = contador%10;//El residuo se almacena en unidades

        if (PIR1bits.TXIF){
                    __delay_ms(20);
                    TXREG = centenas + 48; //Envía el valor de centenas del contador
                    __delay_ms(20);
                    TXREG = decenas + 48; //Envía el valor de decenas del contador
                    __delay_ms(20);
                    TXREG = unidades + 48; //Envía el valor de unidades del contador
                    __delay_ms(20);
                    TXREG = 0x0D; //Envia el valor de enter
                    __delay_ms(20);
            }
        PIR1bits.TXIF = 0; //Se limpia la bandera
        }
    
    return;
}
//------------------------------------------------------------------------------
//                          Interrupciones
//------------------------------------------------------------------------------
void __interrupt() isr(void){    
    if(INTCONbits.RBIF){
            if (PORTBbits.RB0 == 0){ //Si el botón de incremento está presionado,
                PORTD++; //se incrementa PORTD
            }
            if(PORTBbits.RB1 == 0) {//Si el botón de decremento está presionado,
                PORTD--; //se decrementa PORTD
            }
        INTCONbits.RBIF = 0; //Se limpia la bandera
        }
    
        
    if(PIR1bits.RCIF == 1){ //Empieza a recibir datos del USART
        if (RCREG ==  0x0D){
        PORTA = valor; //Se coloca el valor que recibe el pic a PORTA
        cont_temp = 0;
        valor = 0;
        u_flag = 1;
        d_flag = 0;
        c_flag = 0;
        }
        
        if (RCREG !=  0x0D){
        var_temp = RCREG; //Cambio de valor ASCII a decimal
            if(var_temp==48){
                cont_temp = 0;
            }else if(var_temp==49){
                cont_temp = 1;
            }else if(var_temp==50){
                cont_temp = 2;
            }else if(var_temp==51){
                cont_temp = 3;
            }else if(var_temp==52){
                cont_temp = 4;
            }else if(var_temp==53){
                cont_temp = 5;
            }else if(var_temp==54){
                cont_temp = 6;
            }else if(var_temp==55){
                cont_temp = 7;
            }else if(var_temp==56){
                cont_temp = 8;
            }else if(var_temp==57){
                cont_temp = 9;
            }
        if (u_flag == 1){
            valor = cont_temp;
            unidad = cont_temp;
            u_flag = 0;
            d_flag = 1; //Se enciende el valor de decenas
        }
        else if (d_flag == 1){
            valor = (unidad*10)+cont_temp;
            decena = cont_temp;
            d_flag = 0;
            c_flag = 1; //Se enciende el valor de centenas
        }
        else if (c_flag == 1){
            valor = (unidad*100)+(decena*10)+cont_temp;
        }     
        }}
}

//------------------------------------------------------------------------------
//                          Configuración
//------------------------------------------------------------------------------
void setup(){
    
    //Configuracion reloj
    OSCCONbits.IRCF2 = 1; //Frecuencia a 8MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    
    //Configurar entradas y salidas
    ANSELH = 0x00;//Pines digitales
    ANSEL = 0x00; 
    
    TRISA = 0x00; //Para salida del contador
    TRISB = 0x03; //Para push buttons
    TRISC = 0x80; //Para FTDI
    TRISD = 0x00; //Para contador
               
    //Habilitar pullups
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0x03;
    
    PORTA = 0x00; //Se limpian los puertos
    PORTB = 0x03; //Para condición de mismatch 
    PORTC = 0x00;
    PORTD = 0x00;
    
    //Configurar la interrupcion
    INTCONbits.GIE = 1;  //Enable interrupciones globales   
    INTCONbits.PEIE = 1; //Enable de periféricos
    PIE1bits.RCIE = 1;   //Enable RC
    PIR1bits.RCIF = 0;   //Se limpia la bandera
    
    //Interrupcion PORTB
    INTCONbits.RBIE = 1; //Enable Interrupt on change
    IOCB = 0x03;
    INTCONbits.RBIF = 0; //Se limpia la bandera de Interrupt on change
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 207;
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;//Configuración del USART y Baud Rate
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;
    
    TXSTAbits.TXEN = 1; 	
    return;
 }  
