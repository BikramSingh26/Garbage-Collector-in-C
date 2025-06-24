#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

void mark(Object *object)
{
    if (object == NULL || object->marked)
        return;
    object->marked = 1;

    if (object->type == OBJ_PAIR)
    {
        mark(object->head);
        mark(object->tail);
    }
}

void markAll(VM *vm)
{
    for (size_t i = 0; i < vm->roots->stackSize; i++)
    {
        mark(vm->roots->stack[i]);
    }
}

void sweep(VM *vm)
{
    Object **object = &vm->firstObject;
    while (*object != NULL)
    {
        if (!(*object)->marked)
        {
            Object *unreached = *object;
            printf("GC: Freeing object at %p of type ", (void*)unreached);
            switch (unreached->type) {
                case OBJ_INT:
                    printf("INT with value %d\n", unreached->value);
                    break;
                case OBJ_STRING:
                    printf("STRING with value '%s'\n", unreached->chars);
                    break;
                case OBJ_PAIR:
                    printf("PAIR at %p\n", (void*)unreached);
                    break;
            }
            if (unreached->type == OBJ_STRING)
            {
                free(unreached->chars);
            }
            *object = unreached->next;
            free(unreached);
            vm->numObjects--;
            vm->totalFreed++;
        }
        else
        {
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }

    if (vm->numObjects > 0)
    {
        vm->gcThreshold = vm->numObjects * 2;
    }
}

void gc(VM *vm)
{
    size_t before = vm->numObjects;
    markAll(vm);
    sweep(vm);
    printf("GC: Collected %d objects, %d remaining. Total allocated: %d, freed: %d\n",
           (int)(before - vm->numObjects), (int)vm->numObjects,
           (int)vm->totalAllocated, (int)vm->totalFreed);
}
