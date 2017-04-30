compile:
	cd src&&make
en:
	cd src&&make en
install:
	cd src&&make install
uninstall:
	rm /usr/bin/judge
remove:
	make uninstall
