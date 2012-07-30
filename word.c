/**
 * @file word.c
 * @author just-to-be
 *
 * Implementation of struct word
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "word.h"


#define ALLOC_STEP 10



/** Reads line from stream, stops when reads newline or sentinel, which is also read but not stored.
 * @param buf pointer to readed string (function allocate memory itself)
 * @param sentinel char on which reading should stop (even before newline)
 * @param stream pointer to stream
 * @return if reading is correct, then return lenght of buf string, else negative number
 * */
int get_line_to_char(char **buf, const char sentinel, FILE *stream)
{
    unsigned int i = 0;
    unsigned int allocated = 0;
    int c = 0;
    *buf = NULL;

    while ((c = getc(stream)) != '\n'){
        if (c == sentinel || c == EOF)
            break;
        if (i >= allocated){
           allocated += ALLOC_STEP;
           *buf = realloc(*buf, allocated*sizeof(char));
           if (*buf == NULL){
               fprintf(stderr, "Error: No free memory for reading next expression\n");
               return -1;
           }
           memset((*buf)+i,0,(allocated - i));

        }
        (*buf)[i] = c;
        i++;
    }

    // write \0 to string
    if (i != 0 && i >= allocated){
        allocated++;
        *buf = realloc(*buf, allocated*sizeof(char));
        if (*buf == NULL){
            fprintf(stderr, "Error: No free memory for reading next expression\n");
            return -1;
        }
        (*buf)[i] = '\0';
    }

    return i;
}

/**
 * Function which from its parameters creats struct word
 *
 * @param foregin foregin expression
 * @param local local expression
 * @param known determines if the user know this word (1 or 0)
 * @param attempts count of answers at this word
 * @param correct number of correct answer in row(not exactly see struct word for more info
 *
 * @return struct word on succes or NULL
 */
WORD_T *create_word(char *foregin, char *local, unsigned short known,
					unsigned attempts, unsigned short correct)
{
    WORD_T *out = NULL;
    out = malloc(sizeof(WORD_T));
    if (out == NULL){
    	fprintf(stderr, "Error: no free memory for create a word.\n");
    	return NULL;
    }
    out->local = local;
    out->foregin = foregin;
    out->known = (known == 1 || known == 0) ? known : 0;
    out->correct = (correct >= 0 && correct <= CORRECT_MAX) ? correct : 0;
    out->attempts = attempts;
    out->wrong = 0;

    return out;
}

WORD_T *load_word(FILE *stream)
{
	WORD_T *out = NULL;
    char *foregin = NULL;
    char *local = NULL;
    unsigned int attempts = 0;
    unsigned short correct = 0;
    unsigned short known = 0;
    int c = 0;
    if (get_line_to_char(&foregin, SENTINEL, stream) <= 0){
        //fprintf(stderr, "1Error: Void or wrong expression in dictionary\n");
        return NULL;
    }
    if (get_line_to_char(&local, SENTINEL, stream) <= 0){
        free(foregin);
        //fprintf(stderr, "2Error: Void or wrong expression in dictionary\n");
        return NULL;
    }
    c = next_char(stream);
    if(isdigit(c)){
    	fscanf(stream,"%hu:%u:%hu", &known, &attempts, &correct);
    	getc(stream); //reads newline
    }

    out = create_word(foregin, local, known, attempts, correct);

    return out;
}
/**
 * Returns foregin expression.
 * @param word from which word should be foregin expression returned
 *
 * @return foregin expression string
 */
char *get_foregin(WORD_T *word)
{
	return word->foregin;
}
/**
 * Returns local expression.
 * @param word from which word should be local expression returned
 *
 * @return local expression string
 */
char *get_local(WORD_T *word)
{
	return word->local;
}

/**
 * Says if the word is known.
 * @param word word
 *
 * @return 1 for known word 0 for unknown
 */
unsigned is_known(WORD_T *word)
{
	return word->known;
}

/**
 * Returns if the word was in actual testing answered at least once wrong.
 * @param word word
 *
 * @return local expression string
 */
unsigned is_wrong(WORD_T *word)
{
	return word->wrong;
}

/**
 * Returns count of users answers on word.
 * @param word from which word should be count of attempts returned
 *
 * @return local expression string
 */
unsigned get_attempts(WORD_T *word)
{
	return word->attempts;
}

/**
 * Returns count of users correct answers on word.
 * @param word from which word should be count of correct answers returned
 *
 * @return local expression string
 */
unsigned get_correct(WORD_T *word)
{
	return word->correct;
}

/**
 * Set word as known.
 * @param word Word which should be known
 *
 * @return nothing
 */
void set_known(WORD_T *word)
{
	word->known = 1;
}

/**
 * Set word as unknown.
 * @param word Word which should be unknown
 *
 * @return nothing
 */
void set_unknown(WORD_T *word)
{
	word->known = 0;
}

/**
 * Increase number of attempts
 * @param word Word whose attempts should be increase
 *
 * @return nothing
 */
void inc_attempts(WORD_T *word)
{
	word->attempts++;
}

/**
 * Increase number of correct answers and maybe set as known (Depend on CORRECT_MAX macro)
 * @param word Word whose correct answers should be increase
 *
 * @return nothing
 */
void inc_correct(WORD_T *word)
{
	if (word->correct < CORRECT_MAX){
		word->correct++;
	}
	else{
		set_known(word);
		word->correct = 0;
	}
}

/**
 * Decrease number of correct answers by WRONG_ANSWER_PENALIZATION macro
 * @param word Word whose correct answers should be increase
 *
 * @return nothing
 */
void dec_correct(WORD_T *word)
{
	word->wrong = 1;
	if (word->correct <= WRONG_ANSWER_PENALIZATION)
		word->correct = 0;
	else
		word->correct -= WRONG_ANSWER_PENALIZATION;
}

/**
 * Function which free word memory
 * @param word whose memory should be released
 * @return nothing
 */
void destroy_word(WORD_T **word)
{
    free((*word)->foregin);
    free((*word)->local);
    free(*word);
    *word = NULL;
}

/**
 * Function which says to you which char is next on the stream (but return's it back to stream)
 * @param stream from which stream should be readed
 * @return character on the stream
 */
int next_char(FILE *stream){
	int c = getc(stream);
	ungetc(c, stream);
	return c;
}
