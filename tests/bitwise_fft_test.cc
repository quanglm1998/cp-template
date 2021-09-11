#include <../src/bitwise_fft.h>
#include <../src/modular.h>
#include <gtest/gtest.h>

mt19937 rng;
using fft = BitwiseFFT;

bool IsPowerOfTwo(int n) { return n > 0 && n == (n & -n); }

template <typename T>
vector<T> XorConvolution(const vector<T> &a, const vector<T> &b) {
  assert(IsPowerOfTwo(a.size()));
  assert(IsPowerOfTwo(b.size()));
  assert(a.size() == b.size());
  vector<T> res(a.size());
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      res[i ^ j] += a[i] * b[j];
    }
  }
  return res;
}

template <typename T>
vector<T> OrConvolution(const vector<T> &a, const vector<T> &b) {
  assert(IsPowerOfTwo(a.size()));
  assert(IsPowerOfTwo(b.size()));
  assert(a.size() == b.size());
  vector<T> res(a.size());
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      res[i | j] += a[i] * b[j];
    }
  }
  return res;
}

template <typename T>
vector<T> AndConvolution(const vector<T> &a, const vector<T> &b) {
  assert(IsPowerOfTwo(a.size()));
  assert(IsPowerOfTwo(b.size()));
  assert(a.size() == b.size());
  vector<T> res(a.size());
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      res[i & j] += a[i] * b[j];
    }
  }
  return res;
}

TEST(BitwiseFFTTest, XOR) {
  for (int it = 0; it <= 10; it++) {
    int n = 1 << it;
    vector<Mint> a(n);
    for (auto &u : a) u = Mint(rng() % MOD);
    vector<Mint> b(n);
    for (auto &u : b) u = Mint(rng() % MOD);
    auto c = XorConvolution(a, b);
    auto d = fft::XorConvolution(a, b);
    EXPECT_EQ(c, d);
  }
}

TEST(BitwiseFFTTest, OR) {
  for (int it = 0; it <= 10; it++) {
    int n = 1 << it;
    vector<Mint> a(n);
    for (auto &u : a) u = Mint(rng() % MOD);
    vector<Mint> b(n);
    for (auto &u : b) u = Mint(rng() % MOD);
    auto c = OrConvolution(a, b);
    auto d = fft::OrConvolution(a, b);
    EXPECT_EQ(c, d);
  }
}

TEST(BitwiseFFTTest, AND) {
  for (int it = 0; it <= 10; it++) {
    int n = 1 << it;
    vector<Mint> a(n);
    for (auto &u : a) u = Mint(rng() % MOD);
    vector<Mint> b(n);
    for (auto &u : b) u = Mint(rng() % MOD);
    auto c = AndConvolution(a, b);
    auto d = fft::AndConvolution(a, b);
    EXPECT_EQ(c, d);
  }
}
