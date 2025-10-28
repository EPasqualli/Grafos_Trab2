CC = gcc
CFLAGS = -g -Wall -std=c11

TARGET = analise
SOURCES = analise.c leitor_rede.c grafo.c lista.c
OBJECTS = $(SOURCES:.c=.o)
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

analise.o: analise.c leitor_rede.h grafo.h lista.h
leitor_rede.o: leitor_rede.c leitor_rede.h grafo.h lista.h
grafo.o: grafo.c grafo.h lista.h
lista.o: lista.c lista.h

clean:
	rm -f $(OBJECTS) $(TARGET) *~