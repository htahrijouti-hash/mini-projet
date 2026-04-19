CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
SRC = src/main.c

ifeq ($(OS),Windows_NT)
    TARGET = Le_Meilleur_Quiz_de_HM.exe
    LIBS = -mwindows -lgdi32
else
    UNAME_S := $(shell uname -s)
    TARGET = le_meilleur_quiz_de_hm
    LIBS =
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(TARGET)

.PHONY: all run clean
