#include "all.h"
using namespace std;

template <typename T>
class BIT {
 public:
  explicit BIT(int n) : n_(n), t_(n, T(0)) {}

  void Add(int x, T v) {
    assert(x >= 0);
    for (; x < n_; x += (x + 1) & -(x + 1)) t_[x] += v;
  }

  // [l, r] INCLUSIVELY
  void Add(int l, int r, T v) {
    assert(l <= r && l >= 0 && r < n_);
    Add(l, v);
    Add(r + 1, -v);
  }

  T Get(int x) const {
    assert(x >= 0 && x < n_);
    T res(0);
    for (; x >= 0; x -= (x + 1) & -(x + 1)) res += t_[x];
    return res;
  }

  // [l, r] INCLUSIVELY
  T Get(int l, int r) const {
    assert(l <= r && r < n_ && l >= 0);
    return Get(r) - (l ? Get(l - 1) : T(0));
  }

 private:
  int n_;
  vector<T> t_;
};
