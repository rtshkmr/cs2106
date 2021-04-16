all:	ex4D.exe ex4M.exe ex4.exe
   
ex4D.exe:	mmalloc.h mmalloc_driver.c ex4.c
	gcc -Wall -DDEBUG -o ex4D.exe ex4.c mmalloc_driver.c

ex4M.exe:	mmalloc.h mmalloc_manual.c ex4.c
	gcc -Wall -DDEBUG -o ex4M.exe ex4.c mmalloc_manual.c

ex4.exe:	mmalloc.h mmalloc_driver.c ex4.c
	gcc -Wall -o ex4.exe ex4.c mmalloc_driver.c

clean:
	rm *.exe
