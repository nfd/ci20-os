PREFIX=mipsel-unknown-elf-
AS=$(PREFIX)as -mips32
CC=$(PREFIX)gcc
LD=$(PREFIX)ld
AR=$(PREFIX)ar
OBJCOPY=$(PREFIX)objcopy
BASEDIR=../..
CFLAGS=-mips32r2 -EL -Os -std=c99 -Wall -Werror -Iinclude -I$(BASEDIR)/include -G0

PORT_LIBS := $(patsubst %,$(BASEDIR)/%,$(PORT_LIBS))
PORT_INCLUDES := $(patsubst %,-I$(BASEDIR)/%,$(PORT_INCLUDES))

# Libraries on which a port depends which are out of date: ones for which make
# in the library directory returns nonzero
out_of_date_library=$(shell make -q -C $(firstword $(subst build/, ,$(1))) build/$(lastword $(subst build/, ,$(1))) || echo $(1))
out_of_date_libraries=$(foreach lib,$(PORT_LIBS),$(call out_of_date_library,$(lib)))

obj = $(patsubst %,build/%,$(addsuffix .o,$(basename $(1))))

ifeq ($(PORT_TYPE), elf)
build/$(PORT_TARGET): $(call obj,$(PORT_SRC)) $(BASEDIR)/ports/baremetal.lds $(out_of_date_libraries)
	$(LD) -T $(BASEDIR)/ports/baremetal.lds -o $@ $(call obj,$(PORT_SRC)) --start-group $(PORT_LIBS) --end-group
endif

ifeq ($(PORT_TYPE), liba)
build/$(PORT_TARGET): $(call obj,$(PORT_SRC))
	$(AR) rcs $@ $(call obj,$(PORT_SRC)) 
endif

# How to build libraries if necessary
define rebuild_external_library
.PHONY: $(1)
$(1):
	make -C $$(firstword $$(subst build/, ,$$@)) build/$$(lastword $$(subst build/, ,$$@))
endef

$(foreach extlib,$(PORT_LIBS),$(eval $(call rebuild_external_library,$(extlib))))


#show_ood:
#	echo $(foreach lib,$(PORT_LIBS),$(call out_of_date_library,$(lib)))

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

.SECONDEXPANSION:

# Directory secondexpansion magic thanks to http://www.cmcrossroads.com/article/making-directories-gnu-make?page=0%2C2
build/%.o: %.[Sc] $$(@D)/.f
	$(CC) $(CFLAGS) $(PORT_INCLUDES) -c -o $@ $<

%/.f:
	mkdir -p $(dir $@) 
	touch $@

.PRECIOUS: %/.f

clean:
	rm -rf build

