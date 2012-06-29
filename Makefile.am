ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = gnu

# include aminclude.am

SUBDIRS = bits++ docs

EXTRA_DIST = bits++.spec m4/Doxygen.m4 aminclude.am TODO defs

PHONY: release rpm rpm-newer 

release:
	echo $(VERSION) | sed "s/-.*$$/-$$( expr $$( echo $(VERSION) | sed 's/^[0-9\.]*-//' | sed 's/[^0-9]\+//' )  + 1 )/"  | xargs -i{}  echo sed -ibak "s/$$( echo $(VERSION) | sed 's/[a-zA-Z]\+$$//')/{}/g"   configure.ac | sh
	echo Release: $$( echo $(VERSION) | sed 's/^[0-9\.]*-//' | sed 's/[^0-9]\+//')/Release: $$( expr $$( echo $(VERSION) | sed 's/^[0-9\.]*-//' | sed 's/[^0-9]\+//')  + 1 )  | xargs -i{}  echo sed -ibak \"s/{}/g\"   $(PACKAGE).spec | sh


rpm: distcheck 
	make rpm-newer

rpm-newer: 
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz


