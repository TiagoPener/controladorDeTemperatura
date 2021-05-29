//código do nodemcu para pegar dado da serial recebido do arduino e depois publicar no mqtt da adafruit

/*********************** Inclusão de bibliotecas ********************/
#include <ESP8266WiFi.h>              //Inclusão da Biblioteca WiFi do ESP8266
#include "Adafruit_MQTT.h"            //Inclusão da Biblioteca MQTT da Adafruit.IO
#include "Adafruit_MQTT_Client.h"     //Inclusão da Biblioteca do cliente MQTT da Adafruit.IO
#include <SoftwareSerial.h> //Inclusão da Biblioteca Software serial para comunicar com o arduino

/******************* Configurações da Rede Wi-Fi ***************************/


#define WLAN_SSID       "fibra ludke"  //Nome da rede WiFi
#define WLAN_PASS       "xxxxx"          //Senha da rede Wifi

/************************* Configurações Adafruit.io *********************************/

#define AIO_SERVER      "io.adafruit.com"      //Servidor MQTT
#define AIO_SERVERPORT  1883                   //Porta do Servidor
#define AIO_USERNAME  "Ludke"         //Nome do usuário na Adafruit.IO
#define AIO_KEY       "xxxxx" //Senha do usuário na Adafruit.IO

/*************************** Configurações Globais  ***********************************/
SoftwareSerial softwareserial(13, 15);                                                    //classe do Software serial  (D7-RX) (D8-TX)
WiFiClient client;                                                                       //classe do WiFiClient
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);  //Configuração de classe do cliente MQTT, servidor, nome do usuário e chave de segurança

int valor_adc;
float temperatura_medida;
float tensao;
unsigned long previousMillis = 0;
const long intervalo = 10000;


//O padrão para subscrição e publicação é: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperatura = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperatura"); //a publicação da temperatura é igual ao padrão do tópico respectivo da temperatura

/*************************** conexão MQTT ************************************/

void MQTT_connect();    

/*************************** Laço de configuração************************************/

void setup() {         
  
  Serial.begin(115200);  //baudrate da serial do nodemcu
  softwareserial.begin(9600); // baudrate do software serial
  delay(10);            //atraso de 0,01s

  WiFi.begin(WLAN_SSID, WLAN_PASS);  //Inicializa WiFi com dados de ID da rede e senha
  while (WiFi.status() != WL_CONNECTED) { //Enquanto o status da rede for diferente de conectado
    delay(500);                    //atraso de 0,5s
                                        }

}

/*************************** Laço de repetição************************************/

void loop() {  
  // caso tenha chegado 3 bytes no rx do sofwtware serial
  if (softwareserial.available() >= 3) {    
    if (softwareserial.read() == 255) // caso o primeiro byte seja 255, identifica que é um dado vindo do arduino e armazena os dois bytes seguintes
    {                                 // que contém a informação do valor da conversão adc do arduino uno
    byte b1  = softwareserial.read(); // armazena o segundo byte recebido da serial
    byte b2 =  softwareserial.read(); // arnazeba o terceiro byte recebido da serial
    valor_adc = ((int)b1) * 256 + b2; // junta os dois bytes para restaurar o valor inteiro obtido na conversão adc do arduino
    //Serial.print(b1);
    //Serial.print(b2);
    Serial.println(valor_adc); 
    tensao = (valor_adc * 5.0) / 1023.0; // converter esse valor para tensão elétrica
    temperatura_medida = tensao / 0.01; // divide a tensão por 0.010 (10 mV) para obter a temperatura    
    Serial.println(temperatura_medida,2);
    }
  }        
  

  // a cada 10 segundos faz a publicação da temperatura no broker mqtt da adafruit
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= intervalo) {
    previousMillis = currentMillis;
    
    MQTT_connect();                           // conexão mqtt

  if (! temperatura.publish(temperatura_medida))        // se a temperatura não foi publicada
    Serial.println(F("Falha na publicação da temperatura")); //printa "Falha na publicação da temperatura"
  else                                        //caso contrário
   Serial.println(F("Temperatura publicada!")); //printa "Temperatura publicada!"
   
  Adafruit_MQTT_Subscribe *subscription;                //verifica as assinaturas
  while ((subscription = mqtt.readSubscription(5000))) { //enquanto é lida a subscrição mqtt

  }

  if(! mqtt.ping()) {      //executa ping para manter a conexão ativa
    mqtt.disconnect();    
                    }
    

  }  



}

/*************************** Laço de conexão - tratamento de conexão ************************************/

void MQTT_connect() {        
  
  int8_t ret;             //variável ret do tipo int8_t 


  if (mqtt.connected()) {   //se estiver conectado
    return;                 //retorna
  }

  Serial.print("Conectando... ");   //printa "Conectando..."

  uint8_t retries = 3;      //realiza 3 tentativas
  while ((ret = mqtt.connect()) != 0) { // Se conectado retorna valor diferente de 0
       Serial.println(mqtt.connectErrorString(ret)); // 
       Serial.println("Reconectando em 5s..."); //printa "Reconectando em 5s..."
       mqtt.disconnect();   //mqtt desconectado
       delay(5000);  //atraso de 5s
       retries--;     //decréscimo de reconexão
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("Conectado!");   //Printa conectado
  delay(5000);
}
