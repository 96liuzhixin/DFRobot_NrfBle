 /*!
  * @file  Peripheral.ino
  * @brief Peripheral
  *
  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  * @licence     The MIT License (MIT)
  * @author      ZhixinLiu(zhixin.liu@dfrobot.com)
  * @version     V1.0
  * @date        2020-6-8
  * @get         from https://www.dfrobot.com
  * @url         https://github.com/DFRobot/DFRobot_NrfBle
  */
#include "DFRobot_NrfBle.h"
#define   CS_PIN   10                       // spi chip selection
DFRobot_NrfBle_SPI nrfble(&SPI, CS_PIN);

void callback(uint8_t modes ,uint8_t *data ,uint8_t len ,int32_t number)
{
  switch(modes)
  {
    case RADIO_STRING:
    case BLE_STRING:
      for(uint8_t i = 0; i < len; i++)
        Serial.print((char)data[i]);
      Serial.println();
      break;
    case RADIO_NUMBER:
      Serial.println(number);
      break;
    case RADIO_VALUE:
      for(uint8_t i = 0; i < len; i++)
        Serial.print((char)data[i]); Serial.print(":");
      Serial.println(number);
      break;
    default:
      break;
  }
}

void setup() 
{
  Serial.begin(115200);
  nrfble.begin();
  delay(1000);
  /*Set startup mode
     RADIO_MODE
     PERIPHERAL_MODE
     CERNTAL_MODE
     KEYBOARD_MODE
  */
  nrfble.setModes(PERIPHERAL_MODE);
  delay(2000);
  nrfble.setCallback(callback);
}

uint8_t number = 0;
void loop()
{
  if(number == 20){
    nrfble.sendData("hello_world" ,11);
    number = 0;
  }else{
    number++;
  }
  nrfble.LOOP();
  delay(5);
}