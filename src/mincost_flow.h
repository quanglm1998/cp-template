#include "all.h"

// Complexity: O(F * VE)
// F = O(2 ^ V/2) phases, O(VE) for SPFA
template <class F, typename enable_if<is_integral<F>::value>::type* = nullptr>
class MinCostFlow {
  inline static const F INF = numeric_limits<F>::max();

 public:
  MinCostFlow(int n, int source, int sink)
      : n_(n),
        source_(source),
        sink_(sink),
        adj_(n, vector<int>()),
        d_(n, -1),
        in_queue_(n, 0),
        prev_(n, -1) {}

  void set_source(int u) {
    assert(u >= 0 && u < n_);
    source_ = u;
  }

  void set_sink(int u) {
    assert(u >= 0 && u < n_);
    sink_ = u;
  }

  int source() { return source_; }
  int sink() { return sink_; }
  F flow(int u) { return f_[u]; }

  // AddEdge returns newly created edge_id
  int AddEdge(int u, int v, F c, F cost) {
    adj_[u].push_back(to_.size());
    to_.push_back(v);
    f_.push_back(0);
    c_.push_back(c);
    cost_.push_back(cost);
    adj_[v].push_back(to_.size());
    to_.push_back(u);
    f_.push_back(0);
    c_.push_back(0);
    cost_.push_back(-cost);
    return static_cast<int>(to_.size()) - 2;
  }

  pair<F, F> MaxFlow(F lim = INF) {
    F res(0);
    F minCost(0);
    while (lim > 0) {
      fill(prev_.begin(), prev_.end(), -1);
      fill(d_.begin(), d_.end(), INF);
      d_[source_] = 0;
      in_queue_[source_] = 1;
      queue<int> q;
      q.push(source_);
      while (!q.empty()) {
        auto u = q.front();
        q.pop();
        in_queue_[u] = 0;
        for (auto id : adj_[u]) {
          auto v = to_[id];
          if (d_[v] > d_[u] + cost_[id] && f_[id] < c_[id]) {
            d_[v] = d_[u] + cost_[id];
            prev_[v] = id;
            if (!in_queue_[v]) {
              in_queue_[v] = 1;
              q.push(v);
            }
          }
        }
      }
      if (prev_[sink_] == -1) break;
      auto x = sink_;
      F now = lim;
      while (x != source_) {
        auto id = prev_[x];
        now = min(now, c_[id] - f_[id]);
        x = to_[id ^ 1];
      }
      x = sink_;
      while (x != source_) {
        auto id = prev_[x];
        minCost += cost_[id] * now;
        f_[id] += now;
        f_[id ^ 1] -= now;
        x = to_[id ^ 1];
      }
      res += now;
      lim -= now;
    }
    return {res, minCost};
  }

 private:
  int n_;
  int source_;
  int sink_;
  vector<vector<int>> adj_;
  vector<int> to_;
  vector<F> f_;
  vector<F> c_;
  vector<F> cost_;

  vector<F> d_;
  vector<bool> in_queue_;
  vector<int> prev_;
};
