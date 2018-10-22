#include <array_lib.h>
#include <assert.h>

void setup() {
  stack_array<int, 3> a{2, 4, 6};
  assert(a[0] == 2);
  assert(a[1] == 4);
  assert(a[2] == 6);
  assert(a.length() == 3);
  assert(length(a.c_array) == 3);

  heap_array<int> b{3};
  b[0] = 2;
  b[1] = 4;
  b[2] = 6;
  assert(b[0] == 2);
  assert(b[1] == 4);
  assert(b[2] == 6);
  assert(b.length() == 3);
  
  growing_array<int> c;
  c.push(2);
  c.push(4);
  c.push(6);
  assert(c[0] == 2);
  assert(c[1] == 4);
  assert(c[2] == 6);
  assert(c.length() == 3);
}

void loop() {
  
}
