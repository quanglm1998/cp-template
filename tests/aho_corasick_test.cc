#include <../src/aho_corasick.h>
#include <gtest/gtest.h>

mt19937 rng;

TEST(AhoCorasickTest, Zero) {
  AhoCorasick ac;
  ac.Build();
  EXPECT_EQ(ac.node(0).suffix_link, 0);
  EXPECT_EQ(ac.node(0).dict_suffix_link, 0);
}

TEST(AhoCorasickTest, One) {
  AhoCorasick ac;
  ac.Add("a");
  ac.Build();
  EXPECT_EQ(ac.node(1).suffix_link, 0);
  EXPECT_EQ(ac.node(1).dict_suffix_link, 0);
  EXPECT_TRUE(ac.node(1).is_ending);
  EXPECT_TRUE(ac.node(1).contain_ending);
}

TEST(AhoCorasickTest, Simple) {
  AhoCorasick ac;
  auto a = ac.Add("a");
  auto b = ac.Add("ba");
  auto c = ac.Add("cb");
  ac.Build();
  auto node_b = ac.node(b);
  EXPECT_EQ(node_b.suffix_link, a);
  EXPECT_EQ(node_b.dict_suffix_link, a);
  EXPECT_TRUE(node_b.is_ending);
  EXPECT_TRUE(node_b.contain_ending);
  auto node_c = ac.node(c);
  EXPECT_EQ(node_c.suffix_link, b - 1);
  EXPECT_EQ(node_c.dict_suffix_link, 0);
  EXPECT_TRUE(node_c.is_ending);
  EXPECT_TRUE(node_c.contain_ending);
}

TEST(AhoCorasickTest, Stress) {
  AhoCorasick ac;
  unordered_set<int> endings;
  int n = 50;
  endings.reserve(n);
  for (int it = 0; it < n; it++) {
    int len = rng() % 50 + 1;
    string s = "";
    for (int i = 0; i < len; i++) {
      s += static_cast<char>(rng() % 26 + 'a');
    }
    endings.insert(ac.Add(s));
  }
  ac.Build();
  auto nodes = ac.nodes();
  vector<string> str_ls(nodes.size());
  str_ls[0] = "";
  for (int i = 0; i < nodes.size(); i++) {
    for (int j = 0; j < 26; j++) {
      auto k = nodes[i].to[j];
      if (nodes[i].len + 1 == nodes[k].len) {
        str_ls[k] = str_ls[i] + static_cast<char>(j + 'a');
      }
    }
  }

  // is ending
  for (int i = 0; i < nodes.size(); i++) {
    if (endings.count(i)) {
      EXPECT_TRUE(nodes[i].is_ending);
    } else {
      EXPECT_FALSE(nodes[i].is_ending);
    }
  }

  auto IsSuffix = [](const string& s, const string& t) -> bool {
    EXPECT_LE(t.size(), s.size());
    for (int i = 0; i < t.size(); i++) {
      if (t[t.size() - i - 1] != s[s.size() - i - 1]) {
        return false;
      }
    }
    return true;
  };

  // contain ending
  for (int i = 0; i < nodes.size(); i++) {
    bool f = false;
    for (int j = 0; j < nodes.size(); j++) {
      if (nodes[i].len <= nodes[j].len) continue;
      if (endings.count(j) && IsSuffix(str_ls[i], str_ls[j])) {
        f = true;
        continue;
      }
    }
    f |= endings.count(i);
    EXPECT_EQ(nodes[i].contain_ending, f);
  }

  // suffix_link
  for (int i = 0; i < nodes.size(); i++) {
    int f = 0;
    for (int j = 0; j < nodes.size(); j++) {
      if (nodes[i].len <= nodes[j].len) continue;
      if (IsSuffix(str_ls[i], str_ls[j])) {
        if (nodes[f].len < nodes[j].len) {
          f = j;
        }
      }
    }
    EXPECT_EQ(nodes[i].suffix_link, f);
  }

  // dict_suffix_link
  for (int i = 0; i < nodes.size(); i++) {
    int f = 0;
    for (int j = 0; j < nodes.size(); j++) {
      if (nodes[i].len <= nodes[j].len) continue;
      if (endings.count(j) && IsSuffix(str_ls[i], str_ls[j])) {
        if (nodes[f].len < nodes[j].len) {
          f = j;
        }
      }
    }
    EXPECT_EQ(nodes[i].dict_suffix_link, f);
  }
}
