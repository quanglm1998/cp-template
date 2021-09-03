#include <../src/dsu_rollback.h>
#include <gtest/gtest.h>

const int N = 100;

mt19937 rng;

TEST(DsuRollbackTest, Zero) {
  DsuRollback dsu(0);
  EXPECT_EQ(dsu.num_comp(), 0);
}

TEST(DsuRollbackTest, One) {
  DsuRollback dsu(1);
  EXPECT_EQ(dsu.num_comp(), 1);
  EXPECT_EQ(dsu.sz(0), 1);
  EXPECT_EQ(dsu.GetRoot(0), 0);
  EXPECT_FALSE(dsu.Unite(0, 0));
}

TEST(DsuRollbackTest, Two) {
  DsuRollback dsu(2);

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

TEST(DsuRollbackTest, Line) {
  DsuRollback dsu(N);
  for (int i = 1; i < N; i++) {
    EXPECT_TRUE(dsu.Unite(i - 1, i));
    EXPECT_EQ(dsu.sz(i), i + 1);
    EXPECT_EQ(dsu.num_comp(), N - i);
  }
}

TEST(DsuRollbackTest, Star) {
  DsuRollback dsu(N);
  for (int i = 1; i < N; i++) {
    EXPECT_TRUE(dsu.Unite(0, i));
    EXPECT_EQ(dsu.sz(i), i + 1);
    EXPECT_EQ(dsu.num_comp(), N - i);
  }
}

TEST(DsuRollbackTest, Random) {
  for (int it = 0; it < N; it++) {
    int n = rng() % N + 1;
    int m = rng() % N + 1;

    vector<vector<int>> adj(n, vector<int>());
    DsuRollback dsu(n);

    using V = vector<pair<vector<int>, int>>;

    auto GetComps = [&]() {
      V res;
      vector<bool> used(n);

      function<void(int, V &)> Dfs = [&](int u, V &res) {
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

    vector<V> gs;
    vector<int> states;

    for (int i = 0; i < m; i++) {
      int u = rng() % n;
      int v = rng() % n;
      adj[u].push_back(v);
      adj[v].push_back(u);

      dsu.Unite(u, v);
      states.push_back(dsu.GetState());

      gs.push_back(GetCmps());
    }

    for (int i = m - 1; i >= 0; i--) {
      auto comps = gs[i];
      dsu.Rollback(states[i]);
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
