CUR_DIR := .
BUILD_DIR := $(CUR_DIR)/build
OUT_DIR := $(BUILD_DIR)/out

MODULE := js_emu

CC  = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
AR  = $(CROSS_COMPILE)ar
AS  = $(CROSS_COMPILE)as
LD  = $(CROSS_COMPILE)ld
NM  = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
STRIP   = $(CROSS_COMPILE)strip
SIZE    = $(CROSS_COMPILE)size
MAKE    = make

.SUFFIXES =
.SUFFIXES = .c .o .cpp .cpp.o .h

SRCS :=
SRCS += $(CUR_DIR)/main.cpp

INC :=
INC += $(CUR_DIR)

DEFINES :=
DEFINES +=

OPTIM :=
ifeq ($(DEBUG),1)
	OPTIM += -g -O0
	DEFINES += -DDEBUG
else
	OPTIM += -O3
endif

CXXFLAGS :=
CXXFLAGS += -Wno-unused-variable -MMD -MP
CXXFLAGS += -Wall -Wextra
CXXFLAGS += -Wno-unused-parameter
CXXFLAGS += -Wno-unused-function
CXXFLAGS += -Wno-missing-field-initializers
CXXFLAGS += -std=c++11
CXXFLAGS += -fno-inline
CXXFLAGS += -fPIE
CXXFLAGS += $(INC) $(DEFINES) $(OPTIM)

LDFLAGS :=
LDFLAGS += -pthread
LDFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -Wl,--gc-sections,-Map=$(OUT_DIR)/mapfile
LDFLAGS += -fPIE

all: $(MODULE)

CXXOBJS :=  $(patsubst %.cpp,$(BUILD_DIR)/%.cpp.o,$(SRCS))

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MODULE): $(CXXOBJS)
	mkdir -p $(OUT_DIR)
	$(file >$(BUILD_DIR)/build.release, $(shell find $(BUILD_DIR) -name "*.o"))
	$(CXX) -o $(OUT_DIR)/$@ "@$(BUILD_DIR)/build.release" $(LDFLAGS)
	@echo done

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean %.cpp.o
