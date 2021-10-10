#include "all.h"

// Node example for range adding and range sum getting
template <typename T>
struct Node {
  Node() { sum = add = T(0); }

  static Node worst() {
    Node res;
    return res;
  }

  template <typename S>
  void Apply(int l, int r, const S &val) {
    sum += val * T(r - l + 1);
    add += val;
  }

  void Pull(int l, int r, const Node &u, const Node &v) {
    sum = u.sum + v.sum;
    add = T(0);
  }

  void Push(int l, int r, Node &u, Node &v) {
    if (!add) return;
    int m = l + r >> 1;
    u.Apply(l, m, add);
    v.Apply(m + 1, r, add);
    add = T(0);
  }

  T sum;
  T add;
};

// Node example for range adding and range max getting
template <typename T>
struct NodeMax {
  NodeMax() { max_val = add = T(0); }

  static NodeMax worst() {
    NodeMax res;
    res.max_val = numeric_limits<T>::min();
    res.add = T(0);
    return res;
  }

  template <typename S>
  void Apply(int l, int r, const S &val) {
    max_val += val;
    add += val;
  }

  void Pull(int l, int r, const NodeMax &u, const NodeMax &v) {
    max_val = max(u.max_val, v.max_val);
    add = T(0);
  }

  void Push(int l, int r, NodeMax &u, NodeMax &v) {
    if (!add) return;
    int m = l + r >> 1;
    u.Apply(l, m, add);
    v.Apply(m + 1, r, add);
    add = T(0);
  }

  T max_val;
  T add;
};

template <typename T>
class SegmentTree {
 public:
  explicit SegmentTree(int n) : n_(n) { t_.assign(n_ << 2 | 1, T()); }

  template <typename S>
  SegmentTree(int n, const vector<S> &val) : n_(n) {
    t_.assign(n_ << 2 | 1, T());
    Init(1, 0, n_ - 1, val);
  }

  // apply `val` to all position in range [x, y]
  template <typename S>
  void Update(int x, int y, const S &val) {
    Update(1, 0, n_ - 1, x, y, val);
  }

  T Get(int x, int y) { return Get(1, 0, n_ - 1, x, y); }

  // LeftMostHas/RightMostHas returns the left/right most position v
  // such that:
  //   - v is in the segment [x, y]
  //   - and f(u) = true if node u contains position v
  // Returns -1 if we can't find such position v
  int LeftMostHas(int x, int y, const function<bool(const T &)> &f) {
    return LeftMostHas(1, 0, n_ - 1, x, y, f);
  }
  int RightMostHas(int x, int y, const function<bool(const T &)> &f) {
    return RightMostHas(1, 0, n_ - 1, x, y, f);
  }

 private:
  template <typename S>
  void Init(int node, int l, int r, const vector<S> &val) {
    if (l == r) {
      t_[node].Apply(l, r, val[l]);
      return;
    }
    auto m = l + r >> 1;
    Init(node << 1, l, m, val);
    Init(node << 1 | 1, m + 1, r, val);
    t_[node].Pull(l, r, t_[node << 1], t_[node << 1 | 1]);
  }

  template <typename S>
  void Update(int node, int l, int r, int x, int y, const S &val) {
    if (x > r || y < l) return;
    if (x <= l && r <= y) {
      t_[node].Apply(l, r, val);
      return;
    }
    auto m = l + r >> 1;
    t_[node].Push(l, r, t_[node << 1], t_[node << 1 | 1]);
    Update(node << 1, l, m, x, y, val);
    Update(node << 1 | 1, m + 1, r, x, y, val);
    t_[node].Pull(l, r, t_[node << 1], t_[node << 1 | 1]);
  }

  T Get(int node, int l, int r, int x, int y) {
    if (x > r || y < l) return T::worst();
    if (x <= l && r <= y) return t_[node];
    auto m = l + r >> 1;
    t_[node].Push(l, r, t_[node << 1], t_[node << 1 | 1]);
    T p1 = Get(node << 1, l, m, x, y);
    T p2 = Get(node << 1 | 1, m + 1, r, x, y);
    T res;
    res.Pull(l, r, p1, p2);
    return res;
  }

  int LeftMostHas(int node, int l, int r, int x, int y,
                  const function<bool(const T &)> &f) {
    if (x > r || y < l) return -1;
    if (!f(t_[node])) return -1;
    if (l == r) return l;
    int m = l + r >> 1;
    t_[node].Push(l, r, t_[node << 1], t_[node << 1 | 1]);
    if (int res = LeftMostHas(node << 1, l, m, x, y, f); res != -1) {
      return res;
    }
    return LeftMostHas(node << 1 | 1, m + 1, r, x, y, f);
  }

  int RightMostHas(int node, int l, int r, int x, int y,
                   const function<bool(const T &)> &f) {
    if (x > r || y < l) return -1;
    if (!f(t_[node])) return -1;
    if (l == r) return l;
    int m = l + r >> 1;
    t_[node].Push(l, r, t_[node << 1], t_[node << 1 | 1]);
    if (int res = RightMostHas(node << 1 | 1, m + 1, r, x, y, f); res != -1) {
      return res;
    }
    return RightMostHas(node << 1, l, m, x, y, f);
  }

  int n_;
  vector<T> t_;
};
