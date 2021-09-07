#include <../src/slope_trick.h>
#include <gtest/gtest.h>

const int N = 1000;

mt19937 rng;
mt19937_64 rng_64;

template <typename T>
struct Func {
  explicit Func(vector<T> val = vector<T>(2 * N + 1, T(0))) : val(val) {}

  static Func Abs(T c) {
    vector<T> val(N * 2 + 1);
    for (int i = 0; i <= N * 2; i++) {
      val[i] = abs(i - N - c);
    }
    return Func(val);
  }

  Func& operator+=(const Func& o) {
    for (int i = 0; i <= N * 2; i++) {
      if (val[i] == -1 || o.val[i] == -1) {
        val[i] = -1;
      } else {
        val[i] += o.val[i];
      }
    }
    return *this;
  }

  Func operator+(const Func& o) const { return Func(*this) += o; }

  Func& Translate(T d) {
    vector<T> new_val(N * 2 + 1, -1);
    for (int i = 0; i <= N * 2; i++) {
      int id = i - d;
      if (id >= 0 && id <= N * 2) {
        new_val[i] = val[id];
      }
    }
    val = new_val;
    return *this;
  }

  Func& Normalize(T d) {
    for (int i = 0; i < N * 2; i++) {
      if (val[i] == -1 || val[i + 1] == -1) continue;
      val[i + 1] = min(val[i + 1], val[i] + d);
    }
    return *this;
  }

  T GetMin() {
    Normalize(0);
    T res = numeric_limits<T>::max();
    int min_pos = -1;
    for (int i = 0; i <= N * 2; i++) {
      if (val[i] != -1) {
        if (res > val[i]) {
          min_pos = i;
        }
        res = min(res, val[i]);
      }
    }
    return res;
  }

  vector<T> val;
};

TEST(SlopeTrickTest, Zero) {
  SlopeTrick<int> s;
  EXPECT_EQ(s.GetMin(), 0);
  s.Normalize(1);
  s.Normalize(-1);
}

TEST(SlopeTrickTest, Abs) {
  int id = rng() % (N + 1) - N;
  SlopeTrick<int> a = SlopeTrick<int>::Abs(id);
  Func<int> b = Func<int>::Abs(id);
  EXPECT_EQ(b.GetMin(), a.GetMin());
}

TEST(SlopeTrickTest, Add) {
  SlopeTrick<int> a;
  Func<int> b;
  int id = rng() % (N + 1) - N;
  a += SlopeTrick<int>::Abs(id);
  b += Func<int>::Abs(id);
  id = rng() % (N + 1) - N;
  a += SlopeTrick<int>::Abs(id);
  b += Func<int>::Abs(id);
  EXPECT_EQ(b.GetMin(), a.GetMin());

  SlopeTrick<int> c;
  Func<int> d;
  id = rng() % (N + 1) - N;
  c += SlopeTrick<int>::Abs(id);
  d += Func<int>::Abs(id);
  id = rng() % (N + 1) - N;
  c += SlopeTrick<int>::Abs(id);
  d += Func<int>::Abs(id);

  auto e = a + c;
  auto f = b + d;
  EXPECT_EQ(e.GetMin(), f.GetMin());
}

TEST(SlopeTrickTest, Translate) {
  SlopeTrick<int> a;
  Func<int> b;
  int id = rng() % 100 - 50;
  a += SlopeTrick<int>::Abs(id);
  b += Func<int>::Abs(id);
  b.GetMin();
  id = rng() % 100 - 50;
  a += SlopeTrick<int>::Abs(id);
  b += Func<int>::Abs(id);
  EXPECT_EQ(b.GetMin(), a.GetMin());

  SlopeTrick<int> c;
  Func<int> d;
  id = rng() % 100 - 50;
  c += SlopeTrick<int>::Abs(id);
  d += Func<int>::Abs(id);
  id = rng() % 100 - 50;
  c += SlopeTrick<int>::Abs(id);
  d += Func<int>::Abs(id);
  int trans = rng() % 100 - 50;

  c.Translate(trans);
  d.Translate(trans);
  EXPECT_EQ(c.GetMin(), d.GetMin());

  auto e = a + c;
  auto f = b + d;

  EXPECT_EQ(e.GetMin(), f.GetMin());
}

TEST(SlopeTrickTest, Stress) {
  SlopeTrick<int64_t> a;
  Func<int64_t> b;
  for (int it = 0; it < 100; it++) {
    int op = rng() % 2;
    int id = rng() % 100 - 50;
    if (op == 0) {
      a += SlopeTrick<int64_t>::Abs(id);
      b += Func<int64_t>::Abs(id);
    } else {
      a.Translate(id);
      b.Translate(id);
    }
    EXPECT_EQ(a.GetMin(), b.GetMin());
  }
}
