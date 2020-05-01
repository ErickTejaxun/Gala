#fichero Makefile

OBJ = sintactico.o lexico.o 

gala : $(OBJ)     #segunda fase de la tradicción. Generación del código ejecutable 
	g++ -o gala $(OBJ)
	#./expresiones entrada.txt resultados.txt

sintactico.o : sintactico.c        #primera fase de la traducción del analizador sintáctico
	g++ -c -o sintactico.o  sintactico.c 
	
lexico.o : lex.yy.c		#primera fase de la traducción del analizador léxico
	g++ -c -o lexico.o  lex.yy.c 	

sintactico.c : sintactico.y       #obtenemos el analizador sintáctico en C
	bison -d   -v sintactico.y -o sintactico.c

lex.yy.c: lexico.l	#obtenemos el analizador léxico en C
	flex lexico.l

clean : 
	rm  -f  *.c *.o 
	rm  sintactico.h gala
	rm  sintactico.output
