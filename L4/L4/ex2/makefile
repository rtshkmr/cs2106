all:	ex2D.exe ex2M.exe ex2.exe
   
ex2D.exe:	mmalloc.h mmalloc_driver.c ex2.c
	gcc -Wall -DDEBUG -o ex2D.exe ex2.c mmalloc_driver.c

ex2M.exe:	mmalloc.h mmalloc_manual.c ex2.c
	gcc -Wall -DDEBUG -o ex2M.exe ex2.c mmalloc_manual.c

ex2.exe:	mmalloc.h mmalloc_driver.c ex2.c
	gcc -Wall -o ex2.exe ex2.c mmalloc_driver.c

clean:
	rm *.exe
