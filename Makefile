PROJECT = main

# Directories
SRCS = $(wildcard src/*.c) \
		$(wildcard src/*/*.c) \
		$(wildcard src/*/*/*.c) \
		$(wildcard asm/*.s)
LIBSRCS = $(filter-out src/main.c src/startup.c src/interrupts.c, $(SRCS))
OBJ = obj
BIN = bin
TESTS = $(wildcard test/*.c)
TESTOBJS = $(addprefix $(OBJ)/, $(notdir $(TESTS:.c=.o)))
OBJS = $(addprefix $(OBJ)/,$(filter-out %.c,$(notdir $(SRCS:.s=.o))) $(filter-out %.s,$(notdir $(SRCS:.c=.o))))
LIB = lib
LIBOBJS = $(addprefix $(LIB)/,$(filter-out %.c,$(notdir $(LIBSRCS:.s=.o))) $(filter-out %.s,$(notdir $(LIBSRCS:.c=.o))))

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
OPT_LEVEL += -Os
CFLAGS = -ggdb3 -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16
CFLAGS += -mfloat-abi=softfp -MD -std=c99 -Wextra -Wall -Wno-missing-braces -Wno-builtin-declaration-mismatch
CFLAGS += -ffreestanding -nostartfiles -nostdlib $(OPT_LEVEL)
LDFLAGS = -T $(LD_SCRIPT) -e Reset_Handler 
INC = -Iinc -Ilib/networking/network-stack/inc

# Etc.
RM = rm -rf
MKDIR = @mkdir -p $(@D) # creates folders if not present

build: $(BIN)/$(PROJECT).bin

library: $(LIBOBJS)

test: $(BIN)/testrunner.bin
	$(FLASHER) -S $(DEV) $(BIN)/testrunner.bin

all: build test

$(OBJ)/%.o: asm/%.s
	$(MKDIR)
	$(CC) -o $@ $^ -c $(CFLAGS)

$(OBJ)/%.o: src/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

$(OBJ)/%.o: src/*/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

$(OBJ)/%.o: src/*/*/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

$(OBJ)/%.o: test/%.c
	$(MKDIR)
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

$(LIB)/%.o: asm/%.s
	$(MKDIR)
	$(CC) -o $@ $^ -c $(CFLAGS)

$(LIB)/%.o: src/*/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)

$(LIB)/%.o: src/*/*/%.c
	$(MKDIR)              
	$(CC) -o $@ $^ -c $(INC) $(CFLAGS)
	
# Generate project binary with debug symbols 
$(BIN)/$(PROJECT).elf: $(OBJS)
	$(MKDIR)           
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS)

# Strip debug symbols
$(BIN)/$(PROJECT).bin: $(BIN)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@

# Same as above two recipes but for the test binary.
$(BIN)/testrunner.elf: $(TESTOBJS) $(filter-out $(OBJ)/main.o, $(OBJS))
	$(MKDIR)           
	$(CC) -o $@ $^ $(LDFLAGS) $(CFLAGS) 

$(BIN)/testrunner.bin: $(BIN)/testrunner.elf
	$(OBJCOPY) -O binary $< $@

flash:
	$(FLASHER) -S $(DEV) $(BIN)/$(PROJECT).bin

debug:
	$(DEBUGGER) $(file) --tui -ex "target remote :3333"

clean:
	-$(RM) $(OBJ)
	-$(RM) $(BIN)
	-$(RM) $(LIB)

.PHONY: all clean
