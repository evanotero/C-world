/**
 * Author: Evan Otero
 * April 27, 2016
 * Finds the instance of a pattern inside a text string
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define TCOUNT 10
#define MAX_PATTERN_LEN 100

struct threadData {
    char *t;      // Text Array
    char *p;      // Pattern Array
    int t_len;    // Text Length
    int p_len;    // Pattern Length
    int startpos; // Thread's Start Position
}; 
typedef struct threadData ThreadData;

void readtext(char *t, int len); // Reads text from Std Input
void *search(void *arg);         // Searches text for matching pattern

// Global Variables
int found = 0;
int p_index;

pthread_mutex_t lock;

int main(int argc, char *argv[]) {
    // Check for correct amount of args
    if (argc != 3)
        exit(1);

    // Length of Text Array
    int t_len = atoi(argv[2]);

    // Allocate Space & Assign Value
    char *pattern = (char *) malloc(MAX_PATTERN_LEN);
    strcpy(pattern, argv[1]);
    char *text = (char *) malloc(t_len);
    readtext(text, t_len);
    
    // Init Thread
    pthread_t tid[TCOUNT];
    ThreadData threadArgs[TCOUNT];
    pthread_mutex_init(&lock, NULL);
    
    // Create Threads
    int id;
    for (id = 0; id < TCOUNT; id++) {
        threadArgs[id].t = text;
        threadArgs[id].p = pattern;
        threadArgs[id].t_len = t_len;
        threadArgs[id].p_len = strlen(pattern);
        threadArgs[id].startpos = id;
        
        int status = pthread_create(&tid[id], NULL, search, &threadArgs[id]);
        if (status != 0) {
            printf("Can't create thread %d.\n", id);
            exit(1);
        }
    }

    // Wait for threads to finish
    for (id = 0; id < TCOUNT; id++)
        pthread_join(tid[id], NULL);

    // Print Result
    if (found)
        printf("Pattern beings at character %d.\n", p_index + 1);
    else
        printf("Pattern not found!\n");

    // Deallocate Space
    free(pattern);
    pattern = NULL;
    free(text);
    text = NULL;

    pthread_exit(NULL);
}

void readtext(char *t, int len) {
    int i = 0;
    char c;
    while (scanf("%c", &c) != EOF && i < len) {
        t[i] = c; // Place character in array
        i++;      // Increment number of chars
    }
}

void *search(void *arg) {
    ThreadData *myargs = (ThreadData *) arg;
    char *text = myargs->t;
    char *pattern = myargs->p;
    int t_len = myargs->t_len;
    int p_len = myargs->p_len;
    int start = myargs->startpos;

    int i, count;
    while (1) {
        count = 0;
        pthread_mutex_lock(&lock);  // Start of Critical Section
        // Check if pattern is found or if search will go past boundry of array
        if (found || start + p_len > t_len) {
            pthread_mutex_unlock(&lock);  // End of Critical Section
            break;
        }
        // Search for pattern
        for (i = 0; i < p_len; i++)
            if (text[start + i] == pattern[i])
                count++;
            else
                break;
        // Check if pattern was found
        if (count == p_len) {
            found = 1;
            p_index = start;
            pthread_mutex_unlock(&lock);  // End of Critical Section
            break;
        } else
            pthread_mutex_unlock(&lock);  // End of Critical Section
        start += TCOUNT;  // Increment to next unexamined position
    }
    pthread_exit(NULL);
}