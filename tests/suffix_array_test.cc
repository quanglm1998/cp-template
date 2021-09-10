#include <../src/suffix_array.h>
#include <gtest/gtest.h>

const int N = 500;

mt19937 rng;

TEST(SuffixArrayTest, Empty) { EXPECT_DEATH(SuffixArray sa(""), ""); }

TEST(SuffixArrayTest, OneOneString) {
  SuffixArray sa("a");
  EXPECT_EQ(sa.sa(), vector<int>({0}));
  EXPECT_EQ(sa.pos(), vector<int>({0}));
  EXPECT_EQ(sa.lcp(), vector<int>());
}

TEST(SuffixArrayTest, OneTwoString) {
  SuffixArray sa("a#a", '#');
  EXPECT_EQ(sa.sa(), vector<int>({1, 2, 0}));
  EXPECT_EQ(sa.pos(), vector<int>({2, 0, 1}));
  EXPECT_EQ(sa.lcp(), vector<int>({0, 1}));
}

TEST(SuffixArrayTest, OneThreeString) {
  SuffixArray sa("a#a#a", '#');
  EXPECT_EQ(sa.sa(), vector<int>({3, 1, 4, 2, 0}));
  EXPECT_EQ(sa.pos(), vector<int>({4, 1, 3, 0, 2}));
  EXPECT_EQ(sa.lcp(), vector<int>({0, 0, 1, 1}));
}

TEST(SuffixArrayTest, TwoOneString) {
  SuffixArray sa("aa");
  EXPECT_EQ(sa.sa(), vector<int>({1, 0}));
  EXPECT_EQ(sa.pos(), vector<int>({1, 0}));
  EXPECT_EQ(sa.lcp(), vector<int>({1}));
}

TEST(SuffixArrayTest, Same) {
  string s = "";
  for (int i = 0; i < N; i++) {
    s += 'a';
  }
  vector<int> exp(N);
  iota(exp.begin(), exp.end(), 0);
  reverse(exp.begin(), exp.end());
  SuffixArray sa(s);
  EXPECT_EQ(sa.sa(), exp);
  EXPECT_EQ(sa.pos(), exp);
  exp.pop_back();
  reverse(exp.begin(), exp.end());
  EXPECT_EQ(sa.lcp(), exp);
}

TEST(SuffixArrayTest, Stress) {
  for (int it = 1; it <= N; it++) {
    string s = "";
    vector<string> ls;
    for (int i = 0; i < it; i++) {
      int u = rng() % 27;
      if (u == 26)
        s += '#';
      else
        s += static_cast<char>(u + 'a');
      ls.push_back(s);
    }
    reverse(s.begin(), s.end());
    for (auto &u : ls) {
      reverse(u.begin(), u.end());
    }
    reverse(ls.begin(), ls.end());

    vector<int> sa(it);
    iota(sa.begin(), sa.end(), 0);
    sort(sa.begin(), sa.end(), [&](int u, int v) {
      int it = 0;
      while (it < ls[u].size() && it < ls[v].size()) {
        if (ls[u][it] != ls[v][it]) {
          return ls[u][it] < ls[v][it];
        }
        it++;
      }
      return u > v;
    });

    SuffixArray suffix_array(s, '#');
    EXPECT_EQ(suffix_array.sa(), sa);
    vector<int> pos(it);
    for (int i = 0; i < it; i++) {
      pos[sa[i]] = i;
    }
    EXPECT_EQ(suffix_array.pos(), pos);
    vector<int> lcp(it - 1);
    for (int i = 0; i + 1 < it; i++) {
      int len = 0;
      int u = sa[i];
      int v = sa[i + 1];
      while (len < ls[u].size() && len < ls[v].size() &&
             ls[u][len] == ls[v][len] && ls[u][len] != '#') {
        len++;
      }
      lcp[i] = len;
    }
    EXPECT_EQ(suffix_array.lcp(), lcp);
  }
}
