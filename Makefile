CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -Werror
PRJ=hinfosvc

hinfosvc: $(PRJ).c
	$(CC) $(CFLAGS) $(PRJ).c -o $(PRJ)

clean:
	rm -f $(PRJ)