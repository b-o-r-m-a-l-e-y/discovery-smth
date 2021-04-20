/*
 * app.cpp
 *
 *  Created on: Mar 29, 2021
 *      Author: bormaley
 */
#include "app.h"
#include "cmsis_os.h"
extern I2C_HandleTypeDef hi2c1;

#include "usbd_cdc_if.h"

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
	osDelay(100);
	lsm303dhlc.initAcc();
	lsm303dhlc.initMag();
	while(1) {
		static int16_t dataMag[3] = {0, 0, 0};
		char buffer[100];
		//uint8_t n = sprintf(buffer, "MagX=%i MagY=%i MagZ=%i ", dataMag[0], dataMag[1], dataMag[2]);
		//CDC_Transmit_FS((uint8_t*)buffer, n);

		static int16_t dataAcc[3] = {0, 0, 0};
		if (lsm303dhlc.getMagData(dataMag) == HAL_OK){
//			int8_t n = sprintf(buffer, "AccX=%d AccY=%d AccZ=%d ", dataAcc[0], dataAcc[1], dataAcc[2]);
//			CDC_Transmit_FS((uint8_t*)buffer, n);
			uint8_t n = sprintf(buffer, "MagX=%i MagY=%i MagZ=%i ", dataMag[0], dataMag[1], dataMag[2]);
			CDC_Transmit_FS((uint8_t*)buffer, n);
		}
		osDelay(10);
		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	}
}

