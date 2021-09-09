#include <../src/kmp.h>
#include <gtest/gtest.h>

const int N = 100;

mt19937 rng;

TEST(KmpTest, Empty) { EXPECT_EQ(vector<int>(), KMP::Build("")); }

TEST(KmpTest, One) { EXPECT_EQ(vector<int>{0}, KMP::Build("a")); }

TEST(KmpTest, Two) {
  vector<int> exp = {0, 1};
  EXPECT_EQ(exp, KMP::Build("aa"));
}

TEST(KmpTest, Same) {
  string s = "";
  for (int i = 0; i < N; i++) {
    s += 'a';
  }
  vector<int> exp(N);
  iota(exp.begin(), exp.end(), 0);
  EXPECT_EQ(exp, KMP::Build(s));
}

TEST(KmpTest, Stress) {
  for (int it = 0; it < 100; it++) {
    string s = "";
    vector<string> str_ls;
    for (int i = 0; i < N; i++) {
      s += static_cast<char>(rng() % (it % 26 + 1) + 'a');
      str_ls.push_back(s);
    }
    vector<int> exp(N);
    auto IsSuffix = [](const string& s, const string& t) -> bool {
      EXPECT_LE(t.size(), s.size());
      for (int i = 0; i < t.size(); i++) {
        if (t[t.size() - i - 1] != s[s.size() - i - 1]) {
          return false;
        }
      }
      return true;
    };
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < i; j++) {
        if (IsSuffix(str_ls[i], str_ls[j])) {
          exp[i] = j + 1;
        }
      }
    }
    EXPECT_EQ(exp, KMP::Build(s));
  }
}
