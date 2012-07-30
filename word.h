/*
 * word.h
 *
 *  Created on: 14.6.2012
 *      Author: kuba
 */

#ifndef WORD_H_
#define WORD_H_

#define SENTINEL ':'
#ifdef DEBUG
#define CORRECT_MAX 3
#else
#define CORRECT_MAX 7
#endif

#define WRONG_ANSWER_PENALIZATION 3

typedef struct word{
	char *local; /** local expression for word (you will be asked on) */
    char *foregin; /** foregin expression for word (you will have to answer) */
    unsigned int attempts; /** count of attempts */
    unsigned short known; /** is word known? 1 = it is, 0 = is not */
    /** count of correct answers (they are decrased with every bad answer)
     *  by WRONG_ANSWER_PENALIZATION macro */
    unsigned short correct;
    /** was word answeret at least once incorrectly in actual examination?
     * It isn't saved to file */
    unsigned short wrong;
} WORD_T;


//int get_line_to_char(char **buf, const char sentinel, FILE *stream);
WORD_T *create_word(char *foregin, char *local, unsigned short known,
					unsigned attempts, unsigned short correct);
WORD_T *load_word(FILE *stream);
char *get_foregin(WORD_T *word);
char *get_local(WORD_T *word);
unsigned is_known(WORD_T *word);
unsigned is_wrong(WORD_T *word);
unsigned get_attempts(WORD_T *word);
unsigned get_correct(WORD_T *word);
void set_known(WORD_T *word);
void set_unknown(WORD_T *word);
void inc_attempts(WORD_T *word);
void inc_correct(WORD_T *word);
void dec_correct(WORD_T *word);
void destroy_word(WORD_T **word);
int next_char(FILE *stream);

#endif /* WORD_H_ */
