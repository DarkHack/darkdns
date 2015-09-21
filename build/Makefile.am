AM_CFMAGS = @POSTGRESQL_CFLAGS@
AM_CPPFLAGS = @POSTGRESQL_CFLAGS@
AM_LDFLAGS = @POSTGRESQL_LDFLAGS@

LDADD = $(top_builddir)/build/kernel/libkernel.a -lpq
INCLUDES = -I$(top_builddir)/build/kernel

bin_PROGRAMS = darkdns
darkdns_SOURCES = $(top_builddir)/trunk/dns/dns.c $(top_builddir)/trunk/dns/DNSClient.c $(top_builddir)/trunk/dns/DNSServer.c $(top_builddir)/trunk/test.c




