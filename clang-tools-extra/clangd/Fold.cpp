#include "Fold.h"
#include <clang/ASTMatchers/ASTMatchFinder.h>

namespace clang {
namespace clangd {

class FoldableMatchCallback : public ast_matchers::MatchFinder::MatchCallback {
public:
  explicit FoldableMatchCallback(std::vector<FoldingRange> &Result)
      : ast_matchers::MatchFinder::MatchCallback(), Result(Result)
  {
  }

  void run(const ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *CompStmt =
        Result.Nodes.getNodeAs<CompoundStmt>("CompoundStmt");
    const auto *NSDecl =
        Result.Nodes.getNodeAs<NamespaceDecl>("NamespaceDecl");
    if (CompStmt) {
      handleCompoundStmt(*CompStmt, *Result.SourceManager);
    } else if (NSDecl) {
      handleNamespaceDecl(*NSDecl, *Result.SourceManager);
    }
  }

private:
  void handleCompoundStmt(const CompoundStmt &CompStmt, SourceManager &SrcMgr) {
    handleFoldable(CompStmt, SrcMgr, "Braces");
  }

  void handleNamespaceDecl(const NamespaceDecl &NSDecl, SourceManager &SrcMgr) {
    handleFoldable(NSDecl, SrcMgr, "Namespace");
  }

  template<typename T>
  void handleFoldable(
      const T &Node, SourceManager &SrcMgr, StringRef FoldingKind) {
    const SourceRange &Range = Node.getSourceRange();

    FoldingRange Foldable;
    Foldable.kind = FoldingKind;

    Position StartPos = sourceLocToPosition(SrcMgr, Range.getBegin());
    Foldable.startLine = StartPos.line;
    Foldable.startCharacter = StartPos.character;

    Position EndPos = sourceLocToPosition(SrcMgr, Range.getEnd());
    Foldable.endLine = EndPos.line;
    Foldable.endCharacter = EndPos.character;

    Result.emplace_back(Foldable);
  }

  std::vector<FoldingRange> &Result;
};

std::vector<FoldingRange> findFoldingRanges(ParsedAST &ParsedAST) {
  using namespace ast_matchers;

  std::vector<FoldingRange> Result;

  FoldableMatchCallback FoldableCallback(Result);

  auto CompoundStmtMatcher = compoundStmt().bind("CompoundStmt");
  auto NamespaceDeclMatcher = namespaceDecl().bind("NamespaceDecl");
  MatchFinder Finder;
  Finder.addMatcher(CompoundStmtMatcher, &FoldableCallback);
  Finder.addMatcher(NamespaceDeclMatcher, &FoldableCallback);

  Finder.matchAST(ParsedAST.getASTContext());

  return Result;
}

} // namespace clangd
} // namespace clang