int GlobalObjectCount = 0;
class Collectable {
private:
  int count;
public:
  Collectable() : count(0) {
    LogGC<<"Creating new object at "<<this<<", total object count: "<<++GlobalObjectCount<<LogEnd;
  }
  virtual ~Collectable() {}
  void dereference() {
    if(this == nullptr)
      return;
    count--;
    if(count == 0) {
      LogGC<<"Deleting new object at "<<this<<", total object count: "<<--GlobalObjectCount<<LogEnd;
      delete this;
    }
  }
  void reference() {
    if(this == nullptr)
      return;
    count++;
  }
};

template<typename T> class Reference {
private:
  T *obj;
public:
  Reference() : obj(nullptr) { }
  Reference(T *obj) : obj(obj) {
    ((Collectable*)obj)->reference();
  }
  ~Reference() {
    ((Collectable*)obj)->dereference();
  }
  Reference(const Reference<T> &that) : obj(that.obj) {
    ((Collectable*)obj)->reference();
  }
  Reference(Reference<T> &&that) {
    obj = that.obj;
    that.obj = nullptr;
  }
  Reference<T> &operator=(Reference<T> &&that) {
    swap(obj, that.obj);
    return *this;
  }
  Reference<T> &operator=(const Reference<T> &that) {
    obj = that.obj;
    ((Collectable*)obj)->reference();
    return *this;
  }
  
  T *operator->() { return obj; }
  T operator*() { return *obj; }
  bool operator==(T *t) { return obj == t; }
  bool operator!=(T *t) { return obj != t; }
  bool operator==(Reference<T> r) { return obj == r.obj; }
  bool operator!=(Reference<T> r) { return obj != r.obj; }
  T *get() { return obj; }  
};
