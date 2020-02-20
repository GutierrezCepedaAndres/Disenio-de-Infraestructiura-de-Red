DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIR := dirs/

CC := mpicc
CR := mpirun


all : dirs RedToroide RedHipercubo

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRHEA) $(DIR)

RedToroide: 
	$(CC) $(DIRSRC)/RedToroide.c -o $(DIROBJ)/RedToroide.o
	$(CC) $(DIRSRC)/RedToroide.c -o $(DIREXE)/RedToroide

RedHipercubo: 
	$(CC) $(DIRSRC)/RedHipercubo.c -o $(DIROBJ)/RedHipercubo.o
	$(CC) $(DIRSRC)/RedHipercubo.c -o $(DIREXE)/RedHipercubo

testToroide:
	$(CR) -n 16 ./$(DIREXE)RedToroide

testHipercubo:
	$(CR) -n 8 ./$(DIREXE)RedHipercubo

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA) 

cleanDirs : 
	rm -rf *~ core $(DIR)