/*
 * array_lib.h - Library to make working with arrays on the stack, on the heap and growing arrays on the heap more comfortable.
 * Created by Timon Paßlick, October 21, 2018.
 * No license, please use this code however you want. No guarantees too, though.
 */

#ifndef TIMON_PASSLICK_ARRAY_LIB
#define TIMON_PASSLICK_ARRAY_LIB

//returns the length of a C array
template <typename T, size_t N>
inline constexpr size_t length(const T(&)[N]) {
  return N;
}

//an array with a size which is known before the program runs
//You can tell with 'constexpr' in front of a variable declaration that you know also the contents of the array before the array runs and they won't change.
template <typename T, size_t N>
struct stack_array {

  //A stack_array is a wrapper around a C array which is a public member.
  T c_array[N];

  //You can access the elements of the array at runtime just like with C arrays.
  //If the index is bigger than the array size, the program will crash.
  T& operator [] (const size_t index) {
    if (index >= N) {
      abort();
    }
    return c_array[index];
  }

  //You can also access the elements of the array before the program runs. Don't forget to declare the variable 'constexpr', though.
  constexpr T operator [] (const size_t index) {
    return (index < N) ? c_array[index] : constexpr_stack_array_index_out_of_bounds();
  }

  //You can access the length before and when the program runs.
  constexpr size_t length() {
    return N;
  }

  private:
  //just a helper function to throw a compiler error when accessing an element out of bounds before the program runs
  static inline T constexpr_stack_array_index_out_of_bounds() {
    abort();
    return *(T*) nullptr; //never called
  }
};


//an array with a size which is known when the program runs and won't change
template <typename T>
class heap_array {

  private:
    //A heap_array is internally a dynamic array with a stored size.
    T* begin;
    size_t size;

  public:

    //When constructing a heap_array, you must provide its size.
    heap_array(size_t length) : begin{new T[length]}, size{length} { }

    //You can access the elements just like with C arrays.
    //If the index is bigger than the array size, the program will crash.
    T& operator [] (const size_t index) {
      if (index >= size) {
        abort();
      }
      return begin[index];
    }

    //You can access the length.
    size_t length() {
      return size;
    }

    ~heap_array(){
      delete[] begin;
    }
};


//a growing array: You can push elements onto its end.
template <typename T>
class growing_array {

  private:
    //A growing_array is internally allocated heap space.
    //It is not reallocated for every new element, so we need to store a capacity.
    T* begin;
    size_t size;
    size_t capacity;


  public:
    //For the sake of simplicity, there is just a default constructor which creates an empty growing_array.
    growing_array() : begin{nullptr}, size{0}, capacity{0} { }

    //You can access the elements just like with C arrays.
    //If the index is bigger than the array size, the program will crash.
    T& operator [] (const size_t index) {
      if (index >= size) {
        abort();
      }
      return begin[index];
    }

    //pushes an item to the back of the vector
    //The item is copied. If you notice a performance bottleneck for growing_array insertion, you might want to extend this class.
    //The array might get reallocated, so references you got by accessing an element get invalidated.
    void push(const T& item) {
      if (size == capacity) {
        if (capacity == 0) {
          capacity = 1;
        }
        //not the usual * 2 because memory space on Arduinos is sparse
        capacity = (capacity * 3) / 2;
        realloc(begin, capacity * sizeof(T));
      }
      begin[size] = item;
      ++size;
    }

    //You can read the size but not change it directly.
    size_t length() {
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
