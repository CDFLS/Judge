compile:
	cd src&&make
install:
	cd src&&make install
uninstall:
	rm /usr/bin/judge
remove:
	make uninstall
dev:
	rm ./src/judge
	git add .
	git commit -a -m "Commit"
	git push origin master
