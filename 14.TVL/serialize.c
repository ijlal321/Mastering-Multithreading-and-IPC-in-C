#include <stdio.h>
#include<stdlib.h>
#include<string.h>

#include "serialize.h"


void init_serialized_buffer(ser_buff_t **b){
    (*b) = calloc(1, sizeof(ser_buff_t));
    (*b)->b = calloc(1, SERIALIZE_BUFFER_DEFAULT_SIZE);
    (*b)->size = SERIALIZE_BUFFER_DEFAULT_SIZE;
    (*b)->next = 0;
    (*b)->checkpoint = 0;
}  

void init_serialized_buffer_of_defined_size(ser_buff_t **b, int size){
    (*b) = calloc(1, sizeof(ser_buff_t));
    (*b)->b = calloc(1, size);
    (*b)->size = size;
    (*b)->next = 0;
    (*b)->checkpoint = 0;
}

void serialize_string(ser_buff_t *b, char *data, int size){
    if (!b || !data || size <= 0) {
        assert(0);  // Defensive: invalid input
        return;
    }

    int remaining_size = b->size - b->next;

    if (remaining_size < size) {
        int new_size = b->size;
        while ((new_size - b->next) < size) {
            new_size *= 2;
        }

        char *new_buf = realloc(b->b, new_size);
        if (!new_buf) {
            perror("Buffer realloc failed");
            exit(EXIT_FAILURE);
        }
        b->b = new_buf;
        b->size = new_size;
    }
    memcpy(b->b + b->next, (void*)data, size);
    b->next += size;

}

int  is_serialized_buffer_empty(ser_buff_t *b){

    if(b->next == 0){
        return 0;
    }
    return -1;
}

void de_serialize_string(char *dest, ser_buff_t *b, int val_size){
    if(!b || !b->b) assert(0);
    if(!val_size) assert(0);;

    if((b->size - b->next)< val_size) assert(0);

    memcpy(dest, b->b + b->next, val_size);

    b->next += val_size;
}

void copy_in_serialized_buffer_by_offset(ser_buff_t *b, int size, char *value, int offset){
    if ((b->size - offset) < size)
    {
        printf("%s(): Error : Insufficient buffer space\n", __FUNCTION__);
        return;
    }

    memcpy(b->b + offset, value, size);
}

int main(){
    
}