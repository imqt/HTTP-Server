#ifndef ASSIGNMENT3_MESSAGE_H
#define ASSIGNMENT3_MESSAGE_H

#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#define MSGBUFFERSIZE 2097152

/**
 * Stores a list of chars, with reference
 * to its current size and its max size.
 */
typedef struct{
    uint8_t *array;
    size_t array_size;
    size_t curr_size;

    uint8_t buffer;
    size_t buffer_count;
} Message;

/**
 * Initialize the char array in the Message struct
 * @param msg the message struct to initialize the array
 */
void msg_init(Message *msg);

/**
 * Add bits from the item to the Message
 * Offsetting num_bits from the right, then writing bits from left to right
 * @param msg the message struct storing bits
 * @param item the buffer containing the data to add
 * @param num_bits the number of bits from the buffer to add to the message
 */
void msg_add(Message *msg, uint8_t item, int num_bits);

/**
 * Add bit to the Message, resizing if necessary
 * @param msg the message struct storing bits
 * @param item the bit to add
 */
void msg_add_bit(Message *msg, uint8_t bit);

/**
 * Adds buffer contents to message
 * @param msg the message struct with the specified array and buffer
 */
void msg_read_buffer(Message *msg);

/**
 * Prints the contents of the message
 * @param msg the message to be printed
 */
void msg_print(Message *msg);

void msg_send(Message *msg, int cfd);

/**
 * Prints the contents of the message with chars
 * @param msg the message to be printed
 */
void msg_print_chars(Message *msg);

/**
 * Null terminate and print the Message struct's array, and
 * then reset the struct to its default values.
 * @param msg the message struct with the specified array
 */
void msg_close(Message *msg);

/**
 * Free the Message struct's array and reset the array_size
 * and curr_size to 0.
 * @param msg the message struct with the specified array
 */
void msg_free(Message *msg);
#endif //ASSIGNMENT3_MESSAGE_H