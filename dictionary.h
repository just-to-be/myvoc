/*
 * dictionary.h
 *
 *  Created on: 9.6.2012
 *      Author: kuba
 */

#ifndef DICTIONARY_H_
#define DICTIONARY_H_


typedef struct dictionary{
	/** array of pointers to words in dictionary. */
    WORD_T **words;
    char *filename; /** Filename or path from which was dictionary loaded. */
    unsigned int length; /** Length of **words array */
    unsigned int allocated; /** For how many words is array **words allocated */
    unsigned int known; /** How many words in dictionary are known. */
} DICTIONARY_T;

/* load and create functions */

DICTIONARY_T *create_dictionary(char *filename, WORD_T **words, unsigned allocated,
								unsigned length, unsigned known);

DICTIONARY_T *load_dictionary(char *filename);
int add_word(WORD_T *word, DICTIONARY_T *dic);
void sort_dictionary(DICTIONARY_T *dic);
int save_dictionary(DICTIONARY_T *dic);

/*get functions */
unsigned get_length(DICTIONARY_T *dic);
unsigned get_known(DICTIONARY_T *dic);
WORD_T *get_word(unsigned i, DICTIONARY_T *dic);

unsigned get_random_unknown(DICTIONARY_T *dic);


/* modifiy functions */
void inc_known(DICTIONARY_T *dic);


/* destroy funtions */

void destroy_dictionary(DICTIONARY_T **dic);

/*others*/

void swap_words(DICTIONARY_T *dic, unsigned a, unsigned b);


#endif /* DICTIONARY_H_ */
