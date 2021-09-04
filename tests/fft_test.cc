#include <../src/fft.h>
#include <gtest/gtest.h>

const int N = 1000;

mt19937 rng;
mt19937_64 rng_64;

class FftTest : public ::testing::Test {
 protected:
  FFT<double> fft_double_;
  FFT<long double> fft_ld_;
};

template <typename T>
vector<T> Naive(const vector<T> &a, const vector<T> &b) {
  if (a.empty() || b.empty()) return {};
  vector<T> res(a.size() + b.size() - 1);
  for (int i = 0; i < a.size(); i++) {
    for (int j = 0; j < b.size(); j++) {
      res[i + j] += a[i] * b[j];
    }
  }
  return res;
}

TEST_F(FftTest, Empty) {
  EXPECT_EQ(vector<int>{}, fft_double_.Mult(vector<int>{}, vector<int>{}));
  EXPECT_EQ(vector<int>{}, fft_double_.Mult(vector<int>{}, vector<int>{1, 2}));
  EXPECT_EQ(vector<int>{}, fft_double_.Mult(vector<int>{1, 2}, vector<int>{}));
  EXPECT_EQ(vector<int>{}, fft_double_.Mult(vector<int>{1}, vector<int>{}));
  EXPECT_EQ(vector<int64_t>{},
            fft_double_.Mult(vector<int64_t>{}, vector<int64_t>{}));
  EXPECT_EQ(vector<int64_t>{},
            fft_double_.Mult(vector<int64_t>{}, vector<int64_t>{1, 2}));
}

TEST_F(FftTest, ConvLL) {
  for (int n = 1; n < 20; n++) {
    for (int m = 1; m < 20; m++) {
      vector<int64_t> a(n), b(m);
      for (int i = 0; i < n; i++) {
        a[i] = int64_t(rng() % 1'000'000) - 500'000;
      }
      for (int i = 0; i < m; i++) {
        b[i] = int64_t(rng() % 1'000'000) - 500'000;
      }
      EXPECT_EQ(Naive(a, b), fft_double_.Mult(a, b));
    }
  }
}

TEST_F(FftTest, ConvLLBound) {
  static constexpr uint64_t MOD1 = 469762049;  // 2^26
  static constexpr uint64_t MOD2 = 167772161;  // 2^25
  static constexpr uint64_t MOD3 = 754974721;  // 2^24
  static constexpr uint64_t M2M3 = MOD2 * MOD3;
  static constexpr uint64_t M1M3 = MOD1 * MOD3;
  static constexpr uint64_t M1M2 = MOD1 * MOD2;
  for (int i = -1000; i <= 1000; i++) {
    vector<int64_t> a = {(int64_t)(0ULL - M1M2 - M1M3 - M2M3 + i) / 100};
    vector<int64_t> b = {1};

    EXPECT_EQ(a, fft_double_.Mult(a, b));
  }

  for (int i = -1000; i <= 1000; i++) {
    vector<int64_t> a = {(int64_t)(0ULL - M1M2 - M1M3 - M2M3 + i)};
    vector<int64_t> b = {1};

    EXPECT_EQ(a, fft_ld_.Mult(a, b));
  }

  for (int i = 0; i < 1000; i++) {
    vector<int64_t> a = {numeric_limits<int64_t>::min() + i};
    vector<int64_t> b = {1};

    EXPECT_EQ(a, fft_ld_.Mult(a, b));
  }
  for (int i = 0; i < 1000; i++) {
    vector<int64_t> a = {numeric_limits<int64_t>::max() - i};
    vector<int64_t> b = {1};

    EXPECT_EQ(a, fft_ld_.Mult(a, b));
  }
}

TEST_F(FftTest, FftBound) {
  vector<int64_t> a(N), b(N);
  for (auto &u : a) {
    u = rng() % 4'000'000 - 2'000'000;
  }
  for (auto &u : b) {
    u = rng() % 2'000'000 - 1'000'000;
  }
  EXPECT_EQ(Naive(a, b), fft_double_.Mult(a, b));

  vector<int64_t> c(N), d(N);
  for (auto &u : c) {
    u = rng() % 200'000'000 - 100'000'000;
  }
  for (auto &u : d) {
    u = rng() % 100'000'000 - 50'000'000;
  }
  EXPECT_EQ(Naive(c, d), fft_ld_.Mult(c, d));
}

TEST_F(FftTest, Size) {
  for (int it = 0; it < 100; it++) {
    int n = rng() % 100 + 1;
    int m = rng() % 100 + 1;
    vector<int64_t> a(n), b(m);
    for (auto &u : a) {
      u = rng() % 4'000'000 - 2'000'000;
    }
    for (auto &u : b) {
      u = rng() % 2'000'000 - 1'000'000;
    }
    EXPECT_EQ(Naive(a, b), fft_double_.Mult(a, b));
  }
}
