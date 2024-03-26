// Test this without pch.
// RUN: %clang_cc1 -include %S/cxx-lambda.h -fsyntax-only -verify -Wno-dynamic-exception-spec %std_cxx98- %s

// RUN: %clang_cc1 -x c++-header -Wno-dynamic-exception-spec -emit-pch -o %t %S/cxx-lambda.h
// RUN: %clang_cc1 -include-pch %t -fsyntax-only -verify -Wno-dynamic-exception-spec %s

// expected-no-diagnostics

auto x = cmp;
