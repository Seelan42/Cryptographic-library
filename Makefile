CC = gcc
CFLAGS = -Wall -Iinclude -Isrc

BUILD_DIR = build
BIN_DIR = bin

SRC_COMMON = src/aes.c src/utils.c src/keygen.c
TEST_SRC = test/test_crypto.c
CHAT_SRC = test/chat_simulator.c

# Generate object lists with proper prefixes
TEST_OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(SRC_COMMON:.c=.o) $(TEST_SRC:.c=.o)))
CHAT_OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(SRC_COMMON:.c=.o) $(CHAT_SRC:.c=.o)))

TEST_TARGET = $(BIN_DIR)/test_crypto
CHAT_TARGET = $(BIN_DIR)/chat_simulator

all: $(TEST_TARGET) $(CHAT_TARGET)

# Target for test_crypto
$(TEST_TARGET): $(TEST_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Target for chat_simulator
$(CHAT_TARGET): $(CHAT_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule for src files
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for test files
$(BUILD_DIR)/%.o: test/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure build and bin directories exist
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
