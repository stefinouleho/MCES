CFLAGS=-g -Wall
#CFLAGS=-O2 -Wall

run: mesure_similarite
	./mesure_similarite 28973 4672 600 1600

val: mesure_similarite
	valgrind --leak-check=full --show-leak-kinds=all ./mesure_similarite 28973 4672 20 600	


mesure_similarite: mesure_similarite.o lecture_molecule_sdf.o fonctions_mces.o
	gcc ${CFLAGS} mesure_similarite.o lecture_molecule_sdf.o fonctions_mces.o -o mesure_similarite
	

mesure_similarite.o: mesure_similarite.c mesure_similarite.h
	gcc ${CFLAGS} -c mesure_similarite.c

fonctions_mces.o: fonctions_mces.c fonctions_mces.h 
	gcc ${CFLAGS} -c fonctions_mces.c
	
lecture_molecule_sdf.o: lecture_molecule_sdf.c lecture_molecule_sdf.h
	gcc ${CFLAGS} -c lecture_molecule_sdf.c
	
clean: 
	rm -f mesure_similarite
	rm -f mesure_similarite.o
	rm -f lecture_molecule_sdf.o
	rm -f fonctions_mces.o

