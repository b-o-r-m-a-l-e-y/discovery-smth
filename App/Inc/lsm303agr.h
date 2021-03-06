/*
 * lsm303agr.h
 *
 *  Created on: Mar 30, 2021
 *      Author: bormaley
 */

#ifndef LSM303AGR_H_
#define LSM303AGR_H_

#include "main.h"

struct CTRL_REG1_A {
	uint8_t ODR   : 4;  //Data rate selection
	uint8_t LPen  : 1;  //Low-power mode enable
	uint8_t Zen   : 1;  //Z-axis enable
	uint8_t Yen   : 1;  //Y-axis enable
	uint8_t Xen   : 1;  //Z-axis enable
} __attribute__((packed));

struct CTRL_REG2_A {
	uint8_t HPM     : 2; //High-pass filter mode
	uint8_t HPCF    : 2; //High-pass filter cutoff frequency
	uint8_t FDS     : 1; //Filtered data selection. 0 - bypass.
	uint8_t HPCLICK : 1; //High-pass filter enabled for click function
	uint8_t HPIS2   : 1; //High-pass filter enabled for AOI function on Interrupt 2
	uint8_t HPIS1   : 1; //High-pass filter enabled for AOI function on Interrupt 1
} __attribute__((packed));

struct CTRL_REG3_A {
	uint8_t I1_CLICK   : 1;
	uint8_t I1_AOI1    : 1;
	uint8_t I1_AOI2    : 1;
	uint8_t I1_DRDY1   : 1;
	uint8_t I1_DRDY2   : 1;
	uint8_t I1_WTM     : 1;
	uint8_t I1_OVERRUN : 1;
	uint8_t reserved   : 1;
} __attribute__((packed));

struct CTRL_REG4_A {
	uint8_t BDU        : 1;
	uint8_t BLE        : 1;
	uint8_t FS         : 2;
	uint8_t HR         : 1;
	uint8_t reserved   : 2;
	uint8_t SIM        : 1;
} __attribute__((packed));


enum LSM303AGR_ACC_ODR {
	LSM303AGR_ODR_1_HZ         = 0x10,
	LSM303AGR_ODR_10_HZ        = 0x20,
	LSM303AGR_ODR_25_HZ        = 0x30,
	LSM303AGR_ODR_50_HZ        = 0x40,
	LSM303AGR_ODR_100_HZ       = 0x50,
	LSM303AGR_ODR_200_HZ       = 0x60,
	LSM303AGR_ODR_400_HZ       = 0x70,
	LSM303AGR_ODR_1620_HZ_LP   = 0x80,
	LSM303AGR_ODR_1344_HZ      = 0x90
};

enum LSM303AGR_ACC_SENSITIVITY {
	LSM303AGR_ACC_SENSITIVITY_2G   = 0x00,
	LSM303AGR_ACC_SENSITIVITY_4G   = 0x10,
	LSM303AGR_ACC_SENSITIVITY_8G   = 0x20,
	LSM303AGR_ACC_SENSITIVITY_16G  = 0x30
};

enum LSM303AGR_ACC_MODE {
	LSM303AGR_ACC_MODE_Normal,
	LSM303AGR_ACC_MODE_Low_power,
	LSM303AGR_ACC_MODE_High_res
};

class LSM303AGR {
public:
	LSM303AGR(I2C_HandleTypeDef* hi2c, GPIO_TypeDef* DRDY_GPIO, uint32_t DRDY_pin);
	~LSM303AGR();

	/*
	 * Accelerometer functions
	 */
	HAL_StatusTypeDef initAcc();
	uint8_t readAccID();
	HAL_StatusTypeDef writeRegisterAcc(uint8_t regAddr, uint8_t reg);
	uint8_t readRegisterAcc(uint8_t regAddr);
	HAL_StatusTypeDef getAccData(int16_t* pData);
	HAL_StatusTypeDef enableTemperatureSensor();

	/*
	 * Magnetometer functions
	 */
	HAL_StatusTypeDef initMag();
	uint8_t readMagID();
	HAL_StatusTypeDef writeRegisterMag(uint8_t regAddr, uint8_t reg);
	uint8_t readRegisterMag(uint8_t regAddr);
	HAL_StatusTypeDef getMagData(int16_t* pData);
	int16_t getTemperature();

private:

	I2C_HandleTypeDef* hi2c;
	uint32_t       DRDY_pin;
	GPIO_TypeDef*  DRDY_GPIO;
	HAL_StatusTypeDef error_status = HAL_OK;

	uint8_t accStatus = 0;
	uint8_t magStatus = 0;

	LSM303AGR_ACC_MODE powerMode;
	LSM303AGR_ACC_SENSITIVITY sensitivity;
};

#endif /* LSM303AGR_H_ */
