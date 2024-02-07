/******************************************************************
 * Program: Assingment 3
 * Author: Zack Chand
 * Description:
 * Date: 07/25/2022
 ******************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 80
#define MAX_INPUT 1000
/****************************
 *  GLOBAL VARIABLES
 *****************************/
// Buffer 1
char buffer_1[SIZE][MAX_INPUT];

pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;
int count_1 = 0;
int prod_idx_1 = 0;
int con_idx_1 = 0;
// Buffer 2
char buffer_2[SIZE][MAX_INPUT];
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;
int count_2 = 0;
int prod_idx_2 = 0;
int con_idx_2 = 0;
// Buffer 3
char buffer_3[SIZE][MAX_INPUT];
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;
int count_3 = 0;
int prod_idx_3 = 0;
int con_idx_3 = 0;
// Buffer 4
char buffer_4[SIZE][MAX_INPUT];
pthread_mutex_t mutex_4 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_4 = PTHREAD_COND_INITIALIZER;

/***************************************************************
*  Function: initalizeBuffer
*  Description:initialize all four buffers to termiating null so its ready
*  for input
*  Recieves: All buffer arrays
*  Outputs: None
*
* ////////////SOURCES CITED////////////////////////
// https://stackoverflow.com/questions/33843233/initialize-2d-array-of-chars
***************************************************************/
void initalizeBuffers() {

  memset(buffer_1, '\0', sizeof(buffer_1));
  memset(buffer_2, '\0', sizeof(buffer_2));
  memset(buffer_3, '\0', sizeof(buffer_3));
  memset(buffer_4, '\0', sizeof(buffer_4));
}
/***************************************************************
 *  Function: getBuff1
 *  Description: Accesses buffer1 and returns strings to calling command
 *  Recieves: Buffer 2D array from globa;
 *  Outputs:  returns a string based on buffer index
 ***************************************************************/

char *getBuff1() {
  char *string = malloc(sizeof(buffer_1[con_idx_1]));

  pthread_mutex_lock(&mutex_1);
  while (count_1 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_1, &mutex_1);
  strcpy(string, buffer_1[con_idx_1]);
  con_idx_1 = con_idx_1 + 1;
  count_1--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_1);

  return string;
}
/***************************************************************
 *  Function: put_in_buff2
 *  Description:Puts strings into secondary buffer
 *  Recieves: char array of string
 *  Outputs:none
 ***************************************************************/

void put_in_buff2(char array[]) {

  pthread_mutex_lock(&mutex_2);
  strcpy(buffer_2[prod_idx_2], array);
  // printf("%s" , buffer_2[prod_idx_2]);
  prod_idx_2 = prod_idx_2 + 1;
  count_2++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_2);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);
}
/***************************************************************
 *  Function: get_buff2
 *  Description:gets strings from buffer 2
 *  Recieves:buffer2 as a global variable
 *  Outputs:None
 ***************************************************************/

char *get_buff2() {
  char *character = NULL;
  character = malloc(sizeof(buffer_2[con_idx_2]));
  pthread_mutex_lock(&mutex_2);

  while (count_2 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_2, &mutex_2);

  strcpy(character, buffer_2[con_idx_2]);

  con_idx_2 = con_idx_2 + 1;
  count_2--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_2);

  return character;
}
/***************************************************************
 *  Function: replacePlus
 *  Description:
 *  Recieves:
 *  Outputs:
 ***************************************************************/

void *replacePlus(void *args) {
  char helperBuffer2[MAX_INPUT];
  int line_count = 0;
  int i = 0;
  char *string = NULL;
  char replace[20] = "^";
  char plus[10] = "++";
  int ret;

  while (line_count != 80) {

    string = get_buff2();

    strcpy(helperBuffer2, string);

    
    printf("%s", helperBuffer2);
    if (strstr(helperBuffer2, "++")) {
      printf("%lu" , strlen(helperBuffer2));
      
    }

    if (strcmp(helperBuffer2, "STOP\n") == 0) {
      break;
    }
    ++line_count;
  }

  return NULL;
}
void *lineSeperator(void *args) {

  char helperBuffer[MAX_INPUT]; /// Temp array to hold characters to be modified

  int stop = 0;
  int count = 0;
  while (count != 80) {

    // Put the item in the buffer
    char *string = getBuff1(); // get string from the first buffer
    strcpy(helperBuffer, string);

    if (strcmp(helperBuffer, "STOP\n") != 0) {
      helperBuffer[strlen(helperBuffer) - 1] = ' ';
    }

    put_in_buff2(helperBuffer);

    if (strcmp(string, "STOP\n") == 0) {
      break;
    }
    ++count;
  }

  return NULL;
}
/***************************************************************
 *  Function: write_buf_1
 *  Description: takes users input and writes to buffer_1 whcih is the input
 *buffer Recieves:char array from input Outputs: none
 ***************************************************************/

void write_buf_1(char input[]) {
  pthread_mutex_lock(&mutex_1);
  strcpy(buffer_1[prod_idx_1], input);
  prod_idx_1 = prod_idx_1 + 1;
  count_1++;

  pthread_cond_signal(&full_1);

  pthread_mutex_unlock(&mutex_1);
}

void *getUserInput() {

  int stop = 0;
  int char_count = 0;

  char inputBuff[MAX_INPUT];

  while (stop != 1) {

    if (fgets(inputBuff, MAX_INPUT, stdin)) {
    };

    write_buf_1(inputBuff); // Write to buffer one based on lecture

    if (strcmp(inputBuff, "STOP\n") == 0) {
      break;
    }

    char_count += 1;
  }

  return NULL;
}
/***************************************************************
 *  Function:main
 *  Description:Driver code
 *  Recieves:none
 *  Outputs:None
 ***************************************************************/

int main() {
  initalizeBuffers();

  pthread_t input_t, line_sep, plus_sign, output;

  // input
  pthread_create(&input_t, NULL, getUserInput, NULL);
  pthread_create(&line_sep, NULL, lineSeperator, NULL);
  pthread_create(&plus_sign, NULL, replacePlus, NULL);

  // output
  // pthread_create(&output ,NULL , outputLines , NULL);
  pthread_join(input_t, NULL);
  pthread_join(line_sep, NULL);
  pthread_join(plus_sign, NULL);
  // pthread_join(output, NULL);

  // get info from stdin into buffer
}