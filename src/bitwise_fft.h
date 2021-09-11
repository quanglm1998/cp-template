#include "all.h"

class BitwiseFFT {
 public:
  /*
   * matrix:
   * +1 +1
   * +1 -1
   */
  template <typename T>
  static void XorFFT(vector<T> &a, bool invert) {
    auto n = static_cast<int>(a.size());
    for (int i = 1; i < n; i <<= 1) {
      for (int j = 0; j < n; j += i << 1) {
        for (int k = 0; k < i; k++) {
          T u = a[j + k], v = a[i + j + k];
          a[j + k] = u + v;
          a[i + j + k] = u - v;
        }
      }
    }
    if (invert) {
      T inv = T(n).Inv();
      for (auto &u : a) u *= inv;
    }
  }

  template <typename T>
  static vector<T> XorConvolution(vector<T> a, vector<T> b) {
    assert(IsPowerOfTwo(a.size()));
    assert(IsPowerOfTwo(b.size()));
    assert(a.size() == b.size());
    XorFFT(a, 0);
    XorFFT(b, 0);
    for (int i = 0; i < a.size(); i++) a[i] *= b[i];
    XorFFT(a, 1);
    return a;
  }

  /*
   * Matrix:
   * +1 +1
   * +1 +0
   */
  template <typename T>
  static void OrFFT(vector<T> &a, bool invert) {
    auto n = static_cast<int>(a.size());
    for (int i = 1; i < n; i <<= 1) {
      for (int j = 0; j < n; j += i << 1) {
        for (int k = 0; k < i; k++) {
          T u = a[j + k], v = a[i + j + k];
          if (!invert) {
            a[j + k] = u + v;
            a[i + j + k] = u;
          } else {
            a[j + k] = v;
            a[i + j + k] = u - v;
          }
        }
      }
    }
  }

  template <typename T>
  static vector<T> OrConvolution(vector<T> a, vector<T> b) {
    assert(IsPowerOfTwo(a.size()));
    assert(IsPowerOfTwo(b.size()));
    assert(a.size() == b.size());
    OrFFT(a, 0);
    OrFFT(b, 0);
    for (int i = 0; i < a.size(); i++) a[i] *= b[i];
    OrFFT(a, 1);
    return a;
  }

  /*
   * matrix:
   * +0 +1
   * +1 +1
   */
  template <typename T>
  static void AndFFT(vector<T> &a, int invert) {
    auto n = static_cast<int>(a.size());
    for (int i = 1; i < n; i <<= 1) {
      for (int j = 0; j < n; j += i << 1) {
        for (int k = 0; k < i; k++) {
          T u = a[j + k], v = a[i + j + k];
          if (!invert) {
            a[j + k] = v;
            a[i + j + k] = u + v;
          } else {
            a[j + k] = v - u;
            a[i + j + k] = u;
          }
        }
      }
    }
  }

  template <typename T>
  static vector<T> AndConvolution(vector<T> a, vector<T> b) {
    assert(IsPowerOfTwo(a.size()));
    assert(IsPowerOfTwo(b.size()));
    assert(a.size() == b.size());
    AndFFT(a, 0);
    AndFFT(b, 0);
    for (int i = 0; i < a.size(); i++) a[i] *= b[i];
    AndFFT(a, 1);
    return a;
  }

 private:
  static bool IsPowerOfTwo(int n) { return n > 0 && n == (n & -n); }
};
