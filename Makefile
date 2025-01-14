CXX = g++
CXXFLAGS = -Wall -std=c++17

SRC_DIR = src
CLIENT_SRC = $(SRC_DIR)/client/main.cpp
SERVER_SRC = $(SRC_DIR)/server/main.cpp
BIN_DIR = bin

CLIENT_BIN = $(BIN_DIR)/client
SERVER_BIN = $(BIN_DIR)/server

all: build run

build: $(CLIENT_BIN) $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_BIN): $(SERVER_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

run:
	@echo "Launching server and client in new terminals..."
	gnome-terminal -- bash -c "$(BIN_DIR)/server; exec bash" &
	gnome-terminal -- bash -c "$(BIN_DIR)/client; exec bash" &

clean:
	rm -rf $(BIN_DIR)

.PHONY: all build run clean

