CC = gcc
CFLAGS = -g
CPPFLAGS = -std=gnu90 -I. -Wall -Wextra -pedantic
LIBS = -L. -llist  
       

# OS_HEADERPATH = /student/cmpt332/pthreads
OS_HEADERPATH = .
THREADLIBPATH = .


TARGET = liblist.a s-chat

all: $(TARGET)
	
liblist.a: list_adders.o list_movers.o list_removers.o
	ar -r -c -s liblist.a list_adders.o list_movers.o list_removers.o
	
list_adders.o: list_adders.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_adders.c

list_movers.o: list_movers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_movers.c

list_removers.o: list_removers.c list.h
	$(CC) $(CFLAGS) $(CPPFLAGS) -c list_removers.c


s-chat: s-chat.o
	$(CC) $(CFLAGS) -o s-chat s-chat.o -L$(THREADLIBPATH) \
		-lpthreads $(LIBS)

s-chat.o: s-chat.c
	$(CC) $(CFLAGS) -I$(OS_HEADERPATH) -I. -o s-chat.o -c s-chat.c

# ----------------------------------------------------


clean:
	rm -f *.o reader_writer_test liblist_arm.a liblist_ppc.a \
		liblist.a libMonitor.a s-chat 
