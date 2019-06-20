.SILENT:
FLAG = --std=c++11 -Dlinux -Wno-write-strings
CPP = g++
DEPENDENCIES = ./sandbox/execute.o ./ConfGen.o ./ConfigJudge.o ./interpreter/Any.o ./interpreter/Message.o ./interpreter/AST.o ./interpreter/STDFunc.o ./Bar.o ./TrieTree.o ./main.o ./Functions_Linux.o ./Conio.o ./JudgeScript.o ./judge_cui.o ./judge.o
ifdef MAKE_RELEASE
	FLAG += -O2
endif
ifdef MAKE_DEBUG
	FLAG += -DDEBUG -g
endif
ifdef MAKE_STATIC
	FLAG += -static
endif
ifdef MAKE_EN_VERSION
	FLAG += -DEN
endif

default:
	make release

judge: $(DEPENDENCIES)
	 echo "[16/16] Building judge"
	$(CPP) $(DEPENDENCIES) $(FLAG) -o judge

./sandbox/execute.o: ./sandbox/execute.cpp
	echo "[1/16] Compiling ./sandbox/execute.cpp"
	$(CPP) -c $(FLAG) $< -o ./sandbox/execute.o
./ConfGen.o: ./ConfGen.cpp
	echo "[2/16] Compiling ./ConfGen.cpp"
	$(CPP) -c $(FLAG) $< -o ./ConfGen.o
./ConfigJudge.o: ./ConfigJudge.cpp
	echo "[3/16] Compiling ./ConfigJudge.cpp"
	$(CPP) -c $(FLAG) $< -o ./ConfigJudge.o
./interpreter/Any.o: ./interpreter/Any.cpp
	echo "[4/16] Compiling ./interpreter/Any.cpp"
	$(CPP) -c $(FLAG) $< -o ./interpreter/Any.o
./interpreter/Message.o: ./interpreter/Message.cpp
	echo "[5/16] Compiling ./interpreter/Message.cpp"
	$(CPP) -c $(FLAG) $< -o ./interpreter/Message.o
./interpreter/AST.o: ./interpreter/AST.cpp
	echo "[6/16] Compiling ./interpreter/AST.cpp"
	$(CPP) -c $(FLAG) $< -o ./interpreter/AST.o
./interpreter/STDFunc.o: ./interpreter/STDFunc.cpp
	echo "[7/16] Compiling ./interpreter/STDFunc.cpp"
	$(CPP) -c $(FLAG) $< -o ./interpreter/STDFunc.o
./Bar.o: ./Bar.cpp
	echo "[8/16] Compiling ./Bar.cpp"
	$(CPP) -c $(FLAG) $< -o ./Bar.o
./TrieTree.o: ./TrieTree.cpp
	echo "[9/16] Compiling ./TrieTree.cpp"
	$(CPP) -c $(FLAG) $< -o ./TrieTree.o
./main.o: ./main.cpp
	echo "[10/16] Compiling ./main.cpp"
	$(CPP) -c $(FLAG) $< -o ./main.o
./Functions_Linux.o: ./Functions_Linux.cpp
	echo "[11/16] Compiling ./Functions_Linux.cpp"
	$(CPP) -c $(FLAG) $< -o ./Functions_Linux.o
./Conio.o: ./Conio.cpp
	echo "[12/16] Compiling ./Conio.cpp"
	$(CPP) -c $(FLAG) $< -o ./Conio.o
./JudgeScript.o: ./JudgeScript.cpp
	echo "[13/16] Compiling ./JudgeScript.cpp"
	$(CPP) -c $(FLAG) $< -o ./JudgeScript.o
./judge_cui.o: ./judge_cui.cpp
	echo "[14/16] Compiling ./judge_cui.cpp"
	$(CPP) -c $(FLAG) $< -o ./judge_cui.o
./judge.o: ./judge.cpp
	echo "[15/16] Compiling ./judge.cpp"
	$(CPP) -c $(FLAG) $< -o ./judge.o

clean:
	echo "Cleaning files"
	rm -rf ./sandbox/execute.o ./ConfGen.o ./ConfigJudge.o ./interpreter/Any.o ./interpreter/Message.o ./interpreter/AST.o ./interpreter/STDFunc.o ./Bar.o ./TrieTree.o ./main.o ./Functions_Linux.o ./Conio.o ./JudgeScript.o ./judge_cui.o ./judge.o
install:
	echo "Installing judge to /usr/local/bin"
	cp ./judge /usr/local/bin/
debug:
	echo "Building a debug version"
	env MAKE_DEBUG=true make judge
static:
	echo "Building a statically linked version"
	env MAKE_STATIC=true make judge
release:
	echo "Building a release version"
	env MAKE_RELEASE=true make judge
en:
	echo "Building an English version"
	env MAKE_EN_VERSION=true make judge
