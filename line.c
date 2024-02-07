/******************************************************************
 * Program: Assingment 3
 * Author: Zack Chand
 * Description: Multi Producer Consumer Pipeline
 * Date: 07/25/2022
 *
 MAIN CITAION: https://replit.com/@cs344/65prodconspipelinec

 DESCRIPTION: This assignment is based of the example given in the canvas module as the structure is very similar on locking , unlocking mutex and signaling to if the buffer has data
 ******************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 80
#define MAX_INPUT 1000
/****************************
 *  GLOBAL VARIABLES

* CITATION: https://replit.com/@cs344/65prodconspipelinec
* DESCRIPTION: The structure is similar to the provide code in for the assignment on canvas
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

  memset(buffer_1, '\0', sizeof(buffer_1));     ///Buffer 1
  memset(buffer_2, '\0', sizeof(buffer_2));     //Buffer 2
  memset(buffer_3, '\0', sizeof(buffer_3));     //Buffer 3
  memset(buffer_4, '\0', sizeof(buffer_4));     //Buffer 4
}
/***************************************************************
 *  Function: isStop
 *  Description: determines whether the process should stop or keep going
 *  Recieves: character array buffer from the calling routine
 *  Outputs: integers as C does not support boolean so integers
 *  were used in place of boolean statements
 ***************************************************************/

int isStop(char line[]) {
  if (strcmp(line, "STOP\n") == 0) {
    return 0;
  }

  else {
    return 1;
  }
}
/***************************************************************
 *  Function: getBuff1
 *  Description: Accesses buffer1 and returns strings to calling command
 *  Recieves: Buffer 2D array from global;
 *  Outputs:  returns a string based on buffer index
 ***************************************************************/

char *getBuff1() {
  char *string = malloc(sizeof(buffer_1[con_idx_1]));

  
  //Lock the Mutex
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

CITATION: https://replit.com/@cs344/65prodconspipelinec

DESCRIPTION: structure taken from example on locking mutex, singaling, and unlocking
 ***************************************************************/

void put_in_buff2(char array[]) {

  pthread_mutex_lock(&mutex_2);
  strcpy(buffer_2[prod_idx_2], array);
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

 * CITATION: https://replit.com/@cs344/65prodconspipelinec

 * DESCRIPTION 
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
 *  Function: put_in_buff3
 *  Description:Takes contents of after its been modified from buffer 2 and
 *stores it in buffer3 Recieves: char array helper buffer Outputs: None
 ***************************************************************/

void put_in_buff3(char insertBuff[]) {
  pthread_mutex_lock(&mutex_3);
  strcpy(buffer_3[prod_idx_3], insertBuff);
  prod_idx_3 = prod_idx_3 + 1;
  count_3++;
  // Signal to the consumer that the buffer is no longer empty
  pthread_cond_signal(&full_3);
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);
}
/***************************************************************
 *  Function: get_buff_3
 *  Description: gets strings from buffer 3 and returns it to calling
 *  routine
 *  Recieves: buffer3 from global
 *  Outputs:  strings to the calling routine
 ***************************************************************/

char *get_buff_3() {
  char *word = NULL;
  word = malloc(sizeof(buffer_3[con_idx_3]));

  pthread_mutex_lock(&mutex_3);

  while (count_3 == 0)
    // Buffer is empty. Wait for the producer to signal that the buffer has data
    pthread_cond_wait(&full_3, &mutex_3);

  strcpy(word, buffer_3[con_idx_3]);

  con_idx_3 = con_idx_3 + 1;
  count_3--;
  // Unlock the mutex
  pthread_mutex_unlock(&mutex_3);

  return word;
}
/***************************************************************
 *  Function: outputLine
 *  Description: Outputs the final message to the user
 *  Recieves: None
 *  Outputs: string message to user
 ***************************************************************/

void *outputLine(void *args) {
  char helperBuff3[MAX_INPUT];
  char outputHeper[50 * 1000] = {0};
  int a = 0;
  int b = 0;
  char outputArray[81] = {0};
  int count_char = 0;

  do {
    char *string = get_buff_3();
    strcpy(helperBuff3, string);

    strcat(outputHeper,
           helperBuff3); /// Combine string arrays together using strcat

    while (b < (strlen(outputHeper))) {

      outputArray[a] = outputHeper[b];
      a = a + 1;
      b = b + 1;

      count_char = count_char + 1;

      if (count_char == 80) { // If the characters reach 80 then print

        outputArray[80] =
            '\0'; /// place delimeter after at 80 to only print 80 characters
        printf("%s", outputArray);
        printf("\n"); // After 80 characters print new line
        // Clear the output buffer
        memset(outputArray, 0, 81);
        a = 0;        //Set counter for output index back to 0
        count_char = 0; //Set char counter back to 0 for next 80 characters
      }
    }

  } while (isStop(helperBuff3));

  return NULL;
}
/***************************************************************
 *  Function: replacePlus
 *  Description: pulls from buffer 2 and modifys the strings to replace ++ to ^
 *then stores in buffer 3 Recieves: gets buffer2 Outputs:NONE

SOURCES CITED:
https://stackoverflow.com/questions/3659694/how-to-replace-substring-in-c

description replaces all n occurances in a single string
 ***************************************************************/

void *replacePlus(void *args) {
  char helperBuffer2[MAX_INPUT] = {0};
  char *replace;
  int line_count = 0;
  char *string = NULL;
  int length = 0;
  char *subString;
  char *newsubString;

  int p = 0;

  do {

    string = get_buff2(); // Get from buffer 2

    char *p = strstr(string, "++");    //Find the substring
    do {
      if (p) {
        char buf[1000];
        memset(buf, '\0', strlen(buf));
        if (string == p) {              //If found replace substring
          strcpy(buf, "^");    
          strcat(buf, p + strlen("++"));     //put the string together
        } else {
          strncpy(buf, string, strlen(string) - strlen(p));
          strcat(buf, "^");
          strcat(buf, p + strlen("++"));
        }
        memset(string, '\0', strlen(string));
        strcpy(string, buf);
      }

    } while (p && (p = strstr(string, "++")));

    strcpy(helperBuffer2, string);

    put_in_buff3(helperBuffer2);    //Put modified string into buffer 3

    ++line_count;
  } while (isStop(helperBuffer2));    //If string is STOP then exit loop

  return NULL;
}
/***************************************************************
 *  Function: lineSeperator
 *  Description: Newline character is repalced by space
 *  Recieves: buffer 1 from global 
 *  Outputs: None , jsut puts modified strings into third buffer
 ***************************************************************/
void *lineSeperator(void *args) {

  char helperBuffer[MAX_INPUT]; /// Temp array to hold characters to be
                                /// modified

  int stop = 0;
  int count = 0;
  do {

    // Put the item in the buffer
    char *string = getBuff1(); // get string from the first buffer
    strcpy(helperBuffer, string);

    if (strcmp(helperBuffer, "STOP\n") != 0) {
      helperBuffer[strlen(helperBuffer) - 1] = ' ';
    }

    put_in_buff2(helperBuffer);

    ++count;
  } while (isStop(helperBuffer));

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
/***************************************************************
 *  Function: getUserInput
 *  Description:gets user input using fgets and stdin
 *  Recieves: characters
 *  Outputs: None just stores in buffer_1
 ***************************************************************/

void *getUserInput() {

  int char_count = 0;

  char inputBuff[MAX_INPUT];

  do {

    if (fgets(inputBuff, MAX_INPUT, stdin)) {
    };

    write_buf_1(inputBuff); // Write to buffer one based on lecture

    char_count += 1;
  } while (isStop(inputBuff));

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

  //input
  pthread_create(&input_t, NULL, getUserInput, NULL);
  //line seperator
  pthread_create(&line_sep, NULL, lineSeperator, NULL);
  //Replace plus
  pthread_create(&plus_sign, NULL, replacePlus, NULL);
  //Output
  pthread_create(&output, NULL, outputLine, NULL);

  pthread_join(input_t, NULL);
  pthread_join(line_sep, NULL);
  pthread_join(plus_sign, NULL);
  pthread_join(output, NULL);
}