#include <../src/dinic_flow_scaling.h>
#include <gtest/gtest.h>

mt19937 rng;

TEST(DinicFlowScalingTest, Simple) {
  DinicFlowScaling<int> g(4, 0, 3);
  ASSERT_EQ(0, g.AddEdge(0, 1, 1));
  ASSERT_EQ(2, g.AddEdge(0, 2, 1));
  ASSERT_EQ(4, g.AddEdge(1, 3, 1));
  ASSERT_EQ(6, g.AddEdge(2, 3, 1));
  ASSERT_EQ(8, g.AddEdge(1, 2, 1));
  ASSERT_EQ(2, g.MaxFlow());
}

TEST(DinicFlowScalingTest, NotSimple) {
  DinicFlowScaling<int> g(2, 0, 1);
  ASSERT_EQ(0, g.AddEdge(0, 1, 1));
  ASSERT_EQ(2, g.AddEdge(0, 1, 2));
  ASSERT_EQ(4, g.AddEdge(0, 1, 3));
  ASSERT_EQ(6, g.AddEdge(0, 1, 4));
  ASSERT_EQ(8, g.AddEdge(0, 1, 5));
  ASSERT_EQ(10, g.AddEdge(0, 0, 6));
  ASSERT_EQ(12, g.AddEdge(1, 1, 7));
  ASSERT_EQ(15, g.MaxFlow());
}

TEST(DinicFlowScalingTest, Cut) {
  DinicFlowScaling<int> g(3, 0, 2);
  ASSERT_EQ(0, g.AddEdge(0, 1, 2));
  ASSERT_EQ(2, g.AddEdge(1, 2, 1));
  ASSERT_EQ(1, g.MaxFlow());
}

TEST(DinicFlowScalingTest, Twice) {
  DinicFlowScaling<int> g(3, 0, 2);
  ASSERT_EQ(0, g.AddEdge(0, 1, 1));
  ASSERT_EQ(2, g.AddEdge(0, 2, 1));
  ASSERT_EQ(4, g.AddEdge(1, 2, 1));
  ASSERT_EQ(2, g.MaxFlow());
}

TEST(DinicFlowScalingTest, Bound) {
  const int INF = std::numeric_limits<int>::max();
  DinicFlowScaling<int> g(3, 0, 2);
  ASSERT_EQ(0, g.AddEdge(0, 1, INF));
  ASSERT_EQ(2, g.AddEdge(1, 0, INF));
  ASSERT_EQ(4, g.AddEdge(0, 2, INF));

  ASSERT_EQ(INF, g.MaxFlow());
}

TEST(DinicFlowScalingTest, BoundUint) {
  const unsigned int INF = std::numeric_limits<unsigned int>::max();
  DinicFlowScaling<unsigned int> g(3, 0, 2);
  ASSERT_EQ(0, g.AddEdge(0, 1, INF));
  ASSERT_EQ(2, g.AddEdge(1, 0, INF));
  ASSERT_EQ(4, g.AddEdge(0, 2, INF));

  ASSERT_EQ(INF, g.MaxFlow());
}

TEST(DinicFlowScalingTest, Stress) {
  for (int phase = 0; phase < 1000; phase++) {
    int n = rng() % 20 + 2;
    int m = rng() % 100 + 1;
    int s = rng() % n;
    int t = rng() % (n - 1);
    if (s == t) {
      t++;
    }

    DinicFlowScaling<int> g(n, s, t);
    vector<int> edges;
    for (int i = 0; i < m; i++) {
      int u = rng() % n;
      int v = rng() % n;
      int c = rng() % 10000;
      auto id = g.AddEdge(u, v, c);
      if (u == s) {
        edges.push_back(id);
      }
    }
    int max_flow = g.MaxFlow();
    EXPECT_EQ(g.d(t), -1);
    int tot = 0;
    for (auto id : edges) {
      tot += g.flow(id);
    }
    EXPECT_EQ(tot, max_flow);
  }
}

TEST(DinicFlowScalingTest, Set) {
  DinicFlowScaling<int> g(100, 0, 1);
  EXPECT_EQ(0, g.source());
  EXPECT_EQ(1, g.sink());
  g.set_source(2);
  g.set_sink(3);
  EXPECT_EQ(2, g.source());
  EXPECT_EQ(3, g.sink());
}

TEST(DinicFlowScalingTest, LongLong) {
  DinicFlowScaling<int64_t> g(4, 0, 3);
  int64_t val = 1e15;
  ASSERT_EQ(0, g.AddEdge(0, 1, val));
  ASSERT_EQ(2, g.AddEdge(0, 2, val));
  ASSERT_EQ(4, g.AddEdge(1, 3, val));
  ASSERT_EQ(6, g.AddEdge(2, 3, val));
  ASSERT_EQ(8, g.AddEdge(1, 2, val));
  ASSERT_EQ(val * 2, g.MaxFlow());
}

TEST(DinicFlowScalingTest, ULongLong) {
  DinicFlowScaling<uint64_t> g(4, 0, 3);
  uint64_t val = 1e15;
  ASSERT_EQ(0, g.AddEdge(0, 1, val));
  ASSERT_EQ(2, g.AddEdge(0, 2, val));
  ASSERT_EQ(4, g.AddEdge(1, 3, val));
  ASSERT_EQ(6, g.AddEdge(2, 3, val));
  ASSERT_EQ(8, g.AddEdge(1, 2, val));
  ASSERT_EQ(val * 2, g.MaxFlow());
}

TEST(DinicFlowScalingTest, Uint) {
  DinicFlowScaling<uint32_t> g(4, 0, 3);
  uint32_t val = 2e9;
  ASSERT_EQ(0, g.AddEdge(0, 1, val));
  ASSERT_EQ(2, g.AddEdge(0, 2, val));
  ASSERT_EQ(4, g.AddEdge(1, 3, val));
  ASSERT_EQ(6, g.AddEdge(2, 3, val));
  ASSERT_EQ(8, g.AddEdge(1, 2, val));
  ASSERT_EQ(val * 2, g.MaxFlow());
}
