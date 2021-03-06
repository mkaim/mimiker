#include <mips/asm.h>
#include <errno.h>
#include <sysent.h>

        .set noreorder

AENT(execve)
AENT(link)
AENT(symlink)
AENT(readlink)
AENT(wait)
AENT(isatty)
AENT(times)
AENT(gettimeofday)

SLEAF(no_syscall)
        b       set_errno
        li      $v0, -ENOSYS
END(no_syscall)

SNESTED(set_errno, 24, $ra)
        addiu   $sp, $sp, -24
        sw      $ra, 20($sp)
        sw      $v0, 16($sp)
        jal     __errno
        nop
        lw      $v1, 16($sp)
        negu    $v1, $v1
        sw      $v1, 0($v0)
        li      $v0, -1
        lw      $ra, 20($sp)
        jr      $ra
        addiu   $sp, $sp, 24
SEND(set_errno)

/* This implementation uses newlib-specific errno processing. */
#define DO_SYSCALL(num)         \
        li      $v0, num;       \
        syscall;                \
        bltz    $v0, set_errno; \
        nop;                    \
        jr $ra;                 \
        nop;

#define SYSCALL(name, num)      \
        LEAF(name)              \
        DO_SYSCALL(num)         \
        END(name)

SYSCALL(_exit, SYS_EXIT)
SYSCALL(open, SYS_OPEN)
SYSCALL(close, SYS_CLOSE)
SYSCALL(read, SYS_READ)
SYSCALL(write, SYS_WRITE)
SYSCALL(lseek, SYS_LSEEK)
SYSCALL(unlink, SYS_UNLINK)
SYSCALL(getpid, SYS_GETPID)
SYSCALL(kill, SYS_KILL)
SYSCALL(fstat, SYS_FSTAT)
SYSCALL(sbrk, SYS_SBRK)

# vim: sw=8 ts=8 et
