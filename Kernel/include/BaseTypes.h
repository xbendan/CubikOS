#ifndef BASE_TYPE
#define BASE_TYPE

typedef void* IntPtr;

/**
 * @brief Common function delegate
 * 
 * This an function pointer allows functions with no returned 
 * values and parameters to abstract themselves.
 */
typedef void (*func_t)(void);

#endif