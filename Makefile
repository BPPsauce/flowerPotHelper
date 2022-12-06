# Makefile for building embedded application.
# by Brian Fraser
# Edit this file to compile extra C files into their own programs.
TARGET= flowerPotHelper
SOURCES= main.c joystick.c utils.c display.c leds.c soilSensor.c 
PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc
CFLAGS = -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow -Wall
LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

# -pg for supporting gprof profiling.
#CFLAGS += -pg
all: flowerPotHelper
flowerPotHelper:
	$(CC_C) $(CFLAGS) $(SOURCES) -o $(OUTDIR)/$(TARGET) $(LFLAGS) -lpthread -lasound
clean:
	rm -f $(OUTDIR)/$(TARGET)