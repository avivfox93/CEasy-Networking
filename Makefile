
CC = gcc
OBJS = Main.o network_utils.o ssl_connect.o
EXEC = prog
COMP_FLAG = -std=c99 -Wall -Werror

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $@

main.o: main.c network_utils.o ssl_connect.o
	$(CC) -lssl -lcrypto -c $(COMP_FLAG) $*.c

network_utils.o: network_utils.c

ssl_connect.o: ssl_connect.c

clean:
	rm -f $(OBJS) $(EXEC)
