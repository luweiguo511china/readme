/***************************************************************************//**
 * @file mis.h
 * @brief header file for driver.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 *
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/
#ifndef MIS_H
#define MIS_H

// letimerDelay, button, clock out, bg22 temperature
void initLetimer(void);
void letimerDelay(uint32_t msec);               /* simple delay */
void initButtonEM2(void);                       /* button in EM2 */
void initClockOut(void);                        /* route clock to pin */
void debounceButton(void);                      /* button denouncing */
float getDieTemperature(void);                  /* bg22 emu die temperature */

// dac70501
uint16_t dac70501_init(void);                   /* dac70501 initialization */
float dac70501_readRef(void);                   /* dac70501 voltage read */
/* dac70501 output register set */
uint16_t dac70501_setRef(uint8_t dacValueHigh, uint8_t dacValueLow);
uint16_t dac70501_setVolt(float voltValue);     /* dac70501 voltage set */
/* dac70501 power down */
uint16_t dac070501_powerDown(uint8_t dac_pwdwn, uint8_t ref_pwdwn);
uint16_t dac70501_reStart(void);                /* dac70501 powerup(restart) */

// ads1220
uint32_t ads1220_init(void);                    /* ads1220 initialization */
double ads1220_getAdcTemp(void);                /* ads1220 temperature */
double ads1220_getAdcDataVolt(void);            /* ads1220 get voltage */
void ads1220_Calibrate(void);                   /* ads1220 calibration */
void ads1220_powerDown(void);                   /* ads1220 power down */

// efr32bg22 adc
void resetIADC(void);                           /* bg22 iadc reset */
void rescaleIADC(uint32_t newScale);            /* bg22 iadc rescale */
void initIADC(void);                            /* bg22 iadc initialization */
void bg22SaveCalData(uint32_t scale);           /* bg22 iadc cal data save */
void bg22RestoreCalData(void);                  /* bg22 iadc cal data restore */
double iadcPollSingleResult(void);              /* bg22 iadc voltage polling */
uint32_t iadcDifferentialCalibrate();           /* bg22 iadc calibration */

// global buffer
double rmsCal(double buffer[], double adcAve);
#define ADC_BUFFER_SIZE 1024
extern double buffer[ADC_BUFFER_SIZE];          /* buffer to save adc data */
extern double adcGainResult;                    /* adc gain cal result */
extern double adcOffsetResult;                  /* adc offset cal result */
extern double adcEnobResult;                          /* for enob result */

#endif /* MIS_H */
