#include "ClangdUnit.h"
#include "Protocol.h"
#include <vector>

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANGD_FOLD_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANGD_FOLD_H

namespace clang {
namespace clangd {

std::vector<FoldingRange> findFoldingRanges(ParsedAST &);

} // namespace clangd
} // namespace clang

#endif // include guard
