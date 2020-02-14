PROJ_DIR := .
BUILD_DIR := $(PROJ_DIR)/build
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
SRCS += $(PROJ_DIR)/main.cpp
SRCS += $(PROJ_DIR)/js.cpp
SRCS += $(PROJ_DIR)/clua.cpp
SRCS += $(PROJ_DIR)/cluafunc.cpp

INC :=
INC += -I$(PROJ_DIR)
INC += -I$(PROJ_DIR)/lua-5.3.2/src

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

LIBDIR := -L$(PROJ_DIR)/lua-5.3.2/src

LIBS := $(LIBDIR) -llua -ldl

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

$(MODULE): build_lua $(CXXOBJS)
	mkdir -p $(OUT_DIR)
	$(file >$(BUILD_DIR)/build.release, $(shell find $(BUILD_DIR) -name "*.o"))
	$(CXX) -o $(OUT_DIR)/$@ "@$(BUILD_DIR)/build.release" $(LIBS) $(LDFLAGS)
	@echo done

build_lua:
	make linux -j8 -C lua-5.3.2/src

clean:
	rm -rf $(BUILD_DIR)
	make clean -C lua-5.3.2/src

.PHONY: all clean %.cpp.o
