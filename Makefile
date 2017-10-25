##############################################################################
# Build global options
#
# Copyright (C) 2016, 2017 Baikal Electronics.
# Author: Alexander Osipenko <Alexander.Osipenko@baikalelectronics.ru>
#

BE_BOARD=BE_BT_BFK_30
DEBUG=1

# Compiler options here.
ifeq ($(USE_OPT),)
ifneq ($(DEBUG),)
USE_OPT = -Og -g -ggdb
else
USE_OPT = -O2 -fomit-frame-pointer
endif
#USE_OPT += --specs=nosys.specs -lnosys
endif
#USE_OPT += -Wall -Wextra -Wundef
#USE_OPT += -mfp16-format=alternative

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = -std=gnu11
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti -fno-exceptions
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# Linker extra options here.
ifeq ($(USE_LDOPT),)
  USE_LDOPT =
endif

# Enable this if you want link time optimizations (LTO)
ifeq ($(USE_LTO),)
  USE_LTO = yes
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

# If enabled, this option makes the build process faster by not compiling
# modules not used in the current configuration.
ifeq ($(USE_SMART_BUILD),)
  USE_SMART_BUILD = yes
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Stack size to be allocated to the Cortex-M process stack. This stack is
# the stack used by the main() thread.
ifeq ($(USE_PROCESS_STACKSIZE),)
  USE_PROCESS_STACKSIZE = 0x400
endif

# Stack size to the allocated to the Cortex-M main/exceptions stack. This
# stack is used for processing interrupts and exceptions.
ifeq ($(USE_EXCEPTIONS_STACKSIZE),)
  USE_EXCEPTIONS_STACKSIZE = 0x400
endif

# Enables the use of FPU on Cortex-M4 (no, softfp, hard).
ifeq ($(USE_FPU),)
  USE_FPU = no
endif

#
# Architecture or project specific options
##############################################################################

MIPSGCC = ~/Documents/MIPS/mips-mti-elf/2016.05-03/bin/mips-mti-elf-
#MIPSGCC = ~/Documents/MIPS/mips-2015.11/bin/mips-linux-gnu-

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT_name = bmc
#NOTE: use EVEN version numbers for 16.1.x ChibiOS kernel
PROJECT_version = v3.0.0.2
PROJECT = $(PROJECT_name)_$(PROJECT_version)
# Target compiler path/prefix
#TRGT = arm-none-eabi
TRGT = $(realpath $(PWD)/../gcc-arm-none-eabi-4_9-2015q3)/bin/arm-none-eabi-

#NOTE: GCC 5.x requires 'USE_LDOPT = --build-id=none' to suppress .note.gnu.build-id section
#NOTE: Not run on CentOS release 6.8
#TRGT = $(realpath $(PWD)/../gcc-linaro-5.3-2016.02-x86_64_arm-eabi)/bin/arm-eabi-
#USE_LDOPT := $(if $(USE_LDOPT),$(USE_LDOPT)$(firstword ,),)--build-id==none

# Imported source files and paths


CHIBIOS_VERSION=17.6.0
CHIBIOS = $(PWD)/../ChibiOS_$(CHIBIOS_VERSION)
CHIBIOS_CONTRIB=$(CHIBIOS)/community

CHIBIOS_PORT_PATH = $(CHIBIOS)/os/$1/ARMCMx/compilers/GCC
# Startup files.
include $(call CHIBIOS_PORT_PATH,common/startup)/mk/startup_stm32f2xx.mk
# HAL-OSAL files
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/community/os/hal/hal.mk
include $(CHIBIOS)/os/hal/ports/STM32/STM32F4xx/platform.mk
include boards/${BE_BOARD}/board.mk
# RTOS files.
include $(CHIBIOS)/os/hal/osal/rt/osal.mk
include $(CHIBIOS)/os/rt/rt.mk
include $(call CHIBIOS_PORT_PATH,common/ports)/mk/port_v7m.mk
# Other files (optional).
#include $(CHIBIOS)/test/rt/test.mk

#LDSCRIPT = $(STARTUPLD)/STM32F205xB.ld
LDSCRIPT = boards/STM32F205xB.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(STARTUPSRC) \
       $(KERNSRC) \
       $(PORTSRC) \
       $(OSALSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(CHIBIOS)/os/hal/lib/streams/memstreams.c \
       $(CHIBIOS)/os/hal/lib/streams/chprintf.c \
       cli_readline.c shell.c \
       spi_flash.c spi25.c spi_flash_table.c helpers.c spi25_statusreg.c

BMC_SRC = usbcfg.c dfu.c fxp.c x.c tps56.c clk156.c smbus.c pmbus_cmd.c smb_cmd.c ucd9090_config.c main.c pac1720.c
EXTI_EX_SRC      = exti_ex.c
I2C_EXF_SRC      = i2c_exf.c
CLICMD__SRC      = cli_commands.c
CG_5P49V5901_SRC = cg_5p49v5901.c

CSRC += $(BMC_SRC)
CSRC += $(EXTI_EX_SRC)
CSRC += $(I2C_EXF_SRC)
CSRC += $(CLICMD__SRC)
CSRC += $(CG_5P49V5901_SRC)

#CSRC += $(TESTSRC)

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
#CPPSRC = $(patsubst %.c,%.cpp,$(BMC_SRC))

# C sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACSRC =

# C++ sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACPPSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCPPSRC =

# List ASM source files here
ASMSRC =
ASMXSRC = $(STARTUPASM) $(PORTASM) $(OSALASM)

INCDIR = $(CHIBIOS)/os/license \
         $(STARTUPINC) $(KERNINC) $(PORTINC) $(OSALINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) $(TESTINC) \
         $(CHIBIOS)/os/hal/lib/streams $(CHIBIOS)/os/various \
         $(CHIBIOS)/os/common/ext/CMSIS/ST/STM32F2xx

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m3

CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
AR   = $(TRGT)ar
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary
GDB  = $(TRGT)gdb

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wstrict-prototypes -Wundef

# Define C++ warning options here
CPPWARN = -Wall -Wextra -Wundef

#
# Compiler settings
##############################################################################

##############################################################################
# Start of user section
#
UDEFS += -DCHPRINTF_USE_FLOAT

UDEFS += -DFIRMWARE_VERSION=\"$(PROJECT_version)\"

# List all user C define here, like -D_DEBUG=1
UDEFS += -DBMC_WITH_SYSTEM_CONSOLE
#UDEFS += -DBMC_WITH_PASSWORD
UDEFS += -DBE_INTERNAL

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

ifneq ($(DEBUG),)
UDEFS += -D__CH_DEBUG=TRUE
UADEFS += -D__CH_DEBUG=TRUE
endif

#
# End of user defines
##############################################################################

RULESPATH = $(call CHIBIOS_PORT_PATH,common/startup)
include $(RULESPATH)/rules.mk

.PHONY: gdb pgdb mgdb gdbrun bprog jprog mprog run oocd oocon board

$(RULESPATH)/rules.mk:
	unzip -q -d .. ../ChibiOS/ChibiOS_$(CHIBIOS_VERSION).zip
	cd $(CHIBIOS) && hg init . && hg -q add . && hg ci -m "init"
	test -f $(PWD)/chibios/ChibiOS_$(CHIBIOS_VERSION).diff && patch -d $(CHIBIOS) -p1 -i $(PWD)/chibios/ChibiOS_$(CHIBIOS_VERSION).diff

build/zl_pid: zl_pid.c
	gcc -std=gnu99 -o $@ $<

ospatch:
	test -f $(PWD)/chibios/ChibiOS_$(CHIBIOS_VERSION).diff && patch -d $(CHIBIOS) -p1 -i $(PWD)/chibios/ChibiOS_$(CHIBIOS_VERSION).diff


osdiff:
	cd $(CHIBIOS) && hg diff -r0 . > $(PWD)/chibios/ChibiOS_$(CHIBIOS_VERSION).diff

# openocd must be installed to access default configs
openocd=openocd
OOCD_ADAPTER=interface/ftdi/olimex-arm-usb-ocd-h.cfg
#OOCD_ADAPTER=misc/olimex-arm-usb-ocd-h.cfg
#OOCD_ADAPTER=interface/stlink-v2.cfg

dgdb: DEBUG=1
dgdb: build/$(PROJECT).elf
	$(GDB) build/$(PROJECT).elf -ex "target remote localhost:3333"

pgdb:
	$(GDB) \
	-ex "target remote | openocd -f ${OOCD_ADAPTER} -f target/stm32f2x.cfg -f misc/bfk.cfg -c \"gdb_port pipe; log_output openocd.log\"" \
	-ex "file build/$(PROJECT).elf" \
	-x misc/gdbbmc.cfg

gdb:
	$(GDB) -ex "target remote localhost:3333"

gdbrun:
	$(GDB) -ex "target remote localhost:3333" -ex "monitor reset halt" -ex "load build/$(PROJECT).elf" -ex "monitor reset run"

mmgdb:
	$(MIPSGCC)gdb -ex "target remote localhost:3333" -ex "set endian little"

mgdb:
	$(MIPSGCC)gdb -ex "target remote | openocd -f ${OOCD_ADAPTER} -f misc/p5600-haps.cfg -c \"gdb_port pipe; log_output openocd.log\""

b1prog:
	dfu-util -d abf0:1234 -c 1 -i 0 -a 0 -D boards/BE_BT1_BFK20/boot_rom.text

bprog:
	dfu-util -d abf0:1234 -c 1 -i 0 -a 0 -D misc/boot_rom.text.KVR16S11

mprog:
	dfu-util -d abf0:1234 -c 1 -i 0 -a 0 -D misc/Blink.boot.text

dprog: build/$(PROJECT).bin
	dfu-util -d 0483:df11 -c 1 -i 0 -a 0 -D build/$(PROJECT).bin --dfuse-address 0x08000000

jprog: build/$(PROJECT).elf
	$(openocd) -f ${OOCD_ADAPTER} -f target/stm32f2x.cfg -f misc/bfk.cfg -c "program build/$(PROJECT).elf verify; reset run; shutdown"

japb:
	$(openocd) -s $(PWD)/misc -f japb.cfg

run:
	$(openocd) -f ${OOCD_ADAPTER} -f target/stm32f2x.cfg -f misc/bfk.cfg -c "init; reset run; shutdown"

oocd:
	$(openocd) -f ${OOCD_ADAPTER} -f target/stm32f2x.cfg -f misc/bfk.cfg

mocd:
	$(openocd) -f ${OOCD_ADAPTER} -f misc/p5600-haps.cfg

oocon:
	telnet localhost 4444

tags0:
	ctags  -o tags.c.tags $(CSRC) $(CPPSRC) -R $(INCDIR)

tags:
	ctags --totals --fields=fKsSt --extra=-fq --c-kinds=+p --sort=foldcase --excmd=number -o tags.c.tags $(CSRC) $(CPPSRC) -R $(INCDIR)

gtags:
	CFLAGS="$(addprefix -I,$(INCDIR))" geany -g bmc.c.tags $(CSRC) $(CPPSRC)

tar:
	tar czf $(PROJECT).tar.gz build/$(PROJECT).* misc/bmc_bfk20.pdf

scl:
	scl enable devtoolset-3 bash

test:
	echo $(OSALSRC)
	echo $(OSALASM)
