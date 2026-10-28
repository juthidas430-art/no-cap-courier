CC     = gcc
CFLAGS = -Wall -Wextra
TARGET = no_cap_courier
OBJS   = main.o util.o shop.o user.o order.o runner.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c no_cap_courier.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

re: clean all

.PHONY: all clean re
