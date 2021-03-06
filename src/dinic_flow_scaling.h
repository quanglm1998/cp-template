#include "all.h"

// U = max capacity
// Complexity: O(V * E * log(U))
// O(min(E^{1/2}, V^{2/3}) * E) if U = 1
// O(V^{1/2} * E)$ for bipartite matching.
template <class F, typename enable_if<is_integral<F>::value>::type* = nullptr>
class DinicFlowScaling {
 public:
  const F INF = numeric_limits<F>::max();

  DinicFlowScaling(int n, int source, int sink)
      : n_(n),
        source_(source),
        sink_(sink),
        adj_(n, vector<int>()),
        d_(n, -1),
        cur_(n, 0) {}

  void set_source(int u) {
    assert(u >= 0 && u < n_);
    source_ = u;
  }

  void set_sink(int u) {
    assert(u >= 0 && u < n_);
    sink_ = u;
  }

  int source() const { return source_; }
  int sink() const { return sink_; }
  F flow(int edge_id) const { return f_[edge_id]; }
  int d(int u) const { return d_[u]; }

  // AddEdge returns newly created edge_id
  int AddEdge(int u, int v, F c) {
    adj_[u].push_back(to_.size());
    to_.push_back(v);
    f_.push_back(0);
    c_.push_back(c);
    adj_[v].push_back(to_.size());
    to_.push_back(u);
    f_.push_back(0);
    c_.push_back(0);
    return static_cast<int>(to_.size()) - 2;
  }

  F MaxFlow() {
    F res = 0;
    F lim = INF;
    while (lim >= 1) {
      if (!Bfs(lim)) {
        lim >>= 1;
        continue;
      }
      fill(cur_.begin(), cur_.end(), 0);
      while (F aug = Dfs(source_, lim)) res += aug;
    }
    return res;
  }

 private:
  bool Bfs(F lim) {
    fill(d_.begin(), d_.end(), -1);
    d_[source_] = 0;
    queue<int> q;
    q.push(source_);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto edge_id : adj_[u]) {
        int v = to_[edge_id];
        if (d_[v] == -1 && lim <= c_[edge_id] - f_[edge_id]) {
          d_[v] = d_[u] + 1;
          if (v == sink_) return 1;
          q.push(v);
        }
      }
    }
    return d_[sink_] != -1;
  }

  F Dfs(int u, F res) {
    if (u == sink_ || !res) return res;
    for (int& i = cur_[u]; i < adj_[u].size(); i++) {
      int edge_id = adj_[u][i];
      int v = to_[edge_id];
      if (d_[v] == d_[u] + 1 && res <= c_[edge_id] - f_[edge_id]) {
        F foo = Dfs(v, res);
        if (foo) {
          f_[edge_id] += foo;
          f_[edge_id ^ 1] -= foo;
          return foo;
        }
      }
    }
    return 0;
  }

  int n_;
  int source_;
  int sink_;
  vector<vector<int>> adj_;
  vector<int> d_;  // shortest path from source, -1 means it can't be reached
  vector<int> cur_;
  vector<int> to_;
  vector<F> c_, f_;
};
