#ifndef OBJECT_H
#define OBJECT_H

#include <stdlib.h>

typedef enum {
    OBJ_INT,
    OBJ_PAIR,
    OBJ_STRING
} ObjectType;

typedef struct sObject {
    ObjectType type;
    unsigned char marked;
    struct sObject *next;

    union {
        int value;
        struct {
            struct sObject *head;
            struct sObject *tail;
        };
        struct {
            char *chars;
            size_t length;
        };
    };
} Object;

#endif
