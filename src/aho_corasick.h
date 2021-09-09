#include "all.h"

class AhoCorasick {
 public:
  struct Node {
    vector<int> to;        // next node in the dict
    bool is_ending;        // is this node the ending of a string in the dict?
    bool contain_ending;   // does this node contain a string in the dict
                           // as its SUFFIX
    int suffix_link;       // id of the longest proper SUFFIX of this node
    int dict_suffix_link;  // id of the longest proper SUFFIX and also a string
                           // in the dict of this node
    int len;

    explicit Node(int len)
        : to(vector<int>(kAlphabetSize, -1)),
          is_ending(false),
          contain_ending(false),
          suffix_link(-1),
          dict_suffix_link(-1),
          len(len) {}
  };

  AhoCorasick() { nodes_.push_back(Node(0)); }

  const vector<Node> &nodes() const { return nodes_; }
  const Node &node(int id) const { return nodes_.at(id); }

  // Add a string s which contains LOWERCASE characters to the dict
  // returns the id of the ending node
  int Add(const string &s) {
    int cur_node = 0;
    for (const auto &u : s) {
      if (nodes_[cur_node].to[u - 'a'] == -1) {
        nodes_[cur_node].to[u - 'a'] = static_cast<int>(nodes_.size());
        nodes_.push_back(Node(nodes_[cur_node].len + 1));
      }
      cur_node = nodes_[cur_node].to[u - 'a'];
    }
    nodes_[cur_node].is_ending = nodes_[cur_node].contain_ending = true;
    return cur_node;
  }

  // call Build after calling Add() for all strings
  void Build() {
    queue<int> q;
    q.push(0);
    nodes_[0].suffix_link = nodes_[0].dict_suffix_link = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i = 0; i < kAlphabetSize; i++) {
        int &v = nodes_[u].to[i];
        int nxt = !u ? 0 : nodes_[nodes_[u].suffix_link].to[i];
        if (v == -1) {
          v = nxt;
        } else {
          nodes_[v].suffix_link = nxt;
          nodes_[v].dict_suffix_link =
              nodes_[nxt].is_ending ? nxt : nodes_[nxt].dict_suffix_link;
          nodes_[v].contain_ending |= nodes_[nxt].contain_ending;
          q.push(v);
        }
      }
    }
  }

 private:
  static const int kAlphabetSize = 26;
  vector<Node> nodes_;
};
