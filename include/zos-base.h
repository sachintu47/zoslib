///////////////////////////////////////////////////////////////////////////////
// Licensed Materials - Property of IBM
// ZOSLIB
// (C) Copyright IBM Corp. 2020. All Rights Reserved.
// US Government Users Restricted Rights - Use, duplication
// or disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
///////////////////////////////////////////////////////////////////////////////

#ifndef __ZOS_BASE_H_
#define __ZOS_BASE_H_

#undef __ZOS_EXT
#define __ZOS_EXT__ 1

#if ' ' != 0x20
#error EBCDIC codeset detected. ZOSLIB is compatible with the ASCII codeset only.
#endif

#include <_Nascii.h>
#include <stdarg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/__getipc.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#define __ZOS_CC

#ifdef __cplusplus
extern "C" {
#endif
#ifndef __size_t
typedef unsigned long size_t;
#define __size_t 1
#endif

#define IPC_CLEANUP_ENVAR_DEFAULT "__IPC_CLEANUP"
#define DEBUG_ENVAR_DEFAULT "__RUNDEBUG"
#define RUNTIME_LIMIT_ENVAR_DEFAULT "__RUNTIMELIMIT"
#define FORKMAX_ENVAR_DEFAULT "__FORKMAX"
#define CCSID_GUESS_BUF_SIZE_DEFAULT "__CCSIDGUESSBUFSIZE"
#define UNTAGGED_READ_MODE_DEFAULT "__UNTAGGED_READ_MODE"
#define UNTAGGED_READ_MODE_CCSID1047_DEFAULT "__UNTAGGED_READ_MODE_CCSID1047"

typedef enum {
  ZOSLVL_V1R13 = 0,
  ZOSLVL_V2R1,
  ZOSLVL_V2R2,
  ZOSLVL_V2R3,
  ZOSLVL_V2R4,
  ZOSLVL_V2R5,
  ZOSLVL_UNKNOWN,
} oslvl_t;

struct timespec;

typedef enum {
  CLOCK_REALTIME,
  CLOCK_MONOTONIC,
  CLOCK_HIGHRES,
  CLOCK_THREAD_CPUTIME_ID
} clockid_t;

typedef struct __sem {
  volatile unsigned int value;
  volatile unsigned int id; // 0 for non shared (thread), pid for share
  volatile unsigned int waitcnt;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
} ____sem_t;

typedef struct {
  ____sem_t *_s;
} __sem_t;

extern const char *__zoslib_version;

typedef struct __stack_info {
  void *prev_dsa;
  void *entry_point;
  char entry_name[256];
  int* return_addr;
  int* entry_addr;
  int* stack_addr;
} __stack_info;

/**
 * Convert from EBCDIC to ASCII.
 * \param [out] dst Destination string (must be pre-allocated).
 * \param [in] src Source string.
 * \param [in] size Number of bytes to convert.
 * \return returns destination string.
 */
extern void *_convert_e2a(void *dst, const void *src, size_t size);
/**
 * Convert from ASCII to EBCDIC
 * \param [out] dst Destination string (must be pre-allocated).
 * \param [in] src Source string.
 * \param [in] size Number of bytes to convert
 * \return returns destination string.
 */
extern void *_convert_a2e(void *dst, const void *src, size_t size);
/**
 * Get current time of clock.
 * \param [in] clk_id Clock id.
 * \param [out] tp structure to store the current time to.
 * \return return 0 for success, or -1 for failure.
 */
int clock_gettime(clockid_t clk_id, struct timespec *tp);
/**
 * Get the environ.
 * \return returns pointer to environment list
 */
extern char **__get_environ_np(void);
/**
 * Convert environment variables from EBCDIC to ASCII.
 */
extern void __xfer_env(void);
/**
 * Change file descriptor to CCSID.
 * \param [in] fd file descriptor.
 * \param [in] ccsid CCSID.
 * \return returns 0 if successful, or -1 on failure.
 */
extern int __chgfdccsid(int fd, unsigned short ccsid);
/**
 * Get file descriptor CCSID.
 * \param [in] fd file descriptor.
 * \return returns file descriptors ccsid.
 */
extern int __getfdccsid(int fd);
/**
 * Set file descriptor to the provided CCSID.
 * \param [in] fd file descriptor.
 * \param [in] t_ccsid CCSID.
 * \return returns 0 if successful, or -1 on failure.
 */
extern int __setfdccsid(int fd, int t_ccsid);
/**
 * Remove IPC semaphores and shared memory.
 * \param [in] others non-zero value indicates remove IPC not associated
 * with current process.
 */
extern void __cleanupipc(int others);
/**
 * Retrieves error message from __registerProduct IFAUSAGE macro.
 * \param [in] rc return code from __registerProduct.
 * \return returns error message as C character string.
 */
extern const char *getIFAUsageErrorString(unsigned long rc);
/**
 * Registers product for SMF 89 Type 1 records using IFAUSAGE macro.
 * \param [in] major_version The major version of Product (e.g. 14)
 * \param [in] product_owner The product owner (e.g. IBM)
 * \param [in] feature_name The feature name (e.g. Node.js)
 * \param [in] product_name The product name (e.g. Node.js for z/OS)
 * \param [in] pid The Product ID (e.g. 5676-SDK)
 * \return returns 0 if successful, non-zero if unsuccessful.
 */
extern unsigned long long __registerProduct(const char *major_version,
                                            const char *product_owner,
                                            const char *feature_name,
                                            const char *product_name,
                                            const char *pid);
/**
 * Convert from EBCDIC to ASCII in place.
 * \param [out] bufptr Buffer to convert.
 * \param [in] szLen Number of characters to convert.
 * \return number of characters converted, or -1 if unsuccessful.
 */
extern size_t __e2a_l(char *bufptr, size_t szLen);
/**
 * Convert from ASCII to EBCDIC in place.
 * \param [out] bufptr Buffer to convert.
 * \param [in] szLen Number of characters to convert.
 * \return number of characters converted, or -1 if unsuccessful.
 */
extern size_t __a2e_l(char *bufptr, size_t szLen);
/**
 * Convert null-terminated string from ASCII to EBCDIC in place.
 * \param [out] string String to convert.
 * \return number of characters converted, or -1 if unsuccessful.
 */
extern size_t __e2a_s(char *string);
/**
 * Convert null-terminate string from EBCDIC to ASCII in place.
 * \param [out] string string to convert.
 * \return number of characters converted, or -1 if unsuccessful.
 */
extern size_t __a2e_s(char *string);
extern unsigned int atomic_dec(volatile unsigned int *loc);
extern unsigned int atomic_inc(volatile unsigned int *loc);
/**
 * Get the Thread ID.
 * \return returns the current thread id
 */
extern int gettid();
/**
 * Debug Printf.
 * \return returns total number of bytes written to file descriptor
 */
extern int dprintf(int fd, const char *, ...);
/**
 * Variadic Debug Printf.
 * \return returns total number of bytes written to file descriptor
 */
extern int vdprintf(int fd, const char *, va_list ap);
/**
 * Dump to console.
 */
extern void __dump(int fd, const void *addr, size_t len, size_t bw);
/**
 * Dump title to console.
 */
extern void __dump_title(int fd, const void *addr, size_t len, size_t bw,
                         const char *, ...);
/**
 * Print backtrace of stack to file descriptor.
 * \param [in] fd file descriptor.
 */
extern void __display_backtrace(int fd);
/**
 * Execute a file.
 * \param [in] name used to construct a pathname that identifies the new
 *  process image file.
 * \param [in] argv an array of character pointers to NULL-terminated strings.
 * \param [in] envp an array of character pointers to NULL-terminated strings.
 * \return if successful, it doesn't return; otherwise, it returns -1 and sets
 *  errno.
 */
extern int execvpe(const char *name, char *const argv[], char *const envp[]);
/**
 * Generate a backtrace and store into *Buffer.
 * \param [out] buffer Address of location to store backtrace to.
 * \param [in] size Maximum number of bytes to store.
 * \return if successful, returns 0, otherwise -1
 */
extern int backtrace(void **buffer, int size);
/**
 * Generate a backtrace symbols and store into *Buffer.
 * \param [out] buffer Address of location to store backtrace to.
 * \param [in] size Maximum number of bytes to store.
 * \return if successful, an array of strings, otherwise returns NULL.
 */
extern char **backtrace_symbols(void *const *buffer, int size);
/**
 * Generate a backtrace symbols and store into *Buffer.
 * \param [out] buffer Address of location to store backtrace to.
 * \param [in] size Maximum number of bytes to store.
 * \param [in] fd file descriptor.
 */
extern void backtrace_symbols_fd(void *const *buffer, int size, int fd);
/**
 * Prints information about a file descriptor.
 * \param [in] fd file descriptor.
 */
extern void __fdinfo(int fd);
/**
 * Generates an SVC 13 abend.
 * \param [in] comp_code Completion code.
 * \param [in] reason_code Reason code.
 * \param [in] flat_byte Flat Byte.
 * \param [in] plist Parameter list.
 */
extern void __abend(int comp_code, unsigned reason_code, int flat_byte,
                    void *plist);
/**
 * String case comparision that ignores code page.
 * \param [in] a - Character String.
 * \param [in] b - Character String.
 * \param [in] n - Number of bytes to compare.
 * \return if equal, returns 0, otherwise returns non-zero.
 */
extern int strncasecmp_ignorecp(const char *a, const char *b, size_t n);
/**
 * String case comparision that ignores code page.
 * \param [in] a - null-terminated character string.
 * \param [in] b - null-terminated character string.
 * \return if equal, returns 0, otherwise returns non-zero.
 */
extern int strcasecmp_ignorecp(const char *a, const char *b);
/**
 * Guess if string is UTF8 (ASCII) or EBCDIC based
 * on the first CCSID_GUESS_BUF_SIZE_ENVAR of the file
 * associated with the given fd. CCSID_GUESS_BUF_SIZE_ENVAR
 * is default at 4KB.
 * \param [in] fd - open file descriptor to guess.
 * \return guessed CCSID (819 for UTF8, 1047 for EBCDIC; otherwise
 *  65535 for BINARY and, if not NULL, errmsg will contain details).
 */
extern int __guess_fd_ue(int fd, char *errmsg, size_t er_size, int is_new_fd);
/**
 * Guess if string is UTF8 (ASCII) or EBCDIC.
 * \param [in] src - character string.
 * \param [in] size - number of bytes to analyze.
 * \return guessed CCSID (819 for UTF8, 1047 for EBCDIC; otherwise
 *  65535 for BINARY and, if not NULL, errmsg will contain details).
 */
extern int __guess_ue(const void *src, size_t size,
                     char *errmsg, size_t er_size);
/**
 * Guess if string is ASCII or EBCDIC.
 * \param [in] src - character string.
 * \param [in] size - number of bytes to analyze.
 * \return guessed CCSID.
 */
extern int __guess_ae(const void *src, size_t size);
/**
 * Convert string from UTF8 to UTF16
 */
extern int conv_utf8_utf16(char *, size_t, const char *, size_t);
/**
 * Convert string from UTF16 to UTF8.
 */
extern int conv_utf16_utf8(char *, size_t, const char *, size_t);
/**
 * Print to MVS Console.
 */
extern int __console_printf(const char *fmt, ...);
/**
 * Indicates if zoslib is in debug mode
 * \return returns current debug mode
 */
extern int __indebug(void);
/**
 * Activates debug mode
 */
extern void __setdebug(int);
/**
 * Sets file descriptor to auto convert.
 * \param [in] fd - file descriptor.
 * \param [in] ccsid - CCSID to auto convert to.
 * \param [in] txtflag - Indicates if ccsid is text.
 * \param [in] on_untagged_only - applies only to untagged
 */
extern void __set_autocvt_on_fd_stream(int fd, unsigned short ccsid,
                                       unsigned char txtflag,
                                       int on_untagged_only);
/**
 * Get program argument list of a given process id
 * \param [out] argc - pointer to store count of the arguments
 * \param [out] argv - pointer to store an array of pointers that point to each
 * argument
 * \param [in] pid - process id to obtain the argc and argv for
 * \note
 * Call free(argv) when done accessing argv.
 * \return On success, returns 0, or -1 on error.
 */
extern int __getargcv(int *argc, char ***argv, pid_t pid);
/**
 * Get program argument list
 * \return returns an array of process arguments
 */
extern char **__getargv(void);
/**
 * Get program argument count
 * \return returns count of process arguments
 */
extern int __getargc(void);

/**
 * Get the executable path of a given process id
 * \param [out] path - pointer to the destination array to copy the
 *  null-terminated path to
 * \param [in] pathlen - length of the given array
 * \param [in] pid - process id to obtain the executable path for
 * \return On success, returns 0, or -1 on error.
 */
extern int __getexepath(char *path, int pathlen, pid_t pid);

/**
 * Get the number of online CPUs
 * \return returns the number of online CPUs
 */
extern int __get_num_online_cpus(void);

/**
 * Get the number of frames currently available to the system
 * \return returns the number of available frames
 */
int __get_num_frames(void);

/**
 * Get the stack start address for the current thread
 * \return returns the stack start address
 */
extern int *__get_stack_start();

/**
 * Get the OS level
 * \return the OS level as ZOSLVL_V2R1/2/3/4/5 (values are in ascending order)
 */
oslvl_t __get_os_level(void);

/**
 * Check if current OS is at or above a given level
 * \return true if the current OS level is at or above the given level, and false otherwise
 */
bool __is_os_level_at_or_above(oslvl_t level);

 /* Iterate to next stack dsa based on current dsa
 * \param [in] dsaptr - current dsa entry
 * \param [out] si - stack information of next dsa
 * \return returns the next dsa entry in the chain or 0 if not found
 */
extern void *__iterate_stack_and_get(void *dsaptr, __stack_info *si);

/**
 * Check if STFLE (STORE FACILITY LIST EXTENDED) instruction is available
 * \return true if the STFLE instruction is available
 */
bool __is_stfle_available();

/**
 * Get next dlcb entry
 * \param [in] last - previous dlcb entry
 * \return [in] returns next dlcb entry
 */
extern void *__dlcb_next(void *last);
/**
 * Get entry name of given dlcb
 * \param [out] buf - DLL name of given dlcb
 * \param [in] size - maximum number of bytes
 * \param [in] dlcb - current dlcb
 * \return [in] number of bytes written to buf
 */
extern int __dlcb_entry_name(char *buf, int size, void *dlcb);
/**
 * Get address of dlcb entry
 * \param [in] dlcb - current dlcb
 * \return returns entry address of dlcb
 */
extern void *__dlcb_entry_addr(void *dlcb);
/**
 * Finds file in a given path
 * \param [out] out Found path string
 * \param [in] size Max size of path string
 * \param [in] envar Environment variable to search
 * \param [in] file file to search
 * \return returns non-zero if successful, 0 if not found.
 */
extern int __find_file_in_path(char *out, int size, const char *envvar,
                               const char *file);
/**
 * Determines if file descriptor needs conversion from EBCDIC to ASCII.
 * Call __file_needs_conversion_init first before calling this function.
 * \param [in] fd file descriptor
 * \return returns 1 if file needs conversion, 0 if not.
 */
extern int __file_needs_conversion(int fd);
/**
 * Determines if file needs conversion from EBCDIC to ASCII.
 * \param [in] name path to file
 * \param [in] fd file descriptor
 * \return returns 1 if file needs conversion, 0 if not.
 */
extern int __file_needs_conversion_init(const char *name, int fd);
/**
 * Unsets fd attributes
 * \param [in] fd file descriptor
 */
extern void __fd_close(int fd);

/**
 * Obtain the mach absolute time
 * \return returns mach absolute time
 */
extern unsigned long __mach_absolute_time(void);
/**
 * Generate an anonymous memory map
 * \param [in] _ ignored
 * \param [in] len length in bytes of memory map
 * \return returns start address of anonymous memory map
 */
extern void *anon_mmap(void *_, size_t len);
/**
 * Generate a read only anonymous memory map for a given file
 * \param [in] _ ignored
 * \param [in] len length in bytes of memory map
 * \param [in] prot protection bits
 * \param [in] flags mmap flags
 * \param [in] filename filename to read
 * \param [in] filedes file descriptor
 * \return returns start address of anonymous memory map
 */
extern void *roanon_mmap(void *_, size_t len, int prot, int flags,
                         const char *filename, int fildes, off_t off);
/**
 * Deallocates memory map
 * \param [in] addr start address of memory map
 * \param [in] len length in bytes
 * \return returns 0 if successful, -1 if unsuccessful.
 */
extern int anon_munmap(void *addr, size_t len);
/**
 * Suspend the calling thread until any one of a set of events has occurred
 * or until a specified amount of time has passed.
 * \param [in] secs seconds to suspend
 * \param [in] nsecs nanoseconds to suspend
 * \param [in] event_list events that will trigger thread to resume (CW_INTRPT
 *  or CW_CONDVAR)
 * \param [out] secs_rem seconds remaining
 * \param [out] nsecs_rem nanoseconds remaining
 * \return returns 0 if successful, -1 if unsuccessful.
 */
extern int __cond_timed_wait(unsigned int secs, unsigned int nsecs,
                             unsigned int event_list, unsigned int *secs_rem,
                             unsigned int *nsecs_rem);

enum COND_TIME_WAIT_CONSTANTS { CW_INTRPT = 1, CW_CONDVAR = 32 };

/**
 * Create a child process
 * \return On success, the PID of the child process is returned in the
 *  parent, and 0 is returned in the child.  On failure, -1 is returned in the
 *  parent, no child process is created, and errno is set appropriately.
 */
extern int __fork(void);

/**
 * Fill a buffer with random bytes
 * \param [out] buffer to store random bytes to.
 * \param [in] number of random bytes to generate.
 * \return On success, returns 0, or -1 on error.
 */
extern int getentropy(void *buffer, size_t length);
/**
 * Prints the build version of the library
 */
extern void __build_version(void);
/**
 * Determine the length of a fixed-size string
 * \param [in] str fixed-size character string
 * \param [in] maxlen maximum # of bytes to traverse
 * \return returns the length of the string
 */
extern size_t strnlen(const char *str, size_t maxlen);
/**
 * Attempts to a close a socket for a period of time
 * \param [in] socket socket handle
 * \param [in] secs number of seconds to attempt the close
 */
extern void __tcp_clear_to_close(int socket, unsigned int secs);
/**
 * Returns the overview structure of IPCQPROC
 * \param [out] info address of allocated IPCQPROC structure
 * \return On success, returns 0, or -1 on error.
 */
extern int get_ipcs_overview(IPCQPROC *info);
/**
 * Prints zoslib help information to specified FILE pointer
 * \param [in] FILE pointer to write to
 * \param [in] title header, specify NULL for default
 * \return On success, returns 0, or < 0 on error.
 */
extern int __print_zoslib_help(FILE* fp, const char* title);

typedef struct __cpu_relax_workarea {
  void *sfaddr;
  unsigned long t0;
} __crwa_t;

/**TODO(itodorov) - zos: document these interfaces**/
extern void __cpu_relax(__crwa_t *);
extern int __sem_init(__sem_t *s0, int shared, unsigned int val);
extern int __sem_post(__sem_t *s0);
extern int __sem_trywait(____sem_t *s0);
extern int __sem_timedwait(____sem_t *s0, const struct timespec *abs_timeout);
extern int __sem_wait(__sem_t *s0);
extern int __sem_getvalue(__sem_t *s0, int *sval);
extern int __sem_destroy(__sem_t *s0);

/**TODO(itodorov) - zos: document these interfaces **/
struct __tlsanchor;
extern struct __tlsanchor *__tlsvaranchor_create(size_t sz);
extern void __tlsvaranchor_destroy(struct __tlsanchor *anchor);
extern void *__tlsPtrFromAnchor(struct __tlsanchor *anchor, const void *);
extern int __testread(const void *location);
extern void __tb(void);

#ifdef __cplusplus
/**
 * Configuration for zoslib library
 */
typedef struct zoslib_config {
  /**
   * string to indicate the envar to be used to toggle IPC cleanup
   */
  const char *IPC_CLEANUP_ENVAR = IPC_CLEANUP_ENVAR_DEFAULT;
  /**
   * string to indicate the envar to be used to toggle debug mode
   */
  const char *DEBUG_ENVAR = DEBUG_ENVAR_DEFAULT;
  /**
   * string to indicate the envar to be used to toggle runtime limit
   */
  const char *RUNTIME_LIMIT_ENVAR = RUNTIME_LIMIT_ENVAR_DEFAULT;
  /**
   * string to indicate the envar to be used to toggle max number of forks
   */
  const char *FORKMAX_ENVAR = FORKMAX_ENVAR_DEFAULT;
  /**
   * string to indicate the envar to be used to toggle ccsid guess buf size in bytes 
   */
  const char *CCSID_GUESS_BUF_SIZE_ENVAR = CCSID_GUESS_BUF_SIZE_DEFAULT;
  /**
   * string to indicate the envar to be used to toggle the untagged read mode
   */
  const char *UNTAGGED_READ_MODE_ENVAR = UNTAGGED_READ_MODE_DEFAULT;
  /**
   * string to indicate the envar to be used to toggle the untagged 1047 read mode
   */
  const char *UNTAGGED_READ_MODE_CCSID1047_ENVAR = UNTAGGED_READ_MODE_CCSID1047_DEFAULT;
} zoslib_config_t;

/**
 * Initialize zoslib library
 * \param [in] config struct to configure zoslib.
 */
extern void init_zoslib(const zoslib_config_t config = {});
#else
/**
 * Configuration for zoslib library
 */
typedef struct zoslib_config {
  /**
   * string to indicate the envar to be used to toggle IPC cleanup
   */
  const char *IPC_CLEANUP_ENVAR;
  /**
   * string to indicate the envar to be used to toggle debug mode
   */
  const char *DEBUG_ENVAR;
  /**
   * string to indicate the envar to be used to toggle runtime limit
   */
  const char *RUNTIME_LIMIT_ENVAR;
  /**
   * string to indicate the envar to be used to toggle max number of forks
   */
  const char *FORKMAX_ENVAR;
  /**
   * string to indicate the envar to be used to toggle ccsid guess buf size in bytes 
   */
  const char *CCSID_GUESS_BUF_SIZE_ENVAR;
  /**
   * string to indicate the envar to be used to toggle the untagged read mode
   */
  const char *UNTAGGED_READ_MODE_ENVAR;
  /**
   * string to indicate the envar to be used to toggle the untagged 1047 read mode
   */
  const char *UNTAGGED_READ_MODE_CCSID1047_ENVAR;
} zoslib_config_t;
/**
 * Initialize zoslib library
 * \param [in] config struct to configure zoslib.
 */
extern void init_zoslib(const zoslib_config_t config);
#endif // __cplusplus
/**
 * Initialize the struct used to configure zoslib with default values.
 * \param [in] config struct to configure zoslib.
 */
extern void init_zoslib_config(zoslib_config_t *const config);

/**
 * Suspends the execution of the calling thread until either at least the
 * time specified in *req has elapsed, an event occurs, or a signal arrives.
 * \param [in] req struct used to specify intervals of time with nanosecond
 *  precision
 * \param [out] rem the remaining time if the call is interrupted
 */
extern int nanosleep(const struct timespec *req, struct timespec *rem);
/**
 * Changes the access and modification times of a file in the same way as
 * lutimes, with the difference that microsecond precision is not supported.
 * \param [in] filename the path to file
 * \param [in] tv two structs used to specify the new times
 */
extern int __lutimes(const char *filename, const struct timeval tv[2]);
/**
 * Updates the zoslib global variables associated with the zoslib environment variables
 * \param [in] envar environment variable to update, specify NULL to update all
 * \return On success, returns 0, or < 0 on error.
 */
void __update_envar_settings(const char* envar);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
void init_zoslib_config(zoslib_config_t &config);
#endif

#define _str_e2a(_str)                                                         \
  ({                                                                           \
    const char *src = (const char *)(_str);                                    \
    int len = strlen(src) + 1;                                                 \
    char *tgt = (char *)alloca(len);                                           \
    (char *)_convert_e2a(tgt, src, len);                                       \
  })

#define _str_a2e(_str)                                                         \
  ({                                                                           \
    const char *src = (const char *)(_str);                                    \
    int len = strlen(src) + 1;                                                 \
    char *tgt = (char *)alloca(len);                                           \
    (char *)_convert_a2e(tgt, src, len);                                       \
  })

#define AEWRAP(_rc, _x)                                                        \
  (__isASCII() ? ((_rc) = (_x), 0)                                             \
               : (__ae_thread_swapmode(__AE_ASCII_MODE), ((_rc) = (_x)),       \
                  __ae_thread_swapmode(__AE_EBCDIC_MODE), 1))

#define AEWRAP_VOID(_x)                                                        \
  (__isASCII() ? ((_x), 0)                                                     \
               : (__ae_thread_swapmode(__AE_ASCII_MODE), (_x),                 \
                  __ae_thread_swapmode(__AE_EBCDIC_MODE), 1))

#ifdef __cplusplus
#include <exception>
#include <map>
#include <string>

inline bool operator==(const pthread_t &_a, const pthread_t &_b) {
  return _a.__ == _b.__;
}
inline bool operator!=(const pthread_t &_a, const pthread_t &_b) {
  return _a.__ != _b.__;
}
inline bool operator<=(const pthread_t &_a, const pthread_t &_b) {
  return _a.__ <= _b.__;
}
inline bool operator>=(const pthread_t &_a, const pthread_t &_b) {
  return _a.__ >= _b.__;
}
inline bool operator<(const pthread_t &_a, const pthread_t &_b) {
  return _a.__ < _b.__;
}
inline bool operator>(const pthread_t &_a, const pthread_t &_b) {
  return _a.__ > _b.__;
}

class __auto_ascii {
  int ascii_mode;

public:
  __auto_ascii();
  ~__auto_ascii();
};

class __conv_off {
  int convert_state;

public:
  __conv_off();
  ~__conv_off();
};

template <typename T> class __tlssim {
  struct __tlsanchor *anchor;
  T v;

public:
  __tlssim(const T &initvalue) : v(initvalue) {
    anchor = __tlsvaranchor_create(sizeof(T));
  }
  __tlssim() {
    anchor = __tlsvaranchor_create(sizeof(T));
  }
  ~__tlssim() { __tlsvaranchor_destroy(anchor); }
  T *access(void) { return static_cast<T *>(__tlsPtrFromAnchor(anchor, &v)); }
};

struct zoslibEnvar {
  std::string envarName;
  std::string envarValue;

  zoslibEnvar(std::string name, std::string value) :
    envarName(name), envarValue(value) { }

  bool operator < (const zoslibEnvar& t) const {
    return std::tie(envarName, envarValue) < std::tie(t.envarName, t.envarValue);
  }
};

class __zinit {
  int mode;
  int cvstate;
  std::terminate_handler _th;
  int __forked;
  static __zinit *instance;

public:
  int forkmax;
  int *forkcurr;
  int shmid;
  zoslib_config_t config;
  std::map<zoslibEnvar, std::string> envarHelpMap;

public:
  __zinit(const zoslib_config_t &config);

  bool isValidZOSLIBEnvar(std::string envar);

  static __zinit *init(const zoslib_config_t &config) {
    instance = new __zinit(config);
    instance->initialize();
    return instance;
  }

  void initialize(void);

  static __zinit *getInstance() { return instance; }

  int forked(int newvalue) {
    int old = __forked;
    __forked = newvalue;
    return old;
  }

  int get_forkmax(void) { return forkmax; }

  int inc_forkcount(void) {
    if (0 == forkmax || 0 == shmid)
      return 0;
    int original;
    int new_value;

    do {
      original = *forkcurr;
      new_value = original + 1;
      __asm(" cs %0,%2,%1 \n "
            : "+r"(original), "+m"(*forkcurr)
            : "r"(new_value)
            :);
    } while (original != (new_value - 1));
    return new_value;
  }
  int dec_forkcount(void) {
    if (0 == forkmax || 0 == shmid)
      return 0;
    int original;
    int new_value;

    do {
      original = *forkcurr;
      if (original == 0)
        return 0;
      new_value = original - 1;
      __asm(" cs %0,%2,%1 \n "
            : "+r"(original), "+m"(*forkcurr)
            : "r"(new_value)
            :);
    } while (original != (new_value - 1));
    return new_value;
  }
  int shmid_value(void) { return shmid; }

  ~__zinit() {
    if (_CVTSTATE_OFF == cvstate) {
      __ae_autoconvert_state(cvstate);
    }
    __ae_thread_swapmode(mode);
    if (shmid != 0) {
      if (__forked)
        dec_forkcount();
      shmdt(forkcurr);
      shmctl(shmid, IPC_RMID, 0);
    }
    __cleanupipc(0);
  }
  void __abort() { _th(); }
};

struct __init_zoslib {
  __init_zoslib(const zoslib_config_t &config = {}) { __zinit::init(config); }
};

class __setlibpath {
public:
  __setlibpath();
};

#endif // __cplusplus

#endif //__ZOS_BASE_H_
