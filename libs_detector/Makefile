OUTPUT_NAME = libs_detector


INC+= -I.
INC+= -I../libs
INC+= -I$(LIBS_RYSY_PATH)/libs_common
INC+= -I$(LIBS_RYSY_PATH)/libs_cnn_deployment

SRCS+= $(shell find . -name '*.cpp')

CC = g++
CU = nvcc
LD = g++
OBJCOPY = objcopy
OBJDUMP = objdump
SIZE = size
ARCHIVE = ar

# DEGUBFLAGS = -g -pg

CFLAGS = $(DEGUBFLAGS) -O2 -Os -fPIC -s -Wall -Wextra -pedantic -std=c++17 $(INC)

OBJS = $(SRCS:.cpp=.o)

all: $(OBJS)
	$(ARCHIVE) rc $(OUTPUT_NAME).a $(OBJS)
	$(CC) -shared -fPIC -o $(OUTPUT_NAME).so $(OBJS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@


clean:
	rm $(OBJS) -rf
	rm $(OUTPUT_NAME).* -rf
