/*
 * actions.c
 *
 *  Created on: 10.6.2012
 *      Author: kuba
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "word.h"
#include "dictionary.h"

#define NO_MEMORY_LIMIT 5

void exam(char *file, int length)
{
	DICTIONARY_T *dic = NULL;
	WORD_T *word = NULL;
	int i;
	unsigned rand;
	unsigned no_memory = 0;
	char *answer = NULL;
	unsigned ans_length = 0;
	unsigned int wrong = 0;
	FILE *results = NULL;

	if ((dic = load_dictionary(file)) == NULL){
		fprintf(stderr, "Error: Cannot load dictionary.\n");
		return;
	}

	srand(time(NULL));

	//printf("You will be examined from dictionary %s\n",file);
	//printf("You will be asked for %d expressions\n", length);
	printf("Dictionary: %s\n", file);
	printf("Number of expressions: %u\n", get_length(dic));
	printf("Number of known expressions: %u\n", get_known(dic));
	printf("Known rate: %5.2f%%\n", ((float) get_known(dic)*100)/ get_length(dic));
	for (i = 0; i < length; i++){
		//check if there are some uknonw words
		if (get_known(dic) == get_length(dic)){
			printf("Congratulations you have learned whole dictionary!\n");
			break;
		}
		rand = get_random_unknown(dic);
		word = get_word(rand, dic);
		printf("\t%s: ", get_local(word));
		inc_attempts(word);
		if ((ans_length = get_line_to_char(&answer, '\n', stdin)) < 0){
			no_memory++;
			if (no_memory > NO_MEMORY_LIMIT){
				save_dictionary(dic);
				return;
			}
			continue;
		}
		if (ans_length > 0 && strcmp(answer, get_foregin(word)) == 0){
			inc_correct(word);
			if (is_known(word) && rand < (get_length(dic) - get_known(dic))){
				swap_words(dic, rand, ((get_length(dic)-get_known(dic))-1));
				inc_known(dic);
			}
			printf("\tOk\n");
		}
		else{
			wrong++;
			dec_correct(word);
			printf("\tWrong!\nCorrect answer: %s\n", get_foregin(word));
		}
		free(answer);


	}
	save_dictionary(dic);

	/*
	 * Printing of results should be written into examination,
	 * but it was really different and now I wanted just realize,
	 * if the pipes work, but should be changed in the future !!!!
	 */
	if ((results = popen("less", "w")) == NULL){
		fprintf(stderr, "Error: cannot print results\n");
		return;
	}

	fprintf(results, "Results:\n");
	fprintf(results, "Tested expression: %d\n", length);
	fprintf(results, "Correct answers: %d\n", length - wrong);
	fprintf(results, "Succes rate: %5.2f%%\n", ((length - wrong)*100) / ((float) length));

	fprintf(results, "Words where you made mistake:\n");
	for (i = 0; i < get_length(dic); i++){
		word = get_word(i, dic);
		if (is_wrong(word)){
			fprintf(results, "%s : %s\n", get_local(word), get_foregin(word));

		}
	}

	pclose(results);

}
