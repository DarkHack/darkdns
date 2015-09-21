dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(darkdns, 0.1, www.darkweb.fr)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE(darkdns, 0.1)
#AC_CONFIG_SRCDIR([trunk/src/main.c])
#AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([./m4])


# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB

# Checks for libraries.
# autoreconf -ivf
# Makefile.am in folder of file.c invoke lib :
# AM_CFMAGS = @POSTGRESQL_CFLAGS@
# AM_CPPFLAGS = @POSTGRESQL_CFLAGS@
# AM_LDFLAGS = @POSTGRESQL_LDFLAGS@ @POSTGRESQL_LIBS@
# LDADD = -lpq
AC_ARG_WITH([postgresql],
        AS_HELP_STRING([--with-postgresql=@<:@ARG@:>@],
            [use PostgreSQL library @<:@default=yes@:>@, optionally specify path to pg_config]
        ),
        [
        if test "$withval" = "no"; then
            AC_MSG_ERROR([Damns no postgres? why use this programme?])
        fi
        ],
        [want_postgresql="yes"]
    )
AX_LIB_POSTGRESQL

# Checks for header files.
AC_CHECK_HEADERS

#AC_CHECK_HEADERS([postgresql/libpq-fe.h libpq-fe.h], [break])
#if test x"$ac_cv_header_postgresql_libpq_fe_h" != xyes -a x"$ac_cv_header_libpq_fe_h" != xyes
#then
#AC_MSG_ERROR([PostgreSQL headers not found or not usable])
#fi

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CONFIG_FILES(
 Makefile
 build/kernel/Makefile
 build/kernel/memory/Makefile
 build/kernel/buffer/Makefile
 build/kernel/bdd/Makefile
 build/Makefile
)
AC_OUTPUT

