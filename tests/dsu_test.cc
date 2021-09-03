#include <../src/dsu.h>
#include <gtest/gtest.h>

const int N = 100;

mt19937 rng;

TEST(DsuTest, Zero) {
  DSU dsu(0);
  EXPECT_EQ(dsu.num_comp(), 0);
}

TEST(DsuTest, One) {
  DSU dsu(1);
  EXPECT_EQ(dsu.num_comp(), 1);
  EXPECT_EQ(dsu.sz(0), 1);
  EXPECT_EQ(dsu.GetRoot(0), 0);
  EXPECT_FALSE(dsu.Unite(0, 0));
}

TEST(DsuTest, Two) {
  DSU dsu(2);

  EXPECT_NE(dsu.GetRoot(0), dsu.GetRoot(1));
  EXPECT_EQ(dsu.sz(0), 1);
  EXPECT_EQ(dsu.sz(1), 1);
  EXPECT_EQ(dsu.num_comp(), 2);

  EXPECT_TRUE(dsu.Unite(0, 1));
  EXPECT_EQ(dsu.GetRoot(0), dsu.GetRoot(1));
  EXPECT_EQ(dsu.sz(0), 2);
  EXPECT_EQ(dsu.sz(1), 2);
  EXPECT_EQ(dsu.num_comp(), 1);
}

TEST(DsuTest, Line) {
  DSU dsu(N);
  for (int i = 1; i < N; i++) {
    EXPECT_TRUE(dsu.Unite(i - 1, i));
    EXPECT_EQ(dsu.sz(i), i + 1);
    EXPECT_EQ(dsu.num_comp(), N - i);
  }
}

TEST(DsuTest, Star) {
  DSU dsu(N);
  for (int i = 1; i < N; i++) {
    EXPECT_TRUE(dsu.Unite(0, i));
    EXPECT_EQ(dsu.sz(i), i + 1);
    EXPECT_EQ(dsu.num_comp(), N - i);
  }
}

TEST(DsuTest, Random) {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    int m = rng() % N + 1;

    vector<vector<int>> adj(n, vector<int>());
    DSU dsu(n);

    auto GetComps = [&]() {
      vector<pair<vector<int>, int>> res;
      vector<bool> used(n);

      function<void(int, vector<pair<vector<int>, int>> &)> Dfs =
          [&](int u, vector<pair<vector<int>, int>> &res) {
            used[u] = true;
            res.back().first.push_back(u);
            res.back().second++;
            for (auto v : adj[u]) {
              if (!used[v]) {
                Dfs(v, res);
              }
            }
          };

      for (int i = 0; i < n; i++) {
        if (!used[i]) {
          res.push_back({vector<int>(), 0});
          Dfs(i, res);
        }
      }
      return res;
    };

    for (int i = 0; i < m; i++) {
      int u = rng() % n;
      int v = rng() % n;
      adj[u].push_back(v);
      adj[v].push_back(u);

      dsu.Unite(u, v);

      auto comps = GetComps();
      for (auto [comp, sz] : comps) {
        for (int i = 0; i < comp.size(); i++) {
          EXPECT_EQ(dsu.GetRoot(comp[i]), dsu.GetRoot(comp[0]));
          EXPECT_EQ(dsu.sz(comp[i]), sz);
        }
      }
      EXPECT_EQ(dsu.num_comp(), comps.size());
    }
  }
}