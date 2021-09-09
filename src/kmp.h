#include "all.h"

class KMP {
 public:
  // Build return KMP vector
  // t[i] is the length of the longest PROPER suffix of s[0..i]
  static vector<int> Build(const string &s) {
    vector<int> t(s.size(), 0);
    int cur = 0;
    for (int i = 1; i < s.size(); i++) {
      while (cur > 0 && s[i] != s[cur]) cur = t[cur - 1];
      if (s[i] == s[cur]) cur++;
      t[i] = cur;
    }
    return t;
  }
};
