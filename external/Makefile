SHELL = /bin/bash

DIR = $(EXTERNAL_DIR)
SUB_NAME = "External"

# define print
# printf "%b%s%b\n" "$(1)" "$(2)" "$(NO_COLOR)"
# endef


ifeq ($(PNG),0)
all: entis
else
all: libpng 
endif

.PHONY : clean
ifeq ($(PNG),0)
clean: entis_clean
else
clean: libpng_clean
endif

.PHONY : entis
entis:
	cd entis && $(MAKE)

.PHONY : entis_clean
entis_clean:
	cd entis && $(MAKE) clean

.PHONY : libpng 
libpng:
	if [ ! -f "./libpng/configure" ]; then cd libpng && ./autogen.sh; fi
	if [ ! -f "./libpng/Makefile" ]; then cd libpng && ./configure --prefix=$(BASE_PATH)/$(BUILD_DIR)/libpng; fi
	cd libpng && $(MAKE)
	cd libpng && $(MAKE) install

.PHONY : libpng_clean
libpng_clean:
	cd libpng && $(MAKE) clean

