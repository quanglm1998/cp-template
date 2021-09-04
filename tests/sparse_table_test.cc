#include <../src/sparse_table.h>
#include <gtest/gtest.h>

const int N = 100;

mt19937 rng;
mt19937_64 rng_64;

TEST(SparseTableTest, One) {
  vector<int> a = {1};
  SparseTable<int> t(a, [](int u, int v) { return min(u, v); });
  EXPECT_EQ(1, t.Get(0, 0));
}

template <typename T>
void TestRMQ(const vector<T> &a, const SparseTable<T> &t,
             const function<T(T, T)> &func) {
  int n = a.size();
  for (int i = 0; i < N; i++) {
    int l = rng() % n;
    int r = rng() % n;
    if (l > r) swap(l, r);
    T res = a[l];
    for (int j = l + 1; j <= r; j++) {
      res = func(res, a[j]);
    }
    EXPECT_EQ(res, t.Get(l, r));
  }
}

template <typename T>
T MinVal(const T &u, const T &v) {
  return min(u, v);
}

template <typename T>
T MaxVal(const T &u, const T &v) {
  return max(u, v);
}

template <typename T>
T GCD(T u, T v) {
  return __gcd(u, v);
}

TEST(SparseTableTest, PowerOfTwo) {
  for (int i = 1; i < 12; i++) {
    vector<int> a(1 << i);
    for (auto &u : a) {
      u = rng() % 1000000;
    }
    SparseTable<int> t(a, MinVal<int>);
    TestRMQ(a, t, MinVal<int>);
  }
}

TEST(SparseTableTest, Stress) {
  for (int i = 0; i < N; i++) {
    int n = rng() % N + 1;
    vector<int> a(n);
    for (auto &u : a) {
      u = rng() % 1000000;
    }
    SparseTable<int> t_min(a, MinVal<int>);
    TestRMQ(a, t_min, MinVal<int>);

    SparseTable<int> t_max(a, MaxVal<int>);
    TestRMQ(a, t_max, MaxVal<int>);

    SparseTable<int> t_gcd(a, GCD<int>);
    TestRMQ(a, t_gcd, GCD<int>);
  }
}

TEST(SparseTableTest, Types) {
  for (int i = 0; i < N; i++) {
    int n = rng() % N + 1;
    vector<int64_t> a(n);
    for (auto &u : a) {
      u = rng_64() % (int64_t)1e18;
    }
    SparseTable<int64_t> t_min(a, MinVal<int64_t>);
    TestRMQ(a, t_min, MinVal<int64_t>);

    SparseTable<int64_t> t_max(a, MaxVal<int64_t>);
    TestRMQ(a, t_max, MaxVal<int64_t>);

    SparseTable<int64_t> t_gcd(a, GCD<int64_t>);
    TestRMQ(a, t_gcd, GCD<int64_t>);
  }

  for (int i = 0; i < N; i++) {
    int n = rng() % N + 1;
    vector<__int128_t> a(n);
    for (auto &u : a) {
      u = static_cast<__int128_t>(rng_64() % (int64_t)1e18) +
          rng_64() % (int64_t)1e18;
    }
    SparseTable<__int128_t> t_min(a, MinVal<__int128_t>);
    TestRMQ(a, t_min, MinVal<__int128_t>);

    SparseTable<__int128_t> t_max(a, MaxVal<__int128_t>);
    TestRMQ(a, t_max, MaxVal<__int128_t>);

    SparseTable<__int128_t> t_gcd(a, GCD<__int128_t>);
    TestRMQ(a, t_gcd, GCD<__int128_t>);
  }
}
