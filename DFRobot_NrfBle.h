/*!
 * @file  DFRobot_NrfBle.h
 * @brief Defines the infrastructure of the DFRobot_NrfBle class
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author      [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version     V1.0
 * @date        2020-6-8
 * @url         https://github.com/DFRobot/DFRobot_NrfBle
 */
#ifndef __DFRobot_NRFBLE_H__
#define __DFRobot_NRFBLE_H__
#include "Arduino.h"
#include <stdlib.h>
#include <SPI.h>
#include <math.h>

#define  RADIO_MODE         0
#define  BLE_MODE           1
#define  PERIPHERAL_MODE    1
#define  CERNTAL_MODE       2
#define  KEYBOARD_MODE      3
#define  RADIO_NUMBER       10
#define  RADIO_STRING       11
#define  RADIO_VALUE        12
#define  KEYBOARD_NUMBER    13
#define  BLE_STRING         14



#define ENABLE_DBG

#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

class DFRobot_NrfBle{
public:
  DFRobot_NrfBle();
  ~DFRobot_NrfBle();
  uint8_t setModes(uint8_t modes);
  uint8_t setGroup(uint8_t group);
  void sendData(const char *string, uint8_t len);
  void sendData(int32_t number);
  void sendData(const char *string, uint8_t len, int32_t number);
  void LOOP(void);
  void setCallback(void (*call)(uint8_t, uint8_t* ,uint8_t, int32_t));
  void (* callback)(uint8_t modes ,uint8_t * data ,uint8_t len, int32_t number);
protected:
  virtual void writeReg(void* pBuf, size_t size) = 0;
  virtual uint8_t * readReg(void* pBuf, size_t size) = 0;
  uint8_t _spiData[32] = {0};
  uint8_t _sendArray[32] = {0};
  
private:
  void analysisNumber(int32_t number ,uint8_t * sendNumber);
  uint8_t _mode;
};


class DFRobot_NrfBle_SPI:public DFRobot_NrfBle{
public:
  DFRobot_NrfBle_SPI(SPIClass *spi=&SPI, uint8_t csPin=10);
  void begin(void);
protected:
  virtual void writeReg(void* pBuf, size_t size);
  virtual uint8_t * readReg(void* pBuf, size_t size);

private:
  SPIClass *_pSpi;
  uint8_t _csPin;
};

#endif