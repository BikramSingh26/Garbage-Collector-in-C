#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"
#include "vm.h"

Object *newObject(VM *vm, ObjectType type) {
    if (vm->numObjects >= vm->gcThreshold || vm->numObjects >= GC_MAX_THRESHOLD) {
        gc(vm);
    }

    if (vm->numObjects >= vm->maxObjects) {
        size_t newMax = vm->maxObjects * GC_GROWTH_FACTOR;
        Object **newObjects = realloc(vm->objects, sizeof(Object *) * newMax);
        if (!newObjects) {
            gc(vm);
            newObjects = realloc(vm->objects, sizeof(Object *) * newMax);
            if (!newObjects) {
                fprintf(stderr, "Out of memory\n");
                exit(1);
            }
        }
        vm->objects = newObjects;
        vm->maxObjects = newMax;
    }

    Object *object = malloc(sizeof(Object));
    if (!object) {
        gc(vm);
        object = malloc(sizeof(Object));
        if (!object) {
            fprintf(stderr, "Out of memory\n");
            exit(1);
        }
    }

    object->type = type;
    object->marked = 0;
    object->next = vm->firstObject;
    vm->firstObject = object;
    vm->objects[vm->numObjects++] = object;
    vm->totalAllocated++;
    return object;
}
