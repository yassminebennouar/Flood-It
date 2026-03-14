all: Flood-It

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc -c API_Gene_instance.c `sdl2-config --cflags`

sequence_alea_recursive.o: sequence_alea_recursive.c sequence_alea_recursive.h
	gcc -c sequence_alea_recursive.c `sdl2-config --cflags`
	
S_Zsg.o: S_Zsg.c S_Zsg.h
	gcc -c S_Zsg.c `sdl2-config --cflags`
sequence_alea_rapide.o: sequence_alea_rapide.c sequence_alea_rapide.h
	gcc -c sequence_alea_rapide.c `sdl2-config --cflags`

strategie_max_bordure.o: strategie_max_bordure.c strategie_max_bordure.h
	gcc -c strategie_max_bordure.c `sdl2-config --cflags`

strategie_max_zone.o: strategie_max_zone.c strategie_max_zone.h
	gcc -c strategie_max_zone.c `sdl2-config --cflags`

bonus_strategie.o: bonus_strategie.c bonus_strategie.h
	gcc -c bonus_strategie.c `sdl2-config --cflags`

API_Grille.o: API_Grille.c API_Grille.h
	gcc -c API_Grille.c `sdl2-config --cflags`

Liste_case.o: Liste_case.c Liste_case.h
	gcc -c Liste_case.c `sdl2-config --cflags`

Flood-It.o: Flood-It.c
	gcc -c Flood-It.c `sdl2-config --cflags`

Flood-It: Flood-It.o Liste_case.o API_Grille.o API_Gene_instance.o \
          sequence_alea_recursive.o sequence_alea_rapide.o \
          strategie_max_bordure.o strategie_max_zone.o bonus_strategie.o \
          S_Zsg.o
	gcc -o Flood-It Flood-It.o Liste_case.o API_Grille.o API_Gene_instance.o \
           sequence_alea_recursive.o sequence_alea_rapide.o \
           strategie_max_bordure.o strategie_max_zone.o bonus_strategie.o \
           S_Zsg.o \
           `sdl2-config --libs`
clean:
	rm -f *.o Flood-It

