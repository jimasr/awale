# Define variables
BIN_DIR = bin
TARGET_SERVER = bin/server
TARGET_CLIENT = bin/client
SERVER_DIR = src/server
CLIENT_DIR = src/client
CC = gcc

all: directories $(TARGET_SERVER) $(TARGET_CLIENT)

directories:
	@mkdir -p $(BIN_DIR)

$(TARGET_SERVER): $(SERVER_DIR)/server.c
	$(CC) -o $(TARGET_SERVER) $(SERVER_DIR)/server.c

$(TARGET_CLIENT): $(CLIENT_DIR)/client.c
	$(CC) -o $(TARGET_CLIENT) $(CLIENT_DIR)/client.c

clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT) 