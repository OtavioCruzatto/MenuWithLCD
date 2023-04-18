/*
 * lcdController.h
 *
 *  Created on: Apr 5, 2023
 *      Author: Otavio
 */

#ifndef INC_LCDCONTROLLER_H_
#define INC_LCDCONTROLLER_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define DELAY_10_MICRO_SECONDS		1
#define DELAY_40_MICRO_SECONDS		4
#define DELAY_50_MICRO_SECONDS		5
#define DELAY_60_MICRO_SECONDS		6
#define DELAY_100_MICRO_SECONDS		10
#define DELAY_1000_MICRO_SECONDS	100
#define DELAY_1500_MICRO_SECONDS	150
#define DELAY_2500_MICRO_SECONDS	250
#define DELAY_2000000_MICRO_SECONDS	200000

#define QTY_OF_ROWS					4
#define QTY_OF_CHARS_PER_ROW		16

typedef enum LCD_COMMAND
{
	CLEAR_DISPLAY 										= 0x01,
	RETURN_HOME											= 0x02,
	ENTRY_MODE_SET_DECREMENT_NOTSHIFT 					= 0x04,
	ENTRY_MODE_SET_DECREMENT_SHIFT 						= 0x05,
	ENTRY_MODE_SET_INCREMENT_NOTSHIFT 					= 0x06,
	ENTRY_MODE_SET_INCREMENT_SHIFT 						= 0x07,
	DISPLAY_ON_OFF_CONTROL_DISPLAYOFF 					= 0x08,
	DISPLAY_ON_OFF_CONTROL_DISPLAYON_CURSOROFF 			= 0x0C,
	DISPLAY_ON_OFF_CONTROL_DISPLAYON_CURSORON_BLINKSOFF	= 0x0E,
	DISPLAY_ON_OFF_CONTROL_DISPLAYON_CURSORON_BLINKSON 	= 0x0F,
	CURSOR_OR_DISPLAY_SHIFT_SHIFTCURSORTOTHELEFT 		= 0x10,
	CURSOR_OR_DISPLAY_SHIFT_SHIFTCURSORTOTHERIGHT 		= 0x14,
	CURSOR_OR_DISPLAY_SHIFT_SHIFTDISPLAYTOTHELEFT 		= 0x18,
	CURSOR_OR_DISPLAY_SHIFT_SHIFTDISPLAYTOTHERIGHT 		= 0x1C,
	FUNCTION_SET_4BIT_SINGLELINE_5X8DOTS 				= 0x20,
	FUNCTION_SET_4BIT_MULTILINE_5X8DOTS 				= 0x28,
	FUNCTION_SET_8BIT_SINGLELINE_5X8DOTS 				= 0x30,
	FUNCTION_SET_8BIT_MULTILINE_5X8DOTS 				= 0x38
} LcdCommand;

typedef enum LCD_ROW
{
	ROW_0 = 0x80,
	ROW_1 = 0xC0,
	ROW_2 = 0x90,
	ROW_3 = 0xD0
} LcdRow;

typedef enum LCD_REGISTER_SELECTION
{
	COMMAND = 0x00,
	DATA	= 0x01
} LcdRegisterSelection;

typedef enum LCD_READ_WRITE_SELECTION
{
	WRITE = 0x00,
	READ  = 0x01
} LcdReadWriteSelection;

typedef enum NIBBLE
{
	DIVISION,
	HIGH_NIBBLE,
	LOW_NIBBLE
} Nibble;

typedef enum BOOL
{
	FALSE,
	TRUE
} Bool;

typedef enum STATUS
{
	NOK,
	OK
} Status;

typedef enum COMMAND
{
	COMMAND_0,
	COMMAND_1,
	COMMAND_2,
	COMMAND_3,
	COMMAND_4,
	COMMAND_5,
	COMMAND_6,
	COMMAND_7,
	COMMAND_8,
	COMMAND_9,
	COMMAND_10,
	COMMAND_11,
	COMMAND_12,
	COMMAND_13
} Command;

typedef enum BACKLIGHT_PERCENT
{
	BL_00_PERCENT = 0,
	BL_10_PERCENT = 5,
	BL_20_PERCENT = 10,
	BL_30_PERCENT = 15,
	BL_40_PERCENT = 20,
	BL_50_PERCENT = 25,
	BL_60_PERCENT = 30,
	BL_70_PERCENT = 35,
	BL_80_PERCENT = 40,
	BL_90_PERCENT = 45,
	BL_100_PERCENT = 50
} BacklightPercent;

typedef enum LCD_ON_OFF
{
	LCD_ON = 0x00,
	LCD_OFF = 0x01
} LcdOnOff;

typedef struct
{
	uint8_t stringMatrixBuffer[QTY_OF_ROWS][QTY_OF_CHARS_PER_ROW+1];
	uint8_t stringMatrixShow[QTY_OF_ROWS][QTY_OF_CHARS_PER_ROW+1];
	TIM_HandleTypeDef *backlightPwmTimer;
	uint8_t backlightPwmChannel;
	BacklightPercent backlightPercent;
	Command command;
	Bool initialized;
	uint8_t counterAux;
	uint8_t counterUnlock;
	uint8_t counterInitCycles1;
	uint8_t counterInitCycles2;
	LcdRegisterSelection byteToSend;
	Bool updatePosition;
	uint8_t currentRow;
	uint8_t currentColumm;
	uint32_t delayCounter;
	uint32_t delayCounterEnable;
	uint8_t entry;
	uint8_t highNibbleOfByteToSend;
	uint8_t lowNibbleOfByteToSend;
	Bool byteSended;
	Bool countDelay;
	Bool countEnableDelay;
	Nibble nibble;
	GPIO_TypeDef *rsPort;
	uint16_t rsPin;
	GPIO_TypeDef *rwPort;
	uint16_t rwPin;
	GPIO_TypeDef *enPort;
	uint16_t enPin;
	GPIO_TypeDef *db4Port;
	uint16_t db4Pin;
	GPIO_TypeDef *db5Port;
	uint16_t db5Pin;
	GPIO_TypeDef *db6Port;
	uint16_t db6Pin;
	GPIO_TypeDef *db7Port;
	uint16_t db7Pin;
	GPIO_TypeDef *onOffPort;
	uint16_t onOffPin;
} LcdController;

void lcdControllerInitStruct(LcdController *lcdController,
		GPIO_TypeDef *rsPort, uint16_t rsPin,
		GPIO_TypeDef *rwPort, uint16_t rwPin,
		GPIO_TypeDef *enPort, uint16_t enPin,
		GPIO_TypeDef *db4Port, uint16_t db4Pin,
		GPIO_TypeDef *db5Port, uint16_t db5Pin,
		GPIO_TypeDef *db6Port, uint16_t db6Pin,
		GPIO_TypeDef *db7Port, uint16_t db7Pin,
		GPIO_TypeDef *onOffPort, uint16_t onOffPin,
		TIM_HandleTypeDef *backlightPwmTimer, uint8_t backlightPwmChannel);
void lcdControllerInit(LcdController *lcdController);
void lcdControllerClearRow(LcdController *lcdController, LcdRow lcdRow);
void lcdControllerClearDisplay(LcdController *lcdController);
Bool lcdControllerGetDelayStatus(LcdController *lcdController);
void lcdControllerIncrementDelayCounter(LcdController *lcdController);
Bool lcdControllerGetEnableDelayStatus(LcdController *lcdController);
void lcdControllerIncrementEnableDelayCounter(LcdController *lcdController);
Bool lcdControllerIsInitialized(LcdController *lcdController);
void lcdControllerUpdate(LcdController *lcdController);
void lcdControllerSetString(LcdController *lcdController, LcdRow lcdRow, uint8_t *string);
void lcdControllerSetBacklight(LcdController *lcdController, BacklightPercent backlightPercent);
void lcdControllerOnOff(LcdController *lcdController, LcdOnOff lcdOnOff);

#endif /* INC_LCDCONTROLLER_H_ */
