#include "all.h"
using namespace std;

class DsuRollback {
 public:
  explicit DSU(int n) : num_comp_(n), root_(n, -1), sz_(n, 1) {}

  int num_comp() const { return num_comp_; }
  int sz(int u) { return sz_[GetRoot(u)]; }

  int GetRoot(int u) { return ~root_[u] ? GetRoot(root_[u]) : u; }

  int GetState() const { return change_.size(); }

  bool Unite(int u, int v) {
    u = GetRoot(u);
    v = GetRoot(v);
    if (u == v) return false;
    if (sz_[u] < sz_[v]) swap(u, v);
    changed_.push_back({u, v});
    root_[v] = u;
    sz_[u] += sz_[v];
    return true;
  }

  void Rollback(int k) {
    assert(changed.size() >= k);
    while (changed_.size() > k) {
      auto u = changed_.back();
      changed_.pop_back();
      sz_[u.first] -= sz_[u.second];
      root_[u.second] = 0;
    }
  }

 private:
  int num_comp_;  // number of connected components
  vector<int> sz_;
  vector<int> root_;
  vector<pair<int, int>> changed_;
};
