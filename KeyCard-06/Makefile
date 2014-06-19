MISC_PATH := ../../misc

BIN     := KeyCard
CC		:= avr-gcc
CFLAGS  := -Iincludes -I. -Os -std=gnu99 -Wall -Wextra -mmcu=atmega16u2
CPPFLAGS:= -DF_CPU=16000000

SOURCES := $(shell find . -name '*.c')
OBJECTS := $(subst ./, obj/,$(subst .c,.o,$(SOURCES)))
OBJDIRS := $(sort $(dir $(OBJECTS)))
DEPENDS := $(OBJECTS:.o=.d)

ELF		:= bin/$(BIN).elf
HEX		:= bin/$(BIN).hex

.SUFFIXES: .o .c .d .hex .out
.PHONY: prep all hex

all: prep ELFBUILD HEXBUILD

prep:
	@mkdir -p obj
	@mkdir -p bin
	@mkdir -p $(OBJDIRS)

obj/%.o : %.c
	@echo "[CC ]  $@"
	@$(CC) $(CFLAGS) -c $^ -o $@ $(CPPFLAGS)

ELFBUILD: prep $(OBJECTS)
	@echo "[ELF]  $(ELF)"
	@$(CC) $(CFLAGS) $(OBJECTS) -o $(ELF) $(CPPFLAGS)

HEXBUILD: ELFBUILD
	@echo "[HEX]  $(HEX)"
	@avr-objcopy -O ihex $(ELF) $(HEX)

-include $(MISC_PATH)/flash.mk

clean:
	$(RM) -r {obj,bin}
	$(RM) flash.log
