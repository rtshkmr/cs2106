all:	ex5D.exe ex5.exe
   
ex5D.exe:	mmalloc.h mmalloc_driver.c ex5.c
	gcc -Wall -DDEBUG -o ex5D.exe ex5.c mmalloc_driver.c -lpthread

ex5.exe:	mmalloc.h mmalloc_driver.c ex5.c
	gcc -Wall -o ex5.exe ex5.c mmalloc_driver.c -lpthread

clean:
	rm *.exe
