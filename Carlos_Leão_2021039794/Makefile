CC = gcc
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
ASSETS = ./assets
OUT = ./out
OBJS = $(OBJ)/matop.o $(OBJ)/mat.o $(OBJ)/memlog.o
HDRS = $(INC)/mat.h $(INC)/memlog.h $(INC)/msgassert.h
CFLAGS = -pg -Wall -c -I$(INC)
ANALISAMEM = ./analisamem/bin/analisamem

EXE = $(BIN)/matop

all:  mem perf gprof

mem: $(EXE)
	$(EXE) -s -1 $(ASSETS)/m1_5.txt -2 $(ASSETS)/m2_5.txt -o $(OUT)/resSum.txt -p /tmp/somalog.out -l 
	rm -rf /tmp/somadin
	mkdir /tmp/somadin
	$(ANALISAMEM) -i /tmp/somalog.out -p /tmp/somadin/somadin
	gnuplot /tmp/somadin/*.gp

	$(EXE) -m -1 $(ASSETS)/m1_5.txt -2 $(ASSETS)/m2_5.txt -o $(OUT)/resMult.txt -p /tmp/multlog.out -l 
	rm -rf /tmp/multdin
	mkdir /tmp/multdin
	$(ANALISAMEM) -i /tmp/multlog.out -p /tmp/multdin/multdin
	gnuplot /tmp/multdin/*.gp

	$(EXE) -t -1 $(ASSETS)/m1_5.txt -o $(OUT)/resTrans.txt -p /tmp/translog.out -l 
	rm -rf /tmp/transdin
	mkdir /tmp/transdin
	$(ANALISAMEM) -i /tmp/translog.out -p /tmp/transdin/transdin
	gnuplot /tmp/transdin/*.gp


perf: $(EXE)
	$(EXE) -s -1 $(ASSETS)/m1_1000.txt -2 $(ASSETS)/m2_1000.txt -o $(OUT)/resSum1000.txt -p /tmp/soma1000.out
	$(EXE) -m -1 $(ASSETS)/m1_1000.txt -2 $(ASSETS)/m2_1000.txt -o $(OUT)/resMult1000.txt -p /tmp/mult1000.out
	$(EXE) -t -1 $(ASSETS)/m1_1000.txt -o $(OUT)/resTrans1000.txt -p /tmp/trans1000.out
	# nao foi possivel enviar todos os arquivos de texto das execucoes seguintes
	# $(EXE) -s -1 $(ASSETS)/m1_900.txt -2 $(ASSETS)/m2_900.txt -o $(OUT)/resSum900.txt -p /tmp/soma900.out
	# $(EXE) -m -1 $(ASSETS)/m1_900.txt -2 $(ASSETS)/m2_900.txt -o $(OUT)/resMult900.txt -p /tmp/mult900.out
	# $(EXE) -t -1 $(ASSETS)/m1_900.txt -o $(OUT)/resTrans900.txt -p /tmp/trans900.out
	# $(EXE) -s -1 $(ASSETS)/m1_800.txt -2 $(ASSETS)/m2_800.txt -o $(OUT)/resSum800.txt -p /tmp/soma800.out
	# $(EXE) -m -1 $(ASSETS)/m1_800.txt -2 $(ASSETS)/m2_800.txt -o $(OUT)/resMult800.txt -p /tmp/mult800.out
	# $(EXE) -t -1 $(ASSETS)/m1_800.txt -o $(OUT)/resTrans800.txt -p /tmp/trans800.out
	# $(EXE) -s -1 $(ASSETS)/m1_700.txt -2 $(ASSETS)/m2_700.txt -o $(OUT)/resSum700.txt -p /tmp/soma700.out
	# $(EXE) -m -1 $(ASSETS)/m1_700.txt -2 $(ASSETS)/m2_700.txt -o $(OUT)/resMult700.txt -p /tmp/mult700.out
	# $(EXE) -t -1 $(ASSETS)/m1_700.txt -o $(OUT)/resTrans700.txt -p /tmp/trans700.out
	# $(EXE) -s -1 $(ASSETS)/m1_600.txt -2 $(ASSETS)/m2_600.txt -o $(OUT)/resSum600.txt -p /tmp/soma600.out
	# $(EXE) -m -1 $(ASSETS)/m1_600.txt -2 $(ASSETS)/m2_600.txt -o $(OUT)/resMult600.txt -p /tmp/mult600.out
	# $(EXE) -t -1 $(ASSETS)/m1_600.txt -o $(OUT)/resTrans600.txt -p /tmp/trans600.out
	# $(EXE) -s -1 $(ASSETS)/m1_500.txt -2 $(ASSETS)/m2_500.txt -o $(OUT)/resSum500.txt -p /tmp/soma500.out
	# $(EXE) -m -1 $(ASSETS)/m1_500.txt -2 $(ASSETS)/m2_500.txt -o $(OUT)/resMult500.txt -p /tmp/mult500.out
	# $(EXE) -t -1 $(ASSETS)/m1_500.txt -o $(OUT)/resTrans500.txt -p /tmp/trans500.out
	

gprof: $(BIN)/matop

	$(EXE) -s -1 $(ASSETS)/m1_1000.txt -2 $(ASSETS)/m2_1000.txt -o $(OUT)/resSum.txt -p /tmp/soma1000gprof.out 
	gprof $(EXE) gmon.out > /tmp/soma1000gprof.txt
	$(EXE) -m -1 $(ASSETS)/m1_1000.txt -2 $(ASSETS)/m2_1000.txt -o $(OUT)/resMult.txt -p /tmp/mult1000gprof.out 
	gprof $(EXE) gmon.out > /tmp/mult1000gprof.txt
	$(EXE) -t -1 $(ASSETS)/m1_1000.txt -o $(OUT)/resMult.txt -p /tmp/trans1000gprof.out 
	gprof $(EXE) gmon.out > /tmp/trans1000gprof.txt
	

$(BIN)/matop: $(OBJS)
	$(CC) -pg -o $(BIN)/matop $(OBJS) $(LIBS)

$(OBJ)/matop.o: $(HDRS) $(SRC)/matop.c
	$(CC) $(CFLAGS) -o $(OBJ)/matop.o $(SRC)/matop.c 

$(OBJ)/mat.o: $(HDRS) $(SRC)/mat.c
	$(CC) $(CFLAGS) -o $(OBJ)/mat.o $(SRC)/mat.c 

$(OBJ)/memlog.o: $(HDRS) $(SRC)/memlog.c
	$(CC) $(CFLAGS) -o $(OBJ)/memlog.o $(SRC)/memlog.c 
	
clean:
	rm -f $(EXE) $(OBJS) gmon.out