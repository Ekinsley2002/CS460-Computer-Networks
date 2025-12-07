# Many thanks to Job Vranish - see https://spin.atomicobject.com/2016/08/26/makefile-c-projects/

BUILD_DIR := ./build
SRC_DIRS  := ./src

# Common source files (shared between client and server)
COMMON_SRCS := $(shell find $(SRC_DIRS) -maxdepth 1 -name *.c)
# Server-specific source files (exclude main.c, use server_main.c instead)
SERVER_SRCS := $(filter-out %/main.c,$(shell find $(SRC_DIRS)/server -name *.c))
# Client-specific source files (exclude main.c, use client_main.c instead)
CLIENT_SRCS := $(filter-out %/main.c,$(shell find $(SRC_DIRS)/client -name *.c))

# Object files
COMMON_OBJS := $(COMMON_SRCS:%=$(BUILD_DIR)/%.o)
SERVER_OBJS := $(SERVER_SRCS:%=$(BUILD_DIR)/%.o)
CLIENT_OBJS := $(CLIENT_SRCS:%=$(BUILD_DIR)/%.o)

# All object files for dependencies
ALL_OBJS := $(COMMON_OBJS) $(SERVER_OBJS) $(CLIENT_OBJS)
DEPS := $(ALL_OBJS:.o=.d)

INC_DIRS  := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP
LDFLAGS  := -pthread -lpthread

# Build both executables
all: $(BUILD_DIR)/client $(BUILD_DIR)/server

# Client executable
$(BUILD_DIR)/client: $(COMMON_OBJS) $(CLIENT_OBJS)
	$(CC) $(COMMON_OBJS) $(CLIENT_OBJS) -o $@ $(LDFLAGS)

# Server executable
$(BUILD_DIR)/server: $(COMMON_OBJS) $(SERVER_OBJS)
	$(CC) $(COMMON_OBJS) $(SERVER_OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean all

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
