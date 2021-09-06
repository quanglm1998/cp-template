#include <../src/dinic_flow.h>
#include <../src/mincost_flow.h>
#include <gtest/gtest.h>

mt19937 rng;

TEST(MincostflowTest, Simple) {
  MinCostFlow<int> g(4, 0, 3);
  g.AddEdge(0, 1, 1, 1);
  g.AddEdge(0, 2, 1, 1);
  g.AddEdge(1, 3, 1, 1);
  g.AddEdge(2, 3, 1, 1);
  g.AddEdge(1, 2, 1, 1);
  EXPECT_EQ(pair(2, 4), g.MaxFlow());
}

TEST(MincostflowTest, Usage) {
  MinCostFlow<int> g(2, 0, 1);
  g.AddEdge(0, 1, 1, 2);
  EXPECT_EQ(make_pair(1, 2), g.MaxFlow());
}

TEST(MincostflowTest, OutOfRange) {
  MinCostFlow<int> g(10, 0, 9);
  EXPECT_DEATH(g.set_source(-1), "");
}

TEST(MincostflowTest, SameCostPaths) {
  MinCostFlow<int> g(3, 0, 2);
  EXPECT_EQ(0, g.AddEdge(0, 1, 1, 1));
  EXPECT_EQ(2, g.AddEdge(1, 2, 1, 0));
  EXPECT_EQ(4, g.AddEdge(0, 2, 2, 1));
  EXPECT_EQ(pair(3, 3), g.MaxFlow());
}

TEST(MincostflowTest, Stress) {
  for (int phase = 0; phase < 1000; phase++) {
    int n = rng() % 20 + 2;
    int m = rng() % 100 + 1;
    int s = rng() % n;
    int t = rng() % (n - 1);
    if (s == t) {
      t++;
    }

    DinicFlow<int> g_mf(n, s, t);
    MinCostFlow<int> g(n, s, t);
    vector<int> src_edges, edges;
    vector<int> costs;
    for (int i = 0; i < m; i++) {
      int u = rng() % n;
      int v = rng() % n;
      int cap = rng() % 11;
      int cost = rng() % 10001;
      auto id = g.AddEdge(u, v, cap, cost);
      g_mf.AddEdge(u, v, cap);
      if (u == s) {
        src_edges.push_back(id);
      }
      edges.push_back(id);
      costs.push_back(cost);
    }
    int flow, cost;
    tie(flow, cost) = g.MaxFlow();
    auto real_flow = g_mf.MaxFlow();
    EXPECT_EQ(real_flow, flow);
    int tot = 0;
    for (auto id : src_edges) {
      tot += g.flow(id);
    }
    EXPECT_EQ(tot, flow);
    int tot_cost = 0;
    for (int i = 0; i < m; i++) {
      tot_cost += costs[i] * g.flow(edges[i]);
    }
    EXPECT_EQ(tot_cost, cost);
  }
}

TEST(DinicFlowTest, Set) {
  MinCostFlow<int> g(100, 0, 1);
  EXPECT_EQ(0, g.source());
  EXPECT_EQ(1, g.sink());
  g.set_source(2);
  g.set_sink(3);
  EXPECT_EQ(2, g.source());
  EXPECT_EQ(3, g.sink());
}
