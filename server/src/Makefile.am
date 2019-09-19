AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = server
server_SOURCES = server.c
threads_LIB = -lpthread
threads_CFLAGS = -pthread
server_LDADD = $(top_builddir)/lib/libgpiorasp2.la $(threads_LIB)

AM_CFLAGS = $(threads_CFLAGS)