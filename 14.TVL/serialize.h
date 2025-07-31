#ifndef __SERIALIZE__

#define __SERIALIZE__



typedef struct serialized_buffer{

    #define SERIALIZE_BUFFER_DEFAULT_SIZE 512

    void *b;

    int size;

    int next;

    int checkpoint;

} ser_buff_t;


/* init functions*/

void init_serialized_buffer(ser_buff_t **b);    

void init_serialized_buffer_of_defined_size(ser_buff_t **b, int size); 

/* Serialize Function*/

void serialize_string(ser_buff_t *b, char *data, int size);

int  is_serialized_buffer_empty(ser_buff_t *b);

void de_serialize_string(char *dest, ser_buff_t *b, int val_size);

#endif