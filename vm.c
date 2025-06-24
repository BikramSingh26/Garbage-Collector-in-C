#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

VM *newVM() {
    VM *vm = malloc(sizeof(VM));
    vm->objects = malloc(sizeof(Object *) * GC_POOL_SIZE);
    vm->numObjects = 0;
    vm->maxObjects = GC_POOL_SIZE;
    vm->firstObject = NULL;
    vm->roots = malloc(sizeof(GCRoot));
    vm->roots->stackSize = 0;
    vm->gcThreshold = GC_POOL_SIZE;
    vm->totalAllocated = 0;
    vm->totalFreed = 0;
    return vm;
}

void pushRoot(VM *vm, Object *object) {
    if (vm->roots->stackSize >= GC_POOL_SIZE) {
        fprintf(stderr, "Root stack overflow\n");
        exit(1);
    }
    vm->roots->stack[vm->roots->stackSize++] = object;
}

void popRoot(VM *vm) {
    if (vm->roots->stackSize == 0) {
        fprintf(stderr, "Root stack underflow\n");
        exit(1);
    }
    vm->roots->stackSize--;
}

Object *pushInt(VM *vm, int value) {
    Object *object = newObject(vm, OBJ_INT);
    object->value = value;
    return object;
}

Object *pushPair(VM *vm) {
    Object *object = newObject(vm, OBJ_PAIR);
    object->head = NULL;
    object->tail = NULL;
    return object;
}

Object *pushString(VM *vm, const char *chars, size_t length) {
    Object *object = newObject(vm, OBJ_STRING);
    object->chars = malloc(length + 1);
    memcpy(object->chars, chars, length);
    object->chars[length] = '\0';
    object->length = length;
    return object;
}

void printObject(Object *object) {
    if (object == NULL) {
        printf("NULL");
        return;
    }

    switch (object->type) {
        case OBJ_INT:
            printf("%d", object->value);
            break;
        case OBJ_PAIR:
            printf("(");
            printObject(object->head);
            printf(", ");
            printObject(object->tail);
            printf(")");
            break;
        case OBJ_STRING:
            printf("\"%s\"", object->chars);
            break;
    }
}

void printAllObjects(VM *vm) {
    printf("Current objects in heap (%d):\n", (int)vm->numObjects);
    Object *obj = vm->firstObject;
    int idx = 0;
    while (obj != NULL) {
        printf("  [%d] at %p: ", idx++, (void*)obj);
        switch (obj->type) {
            case OBJ_INT:
                printf("INT, value = %d\n", obj->value);
                break;
            case OBJ_STRING:
                printf("STRING, value = '%s'\n", obj->chars);
                break;
            case OBJ_PAIR:
                printf("PAIR, head = %p, tail = %p\n", (void*)obj->head, (void*)obj->tail);
                break;
        }
        obj = obj->next;
    }
    if (idx == 0) printf("  (none)\n");
}

void freeVM(VM *vm) {
    // First, free all objects
    Object *obj = vm->firstObject;
    while (obj != NULL) {
        Object *next = obj->next;
        if (obj->type == OBJ_STRING) {
            free(obj->chars);
        }
        free(obj);
        obj = next;
    }

    // Free the objects array
    free(vm->objects);

    // Free the roots
    free(vm->roots);

    // Finally, free the VM itself
    free(vm);
}

void testGC() {
    printf("=== Starting GC Test ===\n");

    VM *vm = newVM();
    pushRoot(vm, NULL);

    Object *root1 = pushInt(vm, 42);
    Object *root2 = pushPair(vm);
    Object *root3 = pushString(vm, "Hello, GC!", 11);
    vm->roots->stack[0] = root1;

    for (int i = 0; i < 100; i++) pushInt(vm, i);

    Object *a = pushPair(vm);
    Object *b = pushPair(vm);
    Object *c = pushPair(vm);
    a->head = b; a->tail = c;
    b->head = a; b->tail = c;
    c->head = a; c->tail = b;

    pushRoot(vm, a);
    printf("Objects before GC: %d\n", (int)vm->numObjects);
    gc(vm);
    printf("Objects after GC: %d\n", (int)vm->numObjects);
    popRoot(vm);
    gc(vm);
    printf("Objects after removing root and GC: %d\n", (int)vm->numObjects);
    freeVM(vm);
    printf("=== GC Test Complete ===\n");
}
