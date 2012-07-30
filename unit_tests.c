/*
 * unit_tests.c
 *
 *  Created on: 16.5.2012
 *      Author: kuba
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "minunit.h"
#include "unit_tests.h"
#include "word.h"
#include "dictionary.h"


#define TEST_ARRAY_SIZE 10
#define HEAP_ARRAY_SIZE 10
#define SHORT_LENGTH 4
#define SMALL_HEAP 7
#define SMALL_ARRAY 5
#define TESTFILE "testing.txt"

int tests_run = 0;


static char *all_tests();
static char *test_dictionary_sorting();



int perform_tests()
{
	char *result = all_tests();
	if (result == 0){
		printf("ALL TEST PASSED\n");
	}
	else{
		printf("ERROR SUMMARY\n");
		printf("=============\n\n");
		printf("%s", result);
	}
    printf("\nTests run: %d\n", tests_run);

    if(result == 0){
    	return 0;
    }
    else
    	return 1;
}

static char *all_tests()
{

	mu_run_test(test_dictionary_sorting);

     return 0;
}


static char *test_dictionary_sorting()
{
	DICTIONARY_T *dic = NULL;
	char file[255] = TESTFILE;
	dic = load_dictionary(file);
	sort_dictionary(dic);
	int known_count = 0;
	int unknown_count = 0;
	int i;
	WORD_T *tmp = NULL;

	for (i = 0; i < dic->length; i++){
		tmp = get_word(i, dic);
		if (is_known(tmp) == 1){
			//printf("%s;%s;%d\n",get_word_foregin(i,dic), get_word_local(i, dic), get_word_is_known(i, dic));
			mu_assert("This word should be at the end", (dic->length - dic->known) <= i);
			known_count++;
		}
		if (is_known(tmp) == 0){
			//printf("%s;%s;%d\n",get_word_foregin(i,dic), get_word_local(i, dic), get_word_is_known(i, dic));
			mu_assert("This word should be at the beggining", (dic->length - dic->known) > i);
			unknown_count++;
		}
	}

	mu_assert("Bad count of words", (known_count + unknown_count) == dic->length);

	destroy_dictionary(&dic);

	return 0;
}




