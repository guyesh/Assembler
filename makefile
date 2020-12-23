structuresHfiles=linkedList.h symbolTable.h cmdInfo.h

assembler: main.o firstPass.o fp_func.o secondPass.o sp_func.o linkedList.o symbolTable.o commandsInfo.o handleMemo.o
				gcc -g -ansi -Wall -pedantic main.o firstPass.o fp_func.o secondPass.o sp_func.o linkedList.o symbolTable.o commandsInfo.o handleMemo.o -o assembler -lm

main.o: main.c main.h
				gcc -c -ansi -Wall -pedantic main.c -o main.o

firstPass.o: firstPass.c firstPass.h  $(structuresHfiles)
				gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o 
fp_func.o: fp_func.c functionsFiles.h $(structuresHfiles)
				gcc -c -ansi -Wall -pedantic fp_func.c -o fp_func.o -lm

secondPass.o: secondPass.c secondPass.h  $(structuresHfiles)
				gcc -c -ansi -Wall -pedantic secondPass.c -o secondPass.o 
sp_func.o: sp_func.c functionsFiles.h $(structuresHfiles)
				gcc -c -ansi -Wall -pedantic sp_func.c -o sp_func.o -lm

		
linkedList.o: linkedList.c linkedList.h allfiles.h
				gcc -c -ansi -Wall -pedantic linkedList.c -o linkedList.o 
symbolTable.o: symbolTable.c symbolTable.h allfiles.h
				gcc -c -ansi -Wall -pedantic symbolTable.c -o symbolTable.o 
commandsInfo.o: commandsInfo.c cmdInfo.h allfiles.h
				gcc -c -ansi -Wall -pedantic commandsInfo.c -o commandsInfo.o 
handleMemo.o: handleMemo.c allfiles.h
				gcc -c -ansi -Wall -pedantic handleMemo.c -o handleMemo.o 