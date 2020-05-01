#fichero Makefile

FUENTE = ./gala.cpp

fin: prueba
	./prueba 
	
prueba : prueba.o entorno.o 
	g++ -oprueba entorno.o prueba.o -lallegro -lallegro_acodec -lallegro_audio -lallegro_color -lallegro_dialog -lallegro_font -lallegro_image -lallegro_main -lallegro_memfile -lallegro_physfs -lallegro_primitives -lallegro_ttf

entorno_dspl.o : entorno.cpp
	g++ -c entorno.cpp

prueba.o : $(FUENTE)
	g++ -c -oprueba.o $(FUENTE) 

clean :
	rm -f *.o	
