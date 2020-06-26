CC = gcc
CFLAGS = -g -Wall -std=c99

SpellChecker : SpellChecker.o hashMap.o
	$(CC) $(CFLAGS) -o $@ $^


hashMap.o : hashMap.h hashMap.c

SpellChecker.o : SpellChecker.c hashMap.h

memCheckTests :
	valgrind --tool=memcheck --leak-check=yes tests

memCheckSpellChecker :
	valgrind --tool=memcheck --leak-check=yes SpellChecker

clean :
	-rm *.o
	-rm SpellChecker