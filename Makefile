# Define variables
BIN_DIR = bin
TARGET_SERVER = bin/server
TARGET_CLIENT = bin/client
SERVER_DIR = src/server
CLIENT_DIR = src/client
UTIL_DIR = src/util
CC = gcc

all: directories $(TARGET_SERVER) $(TARGET_CLIENT)

directories:
	@mkdir -p $(BIN_DIR)

$(TARGET_SERVER): $(SERVER_DIR)/server.c $(UTIL_DIR)/logger.c
	$(CC) -o $(TARGET_SERVER) $(SERVER_DIR)/server.c $(UTIL_DIR)/logger.c

$(TARGET_CLIENT): $(CLIENT_DIR)/client.c
	$(CC) -o $(TARGET_CLIENT) $(CLIENT_DIR)/client.c

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT) 