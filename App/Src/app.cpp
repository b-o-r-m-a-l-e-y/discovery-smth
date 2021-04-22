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
	MainApp::get_instance().getAccDataTask();
}

MainApp::MainApp() :
		lsm303agr(&hi2c1, DATA_Ready_GPIO_Port, DATA_Ready_Pin)
{

}

void MainApp::run()
{
	txBuffer = xQueueCreate(128, sizeof(accData_t));
	lsm303agr.initAcc();
	lsm303agr.initMag();
	getDataTaskHandle = osThreadNew(getAccDataTaskC, NULL, NULL);
	while(1) {
		static int16_t dataMag[3] = {0, 0, 0};
		static int16_t dataAcc[3] = {0x0000, 0x0000, 0x0000};
		accData_t accDataStruct;
		char buffer[100];
//		if (lsm303agr.getMagData(dataMag) == HAL_OK) {
//			uint8_t n = sprintf(buffer, "MagX=%i MagY=%i MagZ=%i ", dataMag[0], dataMag[1], dataMag[2]);
//			CDC_Transmit_FS((uint8_t*)buffer, n);
//		}
//		if (lsm303agr.getAccData(dataAcc) == HAL_OK) {
//			uint8_t n = sprintf(buffer, "AccX=%i AccY=%i AccZ=%i ", dataAcc[0], dataAcc[1], dataAcc[2]);
//			CDC_Transmit_FS((uint8_t*)buffer, n);
//		}
		while (xQueueReceive(txBuffer, &accDataStruct, portMAX_DELAY)) {
			uint8_t n = sprintf(buffer, "AccX=%i AccY=%i AccZ=%i ", accDataStruct.accX, accDataStruct.accY, accDataStruct.accZ);
			CDC_Transmit_FS((uint8_t*)buffer, n);
		}
		osDelay(100);
		HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	}
}

void MainApp::getAccDataTask()
{
	int16_t rawAcc[3] = {0, 0, 0};
	static accData_t accStruct;
	TickType_t tick;
	while(1) {
		tick = xTaskGetTickCount();
		if (lsm303agr.getAccData(rawAcc) == HAL_OK) {
			accStruct.accX = rawAcc[0];
			accStruct.accY = rawAcc[1];
			accStruct.accZ = rawAcc[2];
			xQueueSend(txBuffer, &accStruct, portMAX_DELAY);
		}
		vTaskDelayUntil(&tick, 10);
	}
}
