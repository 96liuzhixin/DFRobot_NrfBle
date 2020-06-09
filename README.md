# DFRobot_NrfBle
DFRobot's NrfBle

## DFRobot_NrfBle Library for Arduino
---------------------------------------------------------
Arduino library is provided for wireless communication

## Table of Contents

* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

<snippet>
<content>

## Installation

To use this library download the zip file, uncompress it to a folder named DFRobot_NrfBle.
Download the zip file first to use this library and uncompress it to a folder named DFRobot_NrfBle.

## Methods

```C++

  /**
   * @brief Initialize spI and CS pins
   */
  void begin(void);

  /**
   * @brief set modes
   * @param RADIO_MODE        2.4G mode
   * @param PERIPHERAL_MODE   ble peripheral mode
   * @param CERNTAL_MODE      ble cerntal mode
   * @param KEYBOARD_MODE     ble keyboard mode
   * @return 0 is error 1 is success
   */
  uint8_t setModes(uint8_t modes);

  /**
   * @brief set group
   * @param group range 1 - 255
   * @return 0 is error 1 is success
   */
  uint8_t setGroup(uint8_t group);

  /**
   * @brief Set callback function
   * @param *call is function name
   * @param (uint8_t ,uint8_t * ,uint8_t ,int32_t)  is The parameters of the callback function 
   */
  void setCallback(void (*call)(uint8_t, uint8_t* ,uint8_t, int32_t));

  /**
   * @brief send data
   * @param string send string
   * @param len    string size
   */
  void sendData(const char *string, uint8_t len);

  /**
   * @brief send data
   * @param number send number
   */
  void sendData(int32_t number);

  /**
   * @brief send data
   * @param string send string
   * @param len    string size
   * @param number send number
   */
  void sendData(const char *string, uint8_t len, int32_t number);

  /**
   * @brief Loop Read data
   */
  void LOOP(void);

```
## Compatibility

MCU                | Work Well | Work Wrong | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno  |      √       |             |            | 
Leonardo  |      √       |             |            | 
Meag2560 |      √       |             |            | 
ESP32 |      √       |             |            | 

## History

- date 2020-6-8
- version V1.0

## Credits

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2020. (Welcome to our [website](https://www.dfrobot.com/))