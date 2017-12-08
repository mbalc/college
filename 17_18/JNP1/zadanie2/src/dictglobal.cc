#include "dict.h"
#include "dictglobal.h"
#include <cassert>
#include <iostream>


/* Compilation flag -DNDEBUG disables debug logging */
#ifndef NDEBUG
/* Main debug printer macro; prints message along with details regarding message
 * origin */
#define DEBUG(...)                                                             \
  do {                                                                         \
    dict_debug(__VA_ARGS__, " [{", __FUNCTION__, ", ", __FILE__, ":",          \
               __LINE__, "}]");                                                \
  } while (0)

#else
#define DEBUG(...) ((void)0)
#endif // NDEBUG


namespace {
/* typ dla identyfikatorów słowników */
using identifier_t = unsigned long;

/* Prints all arguments on standard error */
inline void print_debug() {
  std::cerr << "\n";
}
template <typename T, typename... R> void print_debug(T first, R... args) {
  std::cerr << first;
  print_debug(args...);
}

/* Prints a debug message with contents of [args...] */
template <typename... T> void dict_debug(T... args) {
  std::cerr << "DEBUG:  | ";
  print_debug(args...);
}
} /* namespace */


namespace jnp1 {
identifier_t dict_global() {
  static bool isGlobalDictInitiated = false;
  static identifier_t globalDictId;
  if (!isGlobalDictInitiated) {
    isGlobalDictInitiated = true;
    globalDictId = jnp1::dict_new(); // lazy initiation of global dictionary;
                                     // this way we can log exact global
                                     // dictionary initialization moment
    DEBUG("±      Initiating The Global Dictionary with id=", globalDictId,
          "!");
  }
  assert(isGlobalDictInitiated);
  return globalDictId;
}
} /* namespace jnp1 */
