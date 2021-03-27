all:	ex3D.exe ex3M.exe ex3.exe
   
ex3D.exe:	mmalloc.h mmalloc_driver.c ex3.c
	gcc -Wall -DDEBUG -o ex3D.exe ex3.c mmalloc_driver.c

ex3M.exe:	mmalloc.h mmalloc_manual.c ex3.c
	gcc -Wall -DDEBUG -o ex3M.exe ex3.c mmalloc_manual.c

ex3.exe:	mmalloc.h mmalloc_driver.c ex3.c
	gcc -Wall -o ex3.exe ex3.c mmalloc_driver.c

clean:
	rm *.exe
