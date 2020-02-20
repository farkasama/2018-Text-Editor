CC  = gcc -Wall -g 
UNIT = -pthread -lcheck_pic -pthread -lrt -lm -lsubunit
LDLIBS = $(shell pkg-config --libs check)
#mettre ici le nom de Exec, il va chercher le nom.c 
#et le compiler avec les dependances donnees
EXEC= hello testBuff
CHECK = -fprofile-arcs -ftest-coverage

#prend tout les fichiers .h,filter-out: exclusion,
LIB = $(filter-out head.h,$(wildcard *.h))

#donne les noms .h ->.o
OBJ = $(LIB:.h=.o)

MPR = mrproper
CLN = clean

all: $(LIB) $(EXEC) 


# ajout de dependances
testBuff : DLinkedList.o fileio.o editor_buffer.o $(LDLIBS)
hello : DLinkedList.o editor_buffer.o window.o fileio.o -lncurses
editor_buffer.o : DLinkedList.o
# fin 

#compiler les Execs
$(EXEC):%: %.c
	$(CC) -o $@ $^

#regle pour les fichiers .h
$(LIB):%.h: %.c

#regle pour les future fichiers .o
$(OBJ):%.o: %.c %.h
	$(CC) -c -o $@ $*.c


getCut: $(UNIT)
	gcc $@.c $^ -lncurses

#ce qui n'est pas un fichier
.PHONY: all clean mrproper $(UNIT)

#ne garde que les Execs
clean:
	rm -f *.o *~

#supprime toutes generation de make ()
mrproper: 
	rm -f $(EXEC) *.out *.o
