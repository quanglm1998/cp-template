#include "all.h"

// tested: https://atcoder.jp/contests/arc123/tasks/arc123_d
// SlopeTrick represents and manipulates a CONTINUOUS CONVEX function which can
// be devided into multiple LINEAR sections
template <typename T>
class SlopeTrick {
 public:
  SlopeTrick() : delta_(0), a_(0), b_(0) {}

  static SlopeTrick Abs(T c) {
    SlopeTrick res;
    res.points_[c] += 2;
    res.a_ = 1;
    res.b_ = -c;
    return res;
  }

  // adds 2 functions in O(NlogN) N = o.points_.size()
  SlopeTrick& operator+=(const SlopeTrick& o) {
    for (auto [u, v] : o.points_) points_[u + o.delta_ - delta_] += v;
    a_ += o.a_;
    b_ += o.b_;
    return *this;
  }

  // adds 2 functions in O(NlogN) N = o.points_.size() + points_.size()
  SlopeTrick operator+(const SlopeTrick& o) const {
    return SlopeTrick(*this) += o;
  }

  // translates this function by d to the RIGHT
  SlopeTrick& Translate(T d) {
    delta_ += d;
    b_ -= a_ * d;
    return *this;
  }

  // removes all lines with slope > d
  SlopeTrick& Normalize(T d) {
    while (a_ > d && !points_.empty()) {
      a_--;
      auto x = points_.rbegin()->first;
      b_ += x + delta_;
      points_[x]--;
      if (!points_[x]) points_.erase(x);
    }
    return *this;
  }

  T GetMin() {
    Normalize(0);
    if (points_.empty()) return T(0);
    return a_ * (points_.rbegin()->first + delta_) + b_;
  }

 private:
  T delta_;           // real point = point + delta_
  map<T, T> points_;  // point -> number of occurrences
  T a_, b_;           // ax + b_ represents the vector with x >= largest point
};
