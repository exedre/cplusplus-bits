ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = gnu 

docdir=$(prefix)/share/doc/@PACKAGE_NAME@

EXTRA_DIST = $(man_MANS) Doxyfile.in

# ensure doxygen-build.stamp is included,  or else the documentation will
# not be built or distributed
noinst_DATA = doxygen-build.stamp

#html: 

if HAVE_DOXYGEN
doxygen-build.stamp: Doxyfile $(top_srcdir)/@PACKAGE_NAME@/*.*pp 
	doxygen
	touch doxygen-build.stamp
	(cd @PACKAGE_NAME@ && sed -i -e "s/charset=iso-8859-1/charset=utf-8/g" html/*html )
else
doxygen-build.stamp:
	echo "*** Warning: Doxygen not found; documentation will not be built."
	touch doxygen-build.stamp
endif

dist_docdir = $(distdir)/@PACKAGE_NAME@



publish:
	rsync -av @PACKAGE_NAME@/ stux20:/_saee/ufficio/public_html/dev/@PACKAGE_NAME@/



dist-hook:
	if test -d @PACKAGE_NAME@; then \
	  mkdir $(dist_docdir); \
	  for dir in @PACKAGE_NAME@/*; do \
            b=`basename $$dir`; echo $b ;\
            if test $$b != "CVS"; then \
              if test -d $$dir; then \
                mkdir $(dist_docdir)/$$b; \
	        for f in $$dir/*; do \
                  cp -pr $$f $(dist_docdir)/$$b; \
                done \
              fi \
            fi \
          done \
	fi


install-data-local: doxygen-build.stamp
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	if test -d @PACKAGE_NAME@; then \
	  if test -d @PACKAGE_NAME@/html; then \
	      $(mkinstalldirs) -m 0755 $(DESTDIR)$(docdir)/html; \
	      for f in @PACKAGE_NAME@/html/*; do \
				$(INSTALL_DATA) -v $$f $(DESTDIR)$(docdir)/html; \
            done \
	  fi; \
	  if test -d @PACKAGE_NAME@/man; then \
	      $(mkinstalldirs) -m 0755 $(DESTDIR)$(docdir)/man; \
	      for f in @PACKAGE_NAME@/html/*; do \
				$(INSTALL_DATA) -v $$f $(DESTDIR)$(docdir)/man; \
            done \
	  fi; \
	fi

uninstall-local:
	rm -rf $(DESTDIR)$(docdir)

clean-local:
	if test -d @PACKAGE_NAME@; then rm -rf @PACKAGE_NAME@; fi
	if test -f doxygen-build.stamp; then rm -f doxygen-build.stamp; fi
