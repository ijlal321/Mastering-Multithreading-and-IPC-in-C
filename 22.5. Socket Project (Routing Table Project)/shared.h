#ifndef SHARED_H
#define SHARED_H

#define SOCKET_PATH "/tmp/multiplexing"

typedef struct{
    int ip;
    int address;
} TableData;

TableData data[20];

typedef enum {
    SHARE_CREATE = 0,
    SHARE_UPDATE,
    SHARE_DELETE
} share_code_t;

#endif