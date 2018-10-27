/*
 * array_lib.h - Library to make working with arrays on the stack, on the heap and growing arrays on the heap more comfortable.
 * Created by Timon Paßlick, October 21, 2018.
 * No license, please use this code however you want. No guarantees too, though.
 */

#ifndef TIMON_PASSLICK_ARRAY_LIB
#define TIMON_PASSLICK_ARRAY_LIB

#ifndef ARRAY_LIB_PLACEMENT_NEW_DEFINED
//DEFINED FOR THE WHOLE INO FILE, I KNOW NO OTHER WAY
//You can turn this off by defining the flag above
void* operator new(size_t, void* p) { return p; } //placement new
#endif

//returns the length of a C array
template <typename T, size_t N>
inline constexpr size_t length(const T(&)[N]) {
  return N;
}

//an array with a size which is known before the program runs
//You can tell with 'constexpr' in front of a variable declaration that you know also the contents of the array before the array runs and they won't change.
template <typename T, size_t N>
struct StackArray {

  //A stack_array is a wrapper around a C array which is a public member.
  T c_array[const N];

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
class HeapArray {

  private:
    //A HeapArray is internally a dynamic array with a stored size.
    T* begin;
    size_t size;

  public:

    //When constructing a HeapArray, you must provide its size.
    HeapArray(size_t length) : begin{new T[length]}, size{length} { }

    HeapArray(HeapArray<T>&& temp) : begin{temp.begin}, size{temp.size} {
      temp.begin = nullptr;
      temp.size = 0;
    }
    HeapArray(const HeapArray&) = delete; //TODO: implement copy() method
    
    

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

    ~HeapArray(){
      delete[] begin;
    }
};


//a growing array: You can push elements onto its end.
template <typename T>
class GrowingArray {

  private:
    //A GrowingArray is internally allocated heap space.
    //It is not reallocated for every new element, so we need to store a capacity.
    T* begin;
    size_t size;
    size_t capacity;


  public:
    //For the sake of simplicity, there is just a default constructor which creates an empty growing_array.
    GrowingArray() : begin{nullptr}, size{0}, capacity{0} { }

    GrowingArray(GrowingArray&& temp) : begin{temp.begin}, size{temp.size}, capacity{temp.capacity} {
      temp.begin = nullptr;
      temp.size = 0;
    }
    GrowingArray(const GrowingArray&&) = delete; //TODO: implement copy() method

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
        capacity = (capacity * 3 + 1) / 2;
        auto new_begin = reinterpret_cast<T*>(malloc(capacity * sizeof(T)));
        for (size_t i{0}; i != size; ++i) {
          new (&new_begin[i]) T(move(begin[i])); //calling the T move constructor with begin[i] explicitly at new_begin[i]
        }
        begin = new_begin;
      }
      begin[size] = item;
      ++size;
    }

    //You can read the size but not change it directly.
    size_t length() {
      return size;
    }

    ~GrowingArray() {
      for (int i = 0; i != size; ++i) {
        begin[i].~T();
      }
      free(begin);
    }

  private:

    template< typename Ty > struct remove_reference       {using type = Ty;};
    template< typename Ty > struct remove_reference<Ty&>  {using type = Ty;};
    template< typename Ty > struct remove_reference<Ty&&> {using type = Ty;};

    template <typename Ty>
    typename remove_reference<Ty>::type&& move(Ty&& arg) {
      return static_cast<typename remove_reference<Ty>::type&&>(arg);
    }
};

#endif //TIMON_PASSLICK_ARRAY_LIB
