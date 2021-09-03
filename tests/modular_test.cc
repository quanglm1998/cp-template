#include <../src/modular.h>
#include <gtest/gtest.h>

const int N = 10000;

mt19937 rng;

TEST(ModularTest, Zero) { EXPECT_EQ(0, int(Mint(0))); }

TEST(ModularTest, Types) {
  EXPECT_EQ(MOD - 1, int(Mint((int)-1)));
  EXPECT_EQ(MOD - 1, int(Mint((long long)-1)));
  EXPECT_EQ(MOD - 1, int(Mint((__int128)-1)));

  EXPECT_EQ(numeric_limits<int32_t>::max() % MOD,
            int(Mint(numeric_limits<int32_t>::max())));
  EXPECT_EQ(numeric_limits<uint32_t>::max() % MOD,
            int(Mint(numeric_limits<uint32_t>::max())));

  EXPECT_EQ(numeric_limits<int64_t>::max() % MOD,
            int(Mint(numeric_limits<int64_t>::max())));
  EXPECT_EQ(numeric_limits<uint64_t>::max() % MOD,
            int(Mint(numeric_limits<uint64_t>::max())));

  EXPECT_EQ(numeric_limits<__int128_t>::max() % MOD,
            int(Mint(numeric_limits<__int128_t>::max())));
  EXPECT_EQ(numeric_limits<__uint128_t>::max() % MOD,
            int(Mint(numeric_limits<__uint128_t>::max())));
}

TEST(ModularTest, Bool) {
  using Mod2 = Modular<2>;
  for (int it = 0; it < N; it++) {
    int value = rng() % N - N / 2;
    EXPECT_EQ(value % 2 != 0, bool(Mod2(value)));
  }
}

TEST(ModularTest, Mod1) {
  using Mod1 = Modular<1>;
  for (int it = 0; it < N; it++) {
    EXPECT_EQ(Mod1(0), Mod1(rng() % N - N / 2));
  }
}

TEST(ModularTest, Inv) {
  for (int it = 0; it < N; it++) {
    Mint u(rng() % (MOD - 1) + 1);
    auto v = u.Inv();
    EXPECT_EQ(u * v, Mint(1));
  }
}

TEST(ModularTest, Power) {
  for (int it = 0; it < sqrt(N); it++) {
    Mint a(rng());
    Mint cur(1);
    for (int i = 0; i < sqrt(N); i++) {
      EXPECT_EQ(a.Power(i), cur);
      cur *= a;
    }
    auto b = a.Inv();
    cur = Mint(1);
    for (int i = 0; i < sqrt(N); i++) {
      EXPECT_EQ(a.Power(-i), cur);
      cur *= b;
    }

    EXPECT_EQ(a.Power(numeric_limits<int32_t>::max()),
              a.Power((int)(numeric_limits<int32_t>::max() % (MOD - 1))));
    EXPECT_EQ(a.Power(numeric_limits<uint32_t>::max()),
              a.Power((int)(numeric_limits<uint32_t>::max() % (MOD - 1))));

    EXPECT_EQ(a.Power(numeric_limits<int64_t>::max()),
              a.Power((int)(numeric_limits<int64_t>::max() % (MOD - 1))));
    EXPECT_EQ(a.Power(numeric_limits<uint64_t>::max()),
              a.Power((int)(numeric_limits<uint64_t>::max() % (MOD - 1))));

    EXPECT_EQ(a.Power(numeric_limits<__int128_t>::max()),
              a.Power((int)(numeric_limits<__int128_t>::max() % (MOD - 1))));
    EXPECT_EQ(a.Power(numeric_limits<__uint128_t>::max()),
              a.Power((int)(numeric_limits<__uint128_t>::max() % (MOD - 1))));
  }
}

TEST(ModularTest, Div) {
  for (int it = 0; it < N; it++) {
    Mint a(rng() % MOD);
    Mint b(rng() % (MOD - 1) + 1);
    auto c = a / b;
    EXPECT_EQ(b * c, a);
  }

  for (int it = 0; it < N; it++) {
    Mint A(rng() % MOD);
    auto a = A;
    Mint b(rng() % (MOD - 1) + 1);
    a /= b;
    EXPECT_EQ(b * a, A);
  }
}

TEST(ModularTest, IncDec) {
  using Mint167 = Modular<167>;
  for (int it = 0; it < N; it++) {
    {
      int value = rng() % 167;
      Mint167 a(value);
      EXPECT_EQ(int(a++), value);
      value = (value + 1) % 167;
      EXPECT_EQ(int(a), value);
    }
    {
      int value = rng() % 167;
      Mint167 a(value);
      value = (value + 1) % 167;
      EXPECT_EQ(int(++a), value);
    }
    {
      int value = rng() % 167;
      Mint167 a(value);
      EXPECT_EQ(int(a--), value);
      value = (value + 167 - 1) % 167;
      EXPECT_EQ(int(a), value);
    }
    {
      int value = rng() % 167;
      Mint167 a(value);
      value = (value + 167 - 1) % 167;
      EXPECT_EQ(int(--a), value);
    }
  }
}

TEST(ModularTest, PosNeg) {
  for (int it = 0; it < N; it++) {
    int value = rng() % MOD;
    Mint a(value);
    auto b = -a;
    EXPECT_EQ((int(a) + int(b)) % MOD, 0);
    auto c = +a;
    EXPECT_EQ(a, c);
  }
}

TEST(ModularTest, Add) {
  for (int it = 0; it < N; it++) {
    int val_a = rng() % MOD;
    int val_b = rng() % MOD;
    int val_c = (val_a + val_b) % MOD;
    Mint a(val_a), b(val_b);
    auto c = a + b;
    EXPECT_EQ(val_c, int(c));
    a += b;
    EXPECT_EQ(a, c);
  }
}

TEST(ModularTest, Sub) {
  for (int it = 0; it < N; it++) {
    int val_a = rng() % MOD;
    int val_b = rng() % MOD;
    int val_c = (val_a - val_b + MOD) % MOD;
    Mint a(val_a), b(val_b);
    auto c = a - b;
    EXPECT_EQ(val_c, int(c));
    a -= b;
    EXPECT_EQ(a, c);
  }
}

TEST(ModularTest, Mul) {
  for (int it = 0; it < N; it++) {
    int val_a = rng() % MOD;
    int val_b = rng() % MOD;
    int val_c = 1ll * val_a * val_b % MOD;
    Mint a(val_a), b(val_b);
    auto c = a * b;
    EXPECT_EQ(val_c, int(c));
    a *= b;
    EXPECT_EQ(a, c);
  }
}

TEST(ModularTest, Compare) {
  for (int it = 0; it < N; it++) {
    int val_a = rng() % MOD;
    int val_b = rng() % MOD;
    Mint a(val_a), b(val_b);
    auto c = a + b;
    auto d = b + a;
    EXPECT_EQ(c, d);
    d++;
    EXPECT_NE(c, d);
  }
}

TEST(ModularTest, Stream) {
  for (int it = 0; it < N; it++) {
    int val_a = rng() % MOD;
    int val_b = rng() % MOD;
    string foo = to_string(val_a) + ' ' + to_string(val_b);
    stringstream ss(foo);
    Mint a, b;
    ss >> a >> b;
    EXPECT_EQ(val_a, int(a));
    EXPECT_EQ(val_b, int(b));
    stringstream out;
    out << a << ' ' << b;
    EXPECT_EQ(foo, out.str());
    string bar = to_string(a) + ' ' + to_string(b);
    EXPECT_EQ(foo, bar);
  }
}
