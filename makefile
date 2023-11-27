GCC ?= g++
CCMODE = PROGRAM
CFLAGS =  -Wall $(MACRO)
TARGET = CDBCONPOOL
SRCS := $(wildcard src/*.cpp)
INCLUDES =  -I ./include
# LIBS = -lpthread -lncurses

ifeq ($(CCMODE),PROGRAM)
$(TARGET): $(LINKS) $(SRCS) 
	$(GCC) $(CFLAGS) $(INCLUDES) -o $(TARGET)  $(SRCS) $(LIBS)
	@chmod +x $(TARGET)
	@echo make $(TARGET) ok.
clean:
	rm -rf $(TARGET)
endif

OBJ:=$(TARGET).oss
