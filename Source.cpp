#include <memory>
#include <set>

// GOG 2018 =]
// g++ Source.cpp --std=c++14
template<class T>
class foo_ptr {
  private:
  struct metadata {
    std::unique_ptr<T> data;
    std::set<foo_ptr*> refs;
  };
  metadata* metadata_h;

  void perform_deep_deletion() {
    metadata_h->refs.erase(this);
    if (metadata_h->refs.size() == 0) {
      delete metadata_h;
    }
  }
  public:
  foo_ptr(T* data) {
    metadata* ptr = new metadata();
    ptr->data = std::unique_ptr<int>(data);
    ptr->refs.insert(this);

    metadata_h = ptr;
  }

  foo_ptr(const foo_ptr& ptr) {
    if (this != &ptr) {
      metadata_h = ptr.metadata_h;
      metadata_h->refs.insert(this);
    }
  }

  foo_ptr& operator=(const foo_ptr& ptr) {
    if (this != &ptr) {
      if (metadata_h) {
        perform_deep_deletion();
      }
      metadata_h = ptr.metadata_h;
      metadata_h->refs.insert(this);
    }
    return *this;
  }

  ~foo_ptr() {
    if (metadata_h) {
      perform_deep_deletion();
    }
  }

  T* get() {
    if (metadata_h) {
      return metadata_h->data.get();
    }
    return nullptr;
  }

  /**
  * Automatically releases memory and invalidates all references to the owned object.
  */
  void destroy() {
    auto temp = metadata_h;
    for (auto& ptr : metadata_h->refs) {
      ptr->metadata_h = nullptr;
    }
    delete temp; // data and refs will be released
  }
};

int main() {
  foo_ptr<int> a(new int(5));
  foo_ptr<int> b = a;
  foo_ptr<int> c(new int(10));
  b = c;
  b.destroy();
  printf("a[%p] b[%p] c[%p]\n", a.get(), b.get(), c.get());
  return 0;
}