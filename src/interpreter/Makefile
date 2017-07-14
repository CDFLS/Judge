CPP=g++
DEPEND = AST.o Any.o STDFunc.o Message.o
FLAG = --std=c++11 -O3 -Dlinux
ifdef MAKE_EN
	FLAG += -DEN
endif
ifdef MAKE_STATIC
	FLAG += -static
endif
ifdef MAKE_DEBUG
	FLAG += -DDEBUG -g
endif
default:$(DEPEND)
	mv *.o ..
AST.o:
	$(CPP) -c $(FLAG) AST.cpp
Any.o:
	$(CPP) -c $(FLAG) Any.cpp
STDFunc.o:
	$(CPP) -c $(FLAG) STDFunc.cpp
Message.o:
	$(CPP) -c $(FLAG) Message.cpp
clean:
	rm -rf *.o

debug:
	env MAKE_DEBUG=moha make
en:
	env MAKE_EN=moha make
static:
	env MAKE_STATIC=moha make
