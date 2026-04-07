# Compilador a utilizar
CC = gcc

# Opciones de compilación (Advertencias habilitadas y estándar C11)
CFLAGS = -Wall -Wextra -std=c11

# Detección del sistema operativo para compatibilidad Windows/Linux
ifeq ($(OS),Windows_NT)
TARGET = proyecto1.exe
RM = del /Q /F
else
TARGET = proyecto1
RM = rm -f
endif

# Archivos objeto requeridos
OBJS = main.o deportista.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(TARGET)
