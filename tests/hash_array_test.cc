#include <../src/hash_array.h>
#include <gtest/gtest.h>

mt19937 rng;

TEST(HashArrayTest, SameString) {
  string s = "";
  for (int i = 0; i < 100; i++) {
    s += 'a';
  }
  auto t = s;
  HashArray<MOD> h1(s);
  HashArray<MOD> h2(t);
  for (int it = 0; it < 100; it++) {
    int l = rng() % s.size();
    int r = rng() % s.size();
    if (l > r) swap(l, r);
    EXPECT_EQ(h1.GetHash(l, r), h2.GetHash(l, r));
  }
}

TEST(HashArrayTest, RandomString) {
  string s = "";
  for (int i = 0; i < 100; i++) {
    s += static_cast<char>(rng() % 2 + 'a');
  }
  string t = "";
  for (int i = 0; i < 100; i++) {
    t += static_cast<char>(rng() % 2 + 'a');
  }
  HashArray<MOD> h1(s);
  HashArray<MOD> h2(t);
  for (int it = 0; it < 1000; it++) {
    int l = rng() % s.size();
    int r = rng() % s.size();
    if (l > r) swap(l, r);
    bool same = true;
    for (int i = l; i <= r; i++) {
      same &= (s[i] == t[i]);
    }
    if (same) {
      EXPECT_EQ(h1.GetHash(l, r), h2.GetHash(l, r));
    } else {
      EXPECT_NE(h1.GetHash(l, r), h2.GetHash(l, r));
    }
  }
}

TEST(HashArrayTest, SameArray) {
  vector<int> s;
  for (int i = 0; i < 100; i++) {
    s.push_back(rng() % 1000 + 1);
  }
  int t[100];
  for (int i = 0; i < 100; i++) {
    t[i] = s[i];
  }
  HashArray<MOD> h1(s);
  HashArray<MOD> h2(t, 100);
  for (int it = 0; it < 100; it++) {
    int l = rng() % s.size();
    int r = rng() % s.size();
    if (l > r) swap(l, r);
    EXPECT_EQ(h1.GetHash(l, r), h2.GetHash(l, r));
  }
}

TEST(HashArrayTest, RandomArray) {
  vector<int> s;
  for (int i = 0; i < 100; i++) {
    s.push_back(rng() % 2 + 1);
  }
  int t[100];
  for (int i = 0; i < 100; i++) {
    t[i] = s[i];
  }
  HashArray<MOD> h1(s);
  HashArray<MOD> h2(t, 100);
  for (int it = 0; it < 1000; it++) {
    int l = rng() % s.size();
    int r = rng() % s.size();
    if (l > r) swap(l, r);
    bool same = true;
    for (int i = l; i <= r; i++) {
      same &= (s[i] == t[i]);
    }
    if (same) {
      EXPECT_EQ(h1.GetHash(l, r), h2.GetHash(l, r));
    } else {
      EXPECT_NE(h1.GetHash(l, r), h2.GetHash(l, r));
    }
  }
}
