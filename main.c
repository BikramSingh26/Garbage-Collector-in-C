#include "vm.h"
#include <stdio.h>
#include <string.h>

int main()
{
    VM *vm = newVM();
    int n, i;
    printf("Enter number of integers to allocate: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        int val;
        printf("Enter integer %d: ", i + 1);
        scanf("%d", &val);
        Object *obj = pushInt(vm, val);
        pushRoot(vm, obj);
    }
    printf("Enter number of strings to allocate: ");
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        char buf[256];
        printf("Enter string %d: ", i + 1);
        scanf("%s", buf);
        Object *obj = pushString(vm, buf, strlen(buf));
        pushRoot(vm, obj);
    }
    printf("\n--- Objects before GC ---\n");
    printAllObjects(vm);
    printf("Objects before GC: %d\n", (int)vm->numObjects);
    printf("Run garbage collector now? (y/n): ");
    char yn[4];
    scanf("%s", yn);
    if (yn[0] == 'y' || yn[0] == 'Y')
    {
        gc(vm);
        printf("\n--- Objects after GC ---\n");
        printAllObjects(vm);
        printf("Objects after GC: %d\n", (int)vm->numObjects);
    }
    printf("Freeing VM...\n");
    freeVM(vm);
    return 0;
}
