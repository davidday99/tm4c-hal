PROJECT = main

# Directories
SRCS = $(wildcard src/*.c) \
	  $(wildcard lib/**/*.c) \
      $(wildcard lib/**/**/*.c) \
	  $(wildcard lib/networking/network-stack/src/*.c) \
	  $(wildcard asm/*.s)
TESTS = $(wildcard test/*.c)
OBJ = obj/
TESTOBJS = $(addprefix $(OBJ), $(notdir $(TESTS:.c=.o)))
OBJS = $(addprefix $(OBJ),$(filter-out %.c,$(notdir $(SRCS:.s=.o))) $(filter-out %.s,$(notdir $(SRCS:.c=.o))))

# Target specific
MCU = TM4C123GH6PM
LD_SCRIPT = ld/$(MCU).ld
DEV = /dev/ttyACM0

# Toolchain
CC = arm-none-eabi-gcc 
OBJCOPY = arm-none-eabi-objcopy
FLASHER = lm4flash
DEBUGGER = arm-none-eabi-gdb


# Compilation flags
OPT_LEVEL += -O0
CFLAGS = -ggdb3 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 
CFLAGS += -mfloat-abi=softfp -MD -std=c99 -Wextra -Wall -Wno-missing-braces -Wno-builtin-declaration-mismatch
CFLAGS += -nostartfiles -nostdlib $(OPT_LEVEL)
LDFLAGS = -T $(LD_SCRIPT) -e Reset_Handler 
INC = -Iinc -Ilib/networking/network-stack/inc

# Etc.
RM      = rm -rf
MKDIR   = @mkdir -p $(@D) # creates folders if not present

build: bin/$(PROJECT).bin

test: bin/testrunner.bin
	$(FLASHER) -S $(DEV) bin/testrunner.bin

all: build test

$(OBJ)%.o: asm/%.s
	$(MKDIR)
	$(CC) -o $@ $^ -c $(CFLAGS)

$(OBJ)%.o: src/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)


# TODO: clean up this and the following recipe...
$(OBJ)%.o: lib/**/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

$(OBJ)%.o: lib/**/**/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

$(OBJ)%.o: test/%.c
	$(MKDIR)
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

# Build network stack library and copy object files from submodule to top
$(OBJ)%.o: lib/networking/network-stack/src/%.c
	$(MAKE) embedded -C lib/networking/network-stack
	cp lib/networking/network-stack/eobj/*.o $(OBJ)
	
# Generate project binary with debug symbols 
bin/$(PROJECT).elf: $(OBJS)
	$(MKDIR)           
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)


# Strip debug symbols
bin/$(PROJECT).bin: bin/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

# Same as above two recipes but for the test binary.
bin/testrunner.elf: $(TESTOBJS) $(filter-out obj/main.o, $(OBJS))
	$(MKDIR)           
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) 

bin/testrunner.bin: bin/testrunner.elf
	$(OBJCOPY) -O binary $< $@

flash:
	$(FLASHER) -S $(DEV) bin/$(PROJECT).bin

debug:
	$(DEBUGGER) $(file) --tui -ex "target remote :3333"

clean:
	-$(RM) obj
	-$(RM) bin

.PHONY: all clean
