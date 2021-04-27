/*
 * app.h
 *
 *  Created on: Mar 29, 2021
 *      Author: bormaley
 */

#ifndef APP_H_
#define APP_H_

#include "lsm303agr.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"


extern "C" void main_app_wrp();
extern "C" void getAccDataTaskC(void *argument);

struct lsm303agrData_t {
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t magX;
	int16_t magY;
	int16_t magZ;
};

class MainApp {
public:
	static MainApp& get_instance()
	{
		static MainApp mainApp;
		return mainApp;
	}

	MainApp(MainApp const&)           = delete;
	void operator=(MainApp const&)    = delete;
	MainApp(MainApp&&)                = delete;
	MainApp& operator=(MainApp&&)     = delete;

	void run();
	void getAccMagDataTask();
private:
	MainApp();

	osThreadId_t getDataTaskHandle;
	xQueueHandle txBuffer;
	lsm303agrData_t lsm303agrData;

	LSM303AGR lsm303agr;
};

#endif /* APP_H_ */
