/*
 * cg_5p49v5901.c
 *
 *  Created on: Oct 11, 2017
 *      Author: root
 */

#include "cg_5p49v5901.h"


CgStatus_t cgIdtStatus = CG_STAT_ERASED;
/* ********** ********** ********** ********** ********** ********** ********** ********** ********** */
/* This is configuration is generated in Timing Commander                                             */
/* https://www.idt.com/products/clocks-timing/timing-commander-software-download-resource-guide       */
/* ********** ********** ********** ********** ********** ********** ********** ********** ********** */
cgStruct defaultConf[] = { {REG_OTP_CONTROL,                   0xE1}, \
                           {REG_PRIMARY_SOURCE_SHUTDOWN,       0x82}, \
                           {REG_VCO_BAND_AND_FACTORY_RESERVED, 0x0C}, \
                           {REG_CRYSTAL_X1_LOAD_CAPACITOR,     0x81}, \
                           {REG_CRYSTAL_X2_LOAD_CAPACITOR,     0x80}, \
                           {REG_REFERINCE_DRIVER,              0x03}, \
                           {REG_VCO_CR_AND_PRE_DEVIDER,        0x8C}, \
                           {REG_FEEDBACK_INTEGER_DEVIDER,      0x07}, \
                           {REG_FEEDBACK_INTEGER_DEVIDER_BIT,  0x00}, \
                           {REG_FEEDBACK_FRACTIONAL_DEVIDER1,  0x66}, \
                           {REG_FEEDBACK_FRACTIONAL_DEVIDER2,  0x66}, \
                           {REG_FEEDBACK_FRACTIONAL_DEVIDER3,  0x66}, \
                           {REG_RC_CONTROL1,                   0xD0}, \
                           {REG_RC_CONTROL2,                   0x7A}, \
                                                                      \
                           {REG_OUT_DRIVER1_CR1_SET,           0x81}, \
                           {REG_OUT_DRIVER1_FRACT_SET1,        0x00}, \
                           {REG_OUT_DRIVER1_FRACT_SET2,        0x00}, \
                           {REG_OUT_DRIVER1_FRACT_SET3,        0x00}, \
                           {REG_OUT_DRIVER1_FRACT_SET4,        0x00}, \
                           {REG_OUT_DRIVER1_SPREAD_CR1,        0x00}, \
                           {REG_OUT_DRIVER1_SPREAD_CR2,        0x00}, \
                           {REG_OUT_DRIVER1_SPREAD_CR3,        0x00}, \
                           {REG_OUT_DRIVER1_SPREAD_MOD_R_CR1,  0x00}, \
                           {REG_OUT_DRIVER1_SPREAD_MOD_R_CR2,  0x00}, \
                           {REG_OUT_DRIVER1_SKEW_INT_PART1,    0x00}, \
                           {REG_OUT_DRIVER1_SKEW_INT_PART2,    0x00}, \
                           {REG_OUT_DRIVER1_INT_PART1,         0x00}, \
                           {REG_OUT_DRIVER1_INT_PART2,         0xE0}, \
                           {REG_OUT_DRIVER1_SKEW_FRACT_PART,   0x00}, \
                                                                      \
						   {REG_OUT_DRIVER2_CR1_SET,           0x81}, \
                           {REG_OUT_DRIVER2_FRACT_SET1,        0x03}, \
                           {REG_OUT_DRIVER2_FRACT_SET2,        0xD7}, \
                           {REG_OUT_DRIVER2_FRACT_SET3,        0x0A}, \
                           {REG_OUT_DRIVER2_FRACT_SET4,        0x3C}, \
                           {REG_OUT_DRIVER2_SPREAD_CR1,        0x00}, \
                           {REG_OUT_DRIVER2_SPREAD_CR2,        0x00}, \
                           {REG_OUT_DRIVER2_SPREAD_CR3,        0x00}, \
                           {REG_OUT_DRIVER2_SPREAD_MOD_R_CR1,  0x00}, \
                           {REG_OUT_DRIVER2_SPREAD_MOD_R_CR2,  0x00}, \
                           {REG_OUT_DRIVER2_SKEW_INT_PART1,    0x00}, \
                           {REG_OUT_DRIVER2_SKEW_INT_PART2,    0x00}, \
                           {REG_OUT_DRIVER2_INT_PART1,         0x00}, \
                           {REG_OUT_DRIVER2_INT_PART2,         0x80}, \
                           {REG_OUT_DRIVER2_SKEW_FRACT_PART,   0x00}, \
                                                                      \
		                   {REG_OUT_DRIVER3_CR1_SET,           0x81}, \
                           {REG_OUT_DRIVER3_FRACT_SET1,        0x00}, \
                           {REG_OUT_DRIVER3_FRACT_SET2,        0x00}, \
                           {REG_OUT_DRIVER3_FRACT_SET3,        0x00}, \
                           {REG_OUT_DRIVER3_FRACT_SET4,        0x00}, \
                           {REG_OUT_DRIVER3_SPREAD_CR1,        0x00}, \
                           {REG_OUT_DRIVER3_SPREAD_CR2,        0x00}, \
                           {REG_OUT_DRIVER3_SPREAD_CR3,        0x00}, \
                           {REG_OUT_DRIVER3_SPREAD_MOD_R_CR1,  0x00}, \
                           {REG_OUT_DRIVER3_SPREAD_MOD_R_CR2,  0x00}, \
                           {REG_OUT_DRIVER3_SKEW_INT_PART1,    0x00}, \
                           {REG_OUT_DRIVER3_SKEW_INT_PART2,    0x00}, \
                           {REG_OUT_DRIVER3_INT_PART1,         0x00}, \
                           {REG_OUT_DRIVER3_INT_PART2,         0xE0}, \
                           {REG_OUT_DRIVER3_SKEW_FRACT_PART,   0x00}, \
                                                                      \
				           {REG_OUT_DRIVER4_CR1_SET,           0x81}, \
                           {REG_OUT_DRIVER4_FRACT_SET1,        0x00}, \
                           {REG_OUT_DRIVER4_FRACT_SET2,        0x00}, \
                           {REG_OUT_DRIVER4_FRACT_SET3,        0x00}, \
                           {REG_OUT_DRIVER4_FRACT_SET4,        0x00}, \
                           {REG_OUT_DRIVER4_SPREAD_CR1,        0x00}, \
                           {REG_OUT_DRIVER4_SPREAD_CR2,        0x00}, \
                           {REG_OUT_DRIVER4_SPREAD_CR3,        0x00}, \
                           {REG_OUT_DRIVER4_SPREAD_MOD_R_CR1,  0x00}, \
                           {REG_OUT_DRIVER4_SPREAD_MOD_R_CR2,  0x00}, \
                           {REG_OUT_DRIVER4_SKEW_INT_PART1,    0x00}, \
                           {REG_OUT_DRIVER4_SKEW_INT_PART2,    0x00}, \
                           {REG_OUT_DRIVER4_INT_PART1,         0x00}, \
                           {REG_OUT_DRIVER4_INT_PART2,         0xE0}, \
                           {REG_OUT_DRIVER4_SKEW_FRACT_PART,   0x00}, \
                                                                      \
		                   {REG_OUT_CLOCK1_CONF1,              0x7B}, \
                           {REG_OUT_CLOCK1_CONF2,              0x7B}, \
                           {REG_OUT_CLOCK1_CONF3,              0x7B}, \
                           {REG_OUT_CLOCK1_CONF4,              0x7B}, \
                           {REG_OUT_CLOCK2_CONF1,              0x01}, \
                           {REG_OUT_CLOCK2_CONF2,              0x01}, \
                           {REG_OUT_CLOCK2_CONF3,              0x01}, \
                           {REG_OUT_CLOCK2_CONF4,              0x01}, \
                           {REG_CLK_OE_SD_FUNC1,               0xFF}, \
                           {REG_CLK_OE_SD_FUNC2,               0x7C}  \
                         };
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
cgStruct defaultConf1[] = { {0x00, 0x61}, \
                            {0x01, 0xFF}, \
                            {0x02, 0x00}, \
                            {0x03, 0x00}, \
                            {0x04, 0x00}, \
                            {0x05, 0x00}, \
                            {0x06, 0x00}, \
                            {0x07, 0x00}, \
                            {0x08, 0x00}, \
                            {0x09, 0xFF}, \
                            {0x0A, 0x01}, \
                            {0x0B, 0x00}, \
                            {0x0C, 0x00}, \
                            {0x0D, 0xB6}, \
                                                                      \
                            {0x0E, 0xB4}, \
                            {0x0F, 0x92}, \
                            {0x10, 0x8A}, \
                            {0x11, 0x2F}, \
                            {0x12, 0x81}, \
                            {0x13, 0x80}, \
                            {0x14, 0x00}, \
                            {0x15, 0x03}, \
                            {0x16, 0x84}, \
                            {0x17, 0x03}, \
                            {0x18, 0x20}, \
                            {0x19, 0x00}, \
                            {0x1A, 0x00}, \
                            {0x1B, 0x00}, \
                            {0x1C, 0x05}, \
                                                                      \
						    {0x1D, 0xFD}, \
                            {0x1E, 0xD0}, \
                            {0x1F, 0x18}, \
                            {0x20, 0x00}, \
                            {0x21, 0x81}, \
                            {0x22, 0x00}, \
                            {0x23, 0x00}, \
                            {0x24, 0x00}, \
                            {0x25, 0x00}, \
                            {0x26, 0x00}, \
                            {0x27, 0x00}, \
                            {0x28, 0x00}, \
                            {0x29, 0x00}, \
                            {0x2A, 0x04}, \
                            {0x2B, 0x00}, \
                                                                      \
		                    {0x2C, 0x00}, \
                            {0x2D, 0x00}, \
                            {0x2E, 0x80}, \
                            {0x2F, 0x00}, \
                            {0x30, 0x00}, \
                            {0x31, 0x81}, \
                            {0x32, 0x02}, \
                            {0x33, 0x00}, \
                            {0x34, 0x00}, \
                            {0x35, 0x00}, \
                            {0x36, 0x00}, \
                            {0x37, 0x00}, \
                            {0x38, 0x00}, \
                            {0x39, 0x00}, \
                            {0x3A, 0x00}, \
                                                                      \
				            {0x3B, 0x00}, \
                            {0x3C, 0x01}, \
                            {0x3D, 0x00}, \
                            {0x3E, 0xC0}, \
                            {0x3F, 0x00}, \
                            {0x40, 0x00}, \
                            {0x41, 0x0C}, \
                            {0x42, 0x02}, \
                            {0x43, 0x00}, \
                            {0x44, 0x00}, \
                            {0x45, 0x00}, \
                            {0x46, 0x00}, \
                            {0x47, 0x00}, \
                            {0x48, 0x00}, \
                            {0x49, 0x00}, \
                                                                      \
		                    {0x4A, 0x00}, \
                            {0x4B, 0x00}, \
                            {0x4C, 0x00}, \
                            {0x4D, 0x00}, \
                            {0x4E, 0xC0}, \
                            {0x4F, 0x00}, \
                            {0x50, 0x00}, \
                            {0x51, 0x81}, \
                            {0x52, 0x02}, \
                            {0x53, 0x00}, \
											                          \
							{0x54, 0x00}, \
							{0x55, 0x00}, \
							{0x56, 0x00}, \
							{0x57, 0x00}, \
							{0x58, 0x00}, \
							{0x59, 0x00}, \
							{0x5A, 0x00}, \
							{0x5B, 0x00}, \
							{0x5C, 0x00}, \
							{0x5D, 0x00}, \
	                                                                  \
							{0x5E, 0xC0}, \
							{0x5F, 0x00}, \
							{0x60, 0x5B}, \
							{0x61, 0x01}, \
							{0x62, 0x5B}, \
							{0x63, 0x01}, \
							{0x64, 0x5B}, \
							{0x65, 0x01}, \
							{0x66, 0x5B}, \
							{0x67, 0x01}, \
							{0x68, 0x7C}, \
							{0x69, 0x7C}  \
                         };
// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
cgStruct defaultConf2[] = { {0x00, 0xE1}, \
                            {0x01, 0xFF}, \
                            {0x02, 0x00}, \
                            {0x03, 0x00}, \
                            {0x04, 0x00}, \
                            {0x05, 0x00}, \
                            {0x06, 0x00}, \
                            {0x07, 0x00}, \
                            {0x08, 0x00}, \
                            {0x09, 0xFF}, \
                            {0x0A, 0x01}, \
                            {0x0B, 0xC0}, \
                            {0x0C, 0x00}, \
                            {0x0D, 0xB6}, \
                                                                      \
                            {0x0E, 0xB4}, \
                            {0x0F, 0x92}, \
                            {0x10, 0x82}, \
                            {0x11, 0x0C}, \
                            {0x12, 0x81}, \
                            {0x13, 0x80}, \
                            {0x14, 0x00}, \
                            {0x15, 0x03}, \
                            {0x16, 0x8C}, \
                            {0x17, 0x06}, \
                            {0x18, 0x40}, \
                            {0x19, 0x00}, \
                            {0x1A, 0x00}, \
                            {0x1B, 0x00}, \
                            {0x1C, 0x9F}, \
                                                                      \
						    {0x1D, 0xFF}, \
                            {0x1E, 0xE0}, \
                            {0x1F, 0x80}, \
                            {0x20, 0x00}, \
                            {0x21, 0x81}, \
                            {0x22, 0x02}, \
                            {0x23, 0x00}, \
                            {0x24, 0x00}, \
                            {0x25, 0x00}, \
                            {0x26, 0x00}, \
                            {0x27, 0x00}, \
                            {0x28, 0x00}, \
                            {0x29, 0x00}, \
                            {0x2A, 0x04}, \
                            {0x2B, 0x00}, \
                                                                      \
		                    {0x2C, 0x01}, \
                            {0x2D, 0x00}, \
                            {0x2E, 0xC0}, \
                            {0x2F, 0x00}, \
                            {0x30, 0x00}, \
                            {0x31, 0x0C}, \
                            {0x32, 0x00}, \
                            {0x33, 0x00}, \
                            {0x34, 0x00}, \
                            {0x35, 0x00}, \
                            {0x36, 0x00}, \
                            {0x37, 0x00}, \
                            {0x38, 0x00}, \
                            {0x39, 0x00}, \
                            {0x3A, 0x04}, \
                                                                      \
				            {0x3B, 0x00}, \
                            {0x3C, 0x01}, \
                            {0x3D, 0x06}, \
                            {0x3E, 0x40}, \
                            {0x3F, 0x00}, \
                            {0x40, 0x00}, \
                            {0x41, 0x0C}, \
                            {0x42, 0x02}, \
                            {0x43, 0x00}, \
                            {0x44, 0x00}, \
                            {0x45, 0x00}, \
                            {0x46, 0x00}, \
                            {0x47, 0x00}, \
                            {0x48, 0x00}, \
                            {0x49, 0x00}, \
                                                                      \
		                    {0x4A, 0x04}, \
                            {0x4B, 0x00}, \
                            {0x4C, 0x01}, \
                            {0x4D, 0x05}, \
                            {0x4E, 0x70}, \
                            {0x4F, 0x00}, \
                            {0x50, 0x00}, \
                            {0x51, 0x0C}, \
                            {0x52, 0x02}, \
                            {0x53, 0x00}, \
											                          \
							{0x54, 0x00}, \
							{0x55, 0x00}, \
							{0x56, 0x00}, \
							{0x57, 0x00}, \
							{0x58, 0x00}, \
							{0x59, 0x00}, \
							{0x5A, 0x04}, \
							{0x5B, 0x00}, \
							{0x5C, 0x00}, \
							{0x5D, 0x00}, \
	                                                                  \
							{0x5E, 0xC0}, \
							{0x5F, 0x00}, \
							{0x60, 0x7B}, \
							{0x61, 0x01}, \
							{0x62, 0x7B}, \
							{0x63, 0x01}, \
							{0x64, 0x7B}, \
							{0x65, 0x01}, \
							{0x66, 0x7B}, \
							{0x67, 0x01}, \
							{0x68, 0x7F}, \
							{0x69, 0x7C}  \
                         };

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
cgStruct fromMemConf[] = { {0x00, 0xFF}, \
                           {0x01, 0xFF}, \
                           {0x02, 0xFF}, \
                           {0x03, 0xFF}, \
                           {0x04, 0xFF}, \
                           {0x05, 0xFF}, \
                           {0x06, 0xFF}, \
                           {0x07, 0xFF}, \
                           {0x08, 0xFF}, \
                           {0x09, 0xFF}, \
                           {0x0A, 0xFF}, \
                           {0x0B, 0xFF}, \
                           {0x0C, 0xFF}, \
                           {0x0D, 0xFF}, \
                                                                      \
                           {0x0E, 0xFF}, \
                           {0x0F, 0xFF}, \
                           {0x10, 0xFF}, \
                           {0x11, 0xFF}, \
                           {0x12, 0xFF}, \
                           {0x13, 0xFF}, \
                           {0x14, 0xFF}, \
                           {0x15, 0xFF}, \
                           {0x16, 0xFF}, \
                           {0x17, 0xFF}, \
                           {0x18, 0xFF}, \
                           {0x19, 0xFF}, \
                           {0x1A, 0xFF}, \
                           {0x1B, 0xFF}, \
                           {0x1C, 0xFF}, \
                                                                      \
						   {0x1D, 0xFF}, \
                           {0x1E, 0xFF}, \
                           {0x1F, 0xFF}, \
                           {0x20, 0xFF}, \
                           {0x21, 0xFF}, \
                           {0x22, 0xFF}, \
                           {0x23, 0xFF}, \
                           {0x24, 0xFF}, \
                           {0x25, 0xFF}, \
                           {0x26, 0xFF}, \
                           {0x27, 0xFF}, \
                           {0x28, 0xFF}, \
                           {0x29, 0xFF}, \
                           {0x2A, 0xFF}, \
                           {0x2B, 0xFF}, \
                                                                      \
		                   {0x2C, 0xFF}, \
                           {0x2D, 0xFF}, \
                           {0x2E, 0xFF}, \
                           {0x2F, 0xFF}, \
                           {0x30, 0xFF}, \
                           {0x31, 0xFF}, \
                           {0x32, 0xFF}, \
                           {0x33, 0xFF}, \
                           {0x34, 0xFF}, \
                           {0x35, 0xFF}, \
                           {0x36, 0xFF}, \
                           {0x37, 0xFF}, \
                           {0x38, 0xFF}, \
                           {0x39, 0xFF}, \
                           {0x3A, 0xFF}, \
                                                                      \
				           {0x3B, 0xFF}, \
                           {0x3C, 0xFF}, \
                           {0x3D, 0xFF}, \
                           {0x3E, 0xFF}, \
                           {0x3F, 0xFF}, \
                           {0x40, 0xFF}, \
                           {0x41, 0xFF}, \
                           {0x42, 0xFF}, \
                           {0x43, 0xFF}, \
                           {0x44, 0xFF}, \
                           {0x45, 0xFF}, \
                           {0x46, 0xFF}, \
                           {0x47, 0xFF}, \
                           {0x48, 0xFF}, \
                           {0x49, 0xFF}, \
                                                                      \
		                   {0x4A, 0xFF}, \
                           {0x4B, 0xFF}, \
                           {0x4C, 0xFF}, \
                           {0x4D, 0xFF}, \
                           {0x4E, 0xFF}, \
                           {0x4F, 0xFF}, \
                           {0x50, 0xFF}, \
                           {0x51, 0xFF}, \
                           {0x52, 0xFF}, \
                           {0x53, 0xFF}, \
											                          \
						   {0x54, 0xFF}, \
						   {0x55, 0xFF}, \
						   {0x56, 0xFF}, \
						   {0x57, 0xFF}, \
						   {0x58, 0xFF}, \
						   {0x59, 0xFF}, \
						   {0x5A, 0xFF}, \
						   {0x5B, 0xFF}, \
						   {0x5C, 0xFF}, \
						   {0x5D, 0xFF}, \
	                                                                  \
						   {0x5E, 0xFF}, \
						   {0x5F, 0xFF}, \
						   {0x60, 0xFF}, \
						   {0x61, 0xFF}, \
						   {0x62, 0xFF}, \
						   {0x63, 0xFF}, \
						   {0x64, 0xFF}, \
						   {0x65, 0xFF}, \
						   {0x66, 0xFF}, \
						   {0x67, 0xFF}, \
						   {0x68, 0xFF}, \
						   {0x69, 0xFF}  \
                         };

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
cgStruct otpCfg0[] = { {0x73, 0x08}, \
                       {0x74, 0x4E}, \
                       {0x75, 0x34}, \
                       {0x76, 0xE1}, \
                       {0x77, 0x10}, \
                       {0x78, 0x10}, \
                     };

cgStruct otpCfg1[] = { {0x73, 0x35}, \
                       {0x74, 0x4E}, \
                       {0x75, 0x61}, \
                       {0x76, 0xE1}, \
                       {0x77, 0x10}, \
                       {0x78, 0x10}, \
                     };

cgStruct otpCfg2[] = { {0x73, 0x62}, \
                       {0x74, 0x4E}, \
                       {0x75, 0x8E}, \
                       {0x76, 0xE1}, \
                       {0x77, 0x10}, \
                       {0x78, 0x10}, \
                     };

cgStruct otpCfg3[] = { {0x73, 0x8F}, \
                       {0x74, 0x4E}, \
                       {0x75, 0xBB}, \
                       {0x76, 0xE1}, \
                       {0x77, 0x10}, \
                       {0x78, 0x10}, \
                     };

cgStruct *MAIN_CG_CONF = defaultConf;

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void idtOnOff(void)
{
	palTogglePad(GPIOB, 9);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void idtOn(void)
{
	palSetPad(GPIOB, 9);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
void idtOff(void)
{
	palClearPad(GPIOB, 9);
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
uint16_t getIdtAddr(void)
{
	uint16_t result = 0x0000;

	result += readByteBlock(&I2CD2, CG_DEFAULT_ADR, REG_RAM0_DEVICE_ID);
	if (result == 0xFF)
	{
		return CG_DEFAULT_ADR;
	}

	result = 0x0000;
	result += readByteBlock(&I2CD2, CG_ADDITION_ADR, REG_RAM0_DEVICE_ID);
	if (result != 0)
	{
		return CG_ADDITION_ADR;
	}

//  That was an old method to detect CG. Now we trying to use new. It is faster.
//	uint8_t cur = 0;
//	uint8_t defConfSize = CG_REGISTER_NUM;
//
//	uint16_t result = 0x0000;
//	for (cur = 0; cur < defConfSize; ++cur)
//	{
//		result += readByteBlock(&I2CD2, CG_DEFAULT_ADR, MAIN_CG_CONF[cur].regNum);
//	}
//
//	if (result != 0)
//	{
//		return CG_DEFAULT_ADR;
//	}
//
//	result = 0x0000;
//	for (cur = 0; cur < defConfSize; ++cur)
//	{
//		result += readByteBlock(&I2CD2, CG_ADDITION_ADR, MAIN_CG_CONF[cur].regNum);
//	}
//
//	if (result != 0)
//	{
//		return CG_ADDITION_ADR;
//	}
	return 0x00;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
double innerPow16(double exp)
{
	if (exp == 0)
	{
		return 1;
	}

	double res = 1;

	for(uint8_t i = 1; i <= exp; i++)
	{
		res = res * 16;
	}
	return res;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float getFeedBackDivider(void)
{
	float result = 0;

	uint16_t idtAdr = getIdtAddr();

	if (idtAdr == 0)
	{
		return 0;
	}

	uint8_t intPartHi = readByteBlock(&I2CD2, idtAdr, REG_FEEDBACK_INTEGER_DEVIDER);
	uint8_t intPartLo = readByteBlock(&I2CD2, idtAdr, REG_FEEDBACK_INTEGER_DEVIDER_BIT);

	uint16_t intDiv = 0;
	intDiv |= intPartHi;
	intDiv = intDiv << 8;
	intDiv |= intPartLo;
	intDiv = intDiv >> 4;

	uint8_t frPrt[3];
	frPrt[0] = readByteBlock(&I2CD2, idtAdr, REG_FEEDBACK_FRACTIONAL_DEVIDER1);
	frPrt[1] = readByteBlock(&I2CD2, idtAdr, REG_FEEDBACK_FRACTIONAL_DEVIDER2);
	frPrt[2] = readByteBlock(&I2CD2, idtAdr, REG_FEEDBACK_FRACTIONAL_DEVIDER3);

	uint8_t frPrtByBytes[6] = {0, 0, 0, 0, 0, 0};

	uint8_t mask = 0x0F;
	frPrtByBytes[0] = frPrt[2] & mask;
	frPrtByBytes[1] = (frPrt[2] >> 4) & mask;
	frPrtByBytes[2] = frPrt[1] & mask;
	frPrtByBytes[3] = (frPrt[1] >> 4) & mask;
	frPrtByBytes[4] = frPrt[0] & mask;
	frPrtByBytes[5] = (frPrt[0] >> 4) & mask;

	long int fractDiv = 0;
	for (uint8_t i = 0; i <= 5; i++)
	{
		fractDiv += frPrtByBytes[i] * innerPow16(i);
	}

	result = (float)fractDiv / (float)16777216;
	result += intDiv;

	return result;
}

// ---------- ---------- ---------- ---------- ---------- ---------- ---------- ----------
float getFodOut(uint8_t outNumber)
{
	float res = 0;

	uint16_t idtAdr = getIdtAddr();

	if (idtAdr == 0)
	{
		return 0;
	}

	uint16_t intPartBase = 0x2D;
	uint16_t fracPartBase = 0x22;

	uint16_t intPartAdr  = intPartBase  + (0x10 * (outNumber - 1));
	uint16_t fracPartAdr = fracPartBase + (0x10 * (outNumber - 1));

	uint8_t intPartHi = readByteBlock(&I2CD2, idtAdr, intPartAdr);
	uint8_t intPartLo = readByteBlock(&I2CD2, idtAdr, intPartAdr + 0x01);
	uint16_t intDiv = 0;
	intDiv |= intPartHi;
	intDiv = intDiv << 8;
	intDiv |= intPartLo;
	intDiv = intDiv >> 4;

	uint8_t frPrt[4];
	frPrt[0] = readByteBlock(&I2CD2, idtAdr, fracPartAdr);
	frPrt[1] = readByteBlock(&I2CD2, idtAdr, fracPartAdr + 1);
	frPrt[2] = readByteBlock(&I2CD2, idtAdr, fracPartAdr + 2);
	frPrt[3] = readByteBlock(&I2CD2, idtAdr, fracPartAdr + 3);

	uint32_t movedInt = 0;
	movedInt = (movedInt | frPrt[0]) << 8;
	movedInt = (movedInt | frPrt[1]) << 8;
	movedInt = (movedInt | frPrt[2]) << 8;
	movedInt = (movedInt | frPrt[3]);
	movedInt = movedInt >> 2;

	long int fractDiv = 0;
	uint8_t bt = 0;
	for (uint8_t i = 0; i <= 7; i++)
	{
		bt = (movedInt >> i * 4) & 0x0000000F;
		fractDiv += bt * innerPow16(i);
	}

	res = (float)fractDiv / (float)16777216;
	res += intDiv;

	return res;
}
