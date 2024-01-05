
#include <assert.h>
#include <iostream>
#include <map>

// #define DEBUG_ALLOCATOR
static const int NUMBER_ELEMS = 10;

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
#ifdef DEBUG_ALLOCATOR
    std::cout << "[Allocator::allocate] size = " << size << ", n = " << n
              << ", m_allocated_size + n = " << m_allocated_size + n
              << std::endl;
#endif

    assert(size - m_allocated_size >= n);
    m_allocated_size += n;
    return m_data + m_allocated_size - n;
  }
  void deallocate(T*, std::size_t n) {
    m_allocated_size -= n;
#ifdef DEBUG_ALLOCATOR
    std::cout << "[Allocator::deallocate] m_allocated_size = "
              << m_allocated_size + n << ", n = " << n
              << ", new m_allocated_size = " << m_allocated_size << std::endl;
#endif
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

//============================[myList BEGIN]

template <typename T, typename Allocator = std::allocator<T>>
class MyList {
  struct Node {
    Node(T value, Node* next) : value(std::move(value)), next(next) {}

    T value;
    Node* next;
  };

 public:
  MyList() = default;

  ~MyList() {
    assert((size == 0 && head == nullptr) || (size != 0 && head != nullptr));
    while (head != nullptr) {
      Node* ptr = head;
      head = head->next;
      nodeAlloc.destroy(ptr);
      nodeAlloc.deallocate(ptr, 1);
      --size;
    }
  }

  void push_front(const T& value) {
    Node* newNode = nodeAlloc.allocate(1);
    nodeAlloc.construct(newNode, std::move(value), head);
    head = newNode;
    ++size;
  }

  void print_container() {
    assert((size == 0 && head == nullptr) || (size != 0 && head != nullptr));
    Node* local_head = head;
    while (local_head != nullptr) {
      Node* ptr = local_head;
      local_head = local_head->next;
      std::cout << ptr->value << '\n';
    }
  }

 private:
  using NodeAllocator = typename Allocator::template rebind<Node>::other;
  Node* head = nullptr;
  NodeAllocator nodeAlloc;
  std::size_t size = 0;
};

//============================[myList END]

int factorial(int number) {
  int ans = 1;
  for (int i = 2; i <= number; ++i) {
    ans *= i;
  }
  return ans;
}

template <typename Allocator>
void fill_map(std::map<int, int, std::less<int>, Allocator>& map) {
  for (int i = 0; i < NUMBER_ELEMS; ++i) {
    map.emplace(i, factorial(i));
  }
}

template <typename Allocator>
void fill_list(MyList<int, Allocator>& list) {
  for (int i = NUMBER_ELEMS - 1; i >= 0; --i) {
    list.push_front(i);
  }
}

template <typename Container>
void print_container(const Container& container) {
  for (const auto& element : container) {
    std::cout << element << std::endl;
  }
}

int main() {
  try {
    //    std::cout << "[main] Hello world 3" << std::endl;
    // myAllocator
    std::map<int, int> map_std_alloc;
    std::map<int, int, std::less<int>,
             Allocator<std::pair<const int, int>, NUMBER_ELEMS>>
        map_my_alloc;

    fill_map(map_std_alloc);
    fill_map(map_my_alloc);

    std::cout << "map_std_alloc:" << std::endl;
    print_container(map_std_alloc);

    std::cout << "map_my_alloc:" << std::endl;
    print_container(map_my_alloc);

    // myList
    MyList<int> list_std_alloc;
    MyList<int, Allocator<int, NUMBER_ELEMS>> list_my_alloc;

    fill_list(list_std_alloc);
    fill_list(list_my_alloc);

    std::cout << "list_std_alloc:" << std::endl;
    list_std_alloc.print_container();

    std::cout << "list_my_alloc:" << std::endl;
    list_my_alloc.print_container();

  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
