#include <stdarg.h>
#include <ktr.h>
#include <string.h>
#include <stdc.h>

uint32_t ktr_mask = KTR_ALL;

static int ktr_verbose = 1;         // print every log on insert
static int ktr_format_verbose = 1;  // verbose level of print

#define KTR_INIT_ENTRIES 1024
struct ktr_entry ktr_entries_buffer[KTR_INIT_ENTRIES];
struct ktr_entry *ktr_entries = ktr_entries_buffer;
volatile int ktr_entries_size = KTR_INIT_ENTRIES;
volatile int ktr_entry_first = 0, ktr_entry_last = 0;

void ktr_init(int argc, char **argv) {
  // TODO: implement it when kernel arguments passing will be provided
}

void ktr_print_entry(const struct ktr_entry *p_entry);

void ktr_tracepoint(const char *filepath, int fileline, 
                    const char *format, int nargs, ...) {
  if (nargs > KTR_MAX_ARGS)
    return;

  struct ktr_entry *entry;
  va_list va_args;

  entry = &ktr_entries[ktr_entry_last];
  ktr_entry_last = (ktr_entry_last + 1) % ktr_entries_size;
  if (ktr_entry_last == ktr_entry_first)
    ktr_entry_first = (ktr_entry_first + 1) % ktr_entries_size;

  entry->ktr_filepath = filepath;
  entry->ktr_fileline = fileline;
  entry->ktr_format = format;
  va_start(va_args, nargs);
  for(int i = 0; i < nargs; ++i)
    entry->ktr_args[i] = va_arg(va_args, unsigned long);
  va_end(va_args);

  if (ktr_verbose)
    ktr_print_entry(entry);
}

int ktr_get_entry(struct ktr_entry *p_entry) {
  int result = 0;

  if (ktr_entry_first != ktr_entry_last) {
    result = 1;
    *p_entry = ktr_entries_buffer[ktr_entry_first];
    ktr_entry_first = (ktr_entry_first + 1) % ktr_entries_size;
  }

  return result;
}

void ktr_print_entry(const struct ktr_entry *p_entry) {
  if (p_entry == NULL)
    return;

  if (ktr_format_verbose)
    kprintf("%s:%d ", p_entry->ktr_filepath, p_entry->ktr_fileline);

  kprintf(p_entry->ktr_format, 
      p_entry->ktr_args[0], p_entry->ktr_args[1], p_entry->ktr_args[2], 
      p_entry->ktr_args[3], p_entry->ktr_args[4], p_entry->ktr_args[5]);
  kprintf("\n");
}

void ktr_print_next_log() {
  struct ktr_entry entry;
  if(ktr_get_entry(&entry))
    ktr_print_entry(&entry);
}
