#include <vm_pager.h>
#include <stdc.h>
#include <clock.h>
#include <thread.h>
#include <sched.h>

static void demo_thread_1() {
  while (true) {
    realtime_t now = clock_get();
    kprintf("[%8zu] Running '%s' thread.\n", (size_t)now,
            thread_self()->td_name);
    while (clock_get() < now + 20)
      ;
  }
}

static void demo_thread_2() {
  kprintf("Running '%s' thread. Let's yield!\n", thread_self()->td_name);
  sched_yield();
  demo_thread_1();
}

void thread_test()
{
  sched_init();

  thread_t *t1 = thread_create("t1", demo_thread_1, TD_KERNEL);
  thread_t *t2 = thread_create("t2", demo_thread_1, TD_KERNEL);
  thread_t *t3 = thread_create("t3", demo_thread_1, TD_KERNEL);
  thread_t *t4 = thread_create("t4", demo_thread_2, TD_KERNEL);
  thread_t *t5 = thread_create("t5", demo_thread_2, TD_KERNEL);

  sched_add(t1);
  sched_add(t2);
  sched_add(t3);
  sched_add(t4);
  sched_add(t5);

  sched_run();
}

#define USER_START 0x1234000
#define USER_END 0x1235000

#define KERNEL_START 0xc1230000
#define KERNEL_END   0xc1231000

void kernel_thread()
{
    volatile int *ptr = (int*)(KERNEL_START);
    while(1){
        *ptr = *ptr+1;
        kprintf("thread: %s, val: %d\n", thread_self()->td_name, *ptr);
    }
}

void user_thread()
{
    volatile int *ptr = (int*)(USER_START);
    while(1){
        *ptr = *ptr+1;
        kprintf("thread: %s, val: %d\n", thread_self()->td_name, *ptr);
        sched_yield();
    }
}

void kernel_and_user_threads() {
  thread_t *t1 = thread_create("kern_1", kernel_thread, TD_KERNEL);
  thread_t *t2 = thread_create("kern_2", kernel_thread, TD_KERNEL);
  thread_t *t3 = thread_create("user_1", user_thread, TD_USER);
  thread_t *t4 = thread_create("user_2", user_thread, TD_USER);

  vm_map_entry_t *entry1 = vm_map_add_entry(get_active_vm_map(PMAP_KERNEL), KERNEL_START, KERNEL_END, VM_PROT_READ | VM_PROT_WRITE);
  entry1->object = default_pager->pgr_alloc();

  vm_map_t *map1 = t3->user_map;
  set_active_vm_map(t3->user_map);
  vm_map_entry_t *entry2 = vm_map_add_entry(map1, USER_START, USER_END, VM_PROT_READ | VM_PROT_WRITE);
  entry2->object = default_pager->pgr_alloc();

  set_active_vm_map(t4->user_map);
  vm_map_t *map2 = t4->user_map;
  vm_map_entry_t *entry3 = vm_map_add_entry(map2, USER_START, USER_END, VM_PROT_READ | VM_PROT_WRITE);
  entry3->object = default_pager->pgr_alloc();

  sched_add(t1);
  sched_add(t2);
  sched_add(t3);
  sched_add(t4);

  sched_run();
}

int main() {
  kernel_and_user_threads();
  return 0;
}
