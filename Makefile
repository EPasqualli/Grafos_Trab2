CC = gcc
CFLAGS = -g -Wall -std=c11

TARGET = analise
SOURCES = analise.c leitor_rede.c grafo.c lista.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = grafo.h lista.h leitor_rede.h

all: $(TARGET)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

analise.o: analise.c $(HEADERS)
leitor_rede.o: leitor_rede.c $(HEADERS)
grafo.o: grafo.c grafo.h lista.h
lista.o: lista.c lista.h

clean:
	rm -f $(OBJECTS) $(TARGET) *~

purge: clean
	rm -f $(TARGET)