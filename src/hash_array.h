#include "all.h"
#include "modular.h"

// hash function of an 0-indexed array
// make sure that BASE > array's elements' max value
template <int MOD>
class HashArray {
  using ModInt = Modular<MOD>;

 public:
  explicit HashArray(const string &a, int base = 10007)
      : base_(ModInt(base)), sz_(static_cast<int>(a.size()) + 1) {
    pref_hash_.resize(sz_, ModInt(0));
    power_of_base_.resize(sz_, ModInt(1));
    for (int i = 1; i < sz_; i++) {
      assert(a[i - 1] > 0 && a[i - 1] < base);
      power_of_base_[i] = power_of_base_[i - 1] * base_;
      pref_hash_[i] = pref_hash_[i - 1] * base_ + ModInt(a[i - 1]);
    }
  }

  explicit HashArray(const vector<int> &a, int base = 10007)
      : base_(ModInt(base)), sz_(static_cast<int>(a.size()) + 1) {
    pref_hash_.resize(sz_, ModInt(0));
    power_of_base_.resize(sz_, ModInt(1));
    for (int i = 1; i < sz_; i++) {
      assert(a[i - 1] > 0 && a[i - 1] < base);
      power_of_base_[i] = power_of_base_[i - 1] * base_;
      pref_hash_[i] = pref_hash_[i - 1] * base_ + ModInt(a[i - 1]);
    }
  }

  // a[0] -> a[n - 1]
  HashArray(int *a, int n, int base = 10007) : base_(ModInt(base)), sz_(n + 1) {
    pref_hash_.resize(sz_, ModInt(0));
    power_of_base_.resize(sz_, ModInt(1));
    for (int i = 1; i < sz_; i++) {
      assert(a[i - 1] > 0 && a[i - 1] < base);
      power_of_base_[i] = power_of_base_[i - 1] * base_;
      pref_hash_[i] = pref_hash_[i - 1] * base_ + ModInt(a[i - 1]);
    }
  }

  // hash value of subarray [l..r] INCLUSIVELY
  ModInt GetHash(int l, int r) const {
    assert(l <= r && l >= 0 && r + 1 < sz_);
    return pref_hash_[r + 1] - pref_hash_[l] * power_of_base_[r - l + 1];
  }

 private:
  vector<ModInt> pref_hash_;
  vector<ModInt> power_of_base_;
  ModInt base_;
  int sz_;
};
