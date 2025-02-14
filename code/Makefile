#---------------------------------------------------------------------------------
.SUFFIXES:
#---------------------------------------------------------------------------------
export TARGET	:=	$(shell basename $(CURDIR))
export TOPDIR	:=	$(CURDIR)

.PHONY: checklibtwl checklibnds checkbootstrap checktest clean

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
all: checklibtwl checklibnds checkbootstrap

debug: all checktest

#---------------------------------------------------------------------------------
checklibtwl:
	$(MAKE) -C libs/libtwl

#---------------------------------------------------------------------------------
checklibnds:
	$(MAKE) -C libs/libnds

#---------------------------------------------------------------------------------
checkbootstrap: checklibtwl
	$(MAKE) -C bootstrap

#---------------------------------------------------------------------------------
checktest:
	$(MAKE) -C test

#---------------------------------------------------------------------------------
clean:
	$(MAKE) -C libs/libtwl clean
	$(MAKE) -C libs/libnds clean
	$(MAKE) -C bootstrap clean
	$(MAKE) -C test clean
