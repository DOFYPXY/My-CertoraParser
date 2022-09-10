#ifndef CERTORA_PARSER_VISITOR_HPP
#define CERTORA_PARSER_VISITOR_HPP

#include <string>
#include <cassert>
#include "antlr4-runtime.h"
#include "libs/CertoraParser.h"
#include "AST.hpp"

using namespace std;
using namespace ast;

namespace visitor
{
    class Visitor
    {
    public:
        Root *visitRoot(CertoraParser::RootContext *ctx);
        Method *visitMethodSpec(CertoraParser::MethodSpecContext *ctx);
        Rule *visitRuleSpec(CertoraParser::RulespecContext *ctx);
        Param *visitParam(CertoraParser::ParamContext *ctx);
        Stmt *visitStmt(CertoraParser::StmtContext *ctx);
        Expr *visitExpr(CertoraParser::ExprContext *ctx);
        FnCall *visitFnCall(CertoraParser::FnCallContext *ctx);

    private:
    };
}

#endif
