#ifndef JNP1_DICTGLOBAL_H
#define JNP1_DICTGLOBAL_H


#ifdef __cplusplus
#include <cstdlib>
#include <iostream>
extern "C" {
namespace jnp1 {

#else
#include <stdlib.h>
#endif // not __cplusplus


  const size_t MAX_GLOBAL_DICT_SIZE = 42;

  unsigned long dict_global();


#ifdef __cplusplus
} /* namespace jnp1 */
} /* extern "C" */
#endif // __cplusplus


#endif /* JNP1_DICTGLOBAL_H */
