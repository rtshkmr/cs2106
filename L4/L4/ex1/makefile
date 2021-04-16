all:	ex1D.exe ex1M.exe ex1.exe
   
ex1D.exe:	mmalloc.h mmalloc_driver.c ex1.c
	gcc -Wall -DDEBUG -o ex1D.exe ex1.c mmalloc_driver.c

ex1M.exe:	mmalloc.h mmalloc_manual.c ex1.c
	gcc -Wall -DDEBUG -o ex1M.exe ex1.c mmalloc_manual.c

ex1.exe:	mmalloc.h mmalloc_driver.c ex1.c
	gcc -Wall -o ex1.exe ex1.c mmalloc_driver.c

clean:
	rm *.exe
