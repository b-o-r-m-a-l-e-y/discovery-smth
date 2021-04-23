/*
 * lsm303agr.cpp
 *
 *  Created on: Mar 30, 2021
 *      Author: bormaley
 */

#include <lsm303agr.h>


/******************************************************************************/
/*************************** START REGISTER MAPPING  **************************/
/******************************************************************************/
/* Exported constant IO ------------------------------------------------------*/
#define ACC_I2C_ADDRESS                      0x32
#define MAG_I2C_ADDRESS                      0x3C

/* Acceleration Registers */
#define LSM303AGR_STATUS_REG_AUX_A          0x07
#define LSM303AGR_OUT_TEMP_L_A              0x0C
#define LSM303AGR_OUT_TEMP_H_A              0x0D
#define LSM303AGR_INT_COUNTER_REG_A         0x0E
#define LSM303AGR_WHO_AM_I_A                0x0F  /* device identification register */
#define LSM303AGR_TEMP_CFG_REG_A            0x1F
#define LSM303AGR_CTRL_REG1_A               0x20  /* Control register 1 acceleration */
#define LSM303AGR_CTRL_REG2_A               0x21  /* Control register 2 acceleration */
#define LSM303AGR_CTRL_REG3_A               0x22  /* Control register 3 acceleration */
#define LSM303AGR_CTRL_REG4_A               0x23  /* Control register 4 acceleration */
#define LSM303AGR_CTRL_REG5_A               0x24  /* Control register 5 acceleration */
#define LSM303AGR_CTRL_REG6_A               0x25  /* Control register 6 acceleration */
#define LSM303AGR_REFERENCE_A               0x26  /* Reference register acceleration */
#define LSM303AGR_STATUS_REG_A              0x27  /* Status register acceleration */
#define LSM303AGR_OUT_X_L_A                 0x28  /* Output Register X acceleration */
#define LSM303AGR_OUT_X_H_A                 0x29  /* Output Register X acceleration */
#define LSM303AGR_OUT_Y_L_A                 0x2A  /* Output Register Y acceleration */
#define LSM303AGR_OUT_Y_H_A                 0x2B  /* Output Register Y acceleration */
#define LSM303AGR_OUT_Z_L_A                 0x2C  /* Output Register Z acceleration */
#define LSM303AGR_OUT_Z_H_A                 0x2D  /* Output Register Z acceleration */
#define LSM303AGR_FIFO_CTRL_REG_A           0x2E  /* Fifo control Register acceleration */
#define LSM303AGR_FIFO_SRC_REG_A            0x2F  /* Fifo src Register acceleration */

#define LSM303AGR_INT1_CFG_A                0x30  /* Interrupt 1 configuration Register acceleration */
#define LSM303AGR_INT1_SRC_A                0x31  /* Interrupt 1 source Register acceleration */
#define LSM303AGR_INT1_THS_A                0x32  /* Interrupt 1 Threshold register acceleration */
#define LSM303AGR_INT1_DURATION_A           0x33  /* Interrupt 1 DURATION register acceleration */

#define LSM303AGR_INT2_CFG_A                0x34  /* Interrupt 2 configuration Register acceleration */
#define LSM303AGR_INT2_SRC_A                0x35  /* Interrupt 2 source Register acceleration */
#define LSM303AGR_INT2_THS_A                0x36  /* Interrupt 2 Threshold register acceleration */
#define LSM303AGR_INT2_DURATION_A           0x37  /* Interrupt 2 DURATION register acceleration */

#define LSM303AGR_CLICK_CFG_A               0x38  /* Click configuration Register acceleration */
#define LSM303AGR_CLICK_SRC_A               0x39  /* Click 2 source Register acceleration */
#define LSM303AGR_CLICK_THS_A               0x3A  /* Click 2 Threshold register acceleration */

#define LSM303AGR_TIME_LIMIT_A              0x3B  /* Time Limit Register acceleration */
#define LSM303AGR_TIME_LATENCY_A            0x3C  /* Time Latency Register acceleration */
#define LSM303AGR_TIME_WINDOW_A             0x3D  /* Time window register acceleration */
#define LSM303AGR_Act_THS_A                 0x3E
#define LSM303AGR_Act_DUR_A                 0x3F

/* Magnetic field Registers */
#define LSM303AGR_OFFSET_X_REG_L_M          0x45
#define LSM303AGR_OFFSET_X_REG_H_M          0x46
#define LSM303AGR_OFFSET_Y_REG_L_M          0x47
#define LSM303AGR_OFFSET_Y_REG_H_M          0x48
#define LSM303AGR_OFFSET_Z_REG_L_M          0x49
#define LSM303AGR_OFFSET_Z_REG_H_M          0x4A

#define LSM303AGR_WHO_AM_I_M                0x4F

#define LSM303AGR_CFG_REG_A_M               0x60
#define LSM303AGR_CFG_REG_B_M               0x61
#define LSM303AGR_CFG_REG_C_M               0x62

#define LSM303AGR_INT_CTRL_REG_M            0x63
#define LSM303AGR_INT_SOURCE_REG_M          0x64
#define LSM303AGR_INT_THS_L_REG_M           0x65
#define LSM303AGR_INT_THS_H_REG_M           0x66

#define LSM303AGR_STATUS_REG_M              0x67
#define LSM303AGR_OUTX_L_REG_M              0x68
#define LSM303AGR_OUTX_H_REG_M              0x69
#define LSM303AGR_OUTY_L_REG_M              0x6A
#define LSM303AGR_OUTY_H_REG_M              0x6B
#define LSM303AGR_OUTZ_L_REG_M              0x6C
#define LSM303AGR_OUTZ_H_REG_M              0x6D

/******************************************************************************/
/**************************** END REGISTER MAPPING  ***************************/
/******************************************************************************/

/*
 * ---------------------------------------------------------
 * CTRL_REG1_A bits
 */
#define LSM303AGR_ODR_1_HZ                ((uint8_t)0x10)  /*!< Output Data Rate = 1 Hz */
#define LSM303AGR_ODR_10_HZ               ((uint8_t)0x20)  /*!< Output Data Rate = 10 Hz */
#define LSM303AGR_ODR_25_HZ               ((uint8_t)0x30)  /*!< Output Data Rate = 25 Hz */
#define LSM303AGR_ODR_50_HZ               ((uint8_t)0x40)  /*!< Output Data Rate = 50 Hz */
#define LSM303AGR_ODR_100_HZ              ((uint8_t)0x50)  /*!< Output Data Rate = 100 Hz */
#define LSM303AGR_ODR_200_HZ              ((uint8_t)0x60)  /*!< Output Data Rate = 200 Hz */
#define LSM303AGR_ODR_400_HZ              ((uint8_t)0x70)  /*!< Output Data Rate = 400 Hz */
#define LSM303AGR_ODR_1620_HZ_LP          ((uint8_t)0x80)  /*!< Output Data Rate = 1620 Hz only in Low Power Mode */
#define LSM303AGR_ODR_1344_HZ             ((uint8_t)0x90)  /*!< Output Data Rate = 1344 Hz in Normal mode and 5376 Hz in Low Power Mode */

#define LSM303AGR_NORMAL_MODE             ((uint8_t)0x00)
#define LSM303AGR_LOWPOWER_MODE           ((uint8_t)0x08)

#define LSM303AGR_X_ENABLE                ((uint8_t)0x01)
#define LSM303AGR_Y_ENABLE                ((uint8_t)0x02)
#define LSM303AGR_Z_ENABLE                ((uint8_t)0x04)
#define LSM303AGR_AXES_ENABLE             ((uint8_t)0x07)
#define LSM303AGR_AXES_DISABLE            ((uint8_t)0x00)
/*
 * ---------------------------------------------------------
 */

/*
 * ---------------------------------------------------------
 * CTRL_REG4_A bits
 */
#define LSM303AGR_BlockUpdate_Continous   ((uint8_t)0x00) /*!< Continuos Update */
#define LSM303AGR_BlockUpdate_Single      ((uint8_t)0x80) /*!< Single Update: output registers not updated until MSB and LSB reading */

#define LSM303AGR_BLE_LSB                 ((uint8_t)0x00) /*!< Little Endian: data LSB @ lower address */
#define LSM303AGR_BLE_MSB                 ((uint8_t)0x40) /*!< Big Endian: data MSB @ lower address */

#define LSM303AGR_FULLSCALE_2G            ((uint8_t)0x00)  /*!< 2 g */
#define LSM303AGR_FULLSCALE_4G            ((uint8_t)0x10)  /*!< 4 g */
#define LSM303AGR_FULLSCALE_8G            ((uint8_t)0x20)  /*!< 8 g */
#define LSM303AGR_FULLSCALE_16G           ((uint8_t)0x30)  /*!< 16 g */

#define LSM303AGR_HR_ENABLE              ((uint8_t)0x08)
#define LSM303AGR_HR_DISABLE             ((uint8_t)0x00)

#define LSM303AGR_NORMAL_MODE             ((uint8_t)0x00)
#define LSM303AGR_SELF_TEST_0             ((uint8_t)0x02)
#define LSM303AGR_SELF_TEST_1             ((uint8_t)0x04)
/*
 * ---------------------------------------------------------
 */

/*
 * ---------------------------------------------------------
 * CTRL_REG5_A bits
 */
#define LSM303AGR_BOOT_NORMAL             ((uint8_t)0x00)
#define LSM303AGR_REBOOT_MEMORY           ((uint8_t)0x80)

#define LSM303AGR_FIFO_DISABLE            ((uint8_t)0x00)
#define LSM303AGR_FIFO_ENABLE             ((uint8_t)0x40)

#define LSM303AGR_LIR_INT1_NOT_LATCHED    ((uint8_t)0x00)
#define LSM303AGR_LIR_INT1_LATCHED        ((uint8_t)0x08)

#define LSM303AGR_D4D_INT1_DISABLE        ((uint8_t)0x00)
#define LSM303AGR_D4D_INT1_ENABLE         ((uint8_t)0x04)

#define LSM303AGR_LIR_INT2_NOT_LATCHED    ((uint8_t)0x00)
#define LSM303AGR_LIR_INT2_LATCHED        ((uint8_t)0x02)

#define LSM303AGR_D4D_INT2_DISABLE        ((uint8_t)0x00)
#define LSM303AGR_D4D_INT2_ENABLE         ((uint8_t)0x01)
/** @defgroup Acc_Full_Scale_Selection
  * @{
  */
#define LSM303DLHC_ACC_SENSITIVITY_2G     ((uint8_t)1)  /*!< accelerometer sensitivity with 2 g full scale [mg/LSB] */
#define LSM303DLHC_ACC_SENSITIVITY_4G     ((uint8_t)2)  /*!< accelerometer sensitivity with 4 g full scale [mg/LSB] */
#define LSM303DLHC_ACC_SENSITIVITY_8G     ((uint8_t)4)  /*!< accelerometer sensitivity with 8 g full scale [mg/LSB] */
#define LSM303DLHC_ACC_SENSITIVITY_16G    ((uint8_t)12) /*!< accelerometer sensitivity with 12 g full scale [mg/LSB] */
/**
  * @}
  */

/** @defgroup Acc_Block_Data_Update
  * @{
  */

/** @defgroup Acc_Endian_Data_selection
  * @{
  */

/**
  * @}
  */

/** @defgroup Acc_Boot_Mode_selection
  * @{
  */
#define LSM303DLHC_BOOT_NORMALMODE         ((uint8_t)0x00)
#define LSM303DLHC_BOOT_REBOOTMEMORY       ((uint8_t)0x80)
/**
  * @}
  */

/** @defgroup Acc_High_Pass_Filter_Mode
  * @{
  */
#define LSM303DLHC_HPM_NORMAL_MODE_RES     ((uint8_t)0x00)
#define LSM303DLHC_HPM_REF_SIGNAL          ((uint8_t)0x40)
#define LSM303DLHC_HPM_NORMAL_MODE         ((uint8_t)0x80)
#define LSM303DLHC_HPM_AUTORESET_INT       ((uint8_t)0xC0)
/**
  * @}
  */

/** @defgroup Acc_High_Pass_CUT OFF_Frequency
  * @{
  */
#define LSM303DLHC_HPFCF_8                 ((uint8_t)0x00)
#define LSM303DLHC_HPFCF_16                ((uint8_t)0x10)
#define LSM303DLHC_HPFCF_32                ((uint8_t)0x20)
#define LSM303DLHC_HPFCF_64                ((uint8_t)0x30)
/**
  * @}
  */

/** @defgroup Acc_High_Pass_Filter_status
  * @{
  */
#define LSM303DLHC_HIGHPASSFILTER_DISABLE  ((uint8_t)0x00)
#define LSM303DLHC_HIGHPASSFILTER_ENABLE   ((uint8_t)0x08)
/**
  * @}
  */

/** @defgroup Acc_High_Pass_Filter_Click_status
  * @{
  */
#define LSM303DLHC_HPF_CLICK_DISABLE       ((uint8_t)0x00)
#define LSM303DLHC_HPF_CLICK_ENABLE        ((uint8_t)0x04)
/**
  * @}
  */

/** @defgroup Acc_High_Pass_Filter_AOI1_status
  * @{
  */
#define LSM303DLHC_HPF_AOI1_DISABLE        ((uint8_t)0x00)
#define LSM303DLHC_HPF_AOI1_ENABLE	       ((uint8_t)0x01)
/**
  * @}
  */

/** @defgroup Acc_High_Pass_Filter_AOI2_status
  * @{
  */
#define LSM303DLHC_HPF_AOI2_DISABLE        ((uint8_t)0x00)
#define LSM303DLHC_HPF_AOI2_ENABLE         ((uint8_t)0x02)
/**
  * @}
  */

/** @defgroup Acc_Interrupt1_Configuration_definition
  * @{
  */
#define LSM303DLHC_IT1_CLICK               ((uint8_t)0x80)
#define LSM303DLHC_IT1_AOI1                ((uint8_t)0x40)
#define LSM303DLHC_IT1_AOI2                ((uint8_t)0x20)
#define LSM303DLHC_IT1_DRY1                ((uint8_t)0x10)
#define LSM303DLHC_IT1_DRY2                ((uint8_t)0x08)
#define LSM303DLHC_IT1_WTM                 ((uint8_t)0x04)
#define LSM303DLHC_IT1_OVERRUN             ((uint8_t)0x02)
/**
  * @}
  */

/** @defgroup Acc_Interrupt2_Configuration_definition
  * @{
  */
#define LSM303DLHC_IT2_CLICK               ((uint8_t)0x80)
#define LSM303DLHC_IT2_INT1                ((uint8_t)0x40)
#define LSM303DLHC_IT2_INT2                ((uint8_t)0x20)
#define LSM303DLHC_IT2_BOOT                ((uint8_t)0x10)
#define LSM303DLHC_IT2_ACT                 ((uint8_t)0x08)
#define LSM303DLHC_IT2_HLACTIVE            ((uint8_t)0x02)
/**
  * @}
  */

/** @defgroup Acc_INT_Combination_Status
  * @{
  */
#define LSM303DLHC_OR_COMBINATION          ((uint8_t)0x00)  /*!< OR combination of enabled IRQs */
#define LSM303DLHC_AND_COMBINATION         ((uint8_t)0x80)  /*!< AND combination of enabled IRQs */
#define LSM303DLHC_MOV_RECOGNITION         ((uint8_t)0x40)  /*!< 6D movement recognition */
#define LSM303DLHC_POS_RECOGNITION         ((uint8_t)0xC0)  /*!< 6D position recognition */
/**
  * @}
  */

/** @defgroup Acc_INT_Axes
  * @{
  */
#define LSM303DLHC_Z_HIGH                  ((uint8_t)0x20)  /*!< Z High enabled IRQs */
#define LSM303DLHC_Z_LOW                   ((uint8_t)0x10)  /*!< Z low enabled IRQs */
#define LSM303DLHC_Y_HIGH                  ((uint8_t)0x08)  /*!< Y High enabled IRQs */
#define LSM303DLHC_Y_LOW                   ((uint8_t)0x04)  /*!< Y low enabled IRQs */
#define LSM303DLHC_X_HIGH                  ((uint8_t)0x02)  /*!< X High enabled IRQs */
#define LSM303DLHC_X_LOW                   ((uint8_t)0x01)  /*!< X low enabled IRQs */
/**
  * @}
  */

/** @defgroup Acc_INT_Click
* @{
*/
#define LSM303DLHC_Z_DOUBLE_CLICK          ((uint8_t)0x20)  /*!< Z double click IRQs */
#define LSM303DLHC_Z_SINGLE_CLICK          ((uint8_t)0x10)  /*!< Z single click IRQs */
#define LSM303DLHC_Y_DOUBLE_CLICK          ((uint8_t)0x08)  /*!< Y double click IRQs */
#define LSM303DLHC_Y_SINGLE_CLICK          ((uint8_t)0x04)  /*!< Y single click IRQs */
#define LSM303DLHC_X_DOUBLE_CLICK          ((uint8_t)0x02)  /*!< X double click IRQs */
#define LSM303DLHC_X_SINGLE_CLICK          ((uint8_t)0x01)  /*!< X single click IRQs */
/**
* @}
*/

/** @defgroup Acc_INT1_Interrupt_status
  * @{
  */
#define LSM303DLHC_INT1INTERRUPT_DISABLE   ((uint8_t)0x00)
#define LSM303DLHC_INT1INTERRUPT_ENABLE    ((uint8_t)0x80)
/**
  * @}
  */

/** @defgroup Acc_INT1_Interrupt_ActiveEdge
  * @{
  */
#define LSM303DLHC_INT1INTERRUPT_LOW_EDGE  ((uint8_t)0x20)
#define LSM303DLHC_INT1INTERRUPT_HIGH_EDGE ((uint8_t)0x00)
/**
  * @}
  */

#define LSM303AGR_COMP_TEMP_EN              ((uint8_t)0x80)
#define LSM303AGR_REBOOT_MEM                ((uint8_t)0x40)

#define LSM303AGR_ODR_10_HZ_M                 ((uint8_t)0x00)
#define LSM303AGR_ODR_20_HZ_M                 ((uint8_t)0x04)
#define LSM303AGR_ODR_50_HZ_M                 ((uint8_t)0x08)
#define LSM303AGR_ODR_100_HZ_M                ((uint8_t)0x0C)

#define LSM303AGR_MODE_CONTINUOUS           (uint8_t)0x00)

#define LSM303AGR_BDU_ENABLE_M              ((uint8_t)0x10)
#define LSM303AGR_BDU_DISABLE_M             ((uint8_t)0x00)
/** @defgroup Mag_Full_Scale
  * @{
  */
#define  LSM303DLHC_FS_1_3_GA               ((uint8_t) 0x20)  /*!< Full scale = �1.3 Gauss */
#define  LSM303DLHC_FS_1_9_GA               ((uint8_t) 0x40)  /*!< Full scale = �1.9 Gauss */
#define  LSM303DLHC_FS_2_5_GA               ((uint8_t) 0x60)  /*!< Full scale = �2.5 Gauss */
#define  LSM303DLHC_FS_4_0_GA               ((uint8_t) 0x80)  /*!< Full scale = �4.0 Gauss */
#define  LSM303DLHC_FS_4_7_GA               ((uint8_t) 0xA0)  /*!< Full scale = �4.7 Gauss */
#define  LSM303DLHC_FS_5_6_GA               ((uint8_t) 0xC0)  /*!< Full scale = �5.6 Gauss */
#define  LSM303DLHC_FS_8_1_GA               ((uint8_t) 0xE0)  /*!< Full scale = �8.1 Gauss */
/**
  * @}
  */

/**
 * @defgroup Magnetometer_Sensitivity
 * @{
 */
#define LSM303DLHC_M_SENSITIVITY_XY_1_3Ga     1100  /*!< magnetometer X Y axes sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_1_9Ga     855   /*!< magnetometer X Y axes sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_2_5Ga     670   /*!< magnetometer X Y axes sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4Ga       450   /*!< magnetometer X Y axes sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_4_7Ga     400   /*!< magnetometer X Y axes sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_5_6Ga     330   /*!< magnetometer X Y axes sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_XY_8_1Ga     230   /*!< magnetometer X Y axes sensitivity for 8.1 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_3Ga      980   /*!< magnetometer Z axis sensitivity for 1.3 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_1_9Ga      760   /*!< magnetometer Z axis sensitivity for 1.9 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_2_5Ga      600   /*!< magnetometer Z axis sensitivity for 2.5 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4Ga        400   /*!< magnetometer Z axis sensitivity for 4 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_4_7Ga      355   /*!< magnetometer Z axis sensitivity for 4.7 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_5_6Ga      295   /*!< magnetometer Z axis sensitivity for 5.6 Ga full scale [LSB/Ga] */
#define LSM303DLHC_M_SENSITIVITY_Z_8_1Ga      205   /*!< magnetometer Z axis sensitivity for 8.1 Ga full scale [LSB/Ga] */
/**
 * @}
 */

/** @defgroup Mag_Working_Mode
  * @{
  */
#define LSM303DLHC_CONTINUOS_CONVERSION      ((uint8_t) 0x00)   /*!< Continuous-Conversion Mode */
#define LSM303DLHC_SINGLE_CONVERSION         ((uint8_t) 0x01)   /*!< Single-Conversion Mode */
#define LSM303DLHC_SLEEP                     ((uint8_t) 0x02)   /*!< Sleep Mode */
/**
  * @}
  */

/** @defgroup Mag_Temperature_Sensor
  * @{
  */
#define LSM303DLHC_TEMPSENSOR_ENABLE         ((uint8_t) 0x80)   /*!< Temp sensor Enable */
#define LSM303DLHC_TEMPSENSOR_DISABLE        ((uint8_t) 0x00)   /*!< Temp sensor Disable */

#define I2C_TIMEOUT   100

LSM303AGR::~LSM303AGR()
{

}

LSM303AGR::LSM303AGR(I2C_HandleTypeDef* hi2c, GPIO_TypeDef* DRDY_GPIO, uint32_t DRDY_pin)
{
	this->hi2c = hi2c;
	this->DRDY_pin = DRDY_pin;
	this->DRDY_GPIO = DRDY_GPIO;
}

HAL_StatusTypeDef LSM303AGR::initAcc()
{
	if (readAccID() & ACC_I2C_ADDRESS) accStatus = 1;
	else accStatus = 0;

	uint8_t ctrl_reg_1 = LSM303AGR_ODR_100_HZ | LSM303AGR_AXES_ENABLE;
	error_status = writeRegisterAcc(LSM303AGR_CTRL_REG1_A, ctrl_reg_1);

	error_status = writeRegisterAcc(LSM303AGR_CTRL_REG2_A, 0x00);
	error_status = writeRegisterAcc(LSM303AGR_CTRL_REG3_A, 0x00);

	uint8_t ctrl_reg_4 = LSM303AGR_FULLSCALE_2G | LSM303AGR_HR_ENABLE;
	error_status = writeRegisterAcc(LSM303AGR_CTRL_REG4_A, ctrl_reg_4);

	return error_status;
}

HAL_StatusTypeDef LSM303AGR::initMag()
{
	if (readMagID() & MAG_I2C_ADDRESS) magStatus = 1;
	else magStatus = 0;

	error_status = writeRegisterMag(LSM303AGR_CFG_REG_A_M, LSM303AGR_ODR_100_HZ_M);
	error_status = writeRegisterMag(LSM303AGR_CFG_REG_C_M, LSM303AGR_BDU_ENABLE_M);
	return error_status;
}

uint8_t LSM303AGR::readAccID()
{
	return readRegisterAcc(LSM303AGR_WHO_AM_I_A);
}

uint8_t LSM303AGR::readMagID()
{
	return readRegisterMag(LSM303AGR_WHO_AM_I_M);
}

HAL_StatusTypeDef LSM303AGR::writeRegisterAcc(uint8_t regAddr, uint8_t reg)
{
	return HAL_I2C_Mem_Write(this->hi2c, ACC_I2C_ADDRESS, regAddr, 1, &reg, 1, I2C_TIMEOUT);
}

uint8_t LSM303AGR::readRegisterAcc(uint8_t regAddr)
{
	uint8_t retVal = 0;
	HAL_I2C_Mem_Read(this->hi2c, ACC_I2C_ADDRESS, regAddr, 1, &retVal, 1, I2C_TIMEOUT);
	return retVal;
}

HAL_StatusTypeDef LSM303AGR::writeRegisterMag(uint8_t regAddr, uint8_t reg)
{
	return HAL_I2C_Mem_Write(this->hi2c, MAG_I2C_ADDRESS, regAddr, 1, &reg, 1, I2C_TIMEOUT);
}

uint8_t LSM303AGR::readRegisterMag(uint8_t regAddr)
{
	uint8_t retVal = 0;
	HAL_I2C_Mem_Read(this->hi2c, MAG_I2C_ADDRESS, regAddr, 1, &retVal, 1, I2C_TIMEOUT);
	return retVal;
}

HAL_StatusTypeDef LSM303AGR::getAccData(int16_t* pData)
{
	HAL_StatusTypeDef retVal = HAL_OK;
	uint8_t buffer[6] = {0, 0, 0, 0, 0, 0};
	uint8_t reg = LSM303AGR_OUT_X_L_A;
	for (uint8_t i = 0; i < 6; i ++) {
		retVal = HAL_I2C_Mem_Read(this->hi2c, ACC_I2C_ADDRESS, reg++, 1, (uint8_t*)&buffer[i], 1, I2C_TIMEOUT);
	}
	for (uint8_t i = 0; i < 3; i++) {
		pData[i] = (static_cast<int16_t>(buffer[2*i+1] << 8 | buffer[2*i]) >> 4);
	}
	return retVal;
}

HAL_StatusTypeDef LSM303AGR::getMagData(int16_t* pData)
{
	HAL_StatusTypeDef retVal = HAL_OK;
	retVal = HAL_I2C_Mem_Read(this->hi2c, MAG_I2C_ADDRESS, LSM303AGR_OUTX_L_REG_M, 1, (uint8_t*)pData, 6, I2C_TIMEOUT);
	return retVal;
}

int16_t LSM303AGR::getTemperature()
{
	int16_t pnRawData;
	uint8_t ctrlx[2] = {0,0};
	int8_t buffer[2];

	/* Read the acceleration control register content */
	ctrlx[0] = readRegisterAcc(LSM303AGR_CTRL_REG4_A);

	buffer[0] = readRegisterAcc(LSM303AGR_OUT_TEMP_H_A);
	buffer[1] = readRegisterAcc(LSM303AGR_OUT_TEMP_L_A);

	return pnRawData;
}
