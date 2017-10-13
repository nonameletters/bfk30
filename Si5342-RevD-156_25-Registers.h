/*
 * Si5342 Rev D Configuration Register Export Header File
 *
 * This file represents a series of Silicon Labs Si5342 Rev D
 * register writes that can be performed to load a single configuration
 * on a device. It was created by a Silicon Labs ClockBuilder Pro
 * export tool.
 *
 * Part:		                                       Si5342 Rev D
 * Design ID:                                          156_25
 * Includes Pre/Post Download Control Register Writes: Yes
 * Created By:                                         ClockBuilder Pro v2.9 [2016-07-29]
 * Timestamp:                                          2016-08-30 16:48:13 GMT+03:00
 *
 * A complete design report corresponding to this export is included at the end
 * of this header file.
 *
 */

#ifndef SI5342_REVD_REG_CONFIG_HEADER
#define SI5342_REVD_REG_CONFIG_HEADER

#define SI5342_REVD_REG_CONFIG_NUM_REGS_				397

typedef struct
{
	unsigned int address; /* 16-bit register address */
	unsigned char value; /* 8-bit register data */

} si5342_revd_register_t;

si5342_revd_register_t const si5342_revd_registers[] =
{
	{ 0x0B24, 0xC0 },
	{ 0x0B25, 0x00 },
	{ 0x0004, 0x00 },	//* RO, read as 03
	{ 0x0006, 0x00 },	//* RO? =0
	{ 0x0007, 0x00 },	//* RO? =0 documentation:RW
	{ 0x0008, 0x00 },	//* RO? =0

	{ 0x000B, 0x68 },
	{ 0x0016, 0x02 },	//* not described in doc - writing FF wastes one bank
	{ 0x0017, 0x1C },	//* actual 1C - reserved bits (was: DC)
	{ 0x0018, 0xFF },
	{ 0x0019, 0xFF },
	{ 0x001A, 0xFF },
	{ 0x002B, 0x02 },
	{ 0x002C, 0x00 },
	{ 0x002D, 0x00 },
	{ 0x002E, 0x00 },
	{ 0x002F, 0x00 },
	{ 0x0030, 0x00 },
	{ 0x0031, 0x00 },
	{ 0x0032, 0x00 },
	{ 0x0033, 0x00 },
	{ 0x0034, 0x00 },
	{ 0x0035, 0x00 },
	{ 0x0036, 0x00 },
	{ 0x0037, 0x00 },
	{ 0x0038, 0x00 },
	{ 0x0039, 0x00 },
	{ 0x003A, 0x00 },
	{ 0x003B, 0x00 },
	{ 0x003C, 0x00 },
	{ 0x003D, 0x00 },
	{ 0x003F, 0x00 },
	{ 0x0040, 0x04 },
	{ 0x0041, 0x00 },
	{ 0x0042, 0x00 },
	{ 0x0043, 0x00 },
	{ 0x0044, 0x00 },
	{ 0x0045, 0x0C },
	{ 0x0046, 0x00 },
	{ 0x0047, 0x00 },
	{ 0x0048, 0x00 },
	{ 0x0049, 0x00 },
	{ 0x004A, 0x00 },
	{ 0x004B, 0x00 },
	{ 0x004C, 0x00 },
	{ 0x004D, 0x00 },
	{ 0x004E, 0x00 },
	{ 0x004F, 0x00 },
	{ 0x0051, 0x00 },
	{ 0x0052, 0x00 },
	{ 0x0053, 0x00 },
	{ 0x0054, 0x00 },
	{ 0x0055, 0x00 },
	{ 0x0056, 0x00 },
	{ 0x0057, 0x00 },
	{ 0x0058, 0x00 },
	{ 0x0059, 0x00 },
	{ 0x005A, 0x00 },
	{ 0x005B, 0x00 },
	{ 0x005C, 0x00 },
	{ 0x005D, 0x00 },
	{ 0x005E, 0x00 },
	{ 0x005F, 0x00 },
	{ 0x0060, 0x00 },
	{ 0x0061, 0x00 },
	{ 0x0062, 0x00 },
	{ 0x0063, 0x00 },
	{ 0x0064, 0x00 },
	{ 0x0065, 0x00 },
	{ 0x0066, 0x00 },
	{ 0x0067, 0x00 },
	{ 0x0068, 0x00 },
	{ 0x0069, 0x00 },
	{ 0x0092, 0x00 },
	{ 0x0093, 0x00 },
	{ 0x0095, 0x00 },
	{ 0x0096, 0x00 },
	{ 0x0098, 0x00 },
	{ 0x009A, 0x00 },
	{ 0x009B, 0x00 },
	{ 0x009D, 0x00 },
	{ 0x009E, 0x00 },
	{ 0x00A0, 0x00 },
	{ 0x00A2, 0x00 },
	{ 0x00A9, 0x00 },
	{ 0x00AA, 0x00 },
	{ 0x00AB, 0x00 },
	{ 0x00AC, 0x00 },
	{ 0x80E5, 0x01 },	//*? only E5.5 defined in comments
	{ 0x00EA, 0x00 },
	{ 0x00EB, 0x00 },
	{ 0x00EC, 0x00 },
	{ 0x00ED, 0x00 },
	{ 0x0102, 0x01 },
	{ 0x0112, 0x06 },
	{ 0x0113, 0x09 },
	{ 0x0114, 0x30 },	// 3B -> 30
	{ 0x0115, 0x00 },	// 28 -> 00
	{ 0x0117, 0x06 },
	{ 0x0118, 0x09 },
	{ 0x0119, 0x30 },	// 3B -> 30
	{ 0x011A, 0x00 },	// 28 -> 00
	{ 0x013F, 0x00 },
	{ 0x0140, 0x00 },
	{ 0x0141, 0x40 },
	{ 0x0142, 0xFF },
	{ 0x0206, 0x00 },
	{ 0x0208, 0x00 },
	{ 0x0209, 0x00 },
	{ 0x020A, 0x00 },
	{ 0x020B, 0x00 },
	{ 0x020C, 0x00 },
	{ 0x020D, 0x00 },
	{ 0x020E, 0x00 },
	{ 0x020F, 0x00 },
	{ 0x0210, 0x00 },
	{ 0x0211, 0x00 },
	{ 0x0212, 0x00 },
	{ 0x0213, 0x00 },
	{ 0x0214, 0x00 },
	{ 0x0215, 0x00 },
	{ 0x0216, 0x00 },
	{ 0x0217, 0x00 },
	{ 0x0218, 0x00 },
	{ 0x0219, 0x00 },
	{ 0x021A, 0x00 },
	{ 0x021B, 0x00 },
	{ 0x021C, 0x00 },
	{ 0x021D, 0x00 },
	{ 0x021E, 0x00 },
	{ 0x021F, 0x00 },
	{ 0x0220, 0x00 },
	{ 0x0221, 0x00 },
	{ 0x0222, 0x00 },
	{ 0x0223, 0x00 },
	{ 0x0224, 0x00 },
	{ 0x0225, 0x00 },
	{ 0x0226, 0x00 },
	{ 0x0227, 0x00 },
	{ 0x0228, 0x00 },
	{ 0x0229, 0x00 },
	{ 0x022A, 0x00 },
	{ 0x022B, 0x00 },
	{ 0x022C, 0x00 },
	{ 0x022D, 0x00 },
	{ 0x022E, 0x00 },
	{ 0x022F, 0x00 },
	{ 0x0231, 0x0B },
	{ 0x0232, 0x0B },
	{ 0x0233, 0x0B },
	{ 0x0234, 0x0B },
	{ 0x0235, 0x00 },
	{ 0x0236, 0x00 },
	{ 0x0237, 0x00 },
	{ 0x0238, 0xF6 },
	{ 0x0239, 0xD1 },
	{ 0x023A, 0x00 },
	{ 0x023B, 0x00 },
	{ 0x023C, 0x00 },
	{ 0x023D, 0x00 },
	{ 0x023E, 0xC0 },
	{ 0x0250, 0x00 },
	{ 0x0251, 0x00 },
	{ 0x0252, 0x00 },
	{ 0x0253, 0x00 },
	{ 0x0254, 0x00 },
	{ 0x0255, 0x00 },
	{ 0x026B, 0x31 },
	{ 0x026C, 0x35 },
	{ 0x026D, 0x36 },
	{ 0x026E, 0x2E },	// .
	{ 0x026F, 0x32 },
	{ 0x0270, 0x35 },
	{ 0x0271, 0x00 },
	{ 0x0272, 0x03 },	// revision
	{ 0x8294, 0x80 },	//* read as 0
	{ 0x0296, 0x00 },
	{ 0x0297, 0x00 },
	{ 0x0299, 0x00 },
	{ 0x029D, 0x00 },
	{ 0x029E, 0x00 },
	{ 0x029F, 0x00 },
	{ 0x02A9, 0x00 },
	{ 0x02AA, 0x00 },
	{ 0x02AB, 0x00 },
	{ 0x82B7, 0xFF },	//* read as 0, only [3:2] defined
	{ 0x0302, 0x00 },
	{ 0x0303, 0x00 },
	{ 0x0304, 0x00 },
	{ 0x0305, 0x80 },
	{ 0x0306, 0x15 },
	{ 0x0307, 0x00 },
	{ 0x0308, 0x00 },
	{ 0x0309, 0x00 },
	{ 0x030A, 0x00 },
	{ 0x030B, 0x80 },
	{ 0x030C, 0x00 },
	{ 0x030D, 0x00 },
	{ 0x030E, 0x00 },
	{ 0x030F, 0x00 },
	{ 0x0310, 0x00 },
	{ 0x0311, 0x00 },
	{ 0x0312, 0x00 },
	{ 0x0313, 0x00 },
	{ 0x0314, 0x00 },
	{ 0x0315, 0x00 },
	{ 0x0316, 0x00 },
	{ 0x0317, 0x00 },
	{ 0x0338, 0x00 },
	{ 0x0339, 0x1F },
	{ 0x033B, 0x00 },
	{ 0x033C, 0x00 },
	{ 0x033D, 0x00 },
	{ 0x033E, 0x00 },
	{ 0x033F, 0x00 },
	{ 0x0340, 0x00 },
	{ 0x0341, 0x00 },
	{ 0x0342, 0x00 },
	{ 0x0343, 0x00 },
	{ 0x0344, 0x00 },
	{ 0x0345, 0x00 },
	{ 0x0346, 0x00 },
	{ 0x0359, 0x00 },
	{ 0x035A, 0x00 },
	{ 0x035B, 0x00 },
	{ 0x035C, 0x00 },
	{ 0x0487, 0x00 },
	{ 0x0502, 0x01 },
	{ 0x0508, 0x00 },
	{ 0x0509, 0x00 },
	{ 0x050A, 0x00 },
	{ 0x050B, 0x00 },
	{ 0x050C, 0x00 },
	{ 0x050D, 0x00 },
	{ 0x050E, 0x00 },
	{ 0x050F, 0x00 },
	{ 0x0510, 0x00 },
	{ 0x0511, 0x00 },
	{ 0x0512, 0x00 },
	{ 0x0513, 0x00 },
	{ 0x0515, 0x00 },
	{ 0x0516, 0x00 },
	{ 0x0517, 0x00 },
	{ 0x0518, 0x00 },
	{ 0x0519, 0x00 },
	{ 0x051A, 0x00 },
	{ 0x051B, 0x00 },
	{ 0x051C, 0x00 },
	{ 0x051D, 0x00 },
	{ 0x051E, 0x00 },
	{ 0x051F, 0x00 },
	{ 0x0521, 0x2B },
	{ 0x052A, 0x00 },
	{ 0x052B, 0x01 },
	{ 0x052C, 0x0F },
	{ 0x052D, 0x03 },
	{ 0x052E, 0x00 },
	{ 0x052F, 0x00 },
	{ 0x0531, 0x00 },
	{ 0x0532, 0x00 },
	{ 0x0533, 0x04 },
	{ 0x0534, 0x00 },
	{ 0x0535, 0x01 },
	{ 0x0536, 0x06 },
	{ 0x0537, 0x00 },
	{ 0x0538, 0x00 },
	{ 0x0539, 0x00 },
	{ 0x053D, 0x0A },
	{ 0x053E, 0x06 },
	{ 0x8589, 0x0C },	//* read as 0
	{ 0x058A, 0x00 },
	{ 0x859B, 0x18 },	//* read as 0, only [7:6] defined
	{ 0x059D, 0x00 },
	{ 0x059E, 0x00 },
	{ 0x059F, 0x00 },
	{ 0x05A0, 0x00 },
	{ 0x05A1, 0x00 },
	{ 0x05A2, 0x00 },
	{ 0x05A6, 0x00 },
	{ 0x0802, 0x35 },
	{ 0x0803, 0x05 },
	{ 0x0804, 0x01 },
	{ 0x0805, 0x00 },
	{ 0x0806, 0x00 },
	{ 0x0807, 0x00 },
	{ 0x0808, 0x00 },
	{ 0x0809, 0x00 },
	{ 0x080A, 0x00 },
	{ 0x080B, 0x00 },
	{ 0x080C, 0x00 },
	{ 0x080D, 0x00 },
	{ 0x080E, 0x00 },
	{ 0x080F, 0x00 },
	{ 0x0810, 0x00 },
	{ 0x0811, 0x00 },
	{ 0x0812, 0x00 },
	{ 0x0813, 0x00 },
	{ 0x0814, 0x00 },
	{ 0x0815, 0x00 },
	{ 0x0816, 0x00 },
	{ 0x0817, 0x00 },
	{ 0x0818, 0x00 },
	{ 0x0819, 0x00 },
	{ 0x081A, 0x00 },
	{ 0x081B, 0x00 },
	{ 0x081C, 0x00 },
	{ 0x081D, 0x00 },
	{ 0x081E, 0x00 },
	{ 0x081F, 0x00 },
	{ 0x0820, 0x00 },
	{ 0x0821, 0x00 },
	{ 0x0822, 0x00 },
	{ 0x0823, 0x00 },
	{ 0x0824, 0x00 },
	{ 0x0825, 0x00 },
	{ 0x0826, 0x00 },
	{ 0x0827, 0x00 },
	{ 0x0828, 0x00 },
	{ 0x0829, 0x00 },
	{ 0x082A, 0x00 },
	{ 0x082B, 0x00 },
	{ 0x082C, 0x00 },
	{ 0x082D, 0x00 },
	{ 0x082E, 0x00 },
	{ 0x082F, 0x00 },
	{ 0x0830, 0x00 },
	{ 0x0831, 0x00 },
	{ 0x0832, 0x00 },
	{ 0x0833, 0x00 },
	{ 0x0834, 0x00 },
	{ 0x0835, 0x00 },
	{ 0x0836, 0x00 },
	{ 0x0837, 0x00 },
	{ 0x0838, 0x00 },
	{ 0x0839, 0x00 },
	{ 0x083A, 0x00 },
	{ 0x083B, 0x00 },
	{ 0x083C, 0x00 },
	{ 0x083D, 0x00 },
	{ 0x083E, 0x00 },
	{ 0x083F, 0x00 },
	{ 0x0840, 0x00 },
	{ 0x0841, 0x00 },
	{ 0x0842, 0x00 },
	{ 0x0843, 0x00 },
	{ 0x0844, 0x00 },
	{ 0x0845, 0x00 },
	{ 0x0846, 0x00 },
	{ 0x0847, 0x00 },
	{ 0x0848, 0x00 },
	{ 0x0849, 0x00 },
	{ 0x084A, 0x00 },
	{ 0x084B, 0x00 },
	{ 0x084C, 0x00 },
	{ 0x084D, 0x00 },
	{ 0x084E, 0x00 },
	{ 0x084F, 0x00 },
	{ 0x0850, 0x00 },
	{ 0x0851, 0x00 },
	{ 0x0852, 0x00 },
	{ 0x0853, 0x00 },
	{ 0x0854, 0x00 },
	{ 0x0855, 0x00 },
	{ 0x0856, 0x00 },
	{ 0x0857, 0x00 },
	{ 0x0858, 0x00 },
	{ 0x0859, 0x00 },
	{ 0x085A, 0x00 },
	{ 0x085B, 0x00 },
	{ 0x085C, 0x00 },
	{ 0x085D, 0x00 },
	{ 0x085E, 0x00 },
	{ 0x085F, 0x00 },
	{ 0x0860, 0x00 },
	{ 0x0861, 0x00 },
	{ 0x090E, 0x02 },
	{ 0x0943, 0x01 },	// 0=1.8V 1=3.3V
	{ 0x0949, 0x00 },
	{ 0x094A, 0x00 },
	{ 0x094E, 0x49 },
	{ 0x094F, 0x02 },
	{ 0x095E, 0x00 },
	{ 0x0A02, 0x00 },
	{ 0x0A03, 0x01 },
	{ 0x0A04, 0x01 },
	{ 0x0A05, 0x01 },
	{ 0x0B44, 0x0F },
	{ 0x0B46, 0x00 },
	{ 0x0B47, 0x00 },
	{ 0x0B48, 0x0F },
	{ 0x0B4A, 0x02 },
// postabmle
	{ 0x0B57, 0x0E },	// VCO_RESET_CALCODE, self-clear
	{ 0x0B58, 0x01 },	//  B57,B58
	{ 0x0514, 0x01 },	// update pll params, self-clear
	{ 0x001C, 0x01 },	// soft reset, self-clear
	{ 0x0B24, 0xC3 },
	{ 0x0B25, 0x02 },

};

#define SI5342_REVD_REG_CONFIG_NUM_REGS (sizeof(si5342_revd_registers)/sizeof(si5342_revd_registers[0]))

/*
 * Design Report
 *
 * Overview
 * ========
 * Part:         Si5342 Rev D
 * Project File: C:\Users\M.Gryaznov\Documents\Si5342-RevD-156_25-Project.slabtimeproj
 * Design ID:    156.25
 * Created By:   ClockBuilder Pro v2.9 [2016-07-29]
 * Timestamp:    2016-08-30 16:48:13 GMT+03:00
 *
 * Design Rule Check
 * =================
 * Errors:
 * - No errors
 *
 * Warnings:
 * - No warnings
 *
 * Device Grade
 * ============
 * Maximum Output Frequency: 156.25 MHz
 * Frequency Synthesis Mode: Integer
 * Frequency Plan Grade:     D
 * Minimum Base OPN:         Si5342D*
 *
 * Base       Output Clock         Supported Frequency Synthesis Modes
 * OPN Grade  Frequency Range      (Typical Jitter)
 * ---------  -------------------  --------------------------------------------
 * Si5342A    100 Hz to 720 MHz    Integer (< 100 fs) and fractional (< 150 fs)
 * Si5342B    100 Hz to 350 MHz    "
 * Si5342C    100 Hz to 720 MHz    Integer only (< 100 fs)
 * Si5342D*   100 Hz to 350 MHz    "
 *
 * * Based on your calculated frequency plan, a Si5342D grade device is
 * sufficient for your design. For more in-system configuration flexibility
 * (higher frequencies and/or to enable fractional synthesis), consider
 * selecting device grade Si5342A when specifying an ordering part number (OPN)
 * for your application. See the datasheet Ordering Guide for more information.
 *
 * Design
 * ======
 * Host Interface:
 *    I/O Power Supply: VDD (Core)
 *    SPI Mode: 4-Wire
 *    I2C Address Range: 0x68 to 0x6B (selected via A0/A1 pins)
 *
 * XA/XB:
 *    48 MHz (XTAL - Crystal)
 *
 * Inputs:
 *     IN0: Unused
 *     IN1: Unused
 *     IN2: Unused
 *     IN3: Unused
 *
 * Outputs:
 *    OUT0: 156.25 MHz [ 156 + 1/4 MHz ] (Free Run)
 *          Enabled, LVDS 3.3 V
 *    OUT1: 156.25 MHz [ 156 + 1/4 MHz ] (Free Run)
 *          Enabled, LVDS 3.3 V
 *
 * Frequency Plan
 * ==============
 * Fvco = 13.4375 GHz [ 13 + 7/16 GHz ]
 * Fpfd = N/A (DSPLL free run only mode enabled)
 * Fms0 = 312.5 MHz [ 312 + 1/2 MHz ]
 *
 * P dividers:
 *    P0  = Unused
 *    P1  = Unused
 *    P2  = Unused
 *    P3  = Unused
 *    Pxaxb = 1
 *
 * MXAXB = 279.9479166666666666... [ 279 + 91/96 ]
 * M = N/A (DSPLL free run only mode enabled)
 * N dividers:
 *    N0:
 *       Value: 43
 *       Skew:  0.000 s
 *       OUT0: 156.25 MHz [ 156 + 1/4 MHz ]
 *       OUT1: 156.25 MHz [ 156 + 1/4 MHz ]
 *    N1:
 *       Unused
 *
 * R dividers:
 *    R0 = 2
 *    R1 = 2
 *
 * Nominal Bandwidth:
 *   N/A (DSPLL free run only mode enabled)
 * Fastlock Bandwidth:
 *   N/A (DSPLL free run only mode enabled)
 * Holdover Bandwidth:
 *   N/A (DSPLL free run only mode enabled)
 *
 * Dividers listed above show effective values. These values are translated to register settings by ClockBuilder Pro. For the actual register values, see below. Refer to the Family Reference Manual for information on registers related to frequency plan.
 *
 * Digitally Controlled Oscillator (DCO)
 * =====================================
 * Mode: Register Direct Write
 *
 * N0: DCO Disabled
 *
 * N1: DCO Disabled
 *
 * Estimated Power & Junction Temperature
 * ======================================
 * Assumptions:
 *
 * Revision: D
 * VDD:      1.8 V
 * Ta:       70 °C
 * Airflow:  None
 *
 * Total Power: 610 mW, On Chip Power: 598 mW, Tj: 83 °C
 *
 *          Frequency  Format   Voltage   Current     Power
 *         ----------  ------  --------  --------  --------
 * VDD                            1.8 V   79.4 mA    143 mW
 * VDDA                           3.3 V  117.4 mA    387 mW
 * VDDO0   156.25 MHz  LVDS       3.3 V   15.9 mA     40 mW
 * VDDO1   156.25 MHz  LVDS       3.3 V   15.9 mA     40 mW
 *                                       --------  --------
 *                                Total  228.6 mA    610 mW
 *
 * Note:
 *
 * -Total power includes on- and off-chip power. This is a typical value and estimate only.
 * -Use an EVB for a more exact power measurement
 * -On-chip power excludes power dissipated in external terminations.
 * -Tj is junction temperature. When using a crystal (XTAL) reference, Tj must be less than 125 °C (on Si5342 Revision D) for device to comply with datasheet specifications.
 *
 * Settings
 * ========
 *
 * Location      Setting Name           Decimal Value  Hex Value
 * ------------  ---------------------  -------------  ----------------
 * 0x0004[0:7]   GRADE                  0              0x00
 * 0x0006[0:23]  TOOL_VERSION           0              0x000000
 * 0x000B[0:6]   I2C_ADDR               104            0x68
 * 0x0016[1]     LOL_ON_HOLD            1              0x1
 * 0x0017[0]     SYSINCAL_INTR_MSK      0              0x0
 * 0x0017[1]     LOSXAXB_INTR_MSK       0              0x0
 * 0x0017[5]     SMB_TMOUT_INTR_MSK     0              0x0
 * 0x0018[0:3]   LOS_INTR_MSK           15             0xF
 * 0x0018[4:7]   OOF_INTR_MSK           15             0xF
 * 0x0019[1]     LOL_INTR_MSK           1              0x1
 * 0x0019[5]     HOLD_INTR_MSK          1              0x1
 * 0x001A[5]     CAL_PLL_INTR_MSK       1              0x1
 * 0x002B[3]     SPI_3WIRE              0              0x0
 * 0x002B[5]     AUTO_NDIV_UPDATE       0              0x0
 * 0x002C[0:3]   LOS_EN                 0              0x0
 * 0x002C[4]     LOSXAXB_DIS            0              0x0
 * 0x002D[0:1]   LOS0_VAL_TIME          0              0x0
 * 0x002D[2:3]   LOS1_VAL_TIME          0              0x0
 * 0x002D[4:5]   LOS2_VAL_TIME          0              0x0
 * 0x002D[6:7]   LOS3_VAL_TIME          0              0x0
 * 0x002E[0:15]  LOS0_TRG_THR           0              0x0000
 * 0x0030[0:15]  LOS1_TRG_THR           0              0x0000
 * 0x0032[0:15]  LOS2_TRG_THR           0              0x0000
 * 0x0034[0:15]  LOS3_TRG_THR           0              0x0000
 * 0x0036[0:15]  LOS0_CLR_THR           0              0x0000
 * 0x0038[0:15]  LOS1_CLR_THR           0              0x0000
 * 0x003A[0:15]  LOS2_CLR_THR           0              0x0000
 * 0x003C[0:15]  LOS3_CLR_THR           0              0x0000
 * 0x003F[0:3]   OOF_EN                 0              0x0
 * 0x003F[4:7]   FAST_OOF_EN            0              0x0
 * 0x0040[0:2]   OOF_REF_SEL            4              0x4
 * 0x0041[0:4]   OOF0_DIV_SEL           0              0x00
 * 0x0042[0:4]   OOF1_DIV_SEL           0              0x00
 * 0x0043[0:4]   OOF2_DIV_SEL           0              0x00
 * 0x0044[0:4]   OOF3_DIV_SEL           0              0x00
 * 0x0045[0:4]   OOFXO_DIV_SEL          12             0x0C
 * 0x0046[0:7]   OOF0_SET_THR           0              0x00
 * 0x0047[0:7]   OOF1_SET_THR           0              0x00
 * 0x0048[0:7]   OOF2_SET_THR           0              0x00
 * 0x0049[0:7]   OOF3_SET_THR           0              0x00
 * 0x004A[0:7]   OOF0_CLR_THR           0              0x00
 * 0x004B[0:7]   OOF1_CLR_THR           0              0x00
 * 0x004C[0:7]   OOF2_CLR_THR           0              0x00
 * 0x004D[0:7]   OOF3_CLR_THR           0              0x00
 * 0x004E[0:2]   OOF0_DETWIN_SEL        0              0x0
 * 0x004E[4:6]   OOF1_DETWIN_SEL        0              0x0
 * 0x004F[0:2]   OOF2_DETWIN_SEL        0              0x0
 * 0x004F[4:6]   OOF3_DETWIN_SEL        0              0x0
 * 0x0051[0:3]   FAST_OOF0_SET_THR      0              0x0
 * 0x0052[0:3]   FAST_OOF1_SET_THR      0              0x0
 * 0x0053[0:3]   FAST_OOF2_SET_THR      0              0x0
 * 0x0054[0:3]   FAST_OOF3_SET_THR      0              0x0
 * 0x0055[0:3]   FAST_OOF0_CLR_THR      0              0x0
 * 0x0056[0:3]   FAST_OOF1_CLR_THR      0              0x0
 * 0x0057[0:3]   FAST_OOF2_CLR_THR      0              0x0
 * 0x0058[0:3]   FAST_OOF3_CLR_THR      0              0x0
 * 0x0059[0:1]   FAST_OOF0_DETWIN_SEL   0              0x0
 * 0x0059[2:3]   FAST_OOF1_DETWIN_SEL   0              0x0
 * 0x0059[4:5]   FAST_OOF2_DETWIN_SEL   0              0x0
 * 0x0059[6:7]   FAST_OOF3_DETWIN_SEL   0              0x0
 * 0x005A[0:25]  OOF0_RATIO_REF         0              0x0000000
 * 0x005E[0:25]  OOF1_RATIO_REF         0              0x0000000
 * 0x0062[0:25]  OOF2_RATIO_REF         0              0x0000000
 * 0x0066[0:25]  OOF3_RATIO_REF         0              0x0000000
 * 0x0092[1]     LOL_FST_EN             0              0x0
 * 0x0093[4:7]   LOL_FST_DETWIN_SEL     0              0x0
 * 0x0095[2:3]   LOL_FST_VALWIN_SEL     0              0x0
 * 0x0096[4:7]   LOL_FST_SET_THR_SEL    0              0x0
 * 0x0098[4:7]   LOL_FST_CLR_THR_SEL    0              0x0
 * 0x009A[1]     LOL_SLOW_EN_PLL        0              0x0
 * 0x009B[4:7]   LOL_SLW_DETWIN_SEL     0              0x0
 * 0x009D[2:3]   LOL_SLW_VALWIN_SEL     0              0x0
 * 0x009E[4:7]   LOL_SLW_SET_THR        0              0x0
 * 0x00A0[4:7]   LOL_SLW_CLR_THR        0              0x0
 * 0x00A2[1]     LOL_TIMER_EN           0              0x0
 * 0x00A9[0:28]  LOL_CLR_DELAY_DIV256   0              0x00000000
 * 0x00E5[5]     FASTLOCK_EXTEND_EN     0              0x0
 * 0x00EA[0:28]  FASTLOCK_EXTEND        0              0x00000000
 * 0x0102[0]     OUTALL_DISABLE_LOW     1              0x1
 * 0x0112[0]     OUT0_PDN               0              0x0
 * 0x0112[1]     OUT0_OE                1              0x1
 * 0x0112[2]     OUT0_RDIV_FORCE2       1              0x1
 * 0x0113[0:2]   OUT0_FORMAT            1              0x1
 * 0x0113[3]     OUT0_SYNC_EN           1              0x1
 * 0x0113[4:5]   OUT0_DIS_STATE         0              0x0
 * 0x0113[6:7]   OUT0_CMOS_DRV          0              0x0
 * 0x0114[0:3]   OUT0_CM                0              0x0
 * 0x0114[4:6]   OUT0_AMPL              3              0x3
 * 0x0115[0:2]   OUT0_MUX_SEL           0              0x0
 * 0x0115[4:5]   OUT0_VDD_SEL           0              0x0
 * 0x0115[3]     OUT0_VDD_SEL_EN        0              0x0
 * 0x0115[6:7]   OUT0_INV               0              0x0
 * 0x0117[0]     OUT1_PDN               0              0x0
 * 0x0117[1]     OUT1_OE                1              0x1
 * 0x0117[2]     OUT1_RDIV_FORCE2       1              0x1
 * 0x0118[0:2]   OUT1_FORMAT            1              0x1
 * 0x0118[3]     OUT1_SYNC_EN           1              0x1
 * 0x0118[4:5]   OUT1_DIS_STATE         0              0x0
 * 0x0118[6:7]   OUT1_CMOS_DRV          0              0x0
 * 0x0119[0:3]   OUT1_CM                0              0x0
 * 0x0119[4:6]   OUT1_AMPL              3              0x3
 * 0x011A[0:2]   OUT1_MUX_SEL           0              0x0
 * 0x011A[4:5]   OUT1_VDD_SEL           0              0x0
 * 0x011A[3]     OUT1_VDD_SEL_EN        0              0x0
 * 0x011A[6:7]   OUT1_INV               0              0x0
 * 0x013F[0:11]  OUTX_ALWAYS_ON         0              0x000
 * 0x0141[1]     OUT_DIS_MSK            0              0x0
 * 0x0141[5]     OUT_DIS_LOL_MSK        0              0x0
 * 0x0141[6]     OUT_DIS_LOSXAXB_MSK    1              0x1
 * 0x0141[7]     OUT_DIS_MSK_LOS_PFD    0              0x0
 * 0x0142[1]     OUT_DIS_MSK_LOL        1              0x1
 * 0x0142[5]     OUT_DIS_MSK_HOLD       1              0x1
 * 0x0206[0:1]   PXAXB                  0              0x0
 * 0x0208[0:47]  P0_NUM                 0              0x000000000000
 * 0x020E[0:31]  P0_DEN                 0              0x00000000
 * 0x0212[0:47]  P1_NUM                 0              0x000000000000
 * 0x0218[0:31]  P1_DEN                 0              0x00000000
 * 0x021C[0:47]  P2_NUM                 0              0x000000000000
 * 0x0222[0:31]  P2_DEN                 0              0x00000000
 * 0x0226[0:47]  P3_NUM                 0              0x000000000000
 * 0x022C[0:31]  P3_DEN                 0              0x00000000
 * 0x0231[0:3]   P0_FRACN_MODE          11             0xB
 * 0x0231[4]     P0_FRACN_EN            0              0x0
 * 0x0232[0:3]   P1_FRACN_MODE          11             0xB
 * 0x0232[4]     P1_FRACN_EN            0              0x0
 * 0x0233[0:3]   P2_FRACN_MODE          11             0xB
 * 0x0233[4]     P2_FRACN_EN            0              0x0
 * 0x0234[0:3]   P3_FRACN_MODE          11             0xB
 * 0x0234[4]     P3_FRACN_EN            0              0x0
 * 0x0235[0:43]  MXAXB_NUM              901775360000   0x0D1F6000000
 * 0x023B[0:31]  MXAXB_DEN              3221225472     0xC0000000
 * 0x0250[0:23]  R0_REG                 0              0x000000
 * 0x0253[0:23]  R1_REG                 0              0x000000
 * 0x026B[0:7]   DESIGN_ID0             49             0x31
 * 0x026C[0:7]   DESIGN_ID1             53             0x35
 * 0x026D[0:7]   DESIGN_ID2             54             0x36
 * 0x026E[0:7]   DESIGN_ID3             46             0x2E
 * 0x026F[0:7]   DESIGN_ID4             50             0x32
 * 0x0270[0:7]   DESIGN_ID5             53             0x35
 * 0x0271[0:7]   DESIGN_ID6             0              0x00
 * 0x0272[0:7]   DESIGN_ID7             2              0x02		// revision
 * 0x0294[4:7]   FASTLOCK_EXTEND_SCL    8              0x8
 * 0x0296[1]     LOL_SLW_VALWIN_SELX    0              0x0
 * 0x0297[1]     FASTLOCK_DLY_ONSW_EN   0              0x0
 * 0x0299[1]     FASTLOCK_DLY_ONLOL_EN  0              0x0
 * 0x029D[0:19]  FASTLOCK_DLY_ONLOL     0              0x00000
 * 0x02A9[0:19]  FASTLOCK_DLY_ONSW      0              0x00000
 * 0x02B7[2:3]   LOL_NOSIG_TIME         3              0x3
 * 0x0302[0:43]  N0_NUM                 92341796864    0x01580000000
 * 0x0308[0:31]  N0_DEN                 2147483648     0x80000000
 * 0x030C[0]     N0_UPDATE              0              0x0
 * 0x030D[0:43]  N1_NUM                 0              0x00000000000
 * 0x0313[0:31]  N1_DEN                 0              0x00000000
 * 0x0317[0]     N1_UPDATE              0              0x0
 * 0x0338[1]     N_UPDATE               0              0x0
 * 0x0339[0:4]   N_FSTEP_MSK            31             0x1F
 * 0x033B[0:43]  N0_FSTEPW              0              0x00000000000
 * 0x0341[0:43]  N1_FSTEPW              0              0x00000000000
 * 0x0359[0:15]  N0_DELAY               0              0x0000
 * 0x035B[0:15]  N1_DELAY               0              0x0000
 * 0x0487[0]     ZDM_EN                 0              0x0
 * 0x0487[1:2]   ZDM_IN_SEL             0              0x0
 * 0x0487[4]     ZDM_AUTOSW_EN          0              0x0
 * 0x0502[4]     ADD_DIV256             0              0x0
 * 0x0508[0:5]   BW0_PLL                0              0x00
 * 0x0509[0:5]   BW1_PLL                0              0x00
 * 0x050A[0:5]   BW2_PLL                0              0x00
 * 0x050B[0:5]   BW3_PLL                0              0x00
 * 0x050C[0:5]   BW4_PLL                0              0x00
 * 0x050D[0:5]   BW5_PLL                0              0x00
 * 0x050E[0:5]   FASTLOCK_BW0_PLL       0              0x00
 * 0x050F[0:5]   FASTLOCK_BW1_PLL       0              0x00
 * 0x0510[0:5]   FASTLOCK_BW2_PLL       0              0x00
 * 0x0511[0:5]   FASTLOCK_BW3_PLL       0              0x00
 * 0x0512[0:5]   FASTLOCK_BW4_PLL       0              0x00
 * 0x0513[0:5]   FASTLOCK_BW5_PLL       0              0x00
 * 0x0515[0:55]  M_NUM                  0              0x00000000000000
 * 0x051C[0:31]  M_DEN                  0              0x00000000
 * 0x0521[0:3]   M_FRAC_MODE            11             0xB
 * 0x0521[4]     M_FRAC_EN              0              0x0
 * 0x0521[5]     PLL_OUT_RATE_SEL       1              0x1
 * 0x052A[0]     IN_SEL_REGCTRL         0              0x0
 * 0x052A[1:3]   IN_SEL                 0              0x0
 * 0x052B[0]     FASTLOCK_AUTO_EN       1              0x1
 * 0x052B[1]     FASTLOCK_MAN           0              0x0
 * 0x052C[0]     HOLD_EN                1              0x1
 * 0x052C[3]     HOLD_RAMP_BYP          1              0x1
 * 0x052C[4]     HOLDEXIT_BW_SEL1       0              0x0
 * 0x052C[5:7]   RAMP_STEP_INTERVAL     0              0x0
 * 0x052D[1]     HOLD_RAMPBYP_NOHIST    1              0x1
 * 0x052E[0:4]   HOLD_HIST_LEN          0              0x00
 * 0x052F[0:4]   HOLD_HIST_DELAY        0              0x00
 * 0x0531[0:4]   HOLD_REF_COUNT_FRC     0              0x00
 * 0x0532[0:23]  HOLD_15M_CYC_COUNT     1024           0x000400
 * 0x0535[0]     FORCE_HOLD             1              0x1
 * 0x0536[0:1]   CLK_SWITCH_MODE        2              0x2
 * 0x0536[2]     HSW_EN                 1              0x1
 * 0x0537[0:3]   IN_LOS_MSK             0              0x0
 * 0x0537[4:7]   IN_OOF_MSK             0              0x0
 * 0x0538[0:2]   IN0_PRIORITY           0              0x0
 * 0x0538[4:6]   IN1_PRIORITY           0              0x0
 * 0x0539[0:2]   IN2_PRIORITY           0              0x0
 * 0x0539[4:6]   IN3_PRIORITY           0              0x0
 * 0x053D[0:4]   HSW_COARSE_PM_LEN      10             0x0A
 * 0x053E[0:4]   HSW_COARSE_PM_DLY      6              0x06
 * 0x0589[0:12]  PFD_EN_DELAY           12             0x000C
 * 0x059B[6]     HOLDEXIT_BW_SEL0       0              0x0
 * 0x059B[7]     HOLDEXIT_STD_BO        0              0x0
 * 0x059D[0:5]   HOLDEXIT_BW0           0              0x00
 * 0x059E[0:5]   HOLDEXIT_BW1           0              0x00
 * 0x059F[0:5]   HOLDEXIT_BW2           0              0x00
 * 0x05A0[0:5]   HOLDEXIT_BW3           0              0x00
 * 0x05A1[0:5]   HOLDEXIT_BW4           0              0x00
 * 0x05A2[0:5]   HOLDEXIT_BW5           0              0x00
 * 0x05A6[0:2]   RAMP_STEP_SIZE         0              0x0
 * 0x05A6[3]     RAMP_SWITCH_EN         0              0x0
 * 0x0802[0:15]  FIXREGSA0              1333           0x0535
 * 0x0804[0:7]   FIXREGSD0              1              0x01
 * 0x0805[0:15]  FIXREGSA1              0              0x0000
 * 0x0807[0:7]   FIXREGSD1              0              0x00
 * 0x0808[0:15]  FIXREGSA2              0              0x0000
 * 0x080A[0:7]   FIXREGSD2              0              0x00
 * 0x080B[0:15]  FIXREGSA3              0              0x0000
 * 0x080D[0:7]   FIXREGSD3              0              0x00
 * 0x080E[0:15]  FIXREGSA4              0              0x0000
 * 0x0810[0:7]   FIXREGSD4              0              0x00
 * 0x0811[0:15]  FIXREGSA5              0              0x0000
 * 0x0813[0:7]   FIXREGSD5              0              0x00
 * 0x0814[0:15]  FIXREGSA6              0              0x0000
 * 0x0816[0:7]   FIXREGSD6              0              0x00
 * 0x0817[0:15]  FIXREGSA7              0              0x0000
 * 0x0819[0:7]   FIXREGSD7              0              0x00
 * 0x081A[0:15]  FIXREGSA8              0              0x0000
 * 0x081C[0:7]   FIXREGSD8              0              0x00
 * 0x081D[0:15]  FIXREGSA9              0              0x0000
 * 0x081F[0:7]   FIXREGSD9              0              0x00
 * 0x0820[0:15]  FIXREGSA10             0              0x0000
 * 0x0822[0:7]   FIXREGSD10             0              0x00
 * 0x0823[0:15]  FIXREGSA11             0              0x0000
 * 0x0825[0:7]   FIXREGSD11             0              0x00
 * 0x0826[0:15]  FIXREGSA12             0              0x0000
 * 0x0828[0:7]   FIXREGSD12             0              0x00
 * 0x0829[0:15]  FIXREGSA13             0              0x0000
 * 0x082B[0:7]   FIXREGSD13             0              0x00
 * 0x082C[0:15]  FIXREGSA14             0              0x0000
 * 0x082E[0:7]   FIXREGSD14             0              0x00
 * 0x082F[0:15]  FIXREGSA15             0              0x0000
 * 0x0831[0:7]   FIXREGSD15             0              0x00
 * 0x0832[0:15]  FIXREGSA16             0              0x0000
 * 0x0834[0:7]   FIXREGSD16             0              0x00
 * 0x0835[0:15]  FIXREGSA17             0              0x0000
 * 0x0837[0:7]   FIXREGSD17             0              0x00
 * 0x0838[0:15]  FIXREGSA18             0              0x0000
 * 0x083A[0:7]   FIXREGSD18             0              0x00
 * 0x083B[0:15]  FIXREGSA19             0              0x0000
 * 0x083D[0:7]   FIXREGSD19             0              0x00
 * 0x083E[0:15]  FIXREGSA20             0              0x0000
 * 0x0840[0:7]   FIXREGSD20             0              0x00
 * 0x0841[0:15]  FIXREGSA21             0              0x0000
 * 0x0843[0:7]   FIXREGSD21             0              0x00
 * 0x0844[0:15]  FIXREGSA22             0              0x0000
 * 0x0846[0:7]   FIXREGSD22             0              0x00
 * 0x0847[0:15]  FIXREGSA23             0              0x0000
 * 0x0849[0:7]   FIXREGSD23             0              0x00
 * 0x084A[0:15]  FIXREGSA24             0              0x0000
 * 0x084C[0:7]   FIXREGSD24             0              0x00
 * 0x084D[0:15]  FIXREGSA25             0              0x0000
 * 0x084F[0:7]   FIXREGSD25             0              0x00
 * 0x0850[0:15]  FIXREGSA26             0              0x0000
 * 0x0852[0:7]   FIXREGSD26             0              0x00
 * 0x0853[0:15]  FIXREGSA27             0              0x0000
 * 0x0855[0:7]   FIXREGSD27             0              0x00
 * 0x0856[0:15]  FIXREGSA28             0              0x0000
 * 0x0858[0:7]   FIXREGSD28             0              0x00
 * 0x0859[0:15]  FIXREGSA29             0              0x0000
 * 0x085B[0:7]   FIXREGSD29             0              0x00
 * 0x085C[0:15]  FIXREGSA30             0              0x0000
 * 0x085E[0:7]   FIXREGSD30             0              0x00
 * 0x085F[0:15]  FIXREGSA31             0              0x0000
 * 0x0861[0:7]   FIXREGSD31             0              0x00
 * 0x090E[0]     XAXB_EXTCLK_EN         0              0x0
 * 0x0943[0]     IO_VDD_SEL             1              0x1		// changed, 0=1.8V, 1=3.3V
 * 0x0949[0:3]   IN_EN                  0              0x0
 * 0x0949[4:7]   IN_PULSED_CMOS_EN      0              0x0
 * 0x094A[0:3]   INX_TO_PFD_EN          0              0x0
 * 0x094E[0:11]  REFCLK_HYS_SEL         585            0x249
 * 0x095E[0]     MXAXB_INTEGER          0              0x0
 * 0x0A02[0:4]   N_ADD_0P5              0              0x00
 * 0x0A03[0:4]   N_CLK_TO_OUTX_EN       1              0x01
 * 0x0A04[0:4]   N_PIBYP                1              0x01
 * 0x0A05[0:4]   N_PDNB                 1              0x01
 * 0x0B44[0:3]   PDIV_FRACN_CLK_DIS     15             0xF
 * 0x0B44[5]     FRACN_CLK_DIS_PLL      0              0x0
 * 0x0B46[0:3]   LOS_CLK_DIS            0              0x0
 * 0x0B47[0:4]   OOF_CLK_DIS            0              0x00
 * 0x0B48[0:4]   OOF_DIV_CLK_DIS        15             0x0F
 * 0x0B4A[0:4]   N_CLK_DIS              2              0x02
 * 0x0B57[0:11]  VCO_RESET_CALCODE      270            0x10E
 *
 *
 */

#endif
