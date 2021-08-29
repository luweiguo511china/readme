# efr32bg22 iadc enob #

## Description ##

This project uses bgm board to evaluate the EFR32BG22 16 bit ADC performance (14.3 ENOB with 32x OVS). 
a. EMU temperature
b. clock output
c. button
d. adc calibration
e. vcom

Peripherals used: IADC, GPIO, I2C, USART, EMU, CMU

## Gecko SDK Suite version ##

- Gecko SDK Suite v3.2.1
- MCU v6.1.1.0
- Bluetooth v3.2.1

## Hardware Required ##

- One WSTK [mainboard](https://www.silabs.com/development-tools/wireless/wireless-starter-kit-mainboard)
- One [bgm board](https://components101.com/motors/28byj-48-stepper-motor)

- Which include 
  - TI 14-bit Voltage-Output DAC [DAC70501](https://www.ti.com/lit/ds/symlink/dac70501.pdf)
  - TI 24-bit Low Power Delta-sigma ADC [ADC1220](https://www.ti.com/lit/ds/symlink/ads1220.pdf)
  - TI Low Drift Voltage Reference [REF3312](https://www.ti.com/product/REF3312)
- Schematic for the bgm board(https://components101.com/motors/28byj-48-stepper-motor)

## Setup ##

Connect bgm board with WSTK main board via Simplicity 10 pins adater, and connect WSTK main board to PC via mini USB.
a. Set the debug mode as 'OUT'.
b. set target in Simplicity launcher as EFR32BG22C112F352GM32.
c. read back the secure FW version.
d. flash the bootloader first via commander or flash programmer.
C:\SiliconLabs\SimplicityStudio\v5\developer\sdks\gecko_sdk_suite\v3.2\platform\bootloader\sample-apps\bootloader-storage-internal-single-512k\efr32mg22c224f512im40-brd4182a


The final connections should looks like so:

## Hardware ##
Schematic is [here](doc/bgm.pdf)
### Connection Required ###

### Pins Function Map ###

| EFR32BG22 | REF3312       | Direction (wrt BG22) | Comment         |
|-----------|---------------|----------------------|-----------------|
| PA0       | IN1           | OUT1                 | ADC reference   |
| GND       | GND           |                      |                 |


| EFR32BG22 | ADC Input     | Direction (wrt BG22) | Comment   |
|-----------|---------------|----------------------|-----------------|
| PD0       | IN1           | AIN                  | ADC INPUT       |
| PD1       | IN2           | AIN                  | ADC INPUT       |
|-----------|---------------|----------------------|-----------------|
| PC0       | IN1           | OUT1                 | ADC INPUT       |
| PC1       | IN2           | OUT2                 | ADC INPUT       |


| EFR32BG22 | ADC1220       | Direction (wrt BG22) | Comment        |
|-----------|---------------|----------------------|----------------|
| PA3       | IN1           | OUT1                 | SPI MISO       |
| PA4       | IN2           | OUT2                 | SPI MOSI       |
| PC4       | IN3           | OUT3                 | SPI CLK        |
| PC2       | IN4           | OUT4                 | SPI CS         |
| PB0       | IN4           | OUT4                 | SPI INT        |


VCOM:
| PA5       | IN1           | OUT1                 | USART0 TX |
| PA6       | IN1           | OUT1                 | USART0 RX |

LED:
| NO        | IN4           | OUT4                 | SPI INT  |
Button:
| PC5       | IN4           | OUT4                 | SPI INT  |
CLK OUT:
| PC3       | IN4           | OUT4                 | SPI INT  |
PTI:


Import the included .sls file to Simplicity Studio then build and flash the project to the bgm board.
In Simplicity Studio select "File->Import" and navigate to the directory with the .sls project file.
The project is built with relative paths to the STUDIO_SDK_LOC variable which was defined as

C:\SiliconLabs\SimplicityStudio\v4\developer\sdks\gecko_sdk_suite\v3.2

## How the Project Works ##

The application sits in EM1 until an interrupt occurs. The push buttons on the GG11 Starter Kit is used to start the TIMER and choose a rotation direction. TIMER1 is set to overflow at a frequency of 200 Hz and set to interrupt in an overflow event. In the TIMER1 interrupt handler, the software sets the coils to the next state in order to step the motor. In order to rotate the motor counter-clockwise, the motor coils need to be driven in the following order: Coil 1 -> Coil 3 -> Coil 2 -> Coil 4. In order to rotate the motor clockwise, the motor coils need to be driven in the following order: Coil 4 -> Coil 2 -> Coil 3 -> Coil 1. The calculateSteps() function determines the number of full steps required to rotate by a specified angle. The desired delta angle can be set using the ANGLE_PER_TRIGGER macro. TIMER1 will continue to interrupt until the motor shaft rotates by the desired angle. Once the desired angle is reached, TIMER1 stops and the application waits for the next pushbutton press.

### Memory Layout ###
### Software Workflow ###


## API Overview ##
- General
  - void initLetimer(void);
  - void letimerDelay(uint32_t msec);               /* simple delay */
  - void initButtonEM2(void);                       /* button in EM2 */
  - void initClockOut(void);                        /* route clock to pin */
  - void debounceButton(void);                      /* button denouncing */
  - float getDieTemperature(void);                  /* bg22 emu die temperature */

- dac70501 ###
uint16_t dac70501_init(void);                   /* dac70501 initialization */
float dac70501_readRef(void);                   /* dac70501 voltage read */
/* dac70501 output register set */
uint16_t dac70501_setRef(uint8_t dacValueHigh, uint8_t dacValueLow);
uint16_t dac70501_setVolt(float voltValue);     /* dac70501 voltage set */
/* dac70501 power down */
uint16_t dac070501_powerDown(uint8_t dac_pwdwn, uint8_t ref_pwdwn);
uint16_t dac70501_reStart(void);                /* dac70501 powerup(restart) */

### ads1220 ###
uint32_t ads1220_init(void);                    /* ads1220 initialization */
double ads1220_getAdcTemp(void);                /* ads1220 temperature */
double ads1220_getAdcDataVolt(void);            /* ads1220 get voltage */
void ads1220_Calibrate(void);                   /* ads1220 calibration */
void ads1220_powerDown(void);                   /* ads1220 power down */

### efr32bg22 adc ###
void resetIADC(void);                           /* bg22 iadc reset */
void rescaleIADC(uint32_t newScale);            /* bg22 iadc rescale */
void initIADC(void);                            /* bg22 iadc initialization */
void bg22SaveCalData(uint32_t scale);           /* bg22 iadc cal data save */
void bg22RestoreCalData(void);                  /* bg22 iadc cal data restore */
double iadcPollSingleResult(void);              /* bg22 iadc voltage polling */
uint32_t iadcDifferentialCalibrate();           /* bg22 iadc calibration */

### global buffer ####
double rmsCal(double buffer[], double adcAve);
#define ADC_BUFFER_SIZE 1024
extern double buffer[ADC_BUFFER_SIZE];                  /* buffer to save adc data */
extern double adcGainResult;                 /* adc gain cal result */
extern double adcOffsetresult;               /* adc offset cal result */


a. run the code
b. dump the adc data via vcom
c. import the data into excel
d. calcuate the ENOB
## Power Consumption ##
| Components(Peripheral) | Power Up       | Power Down       | Comment         |
|------------------------|----------------|------------------|-----------------|
| REF3312                | 4.9uA          | -                |                 |
| DAC70501               | 1.05mA         | 15uA             | ADC reference   |
| ADC1220                | 15uA           | -                | ADC reference   |
| REF3312 IADC           | 150uA          | 3uA              | ADC reference   |

## .sls Projects Used ##

gg11_stepper_motor.sls

## Steps to Create the Project ##
a. add EFR32BG22C224F512IM32 in my products and select it.
a. For EFR32BG22C224F512IM32 (EFR32BG22C112F352GM32), start with Bluetooth - SoC Empty project.
b. Add software component Services->IO Stream->IO Stream: USART. also configure it.
c. Add Add platform->peripheral->iadc
d. Add platform->peripheral->i2c
e. add folder inc and drv.
f. drag the files into the folder.
g. add the inc path.
h. replace the app.c
e. ignore PTI warning in pintool.

## How to Port to Another Part ##

Open the "Project Properties" and navigate to the "C/C++ Build -> Board/Part/SDK" item. Select the new board or part to target and "Apply" the changes.  Note: there may be dependencies that need to be resolved when changing the target architecture.
note: only EFR32/EFM32 S2 support this.




## Testing ##
## How to test ##

## Known Issues ##
## Reference ##
- DAC70501 [datasheet](https://www.ti.com/lit/ds/symlink/dac70501.pdf)
- ADC1220 [datasheet](https://www.ti.com/lit/ds/symlink/ads1220.pdf)
- REF3312 [datasheet](https://www.ti.com/product/REF3312)
- EFR32BG22 [reference manual](https://www.silabs.com/documents/public/reference-manuals/brd4185a-rm.pdf)
- EFR32BG22 [data sheet](https://www.silabs.com/documents/public/data-sheets/efr32bg22-datasheet.pdf)
- [AN1189](https://www.silabs.com/documents/public/application-notes/an1189-efr32-iadc.pdf): Incremental Analog to DigitalConverter (IADC)
- ENOB [calculation](https://www.tij.co.jp/lit/ug/tiduda7/tiduda7.pdf?ts=1630225963102&ref_url=https%253A%252F%252Fwww.google.com%252F)
