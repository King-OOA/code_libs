#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "except.h"


Except_Frame_T *except_stack = NULL;

void except_raise(const T *e, const char *file, int32_t line)
{
     Except_Frame_T *p = except_stack;

     assert(e);

     if (p == NULL) {
	  fprintf(stderr, "Uncaught exception");
	  if (e->reason)
	       fprintf(stderr, " %s", e->reason);
	  else
	       fprintf(stderr, " at 0x%p", e);
	  if (file && line > 0)
	       fprintf(stderr, " raised at %s: %d\n", file, line);
	  fprintf(stderr, "abort...\n");
	  fflush(stderr);
	  abort();
     }

     p->exception = e;
     p->file = file;
     p->line = line;

     except_stack = except_stack->prev;
     longjmp(p->env, except_raised);
}

