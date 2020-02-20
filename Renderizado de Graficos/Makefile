DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIR := dirs/

CC := mpicc
CR := mpirun
CF := -lX11


all : dirs Pract2

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRHEA) $(DIR)

Pract2: 
	$(CC) $(DIRSRC)/pract2.c -o $(DIROBJ)/pract2.o $(CF)
	$(CC) $(DIRSRC)/pract2.c -o $(DIREXE)/pract2 $(CF)

testBase:
	$(CR) -np 1 ./$(DIREXE)pract2 n

testBlancoYNegro:
	$(CR) -np 1 ./$(DIREXE)pract2 b

testSepia:
	$(CR) -np 1 ./$(DIREXE)pract2 s

testAzul:
	$(CR) -np 1 ./$(DIREXE)pract2 A

testVerde:
	$(CR) -np 1 ./$(DIREXE)pract2 V

testRojo:
	$(CR) -np 1 ./$(DIREXE)pract2 R

testAmarillo:
	$(CR) -np 1 ./$(DIREXE)pract2 a

testVioleta:
	$(CR) -np 1 ./$(DIREXE)pract2 v

testCyan:
	$(CR) -np 1 ./$(DIREXE)pract2 c

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA) 

cleanDirs : 
	rm -rf *~ core $(DIR)