SHELL = /bin/bash

DIR = $(EXTERNAL_DIR)
SUB_NAME = "External"

# define print
# printf "%b%s%b\n" "$(1)" "$(2)" "$(NO_COLOR)"
# endef


all: entis

.PHONY : clean
clean: entis_clean

.PHONY : entis
entis:
	cd entis && $(MAKE)
	mkdir ../build
	cp entis/build/libentis.a ../build/

.PHONY : entis_clean
entis_clean:
	cd entis && $(MAKE) clean


