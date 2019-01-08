#include <stdint.h>

typedef struct {
  uint64_t lo;
  uint64_t hi;
} uint128_t;

uint128_t mac(uint128_t const* a, uint128_t const* x, uint128_t const* y) {
  uint128_t o;
  o.lo = x->lo * y->lo + a->lo;
  o.hi = x->hi * y->lo + x->lo * y->hi + a->hi;

  return o;
}