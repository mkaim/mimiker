#ifndef __KTR_FACILITIES_H__
#define __KTR_FACILITIES_H__

// All kernel facilities to track:
#define KTR_PHYSMEM  0x00000001
#define KTR_TLB      0x00000002
#define KTR_PMAP     0x00000004
#define KTR_SCHED    0x00000008
#define KTR_EXEC     0x00000010
#define KTR_GENERAL  0x80000000
#define KTR_ALL      0xffffffff

#endif // __KTR_FACILITIES_H__
