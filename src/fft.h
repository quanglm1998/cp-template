#include "all.h"

template <typename D>
class FFT {
 public:
  using cp = complex<D>;

  FFT() : PI(acos(static_cast<D>(-1))) {}

  void DFT(vector<cp> &a, bool inverse) {
    auto n = static_cast<int>(a.size());
    assert(n == (n & -n) && "size of a is not a power of 2");
    auto base = n == 1 ? 0 : 32 - __builtin_clz(n - 1);
    for (int i = 1, j = 0; i < n; i++) {
      for (int k = n >> 1; k > (j ^= k); k >>= 1) {
      }
      if (j < i) swap(a[i], a[j]);
    }
    if (w_.size() != n) {
      w_.resize(n);
      for (int i = 0; i < n; i++) w_[i] = polar(static_cast<D>(1), PI * i / n);
    }
    for (int i = 0; i < base; i++) {
      for (int j = 0; j < 1 << i; j++) {
        for (int k = j; k < n; k += 2 << i) {
          auto u = a[k];
          auto v = a[k | 1 << i] * w_[(n >> i) * j];
          a[k] = u + v;
          a[k | 1 << i] = u - v;
        }
      }
    }
    if (inverse) {
      reverse(a.begin() + 1, a.end());
      for (auto &u : a) u /= n;
    }
  }

  template <typename T>
  vector<T> Mult(const vector<T> &a, const vector<T> &b) {
    if (a.empty() || b.empty()) return {};
    auto n = static_cast<int>(a.size() + b.size()) - 1;
    auto cur_base = n == 1 ? 0 : 32 - __builtin_clz(n - 1);
    vector<cp> A(1 << cur_base);
    for (int i = 0; i < 1 << cur_base; i++)
      A[i] = cp(i < a.size() ? a[i] : 0, i < b.size() ? b[i] : 0);
    DFT(A, false);
    vector<cp> B(1 << cur_base);
    for (int i = 0; i < 1 << cur_base; i++) {
      int j = i ? (1 << cur_base) - i : 0;
      B[i] = (A[i] * A[i] - conj(A[j] * A[j])) * cp(0, -0.25);
    }
    DFT(B, true);
    vector<T> res(n);
    for (int i = 0; i < n; i++) res[i] = static_cast<T>(round(B[i].real()));
    return res;
  }

 private:
  D PI;
  vector<cp> w_;
};
