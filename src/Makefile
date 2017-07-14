FLAG=--std=c++11 -Wno-write-strings -Dlinux -O2
FLAG2=
ifdef MAKE_EN
	FLAG += -DEN
	FLAG2 += en
endif
ifdef MAKE_STATIC
	FLAG += -static
	FLAG2 += static
endif
ifdef MAKE_DEBUG
	FLAG += -DDEBUG -g
	FLAG2 += debug
endif
CPP=g++
DEPENDENCIES=main.o Bar.o judge_cui.o ConfigJudge.o Conio.o Functions_Linux.o execute.o judge.o TrieTree.o ConfGen.o AST.o Any.o Message.o STDFunc.o JudgeScript.o
judge:$(DEPENDENCIES)
	$(CPP) $(DEPENDENCIES) $(FLAG) -o judge
	sudo make install

main.o:
	$(CPP) -c $(FLAG) main.cpp
Bar.o:
	$(CPP) -c $(FLAG) Bar.cpp
judge_cui.o:
	$(CPP) -c $(FLAG) judge_cui.cpp
ConfigJudge.o:
	$(CPP) -c $(FLAG) ConfigJudge.cpp
Conio.o:
	$(CPP) -c $(FLAG) Conio.cpp
Functions_Linux.o:
	$(CPP) -c $(FLAG) Functions_Linux.cpp
execute.o:
	$(CPP) -c $(FLAG) sandbox/execute.c -o execute.o
judge.o:
	$(CPP) -c $(FLAG) judge.cpp
TrieTree.o:
	$(CPP) -c $(FLAG) TrieTree.cpp
ConfGen.o:
	$(CPP) -c $(FLAG) ConfGen.cpp
JudgeScript.o:
	$(CPP) -c $(FLAG) JudgeScript.cpp
AST.o:
	cd interpreter && make $(FLAG2)

clean:
	rm *.o -rf
install:
	cp ./judge /usr/bin/judge
debug:
	env MAKE_DEBUG=moha make
en:
	env MAKE_EN=moha make
static:
	env MAKE_STATIC=moha make
