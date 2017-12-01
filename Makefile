BINDIR = bin
SRCDIR = src
INCLUDEDIR = include
APPDIR = application
OBJDIR = build
TESTDIR = test
IMGDIR = images
INPUT = input
OUTPUT = output

PIC1 = Catarata
PIC2 = Catarata2
PIC3 = Normal
PIC4 = Normal2

CC = gcc
CFLAGS = -O3 -Wall -ansi -pedantic -I $(INCLUDEDIR) -lm
LDFLAGS =

BIN = ${BINDIR}/catarata
APP = ${APPDIR}/main.c

SRC = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))
APPOBJ = $(patsubst $(APPDIR)/%.c,$(OBJDIR)/%.o,$(APP))

_TESTS = $(wildcard $(TESTDIR)/*.c)
TESTS = $(patsubst %.c,%,$(_TESTS))

$(BIN): $(OBJS) $(APPOBJ)
	$(CC) -o $(BIN) $(APPOBJ) $(OBJS) $(CFLAGS) $(LDFLAGS)

$(APPOBJ): $(APP)
	$(CC) -c -o $@ $< $(CFLAGS)

${OBJDIR}/%.o: $(SRCDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(TESTS) 
	$(info ************  Testes concluídos com sucesso! ************)

$(TESTDIR)/t_%: $(TESTDIR)/t_%.c $(OBJS)
	$(CC) -o $@ $< $(OBJS) $(CFLAGS) $(LDFLAGS)
	$@

exec: $(BIN)
	./$(BIN) -i $(IMGDIR)/$(INPUT)/$(PIC1).ppm -s $(IMGDIR)/$(OUTPUT)/$(PIC1).ppm
	./$(BIN) -i $(IMGDIR)/$(INPUT)/$(PIC2).ppm -s $(IMGDIR)/$(OUTPUT)/$(PIC2).ppm
	./$(BIN) -i $(IMGDIR)/$(INPUT)/$(PIC3).ppm -s $(IMGDIR)/$(OUTPUT)/$(PIC3).ppm
	./$(BIN) -i $(IMGDIR)/$(INPUT)/$(PIC4).ppm -s $(IMGDIR)/$(OUTPUT)/$(PIC4).ppm

clean:
	rm -f $(BIN) $(OBJS) $(APPOBJ)
	rm -f $(TESTS) $(LDFLAGS)
