SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SERVER_SRC = $(SRC_DIR)/server.c $(SRC_DIR)/server_main.c
SERVER_OBJ = $(OBJ_DIR)/server.o $(OBJ_DIR)/server_main.o
SERVER_BIN = $(BIN_DIR)/server

CLIENT_SRC = $(SRC_DIR)/client.c $(SRC_DIR)/client_main.c
CLIENT_OBJ = $(OBJ_DIR)/client.o $(OBJ_DIR)/client_main.o
CLIENT_BIN = $(BIN_DIR)/client

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_OBJ) | $(BIN_DIR)
	gcc -o $@ $^

$(CLIENT_BIN): $(CLIENT_OBJ) | $(BIN_DIR)
	gcc -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	gcc -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*
