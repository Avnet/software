ifndef PETALINUX
$(error "Error: PETALINUX environment variable not set.  Change to the root of your PetaLinux install, and source the settings.sh file")
endif

include apps.common.mk

PKG = www

all: build install

build:

clean:

.PHONY: install image

install:
	$(TARGETINST) -d -p 0744 $(PKG)/avnet.png /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/avnet_logo.png /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/DBA-logo.gif /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/design_modules.png /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/favicon.ico /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/index.html /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/info.css /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/info.html /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/main.css /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/system.png /srv/www/
	$(TARGETINST) -d -p 0744 $(PKG)/zed.png /srv/www/

help:
	@echo ""
	@echo "Quick reference for various supported build targets for $(INSTANCE)."
	@echo "----------------------------------------------------"
	@echo "  clean                  clean out build objects"
	@echo "  all                    build $(INSTANCE) and install to rootfs host copy"
	@echo "  build                  build subsystem"
	@echo "  install                install built objects to rootfs host copy"
