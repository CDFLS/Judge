DEFAULT:
	g++ judge.cpp -o judge
	sudo make install
install:
	cp ./judge /usr/bin/judge
uninstall:
	rm /usr/bin/judge
dev:
	make
	sudo make install
	rm ./judge
	git add .
	git commit -a -m "Commit"
	git push origin master
