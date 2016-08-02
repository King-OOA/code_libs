#pragma once

#include <setjmp.h>
#include <stdint.h>

#define T Except_T

typedef struct T {
     const char *reason;
} T;

#define RAISE(e) except_raise(&(e), __FILE__, __LINE__)
#define RERAISE except_raise(except_frame.exceptio, except_frame.file, except_frame.line)
#define RETURN switch () default :return

typedef struct except_frame {
  struct except_frame *prev;
  jmp_buf env;
  const char *file;
  int32_t line;
  const T *exception;
} Except_Frame_T;

enum {except_entered, except_raised, except_handled, except_finalized};

extern Except_Frame_T *except_stack;

#define TRY do {				\
    volatile int8_t except_flag;		\
    Except_Frame_T except_frame;		\
    except_frame.prev = except_stack;		\
    except_stack = &except_frame;		\
    if (except_flag == except_entered) {


#define EXCEPT(e)				\
  if (except_flag == except_entered)		\
    except_stack = except_stack->prev;		\
  } else if(except_frame.exception == &(e)) {	\
      except_flag = except_handled;


#define ELSE					\
  if (except_flag == except_entered)		\
    except_stack = except_stack->prev;		\
  } else					\
      except_flag = except_handled;

#define FINALLY					\
  if (except_flag == except_entered)		\
    except_stack = except_stack->prev;		\
  } {						\
  if (except_flag == except_entered)		\
    except_flag = except_finalized;

#define END_TRY					\
  if (except_flag == except_entered)		\
    except_stack = except_stack->prev;		\
  } if (except_flag == except_raised) RERAISE;	\
 } while (0)
  
void except_raise(const T *e, const char *file, int32_t line);

