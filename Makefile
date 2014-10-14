prefix=/usr

all:

install:
	mkdir -p $(DESTDIR) $(DESTDIR)$(prefix)/bin $(DESTDIR)$(prefix)/lib/c-script $(DESTDIR)$(prefix)/share/c-script
	install c-script $(DESTDIR)$(prefix)/bin/c-script
	cp -r include $(DESTDIR)$(prefix)/lib/c-script/include
	cp -r init $(DESTDIR)$(prefix)/lib/c-script/init
	cp -r libs $(DESTDIR)$(prefix)/lib/c-script/libs
	cp -r package $(DESTDIR)$(prefix)/lib/c-script/package
	cp -r sample $(DESTDIR)$(prefix)/share/c-script/sample
