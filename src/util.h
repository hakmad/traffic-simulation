/* Compiler directives. */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

/* Structure definitions. */

/* Boolean values. */
typedef enum {false, true} BOOL;

/* Function prototypes. */

void *safe_malloc(unsigned int size);
