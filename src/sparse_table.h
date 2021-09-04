#include "all.h"

template <typename T>
class SparseTable {
 public:
  SparseTable(const vector<T>& a, const function<T(const T&, const T&)> f)
      : func_(f) {
    assert(!a.empty());
    n_ = static_cast<int>(a.size());
    int lg = 32 - __builtin_clz(n_) - 1;
    rmq_.resize(lg + 1);
    rmq_[0] = a;
    for (int j = 1; j <= lg; j++) {
      rmq_[j].resize(n_ - (1 << j) + 1);
      for (int i = 0; i + (1 << j) - 1 < n_; i++) {
        rmq_[j][i] = func_(rmq_[j - 1][i], rmq_[j - 1][i + (1 << j - 1)]);
      }
    }
  }

  T Get(int l, int r) const {
    assert(0 <= l && l <= r && r < n_);
    int lg = 32 - __builtin_clz(r - l + 1) - 1;
    return func_(rmq_[lg][l], rmq_[lg][r - (1 << lg) + 1]);
  }

 private:
  int n_;
  vector<vector<T>> rmq_;
  function<T(const T&, const T&)> func_;
};
