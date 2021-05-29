# controladorDeTemperatura

###########################################################################################################

O controlador de temperatura identifica variações de temperatura no ambiente e ativa o servo-motor
uma vez que essa temperatura ultrapassa 28º C. Os registros de temperaturas são informados ao
servidor adafruit.

###########################################################################################################

O projeto não se comunica por protocolo firmata, e sim pela leitura do conversor digital analógico, 
que  escreve na serial do arduino para visualizar as informações captadas e utilizamos o software serial
pra comunicar com o nodemcu.

Após a comunicação com o nodemcu, através do protocolo MQTT a informação é enviada ao
servidor broken Adafruit.

No servidor Adafruit é possível visualizar a comunicação da informação captada e sua
interface. Segue o link: https://io.adafruit.com/Ludke/dashboards/monitora

###########################################################################################################

Plataformas de desenvolvimento:

Arduino IDE e servidor broker Adafruit

Principais Componentes:

- Sensor de temperatura lm35 de medidas 0,5×0,5cm
- Atuador servo-motor de medidas 41x20x43mm
- lcd 16x2 i2c de medidas 36mm(L) x 17mm(A) x 83mm(C)
- Arduino Uno de medidas 68,58mm x 53,34mm
- Placa nodemcu ESP8266 de medidas 49 x 25,5 x 7 mm;

###########################################################################################################
