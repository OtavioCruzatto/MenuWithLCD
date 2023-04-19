/*
 * lcdController.c
 *
 *  Created on: Apr 5, 2023
 *      Author: Otavio
 */

#include "lcdController.h"

static void lcdSendByte(LcdController *lcdController, LcdRegisterSelection lcdRs, uint8_t byteToSend)
{
	switch(lcdController->nibble)
	{
		case DIVISION:
			lcdController->highNibbleOfByteToSend = ((byteToSend >> 4) & 0x0F);
			lcdController->lowNibbleOfByteToSend = (byteToSend & 0x0F);
			lcdController->nibble = HIGH_NIBBLE;
			break;

		case HIGH_NIBBLE:
			if(lcdController->entry == 0)
			{
				HAL_GPIO_WritePin(lcdController->rsPort, lcdController->rsPin, lcdRs);
				HAL_GPIO_WritePin(lcdController->rwPort, lcdController->rwPin, WRITE);
				HAL_GPIO_WritePin(lcdController->db7Port, lcdController->db7Pin, (lcdController->highNibbleOfByteToSend & 0x08));
				HAL_GPIO_WritePin(lcdController->db6Port, lcdController->db6Pin, (lcdController->highNibbleOfByteToSend & 0x04));
				HAL_GPIO_WritePin(lcdController->db5Port, lcdController->db5Pin, (lcdController->highNibbleOfByteToSend & 0x02));
				HAL_GPIO_WritePin(lcdController->db4Port, lcdController->db4Pin, (lcdController->highNibbleOfByteToSend & 0x01));
				lcdController->entry = 1;
				lcdController->delayCounterEnable = 0;
				lcdController->countEnableDelay = TRUE;
			}
			else if(lcdController->entry == 1)
			{
				if(lcdController->delayCounterEnable >= DELAY_150_MICRO_SECONDS)
				{
					lcdController->countEnableDelay = FALSE;
					lcdController->delayCounterEnable = 0;
					HAL_GPIO_WritePin(lcdController->enPort, lcdController->enPin, GPIO_PIN_SET);
					lcdController->entry = 2;
					lcdController->countEnableDelay = TRUE;
				}
			}
			else if(lcdController->entry == 2)
			{
				if(lcdController->delayCounterEnable >= DELAY_150_MICRO_SECONDS)
				{
					lcdController->countEnableDelay = FALSE;
					lcdController->delayCounterEnable = 0;
					HAL_GPIO_WritePin(lcdController->enPort, lcdController->enPin, GPIO_PIN_RESET);
					lcdController->entry = 3;
					lcdController->countEnableDelay = TRUE;
				}
			}
			else
			{
				if(lcdController->delayCounterEnable >= DELAY_150_MICRO_SECONDS)
				{
					lcdController->countEnableDelay = FALSE;
					lcdController->delayCounterEnable = 0;
					lcdController->entry = 0;
					lcdController->nibble = LOW_NIBBLE;
					lcdController->byteSended = FALSE;
				}
			}
			break;

		case LOW_NIBBLE:
			if(lcdController->entry == 0)
			{
				HAL_GPIO_WritePin(lcdController->rsPort, lcdController->rsPin, lcdRs);
				HAL_GPIO_WritePin(lcdController->rwPort, lcdController->rwPin, WRITE);
				HAL_GPIO_WritePin(lcdController->db7Port, lcdController->db7Pin, (lcdController->lowNibbleOfByteToSend & 0x08));
				HAL_GPIO_WritePin(lcdController->db6Port, lcdController->db6Pin, (lcdController->lowNibbleOfByteToSend & 0x04));
				HAL_GPIO_WritePin(lcdController->db5Port, lcdController->db5Pin, (lcdController->lowNibbleOfByteToSend & 0x02));
				HAL_GPIO_WritePin(lcdController->db4Port, lcdController->db4Pin, (lcdController->lowNibbleOfByteToSend & 0x01));
				lcdController->entry = 1;
				lcdController->delayCounterEnable = 0;
				lcdController->countEnableDelay = TRUE;
			}
			else if(lcdController->entry == 1)
			{
				if(lcdController->delayCounterEnable >= DELAY_150_MICRO_SECONDS)
				{
					lcdController->countEnableDelay = FALSE;
					lcdController->delayCounterEnable = 0;
					HAL_GPIO_WritePin(lcdController->enPort, lcdController->enPin, GPIO_PIN_SET);
					lcdController->entry = 2;
					lcdController->countEnableDelay = TRUE;
				}
			}
			else if(lcdController->entry == 2)
			{
				if(lcdController->delayCounterEnable >= DELAY_150_MICRO_SECONDS)
				{
					lcdController->countEnableDelay = FALSE;
					lcdController->delayCounterEnable = 0;
					HAL_GPIO_WritePin(lcdController->enPort, lcdController->enPin, GPIO_PIN_RESET);
					lcdController->entry = 3;
					lcdController->countEnableDelay = TRUE;
				}
			}
			else
			{
				if(lcdController->delayCounterEnable >= DELAY_150_MICRO_SECONDS)
				{
					lcdController->countEnableDelay = FALSE;
					lcdController->delayCounterEnable = 0;
					lcdController->entry = 0;
					lcdController->nibble = DIVISION;
					lcdController->byteSended = TRUE;
				}
			}
			break;

		default:
			break;
	}
}

void lcdControllerInitStruct(LcdController *lcdController,
		GPIO_TypeDef *rsPort, uint16_t rsPin,
		GPIO_TypeDef *rwPort, uint16_t rwPin,
		GPIO_TypeDef *enPort, uint16_t enPin,
		GPIO_TypeDef *db4Port, uint16_t db4Pin,
		GPIO_TypeDef *db5Port, uint16_t db5Pin,
		GPIO_TypeDef *db6Port, uint16_t db6Pin,
		GPIO_TypeDef *db7Port, uint16_t db7Pin,
		GPIO_TypeDef *onOffPort, uint16_t onOffPin,
		TIM_HandleTypeDef *backlightPwmTimer, uint8_t backlightPwmChannel)
{
	lcdController->command = COMMAND_0;
	lcdController->backlightPwmTimer = backlightPwmTimer;
	lcdController->backlightPwmChannel = backlightPwmChannel;
	lcdController->backlightPercent = BL_00_PERCENT;
	lcdController->initialized = FALSE;
	lcdController->counterAux = 0;
	lcdController->counterUnlock = 0;
	lcdController->counterInitCycles1 = 0;
	lcdController->counterInitCycles2 = 0;
	lcdController->byteToSend = COMMAND;
	lcdController->updatePosition = FALSE;
	lcdController->currentRow = 0;
	lcdController->currentColumm = 0;
	lcdController->delayCounter = 0;
	lcdController->delayCounterEnable = 0;
	lcdController->entry = 0;
	lcdController->byteSended = FALSE;
	lcdController->countDelay = FALSE;
	lcdController->countEnableDelay = FALSE;
	lcdController->nibble = DIVISION;
	lcdController->rsPort = rsPort;
	lcdController->rsPin = rsPin;
	lcdController->rwPort = rwPort;
	lcdController->rwPin = rwPin;
	lcdController->enPort = enPort;
	lcdController->enPin = enPin;
	lcdController->db4Port = db4Port;
	lcdController->db4Pin = db4Pin;
	lcdController->db5Port = db5Port;
	lcdController->db5Pin = db5Pin;
	lcdController->db6Port = db6Port;
	lcdController->db6Pin = db6Pin;
	lcdController->db7Port = db7Port;
	lcdController->db7Pin = db7Pin;
	lcdController->onOffPort = onOffPort;
	lcdController->onOffPin = onOffPin;

	memset((char *) lcdController->stringMatrixBuffer[0], ' ', QTY_OF_CHARS_PER_ROW);
	memset((char *) lcdController->stringMatrixBuffer[1], ' ', QTY_OF_CHARS_PER_ROW);
	memset((char *) lcdController->stringMatrixBuffer[2], ' ', QTY_OF_CHARS_PER_ROW);
	memset((char *) lcdController->stringMatrixBuffer[3], ' ', QTY_OF_CHARS_PER_ROW);

	memset((char *) lcdController->stringMatrixShow[0], ' ', QTY_OF_CHARS_PER_ROW);
	memset((char *) lcdController->stringMatrixShow[1], ' ', QTY_OF_CHARS_PER_ROW);
	memset((char *) lcdController->stringMatrixShow[2], ' ', QTY_OF_CHARS_PER_ROW);
	memset((char *) lcdController->stringMatrixShow[3], ' ', QTY_OF_CHARS_PER_ROW);
}

void lcdControllerInit(LcdController *lcdController)
{
	lcdControllerSetBacklight(lcdController, BL_00_PERCENT);
	switch(lcdController->command)
	{
		case COMMAND_0:
			lcdControllerOnOff(lcdController, LCD_OFF);
			lcdController->countDelay = TRUE;
			if(lcdController->delayCounter >= DELAY_2000000_MICRO_SECONDS)
			{
				lcdController->countDelay = FALSE;
				lcdController->delayCounter = 0;
				lcdController->command = COMMAND_1;
			}
			break;

		case COMMAND_1:
			HAL_GPIO_WritePin(lcdController->enPort, lcdController->enPin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(lcdController->rwPort, lcdController->rwPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->rsPort, lcdController->rsPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db7Port, lcdController->db7Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db6Port, lcdController->db6Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db5Port, lcdController->db5Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db4Port, lcdController->db4Pin, GPIO_PIN_RESET);

			lcdController->countDelay = TRUE;
			if(lcdController->delayCounter >= DELAY_1000_MICRO_SECONDS)
			{
				lcdController->countDelay = FALSE;
				lcdController->delayCounter = 0;
				lcdController->command = COMMAND_2;
			}
			break;

		case COMMAND_2:
			HAL_GPIO_WritePin(lcdController->enPort, lcdController->enPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->rwPort, lcdController->rwPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->rsPort, lcdController->rsPin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db7Port, lcdController->db7Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db6Port, lcdController->db6Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db5Port, lcdController->db5Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(lcdController->db4Port, lcdController->db4Pin, GPIO_PIN_RESET);

			lcdController->countDelay = TRUE;
			if(lcdController->delayCounter >= DELAY_1000_MICRO_SECONDS)
			{
				lcdController->countDelay = FALSE;
				lcdController->delayCounter = 0;
				lcdController->counterUnlock++;
				if(lcdController->counterUnlock < 16)
				{
					lcdController->command = COMMAND_1;
				}
				else
				{
					lcdController->command = COMMAND_3;
					lcdController->counterUnlock = 0;
				}
			}
			break;

		case COMMAND_3:
			lcdControllerOnOff(lcdController, LCD_ON);
			lcdController->countDelay = TRUE;
			if(lcdController->delayCounter >= DELAY_2000000_MICRO_SECONDS)
			{
				lcdController->countDelay = FALSE;
				lcdController->delayCounter = 0;
				lcdController->command = COMMAND_4;
			}
			break;

		case COMMAND_4:
			if(lcdController->byteSended == TRUE)
			{
				lcdController->countDelay = TRUE;
				if(lcdController->delayCounter >= DELAY_1500_MICRO_SECONDS)
				{
					lcdController->countDelay = FALSE;
					lcdController->delayCounter = 0;
					lcdController->command = COMMAND_5;
					lcdController->byteSended = FALSE;
				}
			}
			else
			{
				lcdSendByte(lcdController, COMMAND, FUNCTION_SET_4BIT_MULTILINE_5X8DOTS);
			}
			break;

		case COMMAND_5:
			if(lcdController->byteSended == TRUE)
			{
				lcdController->countDelay = TRUE;
				if(lcdController->delayCounter >= DELAY_1500_MICRO_SECONDS)
				{
					lcdController->countDelay = FALSE;
					lcdController->delayCounter = 0;
					lcdController->command = COMMAND_6;
					lcdController->byteSended = FALSE;
				}
			}
			else
			{
				lcdSendByte(lcdController, COMMAND, CURSOR_OR_DISPLAY_SHIFT_SHIFTCURSORTOTHERIGHT);
			}
			break;

		case COMMAND_6:
			if(lcdController->byteSended == TRUE)
			{
				lcdController->countDelay = TRUE;
				if(lcdController->delayCounter >= DELAY_3000_MICRO_SECONDS)
				{
					lcdController->countDelay = FALSE;
					lcdController->delayCounter = 0;
					lcdController->command = COMMAND_7;
					lcdController->byteSended = FALSE;
				}
			}
			else
			{
				lcdSendByte(lcdController, COMMAND, CLEAR_DISPLAY);
			}
			break;

		case COMMAND_7:
			if(lcdController->byteSended == TRUE)
			{
				lcdController->countDelay = TRUE;
				if(lcdController->delayCounter >= DELAY_3000_MICRO_SECONDS)
				{
					lcdController->countDelay = FALSE;
					lcdController->delayCounter = 0;
					lcdController->command = COMMAND_8;
					lcdController->byteSended = FALSE;
				}
			}
			else
			{
				lcdSendByte(lcdController, COMMAND, RETURN_HOME);
			}
			break;

		case COMMAND_8:
			if(lcdController->byteSended == TRUE)
			{
				lcdController->countDelay = TRUE;
				if(lcdController->delayCounter >= DELAY_1500_MICRO_SECONDS)
				{
					lcdController->countDelay = FALSE;
					lcdController->delayCounter = 0;
					lcdController->command = COMMAND_9;
					lcdController->byteSended = FALSE;
				}
			}
			else
			{
				lcdSendByte(lcdController, COMMAND, ENTRY_MODE_SET_INCREMENT_NOTSHIFT);
			}
			break;

		case COMMAND_9:
			if(lcdController->byteSended == TRUE)
			{
				lcdController->countDelay = TRUE;
				if(lcdController->delayCounter >= DELAY_1500_MICRO_SECONDS)
				{
					lcdController->countDelay = FALSE;
					lcdController->delayCounter = 0;
					lcdController->byteSended = FALSE;
					lcdController->command = COMMAND_10;
				}
			}
			else
			{
				lcdSendByte(lcdController, COMMAND, DISPLAY_ON_OFF_CONTROL_DISPLAYON_CURSOROFF);
			}
			break;

		case COMMAND_10:
			if(lcdController->counterAux == 0)
			{
				lcdControllerSetString(lcdController, ROW_0, (uint8_t *) "################");
				lcdControllerSetString(lcdController, ROW_1, (uint8_t *) "################");
				lcdControllerSetString(lcdController, ROW_2, (uint8_t *) "################");
				lcdControllerSetString(lcdController, ROW_3, (uint8_t *) "################");
				lcdController->counterAux = 1;
			}
			else
			{
				lcdControllerSetString(lcdController, ROW_0, (uint8_t *) "                ");
				lcdControllerSetString(lcdController, ROW_1, (uint8_t *) "                ");
				lcdControllerSetString(lcdController, ROW_2, (uint8_t *) "                ");
				lcdControllerSetString(lcdController, ROW_3, (uint8_t *) "                ");
				lcdController->counterAux = 0;
			}
			lcdController->command = COMMAND_11;
			break;

		case COMMAND_11:
			lcdController->countDelay = TRUE;
			if(lcdController->delayCounter >= DELAY_3000_MICRO_SECONDS)
			{
				lcdController->counterInitCycles1++;
				if(lcdController->counterInitCycles1 > 2*(QTY_OF_ROWS * QTY_OF_CHARS_PER_ROW))
				{
					lcdController->command = COMMAND_12;
					lcdController->currentRow = 0;
					lcdController->currentColumm = 0;
					lcdController->counterInitCycles1 = 0;
				}
			}
			else
			{
				lcdControllerUpdate(lcdController);
			}
			break;

		case COMMAND_12:
			lcdController->counterInitCycles2++;
			if(lcdController->counterInitCycles2 > 1)
			{
				lcdController->command = COMMAND_13;
				lcdController->counterInitCycles2 = 0;
			}
			else
			{
				lcdController->command = COMMAND_4;
			}
			break;

		case COMMAND_13:
			lcdController->countDelay = TRUE;
			if(lcdController->delayCounter >= DELAY_2000000_MICRO_SECONDS)
			{
				lcdController->countDelay = FALSE;
				lcdController->delayCounter = 0;
				lcdController->command = COMMAND_0;
				lcdController->initialized = TRUE;
				lcdController->byteToSend = COMMAND;
				lcdControllerSetBacklight(lcdController, BL_60_PERCENT);
			}
			break;
	}
}

void lcdControllerUpdate(LcdController *lcdController)
{
	uint8_t currentCharInDisplay = lcdController->stringMatrixShow[lcdController->currentRow][lcdController->currentColumm];
	uint8_t newCharToDisplay = lcdController->stringMatrixBuffer[lcdController->currentRow][lcdController->currentColumm];
	uint8_t position = 0;

	if(currentCharInDisplay != newCharToDisplay)
	{
		lcdController->updatePosition = TRUE;

		if(lcdController->currentRow == 0)
		{
			position = ROW_0 + lcdController->currentColumm;
		}
		else if(lcdController->currentRow == 1)
		{
			position = ROW_1 + lcdController->currentColumm;
		}
		else if(lcdController->currentRow == 2)
		{
			position = ROW_2 + lcdController->currentColumm;
		}
		else if(lcdController->currentRow == 3)
		{
			position = ROW_3 + lcdController->currentColumm;
		}
	}
	else
	{
		lcdController->currentColumm++;
		if(lcdController->currentColumm > QTY_OF_CHARS_PER_ROW-1)
		{
			lcdController->currentColumm = 0;
			lcdController->currentRow++;
			if(lcdController->currentRow > QTY_OF_ROWS-1)
			{
				lcdController->currentRow = 0;
			}
		}
	}

	if (lcdController->updatePosition == TRUE)
	{
		switch (lcdController->byteToSend)
		{
			case COMMAND:
				if(lcdController->byteSended == TRUE)
				{
					lcdController->countDelay = TRUE;
					if(lcdController->delayCounter >= DELAY_150_MICRO_SECONDS)
					{
						lcdController->countDelay = FALSE;
						lcdController->delayCounter = 0;
						lcdController->byteSended = FALSE;
						lcdController->byteToSend = DATA;
					}
				}
				else
				{
					lcdSendByte(lcdController, COMMAND, position);
				}
				break;

			case DATA:
				if(lcdController->byteSended == TRUE)
				{
					lcdController->countDelay = TRUE;
					if(lcdController->delayCounter >= DELAY_150_MICRO_SECONDS)
					{
						lcdController->countDelay = FALSE;
						lcdController->delayCounter = 0;
						lcdController->byteSended = FALSE;
						lcdController->byteToSend = COMMAND;
						lcdController->updatePosition = FALSE;
						lcdController->stringMatrixShow[lcdController->currentRow][lcdController->currentColumm] = lcdController->stringMatrixBuffer[lcdController->currentRow][lcdController->currentColumm];
					}
				}
				else
				{
					lcdSendByte(lcdController, DATA, newCharToDisplay);
				}
				break;

			default:
				break;
		}
	}
}

void lcdControllerSetString(LcdController *lcdController, LcdRow lcdRow, uint8_t *string)
{
	uint8_t stringLength = strlen((char *) string);
	switch (lcdRow)
	{
		case ROW_0:
			memset((char *) lcdController->stringMatrixBuffer[0], ' ', QTY_OF_CHARS_PER_ROW);
			strncpy((char *) lcdController->stringMatrixBuffer[0], (char *) string, stringLength);
			break;

		case ROW_1:
			memset((char *) lcdController->stringMatrixBuffer[1], ' ', QTY_OF_CHARS_PER_ROW);
			strncpy((char *) lcdController->stringMatrixBuffer[1], (char *) string, stringLength);
			break;

		case ROW_2:
			memset((char *) lcdController->stringMatrixBuffer[2], ' ', QTY_OF_CHARS_PER_ROW);
			strncpy((char *) lcdController->stringMatrixBuffer[2], (char *) string, stringLength);
			break;

		case ROW_3:
			memset((char *) lcdController->stringMatrixBuffer[3], ' ', QTY_OF_CHARS_PER_ROW);
			strncpy((char *) lcdController->stringMatrixBuffer[3], (char *) string, stringLength);
			break;

		default:
			break;
	}
}

void lcdControllerSetBacklight(LcdController *lcdController, BacklightPercent backlightPercent)
{
	__HAL_TIM_SET_COMPARE(lcdController->backlightPwmTimer, lcdController->backlightPwmChannel, backlightPercent);
}

void lcdControllerOnOff(LcdController *lcdController, LcdOnOff lcdOnOff)
{
	HAL_GPIO_WritePin(lcdController->onOffPort, lcdController->onOffPin, lcdOnOff);
}

void lcdControllerClearRow(LcdController *lcdController, LcdRow lcdRow)
{
	lcdControllerSetString(lcdController, lcdRow, (uint8_t *) "                ");
}

void lcdControllerClearDisplay(LcdController *lcdController)
{
	lcdControllerSetString(lcdController, ROW_0, (uint8_t *) "                ");
	lcdControllerSetString(lcdController, ROW_1, (uint8_t *) "                ");
	lcdControllerSetString(lcdController, ROW_2, (uint8_t *) "                ");
	lcdControllerSetString(lcdController, ROW_3, (uint8_t *) "                ");
}

Bool lcdControllerGetDelayStatus(LcdController *lcdController)
{
	return lcdController->countDelay;
}

void lcdControllerIncrementDelayCounter(LcdController *lcdController)
{
	lcdController->delayCounter++;
}

Bool lcdControllerGetEnableDelayStatus(LcdController *lcdController)
{
	return lcdController->countEnableDelay;
}

void lcdControllerIncrementEnableDelayCounter(LcdController *lcdController)
{
	lcdController->delayCounterEnable++;
}

Bool lcdControllerIsInitialized(LcdController *lcdController)
{
	return lcdController->initialized;
}
