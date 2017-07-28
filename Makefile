CFLAGS=-g -Wall
#CFLAGS=-O2 -Wall

CC = gcc -Wall -Wextra -ansi
CXX = g++ -Wall -Wextra -ansi

run: mesure_similarite
	./mesure_similarite 28973 4672 600 1600

val: mesure_similarite
	valgrind --leak-check=full --show-leak-kinds=all ./mesure_similarite 28973 4672 20 600	

mesure_similarite: mesure_similarite.o lecture_molecule_sdf.o fonctions_mces.o graph.o
	gcc ${CFLAGS} -o $@ $^
mesure_similarite_scip : mesure_similarite.o lecture_molecule_sdf.o fonctions_mces.o graph.o helpers/proglin_helper_scip.o helpers/sciplib.a
	$(CXX) -I helpers/scip -o $@ $^ -lpopt -lgmp -lm -lz -lreadline -lncurses 	

mesure_similarite.o: mesure_similarite.c
	gcc ${CFLAGS} -c mesure_similarite.c

fonctions_mces.o: fonctions_mces.c fonctions_mces.h 
	gcc ${CFLAGS} -c fonctions_mces.c
	
lecture_molecule_sdf.o: lecture_molecule_sdf.c lecture_molecule_sdf.h
	gcc ${CFLAGS} -c lecture_molecule_sdf.c

helpers/proglin_helper_scip.o : helpers/proglin_helper_scip.c helpers/proglin_helper.h helpers/sciplib.a
	$(CC) -I helpers/scip -o $@ -c $<

	
clean: 
	rm -f mesure_similarite
	rm -f *.o
	rm -f helpers/*.o

