GCC ?= g++
CCMODE = PROGRAM
CFLAGS =  -Wall $(MACRO)
TARGET = connectionPool
SRCS := $(wildcard main.cpp src/*.cpp)
INCLUDES =  -I ./include
LIBS = -lpthread -lmysqlclient

ifeq ($(CCMODE),PROGRAM)
$(TARGET): $(LINKS) $(SRCS) 
	$(GCC) $(CFLAGS) $(INCLUDES) -o $(TARGET)  $(SRCS) $(LIBS)
	@chmod +x $(TARGET)
	@echo make $(TARGET) ok.
clean:
	rm -rf $(TARGET)
endif
