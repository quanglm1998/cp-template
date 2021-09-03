#include <../src/bit.h>
#include <../src/modular.h>
#include <gtest/gtest.h>

const int N = 100;

mt19937 rng;

TEST(BitTest, One) {
  BIT<int> t(1);
  t.Add(0, 1);
}

template <typename T>
void SingleAdd() {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    BIT<T> t(n);
    int tc = rng() % N + 1;
    vector<T> a(n);
    while (tc--) {
      if (rng() % 2) {
        int pos = rng() % n;
        T val(rng() % N - N / 2);
        t.Add(pos, val);
        a[pos] += val;
      } else {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        T res(0);
        for (int i = l; i <= r; i++) {
          res += a[i];
        }
        EXPECT_EQ(res, t.Get(l, r));
      }
    }
  }
}

TEST(BitTest, SingleAdd) {
  SingleAdd<int>();
  SingleAdd<Modular<127>>();
}

template <typename T>
void SingleGet() {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    BIT<T> t(n);
    int tc = rng() % N + 1;
    vector<T> a(n);
    while (tc--) {
      if (rng() % 2) {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        T val(rng() % N - N / 2);
        t.Add(l, r, val);
        for (int i = l; i <= r; i++) {
          a[i] += val;
        }
      } else {
        int pos = rng() % n;
        EXPECT_EQ(t.Get(pos), a[pos]);
      }
    }
  }
}

TEST(BitTest, SingleGet) {
  SingleGet<int>();
  SingleGet<Modular<127>>();
}
