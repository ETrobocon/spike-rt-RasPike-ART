#
# Makefile for a workspace of SPIKE-RT Platform.
#

# Configuration
SRCLANG := c
KERNEL := asp
TARGET := primehub_gcc

#
# Paths
#
SPIKEDIR   = $(abspath ../..)
SDKDIR     = $(abspath ..)
WSPDIR     = $(abspath .)

KERNELDIR    := $(SPIKEDIR)/asp3
COMMONDIR    := $(SDKDIR)/common
LIBKERNELDIR := $(SPIKEDIR)/build/obj-primehub_kernel
PYBRICKSDIR  := $(SPIKEDIR)/build/obj-primehub_pybricks
TARGETDIR    := $(KERNELDIR)/target/$(TARGET)
# Object files
OBJFILENAME := $(KERNEL)
OBJBINARY := $(KERNEL).bin

#
# Determine Makefile for application
# OUTPUT:
# $(APPLDIR):         Absolute path of application folder
# $(MKFILENAME):      File name of Makefile
# $(MKFILE_DIR):      Absolute path of the folder holding Makefile
#

ifdef img

APPLNAME := $(img)
APPLNAME := app

APPLDIR  := $(WSPDIR)/$(img)
MKFILENAME := app.mk
MKFILE_DIR := $(COMMONDIR)

CDLFILE := $(APPLNAME).cdl
CDL_DIR := $(APPLDIR)
ifeq (,$(wildcard $(CDL_DIR)/$(CDLFILE)))
CDL_DIR := $(COMMONDIR)
CDLFILE := app.cdl
endif

BUILDDIR := $(APPLDIR)/build

#
# Target for an application (static)
#
JOB_NUM = ${JOB_NUM:-$(nproc)} 

include $(APPLDIR)/Makefile.inc

img: $(APPLDIR) $(BUILDDIR)/Makefile $(LIBKERNELDIR)/libkernel.a
	@cd $(BUILDDIR) && \
	make -j $(JOB_NUM) && \
	cp $(BUILDDIR)/$(OBJBINARY) $(WSPDIR) && \
	echo "$(APPLDIR)" > $(WSPDIR)/appdir

$(BUILDDIR)/Makefile:
	@mkdir -p $(BUILDDIR)
	@cd $(BUILDDIR) && \
	$(KERNELDIR)/configure.rb -T $(TARGET) -L $(LIBKERNELDIR) \
          -a $(APPLDIR) \
	  -A $(APPLNAME) \
	  -l $(SRCLANG) \
	  -d $(BUILDDIR)/dep \
	  -C $(CDL_DIR)/$(CDLFILE) \
	  -m $(MKFILE_DIR)/$(MKFILENAME)

$(LIBKERNELDIR)/Makefile:
	@mkdir -p $(LIBKERNELDIR)
	@cd $(LIBKERNELDIR) && \
	$(KERNELDIR)/configure.rb -T $(TARGET) -f -m $(COMMONDIR)/kernel.mk

$(LIBKERNELDIR)/libkernel.a: $(LIBKERNELDIR)/Makefile
	@cd $(LIBKERNELDIR) && \
	make libkernel.a -j $(JOB_NUM)

endif

usage:
	@echo "Usage:"
	@echo "  make img=\"<folder>\""
	@echo "  make upload"
	@echo "  make clean"
	@echo "  make realclean"

upload: $(OBJBINARY)
	@echo "Upload $(OBJBINARY) file"
	@$(COMMONDIR)/deploy-dfu.sh $(OBJBINARY) && \
	echo $(OBJBINARY) has been successfully uploaded.

ifneq ($(shell test -f "appdir" && echo 1 || echo 0), 0)
clean:	cleanup
	@cd $(shell cat appdir)/build && make clean
	@rm -f appdir
else
clean:	cleanup
endif

cleanup:	
	@rm -rf $(notdir $(OBJFILENAME)) $(OBJBINARY) firmware.dfu $(LIBKERNELDIR)/libkernel.a

realclean: clean
	@rm -rf $(LIBKERNELDIR) $(PYBRICKSDIR)

.PHONY: usage img upload clean cleanup realclean

