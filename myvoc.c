/**
 * @file myvoc.c
 * @author just-to-be
 *
 * Main module of program which is for memorizing expressions.
 */
#include <stdio.h>
#include <stdlib.h>

#include "word.h"
#include "dictionary.h"
#include "actions.h"

#ifdef DEBUG
#include "minunit.h"
#include "unit_tests.h"
#endif

int main (int argc, char *argv[])
{

#ifdef DEBUG
	perform_tests();
	exam("pokusny.txt", 5);
#else
	int length = 20;
	if (argc > 1){
		if (argc == 2){
			exam(argv[1], length);
		}
		if (argc == 3){
			length = atoi(argv[2]);
			exam(argv[1], length);
		}
	}
#endif

	return 0;
}
