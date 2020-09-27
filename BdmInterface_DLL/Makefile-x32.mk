include ../Common.mk

TARGET = usbdm-interface
MODULE = bdm-interface

DLL_DEFS ='-DCOMPILE_USBDM_INTERFACE_DLL'

$(TARGET)-arm:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-arm CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-arm-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-arm CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-cfv1:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-cfv1 CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-cfv1-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-cfv1 CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-cfvx:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-cfvx CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-cfvx-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-cfvx CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-hcs08:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-hcs08 CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-hcs08-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-hcs08 CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-hcs12:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-hcs12 CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-hcs12-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-hcs12 CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-rs08:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-rs08 CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-rs08-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-rs08 CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-s12z:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-s12z CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-s12z-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-s12z CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-jtag:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-jtag CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-jtag-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f Target.mk BUILDDIR=$@ MODULE=$(MODULE)-jtag CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

$(TARGET)-dsc:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f dscTarget.mk BUILDDIR=$@ MODULE=$(MODULE)-dsc CDEFS='$(DLL_DEFS)' BITNESS=32

$(TARGET)-dsc-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) dll -f dscTarget.mk BUILDDIR=$@ MODULE=$(MODULE)-dsc CDEFS='$(DLL_DEFS)'  DEBUG='Y' BITNESS=32

TestBdmInterface-debug:
	@echo ''
	@echo  Building $@ 32-bit
	@echo "================================================================"
	$(MAKE) exe -f Target.mk BUILDDIR=$@ TARGET=$@ MODULE=TestBdmInterface DEBUG='Y' BITNESS=32

all: $(TARGET)-arm $(TARGET)-arm-debug
all: $(TARGET)-cfv1 $(TARGET)-cfv1-debug
all: $(TARGET)-cfvx $(TARGET)-cfvx-debug
all: $(TARGET)-hcs08 $(TARGET)-hcs08-debug
all: $(TARGET)-hcs12 $(TARGET)-hcs12-debug
all: $(TARGET)-rs08 $(TARGET)-rs08-debug
all: $(TARGET)-s12z $(TARGET)-s12z-debug
all: $(TARGET)-jtag $(TARGET)-jtag-debug
all: $(TARGET)-dsc $(TARGET)-dsc-debug
all: TestBdmInterface-debug

clean:
	${RMDIR} $(TARGET)-arm$(BUILDDIR_SUFFIXx32)    $(TARGET)-arm-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-cfv1$(BUILDDIR_SUFFIXx32)   $(TARGET)-cfv1-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-cfvx$(BUILDDIR_SUFFIXx32)   $(TARGET)-cfvx-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-hcs08$(BUILDDIR_SUFFIXx32)  $(TARGET)-hcs08-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-hcs12$(BUILDDIR_SUFFIXx32)  $(TARGET)-hcs12-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-rs08$(BUILDDIR_SUFFIXx32)   $(TARGET)-rs08-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-s12z$(BUILDDIR_SUFFIXx32)   $(TARGET)-s12z-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-jtag$(BUILDDIR_SUFFIXx32)   $(TARGET)-jtag-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} $(TARGET)-dsc$(BUILDDIR_SUFFIXx32)    $(TARGET)-dsc-debug$(BUILDDIR_SUFFIXx32)
	${RMDIR} TestBdmInterface-debug$(BUILDDIR_SUFFIXx32)

.PHONY: all clean 
.PHONY: $(TARGET)-arm $(TARGET)-arm-debug
.PHONY: $(TARGET)-cfv1 $(TARGET)-cfv1-debug
.PHONY: $(TARGET)-cfvx $(TARGET)-cfvx-debug
.PHONY: $(TARGET)-hcs08 $(TARGET)-hcs08-debug
.PHONY: $(TARGET)-hcs12 $(TARGET)-hcs12-debug
.PHONY: $(TARGET)-rs08 $(TARGET)-rs08-debug
.PHONY: $(TARGET)-s12z $(TARGET)-s12z-debug
.PHONY: $(TARGET)-jtag $(TARGET)-jtag-debug
.PHONY: $(TARGET)-dsc $(TARGET)-dsc-debug
.PHONY: TestBdmInterface-debug
