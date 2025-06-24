#include <stdio.h>
#include <string.h>
#include "vm.h"

int main() {
    VM *vm = newVM();

    // Create and root an integer
    Object *intObj = pushInt(vm, 42);
    pushRoot(vm, intObj);

    // Create and root a string
    Object *strObj = pushString(vm, "hello", strlen("hello"));
    pushRoot(vm, strObj);

    // Create a pair (rooted)
    Object *pairObj = pushPair(vm);
    pairObj->head = intObj;
    pairObj->tail = strObj;
    pushRoot(vm, pairObj);

    // Allocate many temporary (unrooted) integers to trigger automatic GC
    for (int i = 0; i < 100; i++) {
        pushInt(vm, i);
    }

    printf("\n--- Objects after allocations and automatic GC ---\n");
    printAllObjects(vm);
    printf("Objects after allocations: %d\n", (int)vm->numObjects);

    // Remove the string root and force GC
    popRoot(vm); // removes pairObj
    popRoot(vm); // removes strObj
    gc(vm);

    printf("\n--- Objects after removing roots and manual GC ---\n");
    printAllObjects(vm);
    printf("Objects after manual GC: %d\n", (int)vm->numObjects);

    // Remove the last root and force GC again
    popRoot(vm); // removes intObj
    gc(vm);

    printf("\n--- Objects after removing all roots and final GC ---\n");
    printAllObjects(vm);
    printf("Objects after final GC: %d\n", (int)vm->numObjects);

    freeVM(vm);
    return 0;
}


/*
#include <stdio.h>
#include <string.h>

int main() {
    int intVal = 42;

    char strVal[] = "hello";

    struct Pair {
        int *head;
        char *tail;
    };
    struct Pair pairObj;
    pairObj.head = &intVal;
    pairObj.tail = strVal;

    int tempInts[100];
    for (int i = 0; i < 100; i++) {
        tempInts[i] = i;
    }

    printf("\n--- Objects after allocations (plain C) ---\n");
    printf("intVal: %d\n", intVal);
    printf("strVal: %s\n", strVal);
    printf("pairObj: head = %d, tail = %s\n", *pairObj.head, pairObj.tail);
    printf("tempInts[0..2]: %d, %d, %d ...\n", tempInts[0], tempInts[1], tempInts[2]);
    return 0;
}
*/