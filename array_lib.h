/*
 * array_lib.h - Library to make working with arrays on the stack, on the heap and growing arrays on the heap more comfortable.
 * Created by Timon Paßlick, October 21, 2018.
 * No license, please use this code however you want. No guarantees too, though.
 */

#ifndef TIMON_PASSLICK_ARRAY_LIB
#define TIMON_PASSLICK_ARRAY_LIB

template <typename T, size_t N>
inline size_t length(const T(&)[N]) {
  return N;
}

template <typename T, size_t N>
struct stack_array {
  
  T c_array[N];

  T& operator [] (const size_t index) {
    if (index >= N) {
      abort();
    }
    return c_array[index];
  }

  constexpr size_t length() {
    return N;
  }
};


template <typename T>
class heap_array {

  private:
    T* begin;
    size_t size;

  public:

    heap_array(size_t length) : begin{new T[length]}, size{length} { }

    T& operator [] (const size_t index) {
      if (index >= size) {
        abort();
      }
      return begin[index];
    }

    size_t length() {
      return size;
    }

    ~heap_array(){
      delete[] begin;
    }
};


template <typename T>
class growing_array {
  
  private:
    T* begin;
    size_t size;
    size_t capacity;

    
  public:
  
    growing_array() : begin{nullptr}, size{0}, capacity{0} { }
    
    T& operator [] (const size_t index) {
      if (index >= size) {
        abort();
      }
      return begin[index];
    }
    
    void push(const T& item) {
      if (size == capacity) {
        capacity = (capacity * 3) / 2 + 1;
        realloc(begin, capacity * sizeof(T));
      }
      begin[size] = item;
      ++size;
    }

    constexpr size_t length() {
      return size;
    }

    ~growing_array() {
      for (int i = 0; i != size; ++i) {
        begin[i].~T();
      }
      free(begin);
    }
};

#endif //TIMON_PASSLICK_ARRAY_LIB
