CFLAGS += $(shell pkg-config --cflags glib-2.0)
LIBS += $(shell pkg-config --libs glib-2.0)
glib_test:glib_test.c
	gcc -g -Wall glib_test.c $(CFLAGS) -o glib_test $(LIBS)
clean:
	rm -f glib_test
