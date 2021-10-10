#include <../src/modular.h>
#include <../src/segment_tree.h>
#include <gtest/gtest.h>

const int N = 100;

mt19937 rng;

TEST(SegmentTreeTest, One) {
  SegmentTree<Node<int>> t(1);
  t.Update(0, 0, 1);
}

template <typename T>
void SingleAdd() {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    SegmentTree<Node<T>> t(n);
    int tc = rng() % N + 1;
    vector<T> a(n);
    while (tc--) {
      if (rng() % 2) {
        int pos = rng() % n;
        T val(rng() % N - N / 2);
        t.Update(pos, pos, val);
        a[pos] += val;
      } else {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        T res(0);
        for (int i = l; i <= r; i++) {
          res += a[i];
        }
        EXPECT_EQ(res, t.Get(l, r).sum);
      }
    }
  }
}

TEST(SegmentTreeTest, SingleAdd) {
  SingleAdd<int>();
  SingleAdd<Modular<127>>();
}

template <typename T>
void SingleGet() {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    SegmentTree<Node<T>> t(n);
    int tc = rng() % N + 1;
    vector<T> a(n);
    while (tc--) {
      if (rng() % 2) {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        T val(rng() % N - N / 2);
        t.Update(l, r, val);
        for (int i = l; i <= r; i++) {
          a[i] += val;
        }
      } else {
        int pos = rng() % n;
        EXPECT_EQ(t.Get(pos, pos).sum, a[pos]);
      }
    }
  }
}

TEST(SegmentTreeTest, SingleGet) {
  SingleGet<int>();
  SingleGet<Modular<127>>();
}

TEST(SegmentTreeTest, Init) {
  vector<int> a(N);
  for (auto &u : a) {
    u = rng() % N;
  }
  SegmentTree<Node<int>> t(N, a);
  for (int i = 0; i < N; i++) {
    int sum = 0;
    for (int j = i; j < N; j++) {
      sum += a[j];
      EXPECT_EQ(sum, t.Get(i, j).sum);
    }
  }
}

template <typename T>
void RandomTest() {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    int tc = rng() % N + 1;
    vector<T> a(n);
    for (auto &u : a) {
      u = T(rng() % N);
    }
    SegmentTree<Node<T>> t(n, a);
    while (tc--) {
      if (rng() % 2) {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        T val(rng() % N - N / 2);
        t.Update(l, r, val);
        for (int i = l; i <= r; i++) {
          a[i] += val;
        }
      } else {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        T res(0);
        for (int i = l; i <= r; i++) {
          res += a[i];
        }
        EXPECT_EQ(res, t.Get(l, r).sum);
      }
    }
  }
}

TEST(SegmentTreeTest, RandomTest) {
  RandomTest<int>();
  RandomTest<Modular<127>>();
}

TEST(SegmentTreeTest, LeftRightMost) {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    int tc = rng() % N + 1;
    vector<int> a(n);
    SegmentTree<NodeMax<int>> t(n, a);
    while (tc--) {
      if (rng() % 2) {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        int val = rng() % N;
        t.Update(l, r, val);
        for (int i = l; i <= r; i++) {
          a[i] += val;
        }
      } else {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);

        function<bool(const NodeMax<int> &, const int &)> Greater =
            [&](const NodeMax<int> &t, const int &val) {
              return t.max_val >= val;
            };

        int val = rng() % (*max_element(a.begin(), a.end()) + 1);
        int left_pos = -1;
        for (int i = l; i <= r; i++) {
          if (a[i] >= val) {
            left_pos = i;
            break;
          }
        }
        int right_pos = -1;
        for (int i = r; i >= l; i--) {
          if (a[i] >= val) {
            right_pos = i;
            break;
          }
        }
        ASSERT_EQ(left_pos, t.LeftMostHas(l, r, Greater, val));
        ASSERT_EQ(right_pos, t.RightMostHas<int>(l, r, Greater, val));
      }
    }
  }
}

TEST(SegmentTreeTest, MaxTest) {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    int tc = rng() % N + 1;
    vector<int> a(n);
    for (auto &u : a) {
      u = rng() % N;
    }
    SegmentTree<NodeMax<int>> t(n, a);
    while (tc--) {
      if (rng() % 2) {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        int val(rng() % N - N / 2);
        t.Update(l, r, val);
        for (int i = l; i <= r; i++) {
          a[i] += val;
        }
      } else {
        int l = rng() % n;
        int r = rng() % n;
        if (l > r) swap(l, r);
        int res = *max_element(a.begin() + l, a.begin() + r + 1);
        ASSERT_EQ(res, t.Get(l, r).max_val);
      }
    }
  }
}
