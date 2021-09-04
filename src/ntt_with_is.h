#include "all.h"
#include "ntt.h"

class NttWithIS : public NTT {
 public:
  // Inverse polynomial of F(x)
  // I_n(x): I_n(x) * F(x) = 1 (mod x^n)
  // I_1(0) = 1 / F(0)
  // I_2n(x) = 2I_n(x) - I_n(x)^2 * F(x)
  // Inversed polinomial is only TRUE for 0 <= i < F.size()
  // which means F * I = {1, 0, 0, ... 0, X, X, ...}
  vector<Mint> PolyInv(vector<Mint> F) {
    auto n = static_cast<int>(F.size());
    auto cur_base = n == 1 ? 0 : 32 - __builtin_clz(n - 1);
    F.resize(1 << cur_base);
    vector<Mint> I = {F[0].Inv()};
    for (int k = 1; k < 1 << cur_base; k <<= 1) {
      vector<Mint> foo(F.begin(), F.begin() + (k * 2));
      I = Inv2n(I, foo);
    }
    I.resize(n);
    return I;
  }

  // Square root polynomial of F(x)
  // S_n(x) : S_n(x)^2 = F(x) (mod x^n)
  // S_n(0) = sqrt(F(0))
  // S_2n(x) = (S_n(x) + F(x) / S_n(x)) / 2
  // Squareroot polinomial is only TRUE for 0 <= i < F.size()
  // which means S ^ 2 = {F[0], F[1], ..., F[size - 1], X, X, ...}
  vector<Mint> PolySqrt(vector<Mint> F, Mint sqrt_F0) {
    auto n = static_cast<int>(F.size());
    auto cur_base = n == 1 ? 0 : 32 - __builtin_clz(n - 1);
    F.resize(1 << cur_base);
    vector<Mint> S = {sqrt_F0};
    assert(S[0] * S[0] == F[0] && "Can't find sqrt(F[0])");
    vector<Mint> IS = {S[0].Inv()};
    for (int k = 1; k < 1 << cur_base; k <<= 1) {
      vector<Mint> foo(F.begin(), F.begin() + (k * 2));
      auto bar = S;
      bar.resize(bar.size() * 2);
      S = Sqrt2n(S, Inv2n(IS, bar), foo);
      IS = Inv2n(IS, S);
    }
    return S;
  }

 private:
  // calculate I_2n(x) given I_n(x)
  // ensure that I_n(x) * F(x) = 1 (mod x^I.size()) and F.size() = 2I.size()
  vector<Mint> Inv2n(const vector<Mint>& I, const vector<Mint>& F) {
    assert(I.size() * 2 == F.size());
    auto res = Mult(I, F);
    res.resize(F.size());
    for (auto& u : res) u = -u;
    res[0] += Mint(2);
    res = Mult(res, I);
    res.resize(F.size());
    return res;
  }

  // calculate S_2n(x) given S_n(x), 1 / S_n(x) = IS(x)
  // ensure that S_n(x)^2 = F(x) (mod x^I.size()) and F.size() = 2S.size() and
  // 2S.size() == IS.size()
  vector<Mint> Sqrt2n(const vector<Mint>& S, const vector<Mint>& IS,
                      const vector<Mint>& F) {
    assert(S.size() * 2 == F.size());
    assert(S.size() * 2 == IS.size());
    auto res = Mult(F, IS);
    res.resize(F.size());
    for (int i = 0; i < S.size(); i++) res[i] += S[i];
    for (auto& u : res) u *= INV_2;
    return res;
  }

  const Mint INV_2 = Mint(2).Inv();
  int max_base_;
  vector<Mint> roots_;  // 0: -1, 1: i, ...
  vector<int> rev_;
};
