#include "message.h"
#include <unistd.h>
#include <errno.h>

void msg_init(Message *msg){
    uint8_t *arr_pointer;
    arr_pointer = (uint8_t *)malloc(MSGBUFFERSIZE*sizeof(uint8_t));
    if(arr_pointer == NULL){
        fprintf(stderr, "Unable to allocate Message Buffer Memory, exiting.\n");
        free(arr_pointer);
        exit(1);
    }else{
        msg->array = arr_pointer;
        msg->array_size = MSGBUFFERSIZE;
        msg->curr_size = 0;

        msg->buffer = 0;
        msg->buffer_count = 0;
    }
}
void msg_add(Message *msg, uint8_t item, int num_bits){
    //add bit by bit to message
    for(uint8_t i = 1<<(num_bits-1); i>0;i = i/2)
        msg_add_bit(msg, (item&i)>0);
}

void msg_add_bit(Message *msg, uint8_t bit){
    if(bit != 0 && bit !=1){
        fprintf(stderr,"Invalid message bit, exiting.\n");
        exit(1);
    }

    msg->buffer<<=1U;
    msg->buffer|=bit;
    msg->buffer_count++;

    if(msg->buffer_count == 8){
        msg_read_buffer(msg);
        msg->buffer_count = 0;
    }
}

void msg_read_buffer(Message *msg){
    //resize array if not enough space
    if(msg->array_size==msg->curr_size){
        uint8_t *arr_pointer;
        msg->array_size += MSGBUFFERSIZE;
        arr_pointer = (uint8_t *)realloc(msg->array,msg->array_size*sizeof(uint8_t));
        if(arr_pointer == NULL){
            fprintf(stderr,"Unable to allocate Message Buffer Memory, exiting.\n");
            free(arr_pointer);
            exit(1);
        }else{
            msg->array = arr_pointer;
        }
    }
    //add buffer's first 8 bits to message
    msg->array[msg->curr_size++] = msg->buffer;
}

void msg_close(Message *msg){
    if(msg->buffer_count!=0){
        msg->buffer <<= (8-msg->buffer_count);
        msg_read_buffer(msg);
    }
    msg_print_chars(msg);
    msg_print(msg);
    msg_free(msg);
}

void msg_print_chars(Message *msg){
    int i;
    for(i = 0; i < msg->curr_size; i++){
        for(uint8_t b = 1U <<8-1; b>0; b = b/2){
            fprintf(stderr, "%c",(msg->array[i]&b)>0);
        }
        fprintf(stderr," ");
    }
    fprintf(stderr,"\n");
}

void msg_print(Message *msg){
    write(STDOUT_FILENO,msg->array,msg->curr_size);
}

void msg_send(Message *msg, int cfd){
    write(cfd,msg->array,msg->curr_size);
}

void msg_free(Message *msg){
    free(msg->array);
    msg->array = NULL;
    msg->array_size=0;
    msg->curr_size=0;
}