
BUILD_DIR = build
EXECUTABLE = $(BUILD_DIR)/main

CFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c11
CC = gcc $(CFLAGS)
SOURCES = src/main.c

all: app

.PHONY: run
run: $(EXECUTABLE)
	@$(EXECUTABLE)

.PHONY: app
app: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(BUILD_DIR)
	$(CC) $(SOURCES) -o $(EXECUTABLE)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)
