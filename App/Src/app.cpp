/*
 * app.cpp
 *
 *  Created on: Mar 29, 2021
 *      Author: bormaley
 */
#include "app.h"
#include "cmsis_os.h"
#include "usbd_cdc_if.h"

extern I2C_HandleTypeDef hi2c1;

void main_app_wrp()
{
	MainApp::get_instance().run();
}

MainApp::MainApp() :
		lsm303dhlc(&hi2c1, DATA_Ready_GPIO_Port, DATA_Ready_Pin)
{

}

void MainApp::run()
{
	uint8_t val = lsm303dhlc.readID();
	if (val == 0x32) {
		CDC_Transmit_FS((uint8_t*)"Who am I test correct", 20);
	}
	else {
		CDC_Transmit_FS((uint8_t*)"Incorrect who am I", 20);
	}
	lsm303dhlc.initAcc();
	lsm303dhlc.initMagnetometer();
	while(1) {
		int16_t data[3];
		lsm303dhlc.getMagnetomerMeasurements(data);
		char buffer[50];
		uint8_t n = sprintf(buffer, "X: %d\nY: %d\nZ: %d\n", data[0], data[1], data[2]);
		CDC_Transmit_FS((uint8_t*)buffer, n);
		osDelay(250);
		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	}
}

