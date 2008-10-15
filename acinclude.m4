AC_DEFUN([AM_BLABLABLA_VER], [])

AC_DEFUN([CHECK_MYSQL],
[
  AC_MSG_CHECKING([for mysql])

  have_mysql=yes

  AC_ARG_WITH(mysql,
    [  --with-mysql=DIR          base directory where MySQL is installed ],
    [ mysql_lib_dir="$withval/lib/mysql"
      mysql_include_dir="$withval/include" ]
    )

  AC_ARG_WITH(mysql-lib,
    [  --with-mysql-lib=DIR      directory where the MySQL libraries may be found ],
    [ mysql_lib_dir="$withval" ]
    )

  AC_ARG_WITH(mysql-include,
    [  --with-mysql-include=DIR  directory where the MySQL includes may be found ],
    [ mysql_include_dir="$withval" ]
    )

  AC_CHECK_PROG(MYSQL_CONFIG, mysql_config, yes, no)

  dnl try to link to libmysqlclient_r
  if test "x$MYSQL_CONFIG" = "xyes"; then
	mysql_libs="$(mysql_config --libs_r)"
	old_LIBS="$LIBS"
	LIBS="$mysql_libs $LIBS"
	AC_MSG_CHECKING([for mysql_init in -lmysqlclient_r (using mysql_config)])
	AC_TRY_LINK_FUNC(mysql_init, have_libmysqlclient_r=yes)
	LIBS="$old_LIBS"
	if test "x$have_libmysqlclient_r" = "xyes"; then
		AC_MSG_RESULT(yes)
		my_libs="$mysql_libs"
	else
		AC_MSG_RESULT(no)
		have_mysql=no
	fi
  fi

  dnl ############################################################
  dnl # Check for header files
  dnl ############################################################

  if test "x$MYSQL_CONFIG" = "xyes"; then
	mysql_cflags="$(mysql_config --cflags)"
	old_CFLAGS="$CFLAGS"
	CFLAGS="$CFLAGS $mysql_cflags"
	AC_MSG_CHECKING([for mysql.h (using mysql_config)])
	AC_TRY_COMPILE([#include <mysql.h>], [int a = 1;],
		       have_mysql_h=yes)
	if test "x$have_mysql_h" = "xyes"; then
	    AC_MSG_RESULT(yes)
	    my_cflags="$SMART_CFLAGS $mysql_cflags"
	else
	    AC_MSG_RESULT(no)
		have_mysql=no
	fi
	CFLAGS="$old_CFLAGS"
  fi
  
  if test $have_mysql = yes; then
    AC_SUBST(MYSQL_CFLAGS, $my_cflags)
    AC_SUBST(MYSQL_LIBS, $my_libs)
  fi

])



AC_DEFUN([CHECK_ORACLE],
[
  AC_MSG_CHECKING([for oracle])

  dnl the default
  have_oracle=yes

  AC_ARG_WITH(oracle,
  [  --with-oracle=DIR       enable support for Oracle (default ORACLE_HOME)],
  [
    if test $withval = no; then
      have_oracle=no
    elif test $withval != yes; then
      ORACLE_HOME=$withval
    fi
  ], )

  oracle_user_inc=
  AC_ARG_WITH(oracle-includes,
  [  --with-oracle-includes=DIR
                          set oracle include dir (default ORACLE_HOME/subdirs)],
  [
    have_oracle=yes
    oracle_user_inc=$withval
  ], )

  oracle_user_lib=
  AC_ARG_WITH(oracle-libraries,
  [  --with-oracle-libraries=DIR
                          set oracle lib dir (default ORACLE_HOME/lib)],
  [
    have_oracle=yes
    oracle_user_lib=$withval
  ], )

  oracle_user_oci_ver=
  AC_ARG_WITH(oci-version,
  [[  --with-oci-version=[8, 8I, 9I, 10G]
                          this is the version of the client, not the database.]],
  [
    have_oracle=yes
    oracle_user_oci_ver=$withval
  ], )

  oracle_user_instant=
  AC_ARG_WITH(instant-client,
  [[  --with-instant-client
                          define if compiling against Oracle Instant Client.
                          Disables testing for ORACLE_HOME and tnsnames.]],
  [
    have_oracle=yes
    oracle_user_instant=$withval
  ], )

  cflags_ora_save=$CFLAGS
  ldflags_ora_save=$LDFLAGS
  libs_ora_save=$LIBS

  ora_cflags=
  ora_libdir=
  ora_ldflags=
  ora_lib=-lclntsh

  if test $have_oracle = no; then
    dnl yeah, this is backwards.
    AC_MSG_RESULT(no)
  elif test "x$oracle_user_instant" != "x"; then

    if test "x$oracle_user_lib" = "x" && test "x$oracle_user_inc" = "x"; then
      dnl try to find oracle includes for instant client
      dnl these are from the rpm install. they're all i know of so far.

      for dir in `ls /usr/lib/oracle/`; do
        echo "trying $dir" >&5
        if expr $dir \> 10 >/dev/null; then
          oracle_user_oci_ver=10G
        fi
        ora_ldflags="-L/usr/lib/oracle/$dir/client/lib"

        incdir=/usr/include/oracle/$dir/client
        if ! test -d $incdir; then
          AC_MSG_ERROR([$incdir doesn't exist. Please install the sdk package or use --oracle-includes.])
        fi
        ora_cflags="-I$incdir"
	
        break
      done
    fi
  elif test "x$ORACLE_HOME" != "x"; then
    AC_MSG_RESULT($ORACLE_HOME)

    dnl try to find oracle includes
    ora_check_inc="
      $oracle_user_inc
      $ORACLE_HOME/rdbms/demo
      $ORACLE_HOME/plsql/public
      $ORACLE_HOME/rdbms/public
      $ORACLE_HOME/network/public
      $ORACLE_HOME/sdk/
      $ORACLE_HOME/sdk/include/
      $ORACLE_HOME/include/
      /usr/include/oracle"

    for dir in $ora_check_inc; do
      if test -d $dir; then
        ora_cflags="$ora_cflags -I$dir"
      fi
    done

    ora_check_lib="
      $oracle_user_lib
      $ORACLE_HOME/lib
      $ORACLE_HOME/lib32
      $ORACLE_HOME/lib64
      /usr/lib/oracle"

    for dir in $ora_check_lib; do
      if test -d $dir; then
        ora_ldflags="$ora_ldflags -L$dir"
      fi
    done
  else
    dnl test if we have includes or libraries
    if test -z "$oracle_user_lib" || test -z "$oracle_user_inc"; then
       AC_MSG_WARN(no)
       have_oracle=no
     else
      ora_ldflags="-L$oracle_user_lib"
      ora_cflags="-I$oracle_user_inc"
    fi
  fi

  if test "x$ORACLE_HOME" != "x"; then
    dnl check real quick that ORACLE_HOME doesn't end with a slash
    dnl for some stupid reason, the 10g instant client bombs.
    ora_home_oops=`echo $ORACLE_HOME | awk '/\/@S|@/ {print "oops"}'`
    if test "$ora_home_oops" = "oops"; then
      AC_MSG_WARN([Your ORACLE_HOME environment variable ends with a
slash (i.e. /). Oracle 10g Instant Client is known to have a problem
with this.])
    fi
  fi

  if test $have_oracle = yes; then
    AC_MSG_CHECKING([oci works])
    CFLAGS="$CFLAGS $ora_cflags"
    LDFLAGS="$LDFLAGS $ora_ldflags"
    LIBS="$ora_lib"
  
    # i pulled this from one of the examples in the demo dir.
    AC_RUN_IFELSE([[
      #include <oci.h>
      Lda_Def lda;
      ub4     hda [HDA_SIZE/(sizeof(ub4))];
  
      int main(int c, char **v) {
        return 0;
      }
    ]], [found_oracle=yes],
    [found_oracle=no], )
  
    if test $found_oracle = no; then
      AC_MSG_ERROR([Couldn't compile and run a simpile OCI app.
      Try setting ORACLE_HOME or check config.log.
      Otherwise, make sure ORACLE_HOME/lib is in /etc/ld.so.conf or LD_LIBRARY_PATH])
    fi
  
    sqlplus=
    if test -x "$ORACLE_HOME/bin/sqlplus"; then
      sqlplus="$ORACLE_HOME/bin/sqlplus"
    fi
    if test "x${sqlplus}" = "x"; then
      if test -x "$ORACLE_HOME/bin/sqlplusO"; then
        sqlplus="$ORACLE_HOME/bin/sqlplusO"
      fi
    fi
  
    if test "x$oracle_user_oci_ver" != "x"; then
      oci_ver=$oracle_user_oci_ver
    elif test "x${sqlplus}" = "x"; then
      AC_MSG_ERROR([Couldn't find sqlplus. Set the Oracle version manually.])
    else
      # get oracle oci version. know a better way?
      sqlplus_ver=`$sqlplus -? | awk '/Release/ {print @S|@3}'`
      echo "sqlplus_ver: $sqlplus_ver" >&5
  
      if expr $sqlplus_ver \> 10 >/dev/null; then
        dnl our version of oci doesn't have 10g defined yet
        oci_ver=10G
      elif expr $sqlplus_ver \> 9 >/dev/null; then
        oci_ver=9I
      elif expr $sqlplus_ver \< 8.1 >/dev/null; then
        oci_ver=8
      else
        oci_ver=8I
      fi
    fi
  
    ora_cflags="$ora_cflags -DOCI_VERSION${oci_ver}"
  
    # don't change flags for all targets, just export ORA variables.
    CFLAGS=$cflags_ora_save
    AC_SUBST(ORACLE_CFLAGS, $ora_cflags)
  
    LDFLAGS=$ldflags_ora_save
    AC_SUBST(ORACLE_LDFLAGS, $ora_ldflags)
  
    LIBS=$libs_ora_save
    AC_SUBST(ORACLE_LIBS, $ora_lib)
  
    # AM_CONDITIONAL in configure.in uses this variable to enable oracle
    # targets.
    enable_oracle=yes
    AC_MSG_RESULT(yes)
  fi
])


AC_DEFUN([AM_GCONF_SOURCE_2],
[
  AC_PATH_PROG([GCONFTOOL], [gconftool-2], [no])
  
  if test "x$GCONF_SCHEMA_INSTALL_SOURCE" = "x"; then
    GCONF_SCHEMA_CONFIG_SOURCE=`gconftool-2 --get-default-source`
  else
    GCONF_SCHEMA_CONFIG_SOURCE=$GCONF_SCHEMA_INSTALL_SOURCE
  fi

  AC_ARG_WITH(gconf-source,
  [  --with-gconf-source=sourceaddress      Config database for installing schema files.],GCONF_SCHEMA_CONFIG_SOURCE="$withval",)

  AC_SUBST(GCONF_SCHEMA_CONFIG_SOURCE)
  AC_MSG_RESULT([Using config source $GCONF_SCHEMA_CONFIG_SOURCE for schema installation])

  if test "x$GCONF_SCHEMA_FILE_DIR" = "x"; then
    GCONF_SCHEMA_FILE_DIR='$(sysconfdir)/gconf/schemas'
  fi

  AC_ARG_WITH(gconf-schema-file-dir,
  [  --with-gconf-schema-file-dir=dir        Directory for installing schema files.],GCONF_SCHEMA_FILE_DIR="$withval",)

  AC_SUBST(GCONF_SCHEMA_FILE_DIR)
  AC_MSG_RESULT([Using $GCONF_SCHEMA_FILE_DIR as install directory for schema files])

  AC_ARG_ENABLE(schemas-install,
     [  --disable-schemas-install       Disable the schemas installation],
     [case "${enableval}" in
       yes) schemas_install=true ;;
       no)  schemas_install=false ;;
       *) AC_MSG_ERROR(bad value ${enableval} for --disable-schemas-install) ;;
     esac],[schemas_install=true])
     AM_CONDITIONAL(GCONF_SCHEMAS_INSTALL, test x$schemas_install = xtrue)
])

