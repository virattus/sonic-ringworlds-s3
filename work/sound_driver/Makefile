CXX=g++
CXXFLAGS=-std=gnu++11 -O2 -fwrapv -Wall
CPPFLAGS=-D_GNU_SOURCE=1
#
M68K_CC=m68k-elf-gcc
M68K_CFLAGS=-Os -g -std=gnu99 -march=68000 -fwrapv -fsigned-char -fno-strict-aliasing -Wall -Werror -Wstack-usage=64
M68K_AS=m68k-elf-as
M68K_ASFLAGS=-march=68000 -mcpu=68ec000
M68K_OBJCOPY=m68k-elf-objcopy
#
SH2_CC=sh2eb-elf-gcc
SH2_CFLAGS=-O2 -std=gnu99 -m2 -fwrapv -fsigned-char -fno-inline -fno-unit-at-a-time -Wall -Wformat=0
SH2_OBJCOPY=sh2eb-elf-objcopy
#
#
#
all:		adp68k.bin

adp68k.elf:	Makefile adp68k.c adp68k.h adp68k.ld adp68k-data.h types.h scsp.h dsp-macros.h
		$(M68K_CC) $(M68K_CFLAGS) -nostdlib -Xlinker -Tadp68k.ld -o adp68k.elf adp68k.c -lgcc

adp68k.bin:	Makefile adp68k.elf
		$(M68K_OBJCOPY) -O binary adp68k.elf adp68k.bin
		
.PHONY:		clean
clean:
		rm --force --one-file-system -- adp68k.elf adp68k.bin adp68k.bin.inc adp68k-data.h gen-data adpencode adplink bintoinc $(ADP_OBJ) samples.h samples.ladp samples.ladp.inc test.elf test.bin test.iso
