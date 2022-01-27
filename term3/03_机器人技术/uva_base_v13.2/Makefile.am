## Makefile.am for pursuit server top directory

## process subdirecty src
SUBDIRS=src 

## add configuration file of doxygen to distribution
EXTRA_DIST=README start.sh windows/start-team.bat windows/bcc32.cfg windows/ilink32.cfg  windows/Makefile  


## make doc will create doxygen output (if doxygen is installed)
doc: 
	doxygen doc/doxygen.cfg

## Clean doc/html directory where doxygen places its output
CLEANFILES=doc/html/*

## to make sure make doc will always call doxygen rule
.PHONY: doc
