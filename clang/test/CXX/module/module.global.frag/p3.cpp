// RUN: rm -rf %t
// RUN: mkdir %t
// RUN: split-file %s %t

// RUN : %clang_cc1 -std=c++20 -emit-module-interface %t/A.cppm -o %t/A.pcm -ast-dump-all
// RUN: %clang_cc1 -std=c++20 -emit-module-interface %t/A.cppm -o %t/A.pcm -verify
// RUN : %clang_cc1 -std=c++20 -fprebuilt-module-path=%t %t/useA.cpp -fsyntax-only -ast-dump-all
// RUN : %clang_cc1 -std=c++20 -fprebuilt-module-path=%t %t/useA.cpp -fsyntax-only -dump-deserialized-decls
// RUN: %clang_cc1 -std=c++20 -fprebuilt-module-path=%t %t/useA.cpp -fsyntax-only -verify

//--- lambda.h
// volatile int x;

auto cmp1 = [](auto l, auto r) {
  return l < r;
};

// bool cmp2(auto l, auto r) {
//   return l < r;
// }

// bool cmp3impl(auto l, auto r)
// {
//   return l < r;
// }

// struct Cmp3
// {
//   constexpr bool operator()(auto l, auto r) const
//   {
//     return cmp3impl(l, r);
//   }

//   template <typename L, typename R>
//   using fptr_t = decltype(cmp3impl<L, R>);

//   template <typename L, typename R>
//   constexpr operator fptr_t<L, R>() const noexcept
//   {
//     return &cmp3impl;
//   }
// };

// Cmp3 cmp3{};

//--- A.cppm
// expected-no-diagnostics
module;
#include "lambda.h"
export module A;
// auto cmp2 = cmp1;
export using ::cmp1;
// export using ::cmp2;
// export using ::cmp3;

//--- useA.cpp
// expected-no-diagnostics
// export module B;
import A;

// bool x = cmp1(1, 2);

auto cmp1Copy = cmp1;


// auto cmp2 = [](auto l, auto r) {
//   return l < r;
// };

// auto cmp2Copy = &cmp2<int, int>;
// auto cmp3Copy = cmp3;
