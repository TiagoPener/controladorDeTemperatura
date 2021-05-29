#include <Servo.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>


/*************************** Configurações Globais  ***********************************/
SoftwareSerial mySerial(11, 12); // // classe software serial que vai ser ligado no nodemcu 
Servo myservo;  // classe do servo

LiquidCrystal_I2C lcd(0x3E,16,2);  //classe do lcd objeto incia o LCD com endereço da i2c 0x3E e display 16x2


int pinotemperatura = A0;  // pino A0 ligado no Vout do lm35
int valor_adc =0;    // variável que armazena o valor do adc
float temperatura = 0;    //variável que armazena o valor da temperatura.

void setup() {
  
  myservo.attach(9);  // attach servo no pino D9 do arduino
  Serial.begin(9600);
  mySerial.begin(9600); 
  // inicia o lcd
  lcd.init();
  //inicia o led de backlight do lcd
  lcd.backlight();
  
}

void loop() {
  valor_adc = analogRead(pinotemperatura);           // Obtém o valor analógico que varia de 0 a 1023
  //Serial.println(valor_adc);  
  mySerial.write(255); // envia o primeiro byte com valor 255 que representa o inicio da mensagem na serial 
  // como a variável valor_adc é um inteiro de 16 bits, é necessário mandar dividir em 8 bits para enviar na serial.
  mySerial.write((byte)valor_adc / 256); // envia na segunda serial (nodemcu) os 8 bits mais significativos da variável int valor_adc
  mySerial.write((byte)valor_adc % 256); // envia na segunda serial (nodemcu) os 8 bits menos significativos da variável int valor_adc
  float tensao = (valor_adc * 5.0) / 1023.0; // Converte o valor do adc para tensão elétrica
  float temperatura = tensao / 0.010; // dividimos a tensão por 0.010 que representar os 10 mV      
  
  Serial.print("temperatura = ");
  Serial.println(temperatura);

  //imprime no lcd o valor da temperatura
  lcd.setCursor(0, 0);
  lcd.print("T = ");
  lcd.print(temperatura);
  lcd.print((char)223);
  lcd.print("C");

  //caso temporatura for maior que 28 graus movimenta o servomotor
  if (temperatura > 28)
  {  
    myservo.write(0);                  // move servomotor para 0 graus  
    delay(1000);                       
    myservo.write(180);               // move servomotor para 180 graus      
    delay(1000);    
  } 
  delay(1000);
}
