#include <../src/ntt_with_is.h>
#include <gtest/gtest.h>

const int N = 1000;

mt19937 rng;
mt19937_64 rng_64;

class NttTest : public ::testing::Test {
 protected:
  NttWithIS ntt;
};

template <int T>
vector<Modular<T>> Naive(const vector<Modular<T>> &a,
                         const vector<Modular<T>> &b) {
  if (a.empty() || b.empty()) return {};
  vector<Modular<T>> res(a.size() + b.size() - 1);
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      res[i + j] += a[i] * b[j];
    }
  }
  return res;
}

TEST_F(NttTest, Empty) {
  EXPECT_EQ(vector<Mint>{}, ntt.Mult(vector<Mint>{}, vector<Mint>{}));
  EXPECT_EQ(vector<Mint>{},
            ntt.Mult(vector<Mint>{}, vector<Mint>{Mint(1), Mint(2)}));
  EXPECT_EQ(vector<Mint>{},
            ntt.Mult(vector<Mint>{Mint(1), Mint(2)}, vector<Mint>{}));
  EXPECT_EQ(vector<Mint>{}, ntt.Mult(vector<Mint>{Mint(1)}, vector<Mint>{}));
  EXPECT_EQ(vector<Mint>{}, ntt.Mult(vector<Mint>{}, vector<Mint>{}));
  EXPECT_EQ(vector<Mint>{},
            ntt.Mult(vector<Mint>{}, vector<Mint>{Mint(1), Mint(2)}));
}

TEST_F(NttTest, ConvLL) {
  for (int n = 1; n < 20; n++) {
    for (int m = 1; m < 20; m++) {
      vector<Mint> a(n), b(m);
      for (int i = 0; i < n; i++) {
        a[i] = Mint(rng() % 1'000'000 - 500'000);
      }
      for (int i = 0; i < m; i++) {
        b[i] = Mint(rng() % 1'000'000 - 500'000);
      }
      EXPECT_EQ(Naive(a, b), ntt.Mult(a, b));
    }
  }
}

TEST_F(NttTest, ConvLLBound) {
  for (int i = 0; i < 1000; i++) {
    vector<Mint> a = {Mint(i)};
    vector<Mint> b = {Mint(1)};

    EXPECT_EQ(a, ntt.Mult(a, b));
  }
  for (int i = 0; i < 1000; i++) {
    vector<Mint> a = {Mint(-i)};
    vector<Mint> b = {Mint(1)};

    EXPECT_EQ(a, ntt.Mult(a, b));
  }
}

TEST_F(NttTest, FftBound) {
  vector<Mint> c(N), d(N);
  for (auto &u : c) {
    u = Mint(rng() % 200'000'000 - 100'000'000);
  }
  for (auto &u : d) {
    u = Mint(rng() % 100'000'000 - 50'000'000);
  }
  EXPECT_EQ(Naive(c, d), ntt.Mult(c, d));
}

TEST_F(NttTest, Size) {
  for (int it = 0; it < 100; it++) {
    int n = rng() % 100 + 1;
    int m = rng() % 100 + 1;
    vector<Mint> a(n), b(m);
    for (auto &u : a) {
      u = Mint(rng() % 4'000'000 - 2'000'000);
    }
    for (auto &u : b) {
      u = Mint(rng() % 2'000'000 - 1'000'000);
    }
    EXPECT_EQ(Naive(a, b), ntt.Mult(a, b));
  }
}

TEST_F(NttTest, Inv) {
  for (int it = 0; it < 100; it++) {
    int n = rng() % N + 1;
    vector<Mint> a(n);
    for (auto &u : a) {
      u = Mint(rng() % MOD);
    }
    if (a[0] == Mint(0)) a[0]++;
    auto b = ntt.PolyInv(a);
    auto c = ntt.Mult(a, b);
    c.resize(a.size());
    EXPECT_EQ(c[0], Mint(1));
    for (int i = 1; i < c.size(); i++) {
      EXPECT_EQ(c[i], Mint(0));
    }
  }
}

TEST_F(NttTest, Sqrt) {
  for (int it = 0; it < 100; it++) {
    int n = rng() % N + 1;
    vector<Mint> a(n);
    for (auto &u : a) {
      u = Mint(rng() % MOD);
    }
    a[0] = Mint(1);
    auto b = ntt.PolySqrt(a, Mint(1));
    auto c = ntt.Mult(b, b);
    c.resize(a.size());
    EXPECT_EQ(a, c);
  }
}
