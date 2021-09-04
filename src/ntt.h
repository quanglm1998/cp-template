#include "all.h"
#include "modular.h"

class NTT {
 public:
  NTT() {
    max_base_ = __builtin_ctz(MOD - 1);
    auto root = Mint(2);
    while (root.Power(1 << max_base_) != Mint(1) ||
           root.Power(1 << max_base_ - 1) == Mint(1))
      root++;
    for (int i = 0; i < max_base_; i++, root *= root) roots_.push_back(root);
    reverse(roots_.begin(), roots_.end());
    rev_.resize(1 << max_base_);
    for (int i = 0; i < (1 << max_base_); i++)
      rev_[i] = (rev_[i >> 1] >> 1) + ((i & 1) << (max_base_ - 1));
  }

  void FFT(vector<Mint>& a, bool inverse) {
    auto n = static_cast<int>(a.size());
    assert(n == (n & -n) && "size of a is not a power of 2");
    auto base = n == 1 ? 0 : 32 - __builtin_clz(n - 1);
    assert(base <= max_base_ && "size of a is to large");
    auto shift = max_base_ - base;
    for (int i = 0; i < n; i++)
      if (i < (rev_[i] >> shift)) swap(a[i], a[rev_[i] >> shift]);
    for (int i = 0; i < base; i++)
      for (auto [j, w] = pair(0, Mint(1)); j < 1 << i; j++, w *= roots_[i])
        for (int k = j; k < n; k += 2 << i) {
          auto u = a[k];
          auto v = a[k + (1 << i)] * w;
          a[k] = u + v;
          a[k + (1 << i)] = u - v;
        }
    if (inverse) {
      reverse(a.begin() + 1, a.end());
      auto inv_n = Mint(n).Inv();
      for (auto& u : a) u *= inv_n;
    }
  }

  vector<Mint> Mult(vector<Mint> a, vector<Mint> b) {
    if (a.empty() || b.empty()) return {};
    auto n = static_cast<int>(a.size() + b.size()) - 1;
    auto cur_base = n == 1 ? 0 : 32 - __builtin_clz(n - 1);
    a.resize(1 << cur_base), b.resize(1 << cur_base);
    FFT(a, false);
    FFT(b, false);
    for (int i = 0; i < 1 << cur_base; i++) a[i] *= b[i];
    FFT(a, true);
    a.resize(n);
    return a;
  }

 private:
  int max_base_;
  vector<Mint> roots_;  // 0: -1, 1: i, ...
  vector<int> rev_;
};
