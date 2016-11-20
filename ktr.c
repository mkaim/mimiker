#include <common.h>
#include <mips/malta.h>
#include <ktr.h>

#define klog(format, ...) KTR(KTR_GENERAL, "[general] " format, ##__VA_ARGS__)

int main() {
  klog("---- KTR TEST START ----------------");
  klog("this is log");
  klog("this is log with params: %d \"%s\"", 123, "34");
  klog("---- KTR TEST END ------------------");
  return 0;
}
