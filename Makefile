objects = obj/lottery.o obj/buyer.o obj/issue.o obj/menu.o obj/main.o

bin/main:$(objects)
	gcc $(objects) -o bin/main -Wall -g
obj/main.o:src/main.c include/menu.h
	gcc -c src/main.c -Iinclude -o obj/main.o -Wall
obj/menu.o:src/menu.c include/menu.h
	gcc -c src/menu.c -Iinclude -o obj/menu.o -Wall
obj/issue.o:src/issue.c include/issue.h
	gcc -c src/issue.c -Iinclude -o obj/issue.o -Wall
obj/buyer.o:src/buyer.c include/buyer.h
	gcc -c src/buyer.c -Iinclude -o obj/buyer.o -Wall
obj/lottery.o:src/lottery.c include/lottery.h
	gcc -c src/lottery.c -Iinclude -o obj/lottery.o -Wall
clean:
	rm bin/main $(objects)



