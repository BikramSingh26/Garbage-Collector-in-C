#ifndef VM_H
#define VM_H

#include "object.h"

#define GC_POOL_SIZE 1024
#define GC_GROWTH_FACTOR 2
#define GC_MAX_THRESHOLD (1024 * 1024)

typedef struct {
    Object *stack[GC_POOL_SIZE];
    size_t stackSize;
} GCRoot;

typedef struct {
    Object **objects;
    size_t numObjects;
    size_t maxObjects;
    Object *firstObject;
    GCRoot *roots;
    size_t gcThreshold;
    size_t totalAllocated;
    size_t totalFreed;
} VM;

VM *newVM();
void pushRoot(VM *vm, Object *object);
void popRoot(VM *vm);
Object *newObject(VM *vm, ObjectType type);
Object *pushInt(VM *vm, int value);
Object *pushPair(VM *vm);
Object *pushString(VM *vm, const char *chars, size_t length);
void gc(VM *vm);
void testGC();
void printObject(Object *object);
void freeVM(VM *vm);
void printAllObjects(VM *vm);

#endif
