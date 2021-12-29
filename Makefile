#
# Copyright (c) 2018, Shawn D'silva <shawn@shawndsilva.com>
# All rights reserved.
#
#  This file is free software: you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 #
 # This file is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public License
 # along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
# File:			Makefile
# Author:		Shawn D'silva <https://www.shawndsilva.com>.
# Version:		1.0.0.
# Description:	Makefile used to build files for this program


# name of the output file
PROJECT = main

# your TM4C123GH6PM when connected to your system, usually will be /dev/ttyACM0
DEV = /dev/ttyACM0

SRCS = $(wildcard src/*.c) \
	  $(wildcard lib/**/*.c) \
      $(wildcard lib/**/**/*.c) \
	  $(wildcard asm/*.s)
OBJ = obj/
OBJS = $(addprefix $(OBJ),$(filter-out %.c,$(notdir $(SRCS:.s=.o))) $(filter-out %.s,$(notdir $(SRCS:.c=.o))))

INC = -Iinc

MCU = TM4C123GH6PM

LD_SCRIPT = ld/$(MCU).ld


CC = arm-none-eabi-gcc 
OBJCOPY = arm-none-eabi-objcopy
FLASHER = lm4flash
DEBUGGER = arm-none-eabi-gdb
RM      = rm -rf
MKDIR   = @mkdir -p $(@D) # creates folders if not present

OPT += -O3

CFLAGS = -ggdb3 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 
CFLAGS += -mfloat-abi=softfp -MD -std=c99 -c -Wextra -Wall -Wno-missing-braces -Wno-builtin-declaration-mismatch
CFLAGS += $(OPT)

LDFLAGS = -T $(LD_SCRIPT) -e Reset_Handler 

$(info $(OBJS))

all: bin/$(PROJECT).bin

$(OBJ)%.o: asm/%.s
	$(MKDIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ)%.o: src/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ $(INC) $(CFLAGS)

$(OBJ)%.o: lib/**/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ $(INC) $(CFLAGS)

$(OBJ)%.o: lib/**/**/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ $(INC) $(CFLAGS)
	
bin/$(PROJECT).elf: $(OBJS)      # make contains debug symbols for GNU GDB
	$(MKDIR)           
	$(CC) -o $@ $^ $(LDFLAGS) -nostartfiles -nostdlib

bin/$(PROJECT).bin: bin/$(PROJECT).elf    # debug symbols for GNU GDB stripped by objcopy, finished binary ready for flashing
	$(OBJCOPY) -O binary $< $@


flash:
	$(FLASHER) -S $(DEV) bin/$(PROJECT).bin

debug:
	$(DEBUGGER) --tui bin/$(PROJECT).elf -ex "target remote :3333" -ex "monitor reset halt"

clean:
	-$(RM) obj
	-$(RM) bin

.PHONY: all clean
