# https://stackoverflow.com/a/18258352/13830772
rwildcard = $(filter-out \ ,$(foreach pattern,$(2),$(wildcard $(1)/$(pattern)))$(foreach child,$(wildcard $(1)/*),$(call rwildcard,$(child),$(2))))

BUILD_DIR = build
EXECUTABLE = $(BUILD_DIR)/main
WF ?= 
WD ?= .

CFLAGS = -W -Wall -Wextra -Werror -pedantic -std=c11
CC = gcc $(CFLAGS)
SOURCES = $(call rwildcard,src,*.c)

all: app

.PHONY: run
run: $(EXECUTABLE)
	@cd $(WD) && $(EXECUTABLE) $(WF)

.PHONY: app
app: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES) $(BUILD_DIR)
	$(CC) $(SOURCES) -o $(EXECUTABLE)

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)
