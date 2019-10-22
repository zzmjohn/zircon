#ifndef SYSROOT_SYS_SWAP_H_
#define SYSROOT_SYS_SWAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SWAP_FLAG_PREFER 0x8000
#define SWAP_FLAG_PRIO_MASK 0x7fff
#define SWAP_FLAG_PRIO_SHIFT 0
#define SWAP_FLAG_DISCARD 0x10000

int swapon(const char*, int);
int swapoff(const char*);

#ifdef __cplusplus
}
#endif

#endif // SYSROOT_SYS_SWAP_H_