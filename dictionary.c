/**
 * @file dictionary.c
 * @author just-to-be
 *
 * Implementation of dictionary.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "word.h"
#include "dictionary.h"

#define ALLOC_STEP 10

/*==============================================================================
 *  LOADING FUNCTIONS
 *  ============================================================================
 */


/**
 * Constructor of dictionary. (Creates dictionary from parametrs.)
 * @param filename Filename or path to dictionary.
 * @param words Array of pointers to words.
 * @param allocated For how many elements was array **words allocated.
 * @param length Number of words in array **words.
 * @param known Number of known words in dictionary.
 *
 * @return Pointer to dictionary, or NULL on fail.
 */
DICTIONARY_T *create_dictionary(char *filename, WORD_T **words, unsigned allocated, unsigned length, unsigned known)
{
    DICTIONARY_T *dic = NULL;
    dic = malloc(sizeof(DICTIONARY_T));
    if (dic == NULL){
    	fprintf(stderr, "Error: no free memory for create dictionary\n");
    	return NULL;
    }
    dic->words = words;
    dic->allocated = allocated;
    dic->length = length;
    dic->filename = filename;
    dic->known = known;

    return dic;
}

/**
 * Add word to dictionary.
 *
 * @param word Pointer to word which should be added.
 * @param dic Dictionary to which word should be added.
 *
 * @return On succes 0. On fail 1.
 */
int add_word(WORD_T *word, DICTIONARY_T *dic)
{
	WORD_T **reallocated = NULL;
    if (get_length(dic) >= dic->allocated){
        dic->allocated += ALLOC_STEP;
        reallocated = realloc(dic->words, dic->allocated*sizeof(WORD_T *));
        if (reallocated == NULL){
           fprintf(stderr, "Error: Dictionary is too big\n");
           return 1;
        }
        else{
            dic->words = reallocated;
        }
    }
    dic->words[get_length(dic)] = word;
    if (word->known == 1)
    	dic->known++;
    dic->length++;

    return 0;
}

/**
 * Loads dictionary from file.
 *
 * @param filename Filename or path to dictionary.
 * @return Pointer to dictionary on success. If fails then return NULL
 */
DICTIONARY_T *load_dictionary(char *filename)
{
    FILE *fr = NULL;
    WORD_T *readed = NULL;
    DICTIONARY_T *dic = NULL;

    dic = create_dictionary(filename, NULL, 0, 0, 0);
    if ((fr = fopen(filename, "r")) == NULL){
        return NULL;
    }
    while ((readed = load_word(fr)) != NULL){
    	add_word(readed, dic);
	}

    fclose(fr);

    if (get_length(dic) == 0)
        return NULL;

    return dic;
}

/**
 * Sorts dictionary to two parts. There are uknown words in first part
 * and known in the second.
 */
void sort_dictionary(DICTIONARY_T *dic)
{
	unsigned swapped = 0;
	int i;

	for (i = 0 ; i < get_length(dic); i++){
		if (is_known(get_word(i, dic))){
			swap_words(dic, i, (get_length(dic) - swapped)-1);
			swapped++;
		}
		if (swapped >= dic->known)
			break;
	}
}

/**
 * Writes dictionary to file.
 * @param dic Dictionary which should be saved.
 * @return Returns 0 on succes positive int on fail.
 */
int save_dictionary(DICTIONARY_T *dic)
{
	FILE *fw = NULL;
	int i;
	WORD_T *tmp = NULL;

	if ((fw = fopen(dic->filename, "w")) == NULL){
		fprintf(stderr, "Error: file cannot be writen to disk. Have you permission to write in dictionary?\n");
		return 1;
	}

	for (i = 0; i < get_length(dic);i++){
		tmp = get_word(i, dic);
		fprintf(fw, "%s", get_foregin(tmp));
		fprintf(fw, "%c", SENTINEL);
		fprintf(fw, "%s", get_local(tmp));
		fprintf(fw, "%c", SENTINEL);
		fprintf(fw, "%hu", is_known(tmp));
		fprintf(fw, "%c", SENTINEL);
		fprintf(fw, "%u", get_attempts(tmp));
		fprintf(fw, "%c", SENTINEL);
		fprintf(fw, "%hu", get_correct(tmp));
		fprintf(fw, "\n");
		/*fprintf(fw, "%s;%s;%hu;%u;%hu\n", get_word_foregin(i, dic), get_word_local(i, dic),
				get_word_is_known(i, dic), get_word_attempts(i, dic), get_word_correct(i, dic)); */
	}

	fclose(fw);

	return 0;
}

/*==============================================================================
 *  ACCESIBILITY FUNCTIONS
 *  ============================================================================
 */

/**
 * Returns length of dictionary
 * @param dic Dictionary whose length should be returned.
 * @return Returns length of dictionary
 */
unsigned get_length(DICTIONARY_T *dic)
{
	return dic->length;
}
/**
* Returns count of known words dictionary.
* @param dic Dictionary whose number of known words should be returned.
* @return Returns count of known words dictionary.
*/
unsigned get_known(DICTIONARY_T *dic)
{
	return dic->known;
}

/**
 * Returns pointer to word on i position.
 * @param i Position of word which should be returned.
 * @param dic From which dictionary.
 * @return Returns pointer to word on i position.
 */
WORD_T *get_word(unsigned i, DICTIONARY_T *dic)
{
	return dic->words[i];
}



/**
 * Function which returns random index of some unknown word
 * @param dic Dictionary from which word is chosen.
 * @return Random index of some unknown word.
 */
unsigned get_random_unknown(DICTIONARY_T *dic)
{
	return (rand()%(get_length(dic)-get_known(dic)));
}

/*==============================================================================
 *  MODIFY FUNCTIONS
 *  ============================================================================
 */
/**
 * Increases number of known words in dictionary.
 * @param dic Dictionary in which count should be increased.
 * @return nothing
 */
void inc_known(DICTIONARY_T *dic)
{
	dic->known++;
}

/*==============================================================================
 *  DESTROY FUNCTIONS
 *  ============================================================================
 */

/**
 * Functions which free all memory allocated by dictionary.
 * @param dic Which dictionary should be released.
 */
void destroy_dictionary(DICTIONARY_T **dic)
{
    int i;
    for (i = 0; i < (*dic)->length; i++){
        destroy_word(&(((*dic)->words)[i]));
    }
    free((*dic)->words);
    free(*dic);
    *dic = NULL;
}

/*==============================================================================
 *  OTHER
 *  ============================================================================
 */


/**
 * Function which swaps two words positions in **words array in dictionary.
 *
 * @param dic In which dictioanry should be words swapped.
 * @param a First word's index.
 * @param b Second word's index.
 * @return nothing
 */
void swap_words(DICTIONARY_T *dic, unsigned a, unsigned b)
{
	WORD_T *tmp = dic->words[a];
	dic->words[a] = dic->words[b];
	dic->words[b] = tmp;
}
