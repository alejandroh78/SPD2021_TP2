//TP2
//MATEO HOLIK
//ALEJANDRO HOLIK
#include<LiquidCrystal.h>
#include<Keypad.h>

#define NO_BOTON 0
#define BOTON_1  1
#define BOTON_2  2

LiquidCrystal lcd(8,13,A0,A1,A2,A3);

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] =
{
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'x','0','=','/'}
};

byte rowPins[ROWS] = {7, 6, 5, 4}; //filas
byte colPins[COLS] = {3, 2, 1, 0}; //columnas

Keypad myKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

boolean valorActual = false;
boolean final = false;

String num1Str = "";
String num2Str = "";
String numBoton1Str="";
String totalStr = "";

boolean controlBoton1 = false;

int total; 
int movimiento;
char operacion;
char teclaIngresada;
int numLength;
int error=0;

int num1Int = 0;
int num2Int = 0;
int numBoton1Int = 0;
int totalInt = 0;
int botonAhora = 0;
int botonAntes = NO_BOTON;

//variable que presiono igual para controlar negativo
boolean chkResultadoGenerado = false;

//contador operadores
int cantOperadores = 0;

//de esta funcion se obtiene el valor NO_KEY que es vacio
//keypad retorna la tecla ingresada por el usuario


void setup()
{
 lcd.begin(16,2);
 //setea el pin A4 como uno de entrada
 pinMode(A4, INPUT);
}


void loop()
{
  //
  teclaIngresada = myKeypad.getKey();
  
  ingresoNumero(teclaIngresada);
  
  positivoNegativo(teclaIngresada);
  
  inputOperador(valorActual, teclaIngresada);
  //
  calculoOperacion(num1Int, num2Int, operacion, teclaIngresada);
  //
  botonAhora = leerBoton();
  //
  botonPresionado(botonAhora);
  delay(50);
}

/******************************************************************
funcion que identifica si fue presionada uno de los pulsadores
******************************************************************/
void botonPresionado(int botonAhora)
{
  //funcion anti-rebote de boton
  if(botonAhora != NO_BOTON && botonAhora != botonAntes)
  {
      switch(botonAhora)
      {
        case 1:
          if (chkResultadoGenerado == true)
          {
            numBoton1Str = totalStr;
            controlBoton1 = true;
            num1Str = String(totalStr);
            num1Int = totalInt;//toma el valor ingresado primeramente
            num2Str = "";
            lcd.clear();  
            chkResultadoGenerado = false;
          }
        break;

        case 2:
          lcd.clear();
          lcd.print(num1Int);
        break;

      }
  }
  botonAntes = botonAhora;  
}

/******************************************************************
funcion que mide el voltaje y de acuerdo al boton presionado
verifica en que rango de voltaje se encuentra y retonra en una vairable int
el boton presionado
******************************************************************/
int leerBoton()
{
    int valorA4 = analogRead(A4); //devuelve el valor int
    int retorno = NO_BOTON;
  
    if(valorA4>510 && valorA4<531)
    {
        retorno = BOTON_1;
    }
        if(valorA4>681 && valorA4<700)
    {
        retorno = BOTON_2;
    }
        return retorno;
}

/******************************************************************
La funcion recibe la tecla ingresada por el usuario e imprime en
el display el simbolo de la operacion; + - * /
******************************************************************/
void inputOperador(boolean valorActual, char teclaIngresada)
{

    if (valorActual == false &&  num1Str != "" && teclaIngresada != NO_KEY && (teclaIngresada == '/' || teclaIngresada == '*' || teclaIngresada == '-' || teclaIngresada == '+'))
    {
        if (cantOperadores == 0)
        {
          cantOperadores++;
          valorActual = true;
          operacion = teclaIngresada;
          lcd.clear();  
          lcd.print(num1Int);
          lcd.print(operacion);
        }
        else
        {
          cantOperadores++;
          valorActual = true;
          operacion = teclaIngresada;
          lcd.clear();  
          lcd.print(num1Int);
          lcd.print(operacion);
          num2Str = "";
          num2Int = 0;
        }
    }
}

/******************************************************************
La funcion recibe la tecla ingresada por el usuario
transforma el numero de char a entero y lo imprime por pantalla
******************************************************************/
void ingresoNumero(char teclaIngresada)
{

    if (teclaIngresada != NO_KEY && (teclaIngresada=='9'||teclaIngresada=='8'||teclaIngresada=='7'||teclaIngresada=='6'||teclaIngresada=='5'
    ||teclaIngresada=='4'||teclaIngresada=='3'||teclaIngresada=='2'||teclaIngresada=='1'||teclaIngresada=='0'))
    {

      if (cantOperadores == 0)
      {
        num1Str = num1Str + teclaIngresada;
        num1Int = String(num1Str).toInt();
        numLength = num1Str.length();
        movimiento = numLength;
        lcd.setCursor(0, 0);
        lcd.print(num1Int);          
      }
      else
      {
        num2Str = num2Str + teclaIngresada;
        num2Int = String(num2Str).toInt();
        movimiento = (num1Str.length()) + (String(operacion).length());
        lcd.setCursor(movimiento, 0);
        lcd.print(num2Int);
        final = true;          
      }
    }
}

/******************************************************************
La funcion recibe la tecla ingresada y comprueba si es un numero positivo o uno negativo
******************************************************************/
void positivoNegativo(char teclaIngresada)
{

    if (teclaIngresada != NO_KEY && teclaIngresada == 'x')  
    {

        if (chkResultadoGenerado == false)
        {
            if (num1Int > 0)
            {
                num1Int = -abs(num1Int);
                num1Str = String("-" + num1Str);
                lcd.setCursor(0,0);
                lcd.print(num1Int);
            }
            else
            {
                lcd.setCursor(0,0);
                num1Int = abs(num1Int);
                num1Str = String(num1Str);
                movimiento = numLength;
                lcd.clear();      
                lcd.print(num1Int);              
            }
        }

    }
}

/******************************************************************
La funcion recibe los numeros ingresados y el simbolo que se desea aplicar a la cuenta
segun el operador ingresado (+, -, /, *), realiza el calculo y lo imprime por pantalla 
******************************************************************/
void calculoOperacion(int numero1, int numero2, char simbolo, char teclaIngresada)
{
 
    if (final == true && teclaIngresada != NO_KEY && teclaIngresada == '=')
    {  
        if (simbolo == '+')
        {
            totalInt = (numero1) + (numero2) ;
        }
        else if (simbolo == '-')
        {
        if(numero1 >=0)
        {
            totalInt = (numero1) - (numero2);
        }
        else
        {
            totalInt = (numero1) + (-numero2);
        }
        }
        else if (simbolo == '*')
        {
            totalInt = numero1 * numero2 ;
        }
        else if (simbolo == '/')
        {
            if(numero2==0)
            {
                lcd.clear();
                lcd.print(numero1);
                lcd.print(simbolo);
                lcd.print(numero2);

                lcd.setCursor(0,2);

                lcd.print("Error / por 0");    
                error=1;
            }
            else
            {
                totalInt = numero1 / numero2;
                error=0;
            }
        }

        chkResultadoGenerado = true;
        lcd.setCursor(0,2);

        if(error!=1)
        {
            lcd.print("=");
            lcd.print(totalInt);
            totalStr = String(totalInt);
        }
    }
}
