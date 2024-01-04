
#include <assert.h>

#include <iostream>
#include <map>

static const int numberElems = 10;

template <typename T, typename U>
std::ostream& operator<<(std::ostream& stream, const std::pair<T, U>& pair) {
  stream << pair.first << ' ' << pair.second;
  return stream;
}

//============================[allocator BEGIN]

template <typename T, std::size_t size>
class Allocator {
 public:
  using value_type = T;
  using pointer = T*;
  using const_pointer = const T*;
  using reference = T&;
  using const_reference = const T&;

  Allocator() {
    m_data = reinterpret_cast<T*>(std::malloc(size * sizeof(T)));
    if (m_data == nullptr) throw std::bad_alloc();
  }

  ~Allocator() { std::free(m_data); }

  template <typename U>
  struct rebind {
    using other = Allocator<U, size>;
  };

  T* allocate(std::size_t n) {
    std::cout << "[Allocator::allocate] size = " << size << ", n = " << n
              << ", m_allocated_size + n = " << m_allocated_size + n
              << std::endl;

    assert(size - m_allocated_size >= n);
    m_allocated_size += n;
    return m_data + m_allocated_size - n;
  }
  void deallocate(T*, std::size_t n) {
    m_allocated_size -= n;
    std::cout << "[Allocator::deallocate] m_allocated_size = "
              << m_allocated_size + n << ", n = " << n
              << ", new m_allocated_size = " << m_allocated_size << std::endl;

    //    std::size_t from = p - m_data;
    //    std::cout << "[Allocator::deallocate] *p = " << *p << std::endl;
    //    ::operator delete(p);
  }

  template <class Up, class... Args>
  void construct(Up* p, Args&&... args) {
    ::new ((void*)p) Up(std::forward<Args>(args)...);
  }

  void destroy(pointer p) { p->~T(); }

 private:
  T* m_data = nullptr;
  std::size_t m_allocated_size = 0;
};

//============================[allocator END]

int factorial(int number) {
  int ans = 1;
  for (int i = 2; i <= number; ++i) {
    ans *= i;
  }
  return ans;
}

template <typename Allocator>
void fill_map(std::map<int, int, std::less<int>, Allocator>& map) {
  for (int i = 0; i < numberElems; ++i) {
    map.emplace(i, factorial(i));
  }
}

template <typename Container>
void print_container(const Container& container) {
  for (const auto& element : container) {
    std::cout << element << std::endl;
  }
}

// void print_map(const std::map<int, int> m_map) {
//   for (const auto& elem : m_map) {
//     std::cout << elem.first << ", " << elem.second << std::endl;
//   }
// }

int main() {
  try {
    std::cout << "[main] Hello world 3" << std::endl;

    std::map<int, int> map_std_alloc;
    std::map<int, int, std::less<int>, Allocator<std::pair<const int, int>, 10>>
        map_my_alloc;

    fill_map(map_std_alloc);
    fill_map(map_my_alloc);

    //    map_std_alloc.insert(std::pair<int, int>(100, 110));
    //    map_my_alloc.insert(std::pair<int, int>(100, 110));

    std::cout << "map_std_alloc:" << std::endl;
    print_container(map_std_alloc);

    std::cout << "map_my_alloc:" << std::endl;
    print_container(map_my_alloc);

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
