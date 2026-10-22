CC     = gcc
CFLAGS = -Wall -Wextra
TARGET = no_cap_courier
OBJS   = main.o order.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c no_cap_courier.h
	$(CC) $(CFLAGS) -c main.c

order.o: order.c no_cap_courier.h
	$(CC) $(CFLAGS) -c order.c

clean:
	rm -f $(OBJS) $(TARGET)

re: clean all

.PHONY: all clean re
