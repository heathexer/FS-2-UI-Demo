curses-ui-demo : curses-ui-demo.o
	cc -o curses-ui-demo curses-ui-demo.o -lncursesw

curses-ui-demo.o : curses-ui-demo.c
	cc -c curses-ui-demo.c -lncursesw

clean :
	rm curses-ui-demo.o curses-ui-demo