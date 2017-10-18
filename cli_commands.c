/*
 * cli_commands.c
 *
 *  Created on: Oct 11, 2017
 *      Author: root
 */
#include "cli_commands.h"

static uint16_t idtGenAddr = 0x00;

static virtual_timer_t printSenseThreadTimer;
static thread_t* printSenseThreadPtr = NULL;
static uint32_t showTime = 0;
// For debug
uint8_t dfuBuff[1024];

sFLASH_USBDriver SFDU1;

const ShellCommand commands[] =
{
    {"power", cmd_power, "Enable/Disable board. power [on | off | cycle]"},
	{"sense", cmd_sense, "Information from power sensors. sense [p]"},
	{"idt",   cmd_idt, "NO DESRIOPION"},
	{"cg",    cmd_cg, "NO DESRIOPION"},
	{"rcg",   cmd_rcg, "NO DESRIOPION"},
	{"pcg",   cmd_printCgValues, "NO DESRIOPION"},
	{"spi",   cmd_spi, "Boot spi flash information. spi [id | r {bytes}]"},
	{"f",     cmd_flash, "NO DESRIOPION"},
	{"br",    cmd_baikalReset, "Baikal reset."},
	{"rf",    cmd_readFlash, "NO DESRIOPION"},
	{"bdfu",  cmd_bdfu, "NO DESRIOPION"},
	{"pr",    cmd_pr, "NO DESRIOPION"},
	{"cgid",  cmd_cgId, "NO DESRIOPION"},
    {NULL, NULL, NULL}
};

static asciiTable_t table[] = {
		{0x20, 0x20}, \
		{0x30, 0x00}, \
		{0x31, 0x01}, \
		{0x32, 0x02}, \
		{0x33, 0x03}, \
		{0x34, 0x04}, \
		{0x35, 0x05}, \
		{0x36, 0x06}, \
		{0x37, 0x07}, \
		{0x38, 0x08}, \
		{0x39, 0x09}, \
		{0x41, 0x0A}, \
		{0x42, 0x0B}, \
		{0x43, 0x0C}, \
		{0x44, 0x0D}, \
		{0x45, 0x0E}, \
		{0x46, 0x0F}, \
};

// ---------- ---------- ---------- ---------- ---------- ----------
void inverse(uint32_t* valIn, uint8_t* valOut)
{
	uint32_t b1t = 0x000000FF;
	uint32_t b2t = 0x0000FF00;
	uint32_t b3t = 0x00FF0000;
	uint32_t b4t = 0xFF000000;

	uint32_t b1 = ((*valIn) & b1t);
	uint32_t b2 = ((*valIn) & b2t) >> 8;
	uint32_t b3 = ((*valIn) & b3t) >> 16;
	uint32_t b4 = ((*valIn) & b4t) >> 24;

	valOut[0] = b1;
	valOut[1] = b2;
	valOut[2] = b3;
	valOut[3] = b4;

	uint32_t res = (0x00000000 | b1);
	res = (res << 8) | b2;
	res = (res << 8) | b3;
	res = (res << 8) | b4;
	(*valIn) = res;
}

// ---------- ---------- ---------- ---------- ---------- ----------
int8_t asciiToHex(uint8_t ascii)
{
	int8_t res = -1;

	for (uint8_t i = 0; i < sizeof(table) / sizeof(table[0]); ++i)
	{
		if (table[i].asciiHex == ascii)
		{
			res = table[i].decCharHex;
			return res;
		}
	}

	return res;
}
// ---------- ---------- ---------- ---------- ---------- ----------
/*
 *  Convert input buffer ID to Human readable format
 */
void spiIdToHumanRead(const char *inBuff, char* outBuff)
{
	if ((inBuff == NULL) || (outBuff == NULL))
	{
		return;
	}

	uint8_t len = 0;
	char tmpStr[50];

	strcpy(tmpStr, "Manufactor: ");
	memcpy(outBuff + len, tmpStr, strlen(tmpStr));
	len = len + strlen(tmpStr);

	if (inBuff[0] == 0x20)
	{
		strcpy(tmpStr, "Micron\r\n");
	}
	else
	{
		strcpy(tmpStr, "Unknown\r\n");
		memcpy(outBuff + len, tmpStr, strlen(tmpStr));
		return;
	}
	memcpy(outBuff + len, tmpStr, strlen(tmpStr));
	len = len + strlen(tmpStr);

	strcpy(tmpStr, "Memory type: ");
	memcpy(outBuff + len, tmpStr, strlen(tmpStr));
	len = len + strlen(tmpStr);

	strcpy(tmpStr, "BAh\r\n");
	memcpy(outBuff + len, tmpStr, strlen(tmpStr));
	len = len + strlen(tmpStr);

	strcpy(tmpStr, "Memory capacity: ");
	memcpy(outBuff + len, tmpStr, strlen(tmpStr));
	len = len + strlen(tmpStr);
	if (inBuff[2] == 0x18)
	{
		strcpy(tmpStr, "128Mbit\r\n");
	}
	else
	{
		strcpy(tmpStr, "Not Defined\r\n");
	}
	memcpy(outBuff + len, tmpStr, strlen(tmpStr));
	len = len + strlen(tmpStr);

//	sprintf(tmpStr, "Length of data follow: %x\r\n", inBuff[3]);
//	memcpy(outBuff + len, tmpStr, strlen(tmpStr));
//	len = len + strlen(tmpStr);

}

// ---------- ---------- ---------- ---------- ---------- ----------
void printSpiFlashId(BaseSequentialStream *chp)
{
	char buff[256];
	char buff1[256];
	palSetPad(GPIOC, 15);
	//uint8_t read = spi_read_status_register(&SFDU1);
	//static int spi_rdid(sFLASH_USBDriver *flash, unsigned char *readarr, int bytes)
	spi_rdid(&SFDU1, buff, 20);

	spiIdToHumanRead(buff, buff1);
	chprintf(chp, "%s", buff1);

	chprintf(chp, "Length of data to follow: ");
	chprintf(chp, "0x%xH\r\n", buff[3]);

	chprintf(chp, "Extended device ID: ");
	chprintf(chp, "%x %x\r\n", buff[4], buff[5]);

	chprintf(chp, "Unique ID code: ");
	for(uint8_t i = 6; i < 17; i++)
	{
		chprintf(chp, "%x ", buff[i]);
	}

	chprintf(chp, "\r\n");
	// chprintf(chp, "%d.\r\n", read);
	palClearPad(GPIOC, 15);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void printFirstBytes(BaseSequentialStream *chp, uint32_t count)
{
	uint8_t buff;
	uint32_t sourceAdr = 0x00000000;

	palSetPad(GPIOC, 15); // Enable multiplexor


	uint8_t colCount = 0;
	for (uint32_t i = 1; i <= count; i++)
	{
		sFLASH_ReadBuffer(&SFDU1, &buff, sourceAdr, 1);
		sourceAdr++;

		if ((i % 4) == 0)
		{
			chprintf(chp, "%02x\t", buff);
			colCount++;
		}
		else
		{
			chprintf(chp, "%02x", buff);
		}

		if (colCount >= 4)
		{
			chprintf(chp, "\r\n");
			colCount = 0;
		}

	}

	chprintf(chp, "\r\n");
	palClearPad(GPIOC, 15); // Disable multiplexor
}

// ---------- ---------- ---------- ---------- ---------- ----------
void printSenseValues(BaseSequentialStream *chp)
{
	setVSenceSamplingRegister(&I2CD1, DEV_ADR_0V95_3V3_DA1, 0x5F);
	setVSenceSamplingRegister(&I2CD1, DEV_ADR_1V50_1V8_DA2, 0x5F);

	chThdSleepMilliseconds(100);

	float pac1Ch1Vol = readVoltageValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL1);
	float pac1Ch2Vol = readVoltageValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL2);
	float pac2Ch1Vol = readVoltageValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL1);
	float pac2Ch2Vol = readVoltageValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL2);

	float pac1Ch1Cur = (pac1Ch1Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL1, 2)  : 0;
	float pac1Ch2Cur = (pac1Ch2Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL2, 10) : 0;
	float pac2Ch1Cur = (pac2Ch1Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL1, 10) : 0;
	float pac2Ch2Cur = (pac2Ch2Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL2, 10) : 0;

	chprintf(chp, "\33[32m");
	chprintf(chp, "\r");
	chprintf(chp, "Domain\t -=Voltage=-\t -=Current=-\t -=Power=-\r\n");
	chprintf(chp, "\33[37m");

	chprintf(chp, "0.95\t %8.4f\t %8.4f\t %8.4f\r\n", pac1Ch1Vol, pac1Ch1Cur, pac1Ch1Cur * pac1Ch1Vol);
	chprintf(chp, "1.50\t %8.4f\t %8.4f\t %8.4f\r\n", pac2Ch1Vol, pac2Ch1Cur, pac2Ch1Cur * pac2Ch1Vol);
	chprintf(chp, "1.80\t %8.4f\t %8.4f\t %8.4f\r\n", pac2Ch2Vol, pac2Ch2Cur, pac2Ch2Cur * pac2Ch2Vol);
	chprintf(chp, "3.30\t %8.4f\t %8.4f\t %8.4f\r\n", pac1Ch2Vol, pac1Ch2Cur, pac1Ch2Cur * pac1Ch2Vol);

	if (showTime != 0)
	{
		chprintf(chp, "\r\33[5A");
	}

}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
static void timerHook(void *arg)
{
	(void) arg;
}

// ---------- ---------- ---------- ---------- ---------- ----------
static THD_WORKING_AREA(waSensePrintThread, 256);
static __attribute__((noreturn)) THD_FUNCTION(SensePrintThread, arg)
{
    (void)arg;

    chRegSetThreadName("SensePrintThread");

	chSysLock();
		chSchGoSleepS(CH_STATE_SUSPENDED);
	chSysUnlock();

    while (true)
    {

		if (!chVTIsArmed(&printSenseThreadTimer)) // first press
		{
			chVTReset(&printSenseThreadTimer);
			chVTSet(&printSenseThreadTimer, MS2ST(showTime), timerHook, NULL);

		}

		chprintf(sdu_stdio, "\r\n");
		while(chVTIsArmed(&printSenseThreadTimer))
		{
			printSenseValues(sdu_stdio);
		}

		chprintf(sdu_stdio, "\r\n\n\n\n\nbmc>");
		showTime = 0;
		chSysLock();
			chSchGoSleepS(CH_STATE_SUSPENDED);
		chSysUnlock();
    }
}
// ---------- ---------- ---------- ---------- ---------- ----------
void createSensePrintThread(void)
{
	printSenseThreadPtr = chThdCreateStatic(waSensePrintThread, sizeof(waSensePrintThread), NORMALPRIO, SensePrintThread, NULL);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_power(BaseSequentialStream *chp, int argc, char *argv[])
{

	if (argc == 0)
	{
		chprintf(chp, "Bad parameters need on/off\r\n");
		return;
	}

    if(strcmp(argv[0], "on") == 0)
    {
    	powerOn();
    }
    else if (strcmp(argv[0], "off") == 0)
    {
    	powerOff();
    }
    else if (strcmp(argv[0], "cycle") == 0)
    {
    	powerOff();
    	chThdSleepMilliseconds(1000);
    	powerOn();
    }

}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_rcg(BaseSequentialStream *chp, int argc, char *argv[])
{
	chprintf(chp, "Read Clock Generator.\r\n");
	(void) argc;
	(void) argv;


	uint8_t cur = 0;
	uint8_t defConfSize = CG_REGISTER_NUM;

	chprintf(chp, "// ---------- ---------- ---------- ---------- ---------- ----------\r\n");
	uint8_t checkResult = 0;
	uint8_t readedValue1 = 0;
	uint8_t readedValue2 = 0;

	idtGenAddr = getIdtAddr();
	for (cur = 0; cur < defConfSize; ++cur)
	{

		readedValue1 = readByteBlock(&I2CD2, idtGenAddr, MAIN_CG_CONF[cur].regNum);
		readedValue2 = MAIN_CG_CONF[cur].regVal;
		chprintf(chp, "%d %x default: %x\r\n", cur, readedValue1, readedValue2);
		chprintf(chp, "%d dev adr: %x\r\n", cur, idtGenAddr);
		if (readedValue1 != readedValue2)
		{
			checkResult++;
		}

	}


}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_cg(BaseSequentialStream *chp, int argc, char *argv[])
{
	//chprintf(chp, "Clock Generator.\r\n");
	(void) chp;
	(void) argc;
	(void) argv;


	//cgStruct *workConf = MAIN_CG_CONF;
	uint8_t cur = 0;
	uint8_t defConfSize = CG_REGISTER_NUM;
	idtGenAddr = getIdtAddr();

	if (idtGenAddr == 0x0000)
	{
		return;
	}

	for (cur = 0; cur < defConfSize; ++cur)
	{
		writeByteBlock(&I2CD2, idtGenAddr, MAIN_CG_CONF[cur].regNum, MAIN_CG_CONF[cur].regVal);
		//chprintf(chp, "%d %x %x\r\n", cur, MAIN_CG_CONF[cur].regNum, MAIN_CG_CONF[cur].regVal);
		//chprintf(chp, "%d devadr1: %x\r\n", cur, idtGenAddr);
	}

	//chprintf(chp, "// ---------- ---------- ---------- ---------- ---------- ----------\r\n");
	uint8_t checkResult = 0;
	uint8_t readedValue1 = 0;
	uint8_t readedValue2 = 0;
	idtGenAddr = getIdtAddr();

	if (idtGenAddr == 0x0000)
	{
		return;
	}

	for (cur = 0; cur < defConfSize; ++cur)
	{
		readedValue1 = readByteBlock(&I2CD2, idtGenAddr, MAIN_CG_CONF[cur].regNum);
		readedValue2 = MAIN_CG_CONF[cur].regVal;
		//chprintf(chp, "%d %x %x\r\n", cur, readedValue1, readedValue2);
		//chprintf(chp, "%d devadr2: %x\r\n", cur, idtGenAddr);
		if (readedValue1 != readedValue2)
		{
			checkResult++;
		}

	}

	if (checkResult == 0)
	{
		cgIdtStatus = CG_STAT_PROG;
		//chprintf(chp, "CG PROGRAMMING SUCCESS.\r\n");
	}
	else
	{
		//chprintf(chp, "CG PROGRAMMING FALSE.\r\n");
	}

//	uint8_t reg_val = 0;
//	reg_val = readByteBlock(&I2CD2, CG_DEFAULT_ADR, VCO_MONITORING_ADDR);
//	chprintf(chp, "VCO mon readed val: %x\r\n", reg_val);
//	reg_val &= ~0x00000002;
//	writeByteBlock(&I2CD2, CG_DEFAULT_ADR, VCO_MONITORING_ADDR, reg_val);
//
//	reg_val = readByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_CR_AND_PRE_DEVIDER);
//	chprintf(chp, "AFC Enable readed val: %x\r\n", reg_val);
//	reg_val &= ~0x00000008;
//	writeByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_CR_AND_PRE_DEVIDER, reg_val);
//
//	reg_val = readByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_BAND_AND_FACTORY_RESERVED);
//	chprintf(chp, "Test mode readed val: %x\r\n", reg_val);
//	reg_val &= ~0x00000020;
//	writeByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_BAND_AND_FACTORY_RESERVED, reg_val);
//
//
//	for (uint8_t i = 0; i < 10; i++)
//	{
//		// Toggling 0x1C
//		reg_val = readByteBlock(&I2CD2, CG_DEFAULT_ADR, VCO_CALIBRATION_ADDR);
//		reg_val &= ~0x00000080;
//		writeByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_BAND_AND_FACTORY_RESERVED, reg_val); // SET [7] = 0
//		reg_val |= 0x00000080;
//		writeByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_BAND_AND_FACTORY_RESERVED, reg_val); // SET [7] = 1
//		reg_val &= ~0x00000080;
//		writeByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_BAND_AND_FACTORY_RESERVED, reg_val); // SET [7] = 0
//
//		chThdSleepMilliseconds(100);
//
//		reg_val = readByteBlock(&I2CD2, CG_DEFAULT_ADR, BAND_I2C_REGISTER);
//		chprintf(chp, "Read step: %d, value: %X\r\n", i, reg_val);
//		reg_val = reg_val >> 3;
//		reg_val &= ~0xFFFFFFE0;
//
//		if ((reg_val != 0) && (reg_val != 23))
//		{
//			chprintf(chp, "Calibration success step: %d, val: %X\r\n", i, reg_val);
//			break;
//		}
//	}
//
//	if ((reg_val != 0) && (reg_val != 23))
//	{
//		chprintf(chp, "CALIBRATION SUCCESS\r\n");
//	}
//	else
//	{
//		chprintf(chp, "CALIBRATION FALSE\r\n");
//	}
//
//	uint8_t vco_band = 0;
//	vco_band = readByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_BAND_AND_FACTORY_RESERVED);
//	vco_band = vco_band | reg_val; // SET [4:0] to readed value
//	vco_band |= 0x00000020; // SET[5] = 1 Test Mode
//	vco_band &= ~0x000000C0; //SET[7:6] = 0;
//	writeByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_VCO_BAND_AND_FACTORY_RESERVED, vco_band);
//
//	otpProrCfg0(chp);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_cgId(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	idtGenAddr = getIdtAddr();
	chprintf(chp, "CG id: %x\r\n", idtGenAddr);

}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_printCgValues(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	chprintf(chp, "Print Clock Generator values from inner flash:\r\n");

	uint32_t *valAddrBase = (uint32_t*) 0x08060000;
	uint8_t numOfRegVal = 0;
	uint8_t val         = 0;
	uint8_t byteCount   = 0;
	for (uint8_t i = 0; i < 80; ++i)
	{
		for (uint8_t j = 0; j < 4; ++j)
		{
			uint8_t lTmp = ((*valAddrBase) >> (j * 8)) & 0x000000FF;
			lTmp = asciiToHex(lTmp);
			if (lTmp == 0x20) // Check if it was a space symbol.
			{                 // Skip to next byte
				continue;
			}

			if (byteCount >= 2)
			{
				MAIN_CG_CONF[numOfRegVal].regVal = val;
				chprintf(chp, "%d %X\r\n", numOfRegVal, MAIN_CG_CONF[numOfRegVal].regVal);
				numOfRegVal++;
				byteCount = 0;
				val       = 0;
			}
			val = (val << (byteCount * 4)) | lTmp;
			byteCount++;
		}

		valAddrBase++;
	}
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void cmd_baikalReset(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) chp;
	(void) argc;
	(void) argv;

	baikalReset();
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_sense(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	if (argc == 0)
	{
		printSenseValues(chp);
		return;
	}

	if (argc > 0)
	{
		showTime = atoi(argv[0]) * 1000;
		if (showTime == 0)
		{
			printSenseValues(chp);
			return;
		}
		chSysLock();
			chSchWakeupS(printSenseThreadPtr, (msg_t) 1);
		chSysUnlock();
	}

	/*
	setVSenceSamplingRegister(&I2CD1, DEV_ADR_0V95_3V3_DA1, 0x5F);
	setVSenceSamplingRegister(&I2CD1, DEV_ADR_1V50_1V8_DA2, 0x5F);

	chThdSleepMilliseconds(100);

	//pacCh1Cur = readCurrentValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL1, 16);

	float pac1Ch1Vol = readVoltageValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL1);
	float pac1Ch2Vol = readVoltageValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL2);
	float pac2Ch1Vol = readVoltageValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL1);
	float pac2Ch2Vol = readVoltageValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL2);

	float pac1Ch1Cur = (pac1Ch1Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL1, 2)  : 0;
	float pac1Ch2Cur = (pac1Ch2Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL2, 10) : 0;
	float pac2Ch1Cur = (pac2Ch1Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL1, 10) : 0;
	float pac2Ch2Cur = (pac2Ch2Vol > 0.8) ? readCurrentValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL2, 10) : 0;

//  Read power values from sensors
//	float pac1Ch1Pow = readPowerValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL1, 2);
//	float pac1Ch2Pow = readPowerValue(&I2CD1, DEV_ADR_0V95_3V3_DA1, CHANNEL2, 10);
//	float pac2Ch1Pow = readPowerValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL1, 10);
//	float pac2Ch2Pow = readPowerValue(&I2CD1, DEV_ADR_1V50_1V8_DA2, CHANNEL2, 10);

	chprintf(chp, "\33[32m");
	chprintf(chp, "Domain\t -=Voltage=-\t -=Current=-\t -=Power=-\r\n");
	chprintf(chp, "\33[37m");

	for (uint32_t i = 0; i <=10000; i++)
	{
	chprintf(chp, "0.95\t %8.4f\t %8.4f\t %8.4f\r\n", pac1Ch1Vol, pac1Ch1Cur, pac1Ch1Cur * pac1Ch1Vol);
	chprintf(chp, "1.50\t %8.4f\t %8.4f\t %8.4f\r\n", pac2Ch1Vol, pac2Ch1Cur, pac2Ch1Cur * pac2Ch1Vol);
	chprintf(chp, "1.80\t %8.4f\t %8.4f\t %8.4f\r\n", pac2Ch2Vol, pac2Ch2Cur, pac2Ch2Cur * pac2Ch2Vol);
	chprintf(chp, "3.30\t %8.4f\t %8.4f\t %8.4f\r\n", pac1Ch2Vol, pac1Ch2Cur, pac1Ch2Cur * pac1Ch2Vol);
	chprintf(chp, "%d\r\n", i);
	chprintf(chp, "\r\33[5A");
	}

	chprintf(chp, "\32\101\130");
//  Print power values from sensors
//	chprintf(chp, "0.95\t %8.4f\t %8.4f\t %8.4f\r\n", pac1Ch1Vol, pac1Ch1Cur, pac1Ch1Pow);
//	chprintf(chp, "1.50\t %8.4f\t %8.4f\t %8.4f\r\n", pac2Ch1Vol, pac2Ch1Cur, pac2Ch1Pow);
//	chprintf(chp, "1.80\t %8.4f\t %8.4f\t %8.4f\r\n", pac2Ch2Vol, pac2Ch2Cur, pac2Ch2Pow);
//	chprintf(chp, "3.30\t %8.4f\t %8.4f\t %8.4f\r\n", pac1Ch2Vol, pac1Ch2Cur, pac1Ch2Pow);
 *
 */
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_idt(BaseSequentialStream *chp, int argc, char *argv[])
{
	chprintf(chp, "IDT on/off.\r\n");
	(void) argc;
	(void) argv;


	palTogglePad(GPIOB, 9);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_spi(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	if (argc == 0)
	{
		chprintf(chp, "Bad parameters need r|id\r\n");
		return;
	}

    if(strcmp(argv[0], "id") == 0)
    {
    	printSpiFlashId(chp);
    }
    else if (strcmp(argv[0], "r") == 0)
    {
    	if (atoi(argv[1]) > 0)
    	{
    		printFirstBytes(chp, atoi(argv[1]));
    	}
    	else
    	{
    		chprintf(chp, "Print first 100 bytes\r\n");
    		printFirstBytes(chp, 100);
    	}

    }
    else
    {
    	chprintf(chp, "Bad parameter.\r\n");
    }
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_flash(BaseSequentialStream *chp, int argc, char *argv[])
{
	chprintf(chp, "Flash.\r\n");
	(void) argc;
	(void) argv;

	uint8_t  buff[10];
	uint32_t sAdr = 0x08040000;
	uint32_t flashVal = 0x00000000;


	palSetPad(GPIOC, 15);

	sFLASH_EraseBulk(&SFDU1);
//	for (uint8_t i = 0; i < 255; ++i)
//	{
//		sFLASH_EraseSector(&SFDU1, i);
//	}

	//chprintf(chp, "Byte %x\r\n", buff[0]);
	uint32_t dest = 0x00000000;
	for (uint32_t i = 0; i < (20000 / 4); ++i)
	{
		flashVal = *((uint32_t*) sAdr);
		inverse(&flashVal, buff);
		sFLASH_WriteBuffer(&SFDU1, buff, dest, 4);
		sFLASH_WaitForWriteEnd(&SFDU1);
		chprintf(chp, "source: %x dest: %x \r\n", sAdr, dest);
		chprintf(chp, "sVal: %x\r\n", buff[0]);
		chprintf(chp, "sVal: %x\r\n", buff[1]);
		chprintf(chp, "sVal: %x\r\n", buff[2]);
		chprintf(chp, "sVal: %x\r\n", buff[3]);
		sAdr = sAdr + sizeof(uint32_t);
		dest = dest + sizeof(uint32_t);
		//sAdr++;
		//dest++;
	}
	palClearPad(GPIOC, 15);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_readFlash(BaseSequentialStream *chp, int argc, char *argv[])
{
	chprintf(chp, "Read flash.\r\n");
	(void) argc;
	(void) argv;

	//uint8_t buff[10];
	uint8_t byteVal;
	//uint32_t sAdr = 0x08060000;

	chprintf(chp, "// ---------- ---------- ---------- ---------- ---------- ----------\r\n");
	palSetPad(GPIOC, 15);
	//chprintf(chp, "Byte %x\r\n", buff[0]);
	uint32_t dest = 0x00000000;
	for (uint32_t i = 0; i < 100; ++i)
	{
		//buff[0] = *((uint32_t*) sAdr);
		sFLASH_ReadBuffer(&SFDU1, &byteVal, dest, 1);
		//chprintf(chp, "source: %x dest: %x \r\n", sAdr, dest);
		chprintf(chp, "readVal: %x\r\n", byteVal);
		//sAdr = sAdr + sizeof(uint32_t);
		//dest = dest + sizeof(uint8_t);
		dest++;
	}
	palClearPad(GPIOC, 15);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_bdfu(BaseSequentialStream *chp, int argc, char *argv[])
{
	chprintf(chp, "DFU hook enters:\r\n");
	(void) argc;
	(void) argv;
	//spi_rdid(&SFDU1, buff, 20);
	for (uint32_t i = 0; i < 1024 / 4; i = i + 8)
	{
		chprintf(chp, "%d\t", dfuBuff[i]);
		chprintf(chp, "%d\t", dfuBuff[i + 1]);
		chprintf(chp, "%d\t", dfuBuff[i + 2]);
		chprintf(chp, "%d\t", dfuBuff[i + 3]);
		chprintf(chp, "%d\t", dfuBuff[i + 4]);
		chprintf(chp, "%d\t", dfuBuff[i + 5]);
		chprintf(chp, "%d\t", dfuBuff[i + 6]);
		chprintf(chp, "%d\t", dfuBuff[i + 7]);
		chprintf(chp, "\r\n");
	}
	//chprintf((BaseSequentialStream*) SDU1.vmt, "%s", SFDU1.chip->name);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void cmd_pr(BaseSequentialStream *chp, int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	chprintf(chp, "PCI reset:\r\n");

	palTogglePad(GPIOA, 1);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void otpProrCfg0(BaseSequentialStream *chp)
{
	chThdSleepMilliseconds(100);

	uint8_t cur = 0;
	uint8_t defConfSize = CG_REGISTER_NUM;
	for (cur = 0; cur < defConfSize; ++cur)
	{
		writeByteBlock(&I2CD2, idtGenAddr, MAIN_CG_CONF[cur].regNum, MAIN_CG_CONF[cur].regVal);
	}


	// Writing REGS [0x73:0x78]
	cur = 0;
	//defConfSize = sizeof(otpCfg0) / 2;
	defConfSize = 6; // !!!!!!! Check correct value
	for (cur = 0; cur < defConfSize; ++cur)
	{
		writeByteBlock(&I2CD2, idtGenAddr, otpCfg0[cur].regNum, otpCfg0[cur].regVal);
	}

	writeByteBlock(&I2CD2, idtGenAddr, 0x72, 0xF0);
	writeByteBlock(&I2CD2, idtGenAddr, 0x72, 0xF8);
	chThdSleepMilliseconds(500);

	writeByteBlock(&I2CD2, idtGenAddr, 0x72, 0xF0);
	writeByteBlock(&I2CD2, idtGenAddr, 0x72, 0xF8);
	chThdSleepMilliseconds(500);

	writeByteBlock(&I2CD2, idtGenAddr, 0x72, 0xF0);

	// 11 Programming done
	writeByteBlock(&I2CD2, idtGenAddr, 0x72, 0xF2);

	chThdSleepMilliseconds(500); // This dosn't exist in manual
	writeByteBlock(&I2CD2, idtGenAddr, 0x72, 0xF0);

	uint8_t burnRes = 10;
	burnRes = readByteBlock(&I2CD2, idtGenAddr, 0x9F);
	chprintf(chp, "Burn result: %X\r\n", burnRes);
	burnRes &= ~0xFE;
	if (burnRes == 0)
	{
		chprintf(chp, "Burn SUCCESS\r\n");
	}
	else
	{
		chprintf(chp, "Burn FALSE\r\n");
	}

	writeByteBlock(&I2CD2, idtGenAddr, 0x9F, 0x00);

	burnRes = readByteBlock(&I2CD2, idtGenAddr, REG_OTP_CONTROL);
	burnRes &= ~0x00000080;
	writeByteBlock(&I2CD2, idtGenAddr, REG_OTP_CONTROL, burnRes);
}

// ---------- ---------- ---------- ---------- ---------- ----------
void powerOn(void)
{
	ATX_POWER_ON();

    palSetPad(GPIOC, 2); // Enable 3V3
    palSetPad(GPIOC, 8); // Enable 3V3

	idtOn();
	palSetPad(GPIOB, 7);
	palSetPad(GPIOC, 3);
	palSetPad(GPIOC, 13);

	chThdSleepMilliseconds(20);
	palSetPad(GPIOC, 4);
	palSetPad(GPIOC, 5);

	chThdSleepMilliseconds(100);
	palSetPad(GPIOD, 0);

	palSetPad(GPIOD, 3); // PWR_CLOCK_ENABLE


	if (cgIdtStatus == CG_STAT_ERASED)
	{
		BaseSequentialStream *outChannel2 = sdu_stdio;
		cmd_cg(outChannel2, 0, NULL);
	}

	chThdSleepMilliseconds(RESET_HOLD_TIME_MS);
	palSetPad(GPIOD, 2);

	palSetPad(GPIOA, 2);
	palSetPad(GPIOA, 3);

	palSetPad(GPIOA, 1); // PCIE reset
}

// ---------- ---------- ---------- ---------- ---------- ----------
void powerOff(void)
{
	ATX_POWER_OFF();
	palClearPad(GPIOC, 2); // Enable 3V3
	palClearPad(GPIOC, 8); // Enable 3V3

	idtOff();
	palClearPad(GPIOB, 7);
	palClearPad(GPIOC, 3);
	palClearPad(GPIOC, 13);

	//chThdSleepMilliseconds(20);
	palClearPad(GPIOC, 4);
	palClearPad(GPIOC, 5);

	//chThdSleepMilliseconds(100);
	palClearPad(GPIOD, 0);

	palClearPad(GPIOD, 3); // PWR_CLOCK_ENABLE

	cgIdtStatus = CG_STAT_ERASED;

	//chThdSleepMilliseconds(RESET_HOLD_TIME_MS);
	palClearPad(GPIOD, 2);

	palClearPad(GPIOA, 2);
	palClearPad(GPIOA, 3);

	palClearPad(GPIOA, 1); // PCIE reset
}

// ---------- ---------- ---------- ---------- ---------- ----------
void powerToggle(void)
{
	ATX_POWER_TOGGLE();
	palTogglePad(GPIOC, 2); // Enable 3V3
	palTogglePad(GPIOC, 8); // Enable 3V3

	idtOnOff();

	palTogglePad(GPIOB, 7);
	palTogglePad(GPIOC, 3);
	palTogglePad(GPIOC, 13);

	chThdSleepMilliseconds(20);
	palTogglePad(GPIOC, 4);
	palTogglePad(GPIOC, 5);

	chThdSleepMilliseconds(100);
	palTogglePad(GPIOD, 0);

	palTogglePad(GPIOD, 3); // PWR_CLOCK_ENABLE

	volatile uint32_t res = palReadPad(GPIOC, 6);
	if(res == 0)
	{
		BaseSequentialStream *outChannel2 = sdu_stdio;
		cmd_cg(outChannel2, 0, NULL);
	}

	chThdSleepMilliseconds(RESET_HOLD_TIME_MS);
	palTogglePad(GPIOD, 2);

	palTogglePad(GPIOA, 2);
	palTogglePad(GPIOA, 3);

	palTogglePad(GPIOA, 1); // PCIE reset
}

// ---------- ---------- ---------- ---------- ---------- ----------
void baikalReset(void)
{
	// Stop Baikal
	palClearPad(GPIOD, 2);
	palClearPad(GPIOA, 2);
	palClearPad(GPIOA, 3);
	palClearPad(GPIOA, 1); // PCIE reset

	// Start Baikal
	chThdSleepMilliseconds(100);
	palSetPad(GPIOD, 0);

	chThdSleepMilliseconds(RESET_HOLD_TIME_MS);
	palSetPad(GPIOD, 2);

	palSetPad(GPIOA, 2);
	palSetPad(GPIOA, 3);
	palSetPad(GPIOA, 1); // PCIE reset
}
