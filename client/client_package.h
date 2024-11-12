/**
 * @file client_package.h
 * @brief Header file for client package processing.
 *
 * This file contains the declaration of the process function used in the client package.
 */

#ifndef CLIENT_PACKAGE
#define CLIENT_PACKAGE

/**
 * @brief Processes the given client package.
 *
 * This function takes a string representing a client package and processes it.
 *
 * @param package A pointer to a character array representing the client package.
 * @return An integer indicating the result of the processing.
 */
int process(char *);

/**
 * @brief Removes the newline character from the given string.
 *
 * This function takes a string and removes the newline character from it.
 *
 * @param str A pointer to a character array representing the string.
 */
void clean_newline(char *str);


#endif

