PROJ := $(notdir $(CURDIR))

MAKEFLAGS += -j6
MAKEFLAGS += --silent
BUILDDIR := build
#MODULES := . utils pad
LIBRARY := gtk+-3.0 wayland-cursor wayland-egl egl glesv2
MOD1 := $(notdir $(shell find src/ -type d))
MODULES := . $(MOD1)
MODMK := $(addprefix $(BUILDDIR)/,$(MOD1))
MODDEL := $(addsuffix '/*',$(addprefix $(BUILDDIR)/,$(notdir $(MOD1))))

RES_LIB := build/appres.gresource
RES_SRC_DIR := images
RES_SRC := $(RES_SRC_DIR)/res.xml
RES_FILES := $(wildcard images/form0/*.png)

ifeq ($(ARCH),arm)
SYSROOT := staging
PKG_LIB := ./$(SYSROOT)/usr/lib/pkgconfig/

#CROSS_PREFIX := arm-linux-gnueabihf-
CROSS_PREFIX := arm-none-linux-gnueabihf-

GCC = comp/usr/bin/$(CROSS_PREFIX)gcc
GXX = comp/usr/bin/$(CROSS_PREFIX)g++
LD = comp/usr/bin/$(CROSS_PREFIX)ld
BUILDDIR := build

LIBRARY += libgpiod
CFLAGS := $(subst -I/usr/, -I./$(SYSROOT)/usr/, $(shell PKG_CONFIG_PATH=$(PKG_LIB) pkg-config --cflags $(LIBRARY)))
CFLAGS += -I./inc -I./$(SYSROOT)/usr/include/ -O2 -std=gnu11
#C_INCLUDE_PATH=./$(SYSROOT)/usr/include/

LIBS := $(subst -L/usr/lib/arm-linux-gnueabihf, -L./$(SYSROOT)/usr/lib, $(shell PKG_CONFIG_PATH=$(PKG_LIB) pkg-config --libs $(LIBRARY)))
LIBS += -lm
else
GCC = /usr/bin/x86_64-linux-gnu-gcc
GXX = /usr/bin/x86_64-linux-gnu-g++

CFLAGS := $(shell pkg-config --cflags $(LIBRARY))
CFLAGS += -I./inc -O2 -std=gnu18

LIBS := $(shell pkg-config --libs $(LIBRARY))
LIBS += -lm
endif

CXXFLAGS := -Wno-stringop-overflow -Wno-format-truncation -Wno-unused-result -DARMA40I -I./inc -O2 -std=gnu++11

SRCS := $(foreach pmk, $(MODULES), $(addprefix $(pmk)/, $(notdir $(wildcard ./src/$(pmk)/*.c))))
SRCSP := $(foreach pmk, $(MODULES), $(addprefix $(pmk)/, $(notdir $(wildcard ./src/$(pmk)/*.cpp))))
OBJS := $(SRCS:.c=.o)
OBJSP := $(SRCSP:.cpp=.oo)
#DEPENDS := $(addprefix $(BUILDDIR)/, $(OBJS:.o=.d))

HDRS := $(wildcard ./inc/*.h)

#-include $(DEPENDS)

.PHONY: wayland clean install mkdirectory ctags
#.SILENT:

all:	mkdirectory $(PROJ) $(RES_LIB)

$(BUILDDIR)/%.oo:	src/%.cpp $(HDRS) 
	echo 'Compile++ $<'
	$(GXX) -D$(addsuffix __,$(addprefix __,$(subst .,_,$(shell echo $(notdir $<) | tr [:lower:] [:upper:])))) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o:	src/%.c $(HDRS) 
	echo 'Compile $<'
	$(GCC) -D$(addsuffix __,$(addprefix __,$(subst .,_,$(shell echo $(notdir $<) | tr [:lower:] [:upper:])))) $(CFLAGS) -c -o $@ $<

$(PROJ):	$(addprefix $(BUILDDIR)/,$(OBJSP)) $(addprefix $(BUILDDIR)/,$(OBJS))
ifeq ($(ARCH),arm)
	@echo Build ARM $@ 
	$(GXX) -o $@ $^ --sysroot=./$(SYSROOT)/ -L./$(SYSROOT)/usr/lib $(LIBS)
else
	@echo Build X86 $@ 
	$(GXX) -o $@ $^ $(LIBS)
endif

$(RES_LIB):	$(RES_SRC) $(RES_FILES)
	glib-compile-resources --sourcedir=$(RES_SRC_DIR) --target=$(RES_LIB) --generate $(RES_SRC)

#%.o:	%.glsl
#	$(LD) -z noexecstack --relocatable --format=binary -o $@ $^
#

$(MODMK):
	mkdir -p $@
	echo 'Make $@ directory'

mkdirectory: $(MODMK)

wayland:	$(PROJ) $(RES_LIB)
	WAYLAND_DISPLAY=wayland-1 GTK_THEME=Soil ./$(PROJ)

install: $(PROJ)
	scp $(PROJ) root@192.168.0.136:$(PROJ)/
	scp -r ./ui/*.glade root@192.168.0.136:$(PROJ)/ui/

ctags:
	ctags -R src inc

echos:	$(addprefix $(BUILDDIR)/,$(OBJS)) $(addprefix $(BUILDDIR)/,$(OBJSP))
	echo $(SRCS) 
	echo $^

clean:
	@echo $(RES_SRC) $(RES_FILES)
	rm -fr $(PROJ) $(addprefix $(BUILDDIR)/, $(OBJS)) $(addprefix $(BUILDDIR)/, $(OBJSP))
	rm $(RES_LIB)
	@echo Clean complete


