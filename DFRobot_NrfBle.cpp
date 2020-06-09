/*!
 * @file DFRobot_NrfBle.cpp
 * @brief Define the infrastructure of the DFRobot_NrfBle class and the implementation of the underlying methods
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author      [ZhixinLiu](zhixin.liu@dfrobot.com)
 * @version     V1.0
 * @date        2020-6-8
 * @url         https://github.com/DFRobot/DFRobot_NrfBle
 */
#include "DFRobot_NrfBle.h"

DFRobot_NrfBle::DFRobot_NrfBle()
{
}
DFRobot_NrfBle::~DFRobot_NrfBle()
{
}

uint8_t DFRobot_NrfBle::setModes(uint8_t modes)
{
  _sendArray[0] = 0x55;
  _sendArray[2] = 0x05;
  _sendArray[4] = 0xAA;
  switch(modes)
  {
    case RADIO_MODE:
      _mode = RADIO_MODE;
      _sendArray[1] = 0x34;
      _sendArray[3] = 0x34;
      writeReg(_sendArray , _sendArray[2]);
      break;
    case PERIPHERAL_MODE:
      _mode = BLE_MODE;
      _sendArray[1] = 0x35;
      _sendArray[3] = 0x35;
      writeReg(_sendArray , _sendArray[2]);
      break;
    case CERNTAL_MODE:
      _mode = BLE_MODE;
      _sendArray[1] = 0x36;
      _sendArray[3] = 0x36;
      writeReg(_sendArray , _sendArray[2]);
      break;
    case KEYBOARD_MODE:
      _mode = KEYBOARD_MODE;
      _sendArray[1] = 0x38;
      _sendArray[3] = 0x38;
      writeReg(_sendArray , _sendArray[2]);
      break;
    default:
      DBG("mode error ,Please try again!");
      return 0;
      break;
  }
  return 1;
}

uint8_t DFRobot_NrfBle::setGroup(uint8_t group)
{
  _sendArray[0] = 0x55;
  _sendArray[1] = 0x33;
  _sendArray[2] = 0x06;
  _sendArray[3] = 0x0F;
  _sendArray[4] = group;
  _sendArray[5] = 0xAA;
  if(_mode == RADIO_MODE){
    writeReg(_sendArray , _sendArray[2]);
  }else{
    DBG("mode mismatching!");
    return 0;
  }
  return 1;
}

void DFRobot_NrfBle::sendData(const char *string, uint8_t len)
{
  _sendArray[0] = 0x55;
  _sendArray[1] = 0x31;
  if(len <= 19) {
    _sendArray[2] = 6 + len;
    _sendArray[4] = len;
    memcpy(&_sendArray[5] ,string ,len);
    _sendArray[5+len] = 0xAA;
  }else{
    _sendArray[2] = 25;
    _sendArray[4] = 19;
    memcpy(&_sendArray[5] ,string ,19);
    _sendArray[24] = 0xAA;
  }
  if(_mode == RADIO_MODE){
    _sendArray[3] = 0x32;
    writeReg(_sendArray , _sendArray[2]);
  }else if(_mode == BLE_MODE){
    _sendArray[3] = 0x40;
    writeReg(_sendArray , _sendArray[2]);
  }else{
  }
}

void DFRobot_NrfBle::sendData(int32_t number)
{
  uint8_t sendNumber[4] = {0};
  _sendArray[0] = 0x55;
  _sendArray[1] = 0x31;
  if(_mode == RADIO_MODE){
    analysisNumber(number ,sendNumber);
    _sendArray[2] = 0x09;
    _sendArray[3] = 0x30;
    memcpy(&_sendArray[4] ,sendNumber ,4);
    _sendArray[8] = 0xAA;
    writeReg(_sendArray , _sendArray[2]);
  }else if(_mode == KEYBOARD_MODE){
    _sendArray[2] = 0x06;
    _sendArray[3] = 0x41;
    _sendArray[4] = (uint8_t)number;
    _sendArray[5] = 0xAA;
    writeReg(_sendArray , _sendArray[2]);
  }else{
  }
}

void DFRobot_NrfBle::sendData(const char *string, uint8_t len, int32_t number)
{
  uint8_t sendNumber[4] = {0};
  analysisNumber(number ,sendNumber);
  _sendArray[0] = 0x55;
  _sendArray[1] = 0x31;
  _sendArray[3] = 0x31;
  memcpy(&_sendArray[4] ,sendNumber ,4);
  if(len <= 13) {
    _sendArray[2] = 10+len;
    _sendArray[8] = len;
    memcpy(&_sendArray[9] ,string ,len);
    _sendArray[9+len] = 0xAA;
  }else{
    _sendArray[2] = 23;
    _sendArray[8] = 13;
    memcpy(&_sendArray[9] ,string ,13);
    _sendArray[22] = 0xAA;
  }
  if(_mode == RADIO_MODE){
    writeReg(_sendArray , _sendArray[2]);
  }
  
}

void DFRobot_NrfBle::analysisNumber(int32_t number ,uint8_t * sendNumber)
{
  if(number > 0) {
    for(uint8_t i = 0; i < 4; i++) sendNumber[i] = number >> (i*8);
  }else {
    number *= -1;
    for(uint8_t i = 0; i < 4; i++) sendNumber[i] = number >> (i*8);
    sendNumber[0] = (~sendNumber[0]) + 1;
    sendNumber[1] = ~sendNumber[1];
    sendNumber[2] = ~sendNumber[2];
    sendNumber[3] = ~sendNumber[3];
  }
}

void DFRobot_NrfBle::LOOP()
{
  int32_t radionumber = 0;
  _sendArray[0] = 0x55;
  _sendArray[1] = 0x32;
  _sendArray[2] = 0x20;
  memset(&_sendArray[3], 0 ,28);
  _sendArray[31] = 0xAA;
  readReg(_sendArray ,32);
  if(_spiData[0] == 0x55){
    if(_spiData[1] == 0x31){
      if(callback) {
        callback(BLE_STRING ,&_spiData[3] ,_spiData[2] ,radionumber);
      }
    }else{
      switch(_spiData[3]){
        case 0:
          if((_spiData[7] >> 7) == 0x01) {
              _spiData[4] = ~(_spiData[4] - 1);
              _spiData[5] = ~_spiData[5];
              _spiData[6] = ~_spiData[6];
              _spiData[7] = ~_spiData[7];
              radionumber = (((int32_t)_spiData[7] << 24) + ((int32_t)_spiData[6] << 16) + ((int32_t)_spiData[5] << 8) + ((int32_t)_spiData[4])) * -1;
          }else{
            radionumber = ((int32_t)_spiData[7] << 24) + ((int32_t)_spiData[6] << 16) + ((int32_t)_spiData[5] << 8) + ((int32_t)_spiData[4]);
          }
            if(callback) {
              callback(RADIO_NUMBER ,NULL ,0 ,radionumber);
            }
          break;
        case 1:
          if((_spiData[7] >> 7) == 0x01) {
              _spiData[4] = ~(_spiData[4] - 1);
              _spiData[5] = ~_spiData[5];
              _spiData[6] = ~_spiData[6];
              _spiData[7] = ~_spiData[7];
              radionumber = (((int32_t)_spiData[7] << 24) + ((int32_t)_spiData[6] << 16) + ((int32_t)_spiData[5] << 8) + ((int32_t)_spiData[4])) * -1;
          }else{
            radionumber = ((int32_t)_spiData[7] << 24) + ((int32_t)_spiData[6] << 16) + ((int32_t)_spiData[5] << 8) + ((int32_t)_spiData[4]);
          }
            if(callback) {
              callback(RADIO_VALUE ,&_spiData[9] ,_spiData[8] ,radionumber);
            }
          break;
        case 2:
          if(callback) {
              callback(RADIO_STRING ,&_spiData[5] ,_spiData[4] ,0);
          }
          break;
        default:
          break;
      }
    }
  }
}

void DFRobot_NrfBle::setCallback(void (*call)(uint8_t, uint8_t* ,uint8_t , int32_t))
{
  this->callback = call;
}


DFRobot_NrfBle_SPI::DFRobot_NrfBle_SPI(SPIClass *pSpi, uint8_t csPin)
{
  _pSpi = pSpi;
  _csPin = csPin;
}

void DFRobot_NrfBle_SPI::begin(void)
{
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, 1);
  _pSpi->begin();
}

void DFRobot_NrfBle_SPI::writeReg(void * pBuf, size_t size)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  digitalWrite(_csPin, 0);
  for (uint8_t i = 0; i < size; i++) {
    _pSpi->transfer(*_pBuf);
    _pBuf++;
  }
  digitalWrite(_csPin, 1);
}

uint8_t * DFRobot_NrfBle_SPI::readReg(void* pBuf, size_t size)
{
  if(pBuf == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)pBuf;
  digitalWrite(_csPin, 0);
  for (uint8_t i = 0; i < size ; i++) {
    _spiData[i] = _pSpi->transfer(*_pBuf);
    _pBuf++;
  }
  digitalWrite(_csPin, 1);
  return _spiData;
}