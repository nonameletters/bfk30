/*
 * spi_flash.h
 *
 *  Created on: May 6, 2015
 *      Author: k.kranke
 */

#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_

#include <hal.h>
#if CH_HAL_MAJOR >= 5
#include <hal_spi.h>
#else
#include <spi.h>
#endif

#define ERROR_PTR ((void*)-1)

/* Error codes */
#define ERROR_OOM	-100
#define TIMEOUT_ERROR	-101

/*
 * How many different contiguous runs of erase blocks with one size each do
 * we have for a given erase function?
 */
#define NUM_ERASEREGIONS 5

/*
 * How many different erase functions do we have per chip?
 * Atmel AT25FS010 has 6 different functions.
 */
#define NUM_ERASEFUNCTIONS 6

/*
 * The following enum defines possible write granularities of flash chips. These tend to reflect the properties
 * of the actual hardware not necesserily the write function(s) defined by the respective struct flashchip.
 * The latter might (and should) be more precisely specified, e.g. they might bail out early if their execution
 * would result in undefined chip contents.
 */
enum write_granularity {
	/* We assume 256 byte granularity by default. */
	write_gran_256bytes = 0,/* If less than 256 bytes are written, the unwritten bytes are undefined. */
	write_gran_1bit,	/* Each bit can be cleared individually. */
	write_gran_1byte,	/* A byte can be written once. Further writes to an already written byte cause
				 * its contents to be either undefined or to stay unchanged. */
	write_gran_264bytes,	/* If less than 264 bytes are written, the unwritten bytes are undefined. */
	write_gran_512bytes,	/* If less than 512 bytes are written, the unwritten bytes are undefined. */
	write_gran_528bytes,	/* If less than 528 bytes are written, the unwritten bytes are undefined. */
	write_gran_1024bytes,	/* If less than 1024 bytes are written, the unwritten bytes are undefined. */
	write_gran_1056bytes,	/* If less than 1056 bytes are written, the unwritten bytes are undefined. */
	write_gran_1byte_implicit_erase, /* EEPROMs and other chips with implicit erase and 1-byte writes. */
};

enum chipbustype {
	BUS_NONE	= 0,
	BUS_PARALLEL	= 1 << 0,
	BUS_LPC		= 1 << 1,
	BUS_FWH		= 1 << 2,
	BUS_SPI		= 1 << 3,
	BUS_PROG	= 1 << 4,
	BUS_NONSPI	= BUS_PARALLEL | BUS_LPC | BUS_FWH,
};

/* Feature bits used for SPI only */
#define FEATURE_WRSR_EWSR	(1 << 6)
#define FEATURE_WRSR_WREN	(1 << 7)
#define FEATURE_WRSR_EITHER	(FEATURE_WRSR_EWSR | FEATURE_WRSR_WREN)
#define FEATURE_OTP			(1 << 8)
#define FEATURE_QPI			(1 << 9)

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

enum test_state {
	OK = 0,
	NT = 1,	/* Not tested */
	BAD,	/* Known to not work */
	DEP,	/* Support depends on configuration (e.g. Intel flash descriptor) */
	NA,	/* Not applicable (e.g. write support on ROM chips) */
};

#define TEST_UNTESTED	{ .probe = NT, .read = NT, .erase = NT, .write = NT }

#define TEST_OK_PROBE	{ .probe = OK, .read = NT, .erase = NT, .write = NT }
#define TEST_OK_PR		{ .probe = OK, .read = OK, .erase = NT, .write = NT }
#define TEST_OK_PRE		{ .probe = OK, .read = OK, .erase = OK, .write = NT }
#define TEST_OK_PREW	{ .probe = OK, .read = OK, .erase = OK, .write = OK }

#define TEST_BAD_PROBE	{ .probe = BAD, .read = NT, .erase = NT, .write = NT }
#define TEST_BAD_PR		{ .probe = BAD, .read = BAD, .erase = NT, .write = NT }
#define TEST_BAD_PRE	{ .probe = BAD, .read = BAD, .erase = BAD, .write = NT }
#define TEST_BAD_PREW	{ .probe = BAD, .read = BAD, .erase = BAD, .write = BAD }

/* Timing used in probe routines. ZERO is -2 to differentiate between an unset
 * field and zero delay.
 *
 * SPI devices will always have zero delay and ignore this field.
 */
#define TIMING_FIXME	-1
/* this is intentionally same value as fixme */
#define TIMING_IGNORED	-1
#define TIMING_ZERO	-2

typedef struct sFLASH_USBDriver sFLASH_USBDriver;
typedef int (erasefunc_t)(sFLASH_USBDriver *flash, unsigned int addr, unsigned int blocklen);

typedef struct __attribute__((packed)) flashchip {
	const char *vendor;
	const char *name;

	enum chipbustype bustype;

	/*
	 * With 32bit manufacture_id and model_id we can cover IDs up to
	 * (including) the 4th bank of JEDEC JEP106W Standard Manufacturer's
	 * Identification code.
	 */
	uint32_t manufacture_id;
	uint32_t model_id;

	/* Total chip size in kilobytes */
	unsigned int total_size;
	/* Chip page size in bytes */
	unsigned int page_size;
	int feature_bits;

	/* Indicate how well flashrom supports different operations of this flash chip. */
	struct tested {
		enum test_state probe;
		enum test_state read;
		enum test_state erase;
		enum test_state write;
	} tested;

	int (*probe) (sFLASH_USBDriver *flash);

	/* Delay after "enter/exit ID mode" commands in microseconds.
	 * NB: negative values have special meanings, see TIMING_* below.
	 */
	signed int probe_timing;

	/*
	 * Erase blocks and associated erase function. Any chip erase function
	 * is stored as chip-sized virtual block together with said function.
	 * The first one that fits will be chosen. There is currently no way to
	 * influence that behaviour. For testing just comment out the other
	 * elements or set the function pointer to NULL.
	 */
	struct block_eraser {
		struct eraseblock {
			unsigned int size; /* Eraseblock size in bytes */
			unsigned int count; /* Number of contiguous blocks with that size */
		} eraseblocks[NUM_ERASEREGIONS];
		/* a block_erase function should try to erase one block of size
		 * 'blocklen' at address 'blockaddr' and return 0 on success. */
		int (*block_erase) (sFLASH_USBDriver *flash, unsigned int blockaddr, unsigned int blocklen);
	} block_erasers[NUM_ERASEFUNCTIONS];

	int (*printlock) (sFLASH_USBDriver *flash);
	int (*unlock) (sFLASH_USBDriver *flash);
	int (*write) (sFLASH_USBDriver *flash, const uint8_t *buf, unsigned int start, unsigned int len);
	int (*read) (sFLASH_USBDriver *flash, uint8_t *buf, unsigned int start, unsigned int len);
} flashchip_t;

extern const struct flashchip flashchips[];
extern const unsigned int flashchips_size;

// TODO: Add debug output if needed
#define msg_cspew(...) {}
#define msg_cdbg(...) {}
#define msg_cdbg2(...) {}
#define msg_cerr(...) {}
#define msg_cinfo(...) {}
#define msg_gerr(...) {}
#define msg_perr(...) {}

/* helpers.c */
void programmer_delay(unsigned int usecs);
//uint32_t address_to_bits(uint32_t addr);
//int bitcount(unsigned long a);
static inline int __attribute__((const)) max(int a, int b) { return a < b ? b:a; }
static inline int __attribute__((const)) min(int a, int b) { return a < b ? a:b; }
//void tolower_string(char *str);
uint8_t oddparity(uint8_t val);

/* spi.c */
struct spi_command {
	unsigned int writecnt;
	unsigned int readcnt;
	const unsigned char *writearr;
	unsigned char *readarr;
};

/**
 * @brief   DFU USB Driver configuration structure.
 * @details An instance of this structure must be passed to @p dfuStart()
 *          in order to configure and start the driver operations.
 */
typedef struct {
	/**
	 * @brief   SPI driver pointer.
	 */
	SPIDriver *SPID;
	SPIConfig spicfg;
	/**
	 * @brief The chip mux line port.
	 */
	ioportid_t mux_port;
	/**
	 * @brief The chip mux line pad number.
	 */
	uint16_t mux_pad;
} sFLASH_USBConfig;

struct sFLASH_USBDriver {
	const flashchip_t *chip;
	const sFLASH_USBConfig *config;
	// If chip == 0 following keep raw Manufacturer/Device IDs
	uint32_t id1;
	uint32_t id2;
};

/**
  * @brief  M25P SPI Flash supported commands
  */
#define sFLASH_CMD_WRITE          0x02  /*!< Write to Memory instruction */
#define sFLASH_CMD_WRSR           0x01  /*!< Write Status Register instruction */
#define sFLASH_CMD_WREN           0x06  /*!< Write enable instruction */
#define sFLASH_CMD_READ           0x03  /*!< Read from Memory instruction */
#define sFLASH_CMD_RDSR           0x05  /*!< Read Status Register instruction  */
#define sFLASH_CMD_RDID           0x9F  /*!< Read identification */
#define sFLASH_CMD_SE             0xD8  /*!< Sector Erase instruction */
#define sFLASH_CMD_BE             0xC7  /*!< Bulk Erase instruction */

#define sFLASH_WIP_FLAG           0x01  /*!< Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0xA5
#define sFLASH_SPI_PAGESIZE       0x100


#if BOOT_FLASH_SEL_BIT
/**
  * @brief  Select mux to use BMC SPI
  */
#define sFLASH_AQUIRE(SFDU)       (palClearPad(SFDU->config->mux_port, SFDU->config->mux_pad))
/**
  * @brief  Deselect mux from BMC SPI
  */
#define sFLASH_RELEASE(SFDU)      (palSetPad(SFDU->config->mux_port, SFDU->config->mux_pad))

#else
#define sFLASH_RELEASE(SFDU)      (palClearPad(SFDU->config->mux_port, SFDU->config->mux_pad))
#define sFLASH_AQUIRE(SFDU)       (palSetPad(SFDU->config->mux_port, SFDU->config->mux_pad))
#endif

/**
  * @brief  Select sFLASH: Chip Select pin low
  */
#define sFLASH_CS_LOW(SFDU)       spiSelectI(SFDU->config->SPID)
/**
  * @brief  Deselect sFLASH: Chip Select pin high
  */
#define sFLASH_CS_HIGH(SFDU)      spiUnselectI(SFDU->config->SPID)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Flashprog style API
 */
int spi_send_command(sFLASH_USBDriver *flash, unsigned int writecnt, unsigned int readcnt, const unsigned char *writearr, unsigned char *readarr);
int spi_send_multicommand(sFLASH_USBDriver *flash, struct spi_command *cmds);
uint32_t spi_get_valid_read_addr(sFLASH_USBDriver *flash);
uint8_t spi_read_status_register(sFLASH_USBDriver *flash);

/**
  * @brief  High layer functions
  */
void sFLASH_DeInit(sFLASH_USBDriver*);
void sFLASH_Init(sFLASH_USBDriver*, const sFLASH_USBConfig*);
void sFLASH_Start(sFLASH_USBDriver*);
void sFLASH_EraseSector(sFLASH_USBDriver*, uint32_t SectorAddr);
void sFLASH_EraseBulk(sFLASH_USBDriver*);
void sFLASH_WritePage(sFLASH_USBDriver*, const uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(sFLASH_USBDriver*, uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(sFLASH_USBDriver*, uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void sFLASH_ReadID(sFLASH_USBDriver*, uint8_t *idbuffer);
void sFLASH_StartReadSequence(sFLASH_USBDriver*, uint32_t ReadAddr);

/**
  * @brief  Low layer functions
  */
uint8_t sFLASH_ReadByte(sFLASH_USBDriver*);
uint8_t sFLASH_SendByte(sFLASH_USBDriver*, uint8_t byte);
uint16_t sFLASH_SendHalfWord(sFLASH_USBDriver*, uint16_t HalfWord);
void sFLASH_WriteEnable(sFLASH_USBDriver*);
void sFLASH_WaitForWriteEnd(sFLASH_USBDriver*);

#ifdef __cplusplus
}
#endif

#endif /* SPI_FLASH_H_ */
