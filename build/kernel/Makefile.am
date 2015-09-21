AM_CFMAGS = @POSTGRESQL_CFLAGS@
AM_CPPFLAGS = @POSTGRESQL_CFLAGS@
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@

LDADD = -lpq

#noinst_LIBRARIES = libkernel.a
lib_LIBRARIES      = libkernel.a
libkernel_a_SOURCES = $(top_builddir)/trunk/kernel/memory/memory.c $(top_builddir)/trunk/kernel/buffer/buffer.c $(top_builddir)/trunk/kernel/buffer/androBuffer.c $(top_builddir)/trunk/kernel/logger/logger.c $(top_builddir)/trunk/kernel/string/string.c $(top_builddir)/trunk/kernel/conf/conf.c $(top_builddir)/trunk/kernel/socket/udp/udp.c $(top_builddir)/trunk/kernel/bdd/bdd.c $(top_builddir)/trunk/kernel/kernel.c