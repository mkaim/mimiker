#ifndef __KTR_H__
#define __KTR_H__

#include <ktr_facilities.h>
#include <stdint.h>

#define KTR_MAX_ARGS 6

struct ktr_entry {
  const char *ktr_filepath;
  int ktr_fileline;
  const char *ktr_format;
  unsigned long ktr_args[KTR_MAX_ARGS];
};

void ktr_init(int argc, char **argv);

void ktr_tracepoint(const char *filepath, int fileline, 
                    const char *format, int nargs, ...);

int ktr_get_entry(struct ktr_entry *p_entry);

void ktr_print_next_log();

extern uint32_t ktr_mask;
 
// ugly way to count __VA_ARGS__ arguments (up to 15):
#define _GET_NTH_ARG( \
  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, \
  N, ...) N
#define COUNT_VA_ARGS(...) _GET_NTH_ARG( \
  -1, ##__VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#define KTR(facility, format, ...)  \
  ({  \
    if(facility & ktr_mask)  \
      ktr_tracepoint(__FILE__, __LINE__, \
                     format, COUNT_VA_ARGS(__VA_ARGS__), ##__VA_ARGS__);  \
  })

#endif // __KTR_H__
