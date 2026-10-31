CC      = gcc
BUILD   = build
SRC     = $(wildcard *.c)
OBJ     = $(SRC:%.c=$(BUILD)/%.o)
DEP     = $(OBJ:.o=.d)

CFLAGS  = -Wall -Wextra -std=c99 -O2 -MMD -MP
LDFLAGS =

# Use `make DEBUG=1` for a debug build
ifeq ($(DEBUG),1)
    CFLAGS = -Wall -Wextra -std=c99 -g -O0 -MMD -MP
endif

# Platform detection ($(OS) is set to Windows_NT on Windows)
ifeq ($(OS),Windows_NT)
    TARGET  = snake.exe
    LDLIBS  = -lraylib -lopengl32 -lgdi32 -lwinmm
    # Use `make RELEASE=1` to hide the console window
    ifeq ($(RELEASE),1)
        LDFLAGS += -mwindows
    endif
else
    TARGET  = snake
    LDLIBS  = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET)

.PHONY: all run clean

-include $(DEP)