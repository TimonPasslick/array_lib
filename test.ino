#include "array_lib.h"
#include <assert.h>

void setup() {
  constexpr StackArray<int, 3> a{2, 4, 6};
  static_assert(a[0] == 2, "a[0] is not 2");
  static_assert(a[1] == 4, "a[1] is not 4");
  static_assert(a[2] == 6, "a[2] is not 6");
  static_assert(a.length() == 3, "a has not 3 elements");
  static_assert(length(a.c_array) == 3, "a has 3 elements but not its C array");
  
  StackArray<int, 3> b{2, 4, 6};
  assert(b[0] == 2);
  assert(b[1] == 4);
  assert(b[2] == 6);
  assert(b.length() == 3);
  assert(length(b.c_array) == 3);

  HeapArray<int> c{3};
  c[0] = 2;
  c[1] = 4;
  c[2] = 6;
  assert(c[0] == 2);
  assert(c[1] == 4);
  assert(c[2] == 6);
  assert(c.length() == 3);
  
  GrowingArray<int> d;
  d.push(2);
  d.push(4);
  d.push(6);
  assert(d[0] == 2);
  assert(d[1] == 4);
  assert(d[2] == 6);
  assert(d.length() == 3);
}

void loop() {
  
}
