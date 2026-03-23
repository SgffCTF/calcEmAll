CXX := gcc
CXXFLAGS := -static

BUILD_DIR := ./build

EXPLOIT_SRC := exploit.c
LEAK_SRC := leak_kernel_addr.c
EXPLOIT_OBJ := $(BUILD_DIR)/exploit.o
LEAK_OBJ := $(BUILD_DIR)/leak_kernel_addr.o
EXPLOIT_TARGET := exploit
LEAK_TARGET := leak

all: $(EXPLOIT_TARGET) $(LEAK_TARGET)

$(EXPLOIT_TARGET): $(EXPLOIT_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $^

$(LEAK_TARGET): $(LEAK_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/$@ $^

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
