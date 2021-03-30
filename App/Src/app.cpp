/*
 * app.cpp
 *
 *  Created on: Mar 29, 2021
 *      Author: bormaley
 */
#include "app.h"
#include "cmsis_os.h"

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
	while(1) {
		uint8_t val = lsm303dhlc.readID();
		if (val == 0x32)
		{
			HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
		}
		osDelay(250);
	}
}

