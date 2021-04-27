/*
 * app.cpp
 *
 *  Created on: Mar 29, 2021
 *      Author: bormaley
 */
#include "app.h"
extern I2C_HandleTypeDef hi2c1;

extern "C"
{

}

#include "usbd_cdc_if.h"

void main_app_wrp()
{
	MainApp::get_instance().run();
}

void getAccDataTaskC(void* argument)
{
	MainApp::get_instance().getAccMagDataTask();
}

MainApp::MainApp() :
		lsm303agr(&hi2c1, DATA_Ready_GPIO_Port, DATA_Ready_Pin)
{

}

void MainApp::run()
{
	txBuffer = xQueueCreate(128, sizeof(lsm303agrData_t));
	lsm303agr.initAcc();
	lsm303agr.initMag();
	getDataTaskHandle = osThreadNew(getAccDataTaskC, NULL, NULL);
	while(1) {
		char buffer[100];
		if (xQueueReceive(txBuffer, &lsm303agrData, portMAX_DELAY)) {
			uint8_t n = sprintf(buffer, "AccX=%i AccY=%i AccZ=%i MagX=%i MagY=%i MagZ=%i ",
					lsm303agrData.accX, lsm303agrData.accY, lsm303agrData.accZ,
					lsm303agrData.magX, lsm303agrData.magY, lsm303agrData.magZ);
			CDC_Transmit_FS((uint8_t*)buffer, n);
		}
		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	}
}

void MainApp::getAccMagDataTask()
{
	int16_t rawAcc[3] = {0, 0, 0};
	int16_t rawMag[3] = {0, 0, 0};
	static lsm303agrData_t accMagData;
	TickType_t tick;
	while(1) {
		tick = xTaskGetTickCount();
		if (lsm303agr.getAccData(rawAcc) == HAL_OK) {
			accMagData.accX = rawAcc[0];
			accMagData.accY = rawAcc[1];
			accMagData.accZ = rawAcc[2];
		}
		if (lsm303agr.getMagData(rawMag) == HAL_OK) {
			accMagData.magX = rawMag[0];
			accMagData.magY = rawMag[1];
			accMagData.magZ = rawMag[2];
		}
		xQueueSend(txBuffer, &accMagData, portMAX_DELAY);
		vTaskDelayUntil(&tick, 10);
	}
}
