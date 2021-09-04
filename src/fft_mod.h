#include "all.h"
#include "modular.h"

template <typename D>
class FFT {
 public:
  using cp = complex<D>;
  static const int M = 16;

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

  template <int T>
  vector<Modular<T>> Mult(const vector<Modular<T>> &a,
                            const vector<Modular<T>> &b) {
    if (a.empty() || b.empty()) return {};
    auto n = static_cast<int>(a.size() + b.size()) - 1;
    auto cur_base = n == 1 ? 0 : 32 - __builtin_clz(n - 1);
    vector<cp> A(1 << cur_base), B(1 << cur_base);
    for (int i = 0; i < size(a); i++)
      A[i] = cp(static_cast<int>(a[i]) >> M,
                static_cast<int>(a[i]) & (1 << M) - 1);
    for (int i = 0; i < size(b); i++)
      B[i] = cp(static_cast<int>(b[i]) >> M,
                static_cast<int>(b[i]) & (1 << M) - 1);
    DFT(A, false);
    DFT(B, false);
    vector<cp> X(1 << cur_base), Y(1 << cur_base);
    for (int i = 0; i < 1 << cur_base; i++) {
      int j = i ? (1 << cur_base) - i : 0;
      auto x = A[i] + conj(A[j]);
      auto y = B[i] + conj(B[j]);
      auto z = conj(A[j]) - A[i];
      auto t = conj(B[j]) - B[i];
      X[i] = (x * t + y * z) * cp(0, 0.25);
      Y[i] = x * y * cp(0, 0.25) + z * t * cp(-0.25, 0);
    }
    DFT(X, true);
    DFT(Y, true);
    vector<Modular<T>> res(n);
    for (int i = 0; i < n; i++) {
      int64_t u = static_cast<int64_t>(round(X[i].real())) % T;
      int64_t v = static_cast<int64_t>(round(Y[i].real())) % T;
      int64_t w = static_cast<int64_t>(round(Y[i].imag())) % T;
      res[i] = Modular<T>((u << M) + v + (w << M * 2));
    }
    return res;
  }

 private:
  D PI;
  vector<cp> w_;
};
