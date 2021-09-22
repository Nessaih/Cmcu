#include <stdio.h>
#include "vic_mem.h"
#include "string.h"

int main() {


    char *p = NULL;
    vic_show();
    p = vic_malloc(10);
    p = vic_malloc(100);
    vic_show();
    strcpy(p,"123456789");
    printf("%s\r\n",p);
    vic_free(p);
    vic_show();
    return 0;
}

