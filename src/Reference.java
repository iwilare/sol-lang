class Reference<T>  {
    T v;
    Reference() {}
    Reference(T v) { this.v = v; }
    public void set(T nv) { v = nv; }
    public T get() { return v; }
}
