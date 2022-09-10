#include "Visitor.hpp"

using namespace std;
using namespace ast;
namespace visitor
{
    Root *Visitor::visitRoot(CertoraParser::RootContext *ctx)
    {
        auto root = new Root;
        for (const auto &method : ctx->methods()->methodSpec())
            root->methods.push_back(visitMethodSpec(method));

        for (const auto &rule : ctx->rulespec())
            root->rules.push_back(visitRuleSpec(rule));
        return root;
    }

    Method *Visitor::visitMethodSpec(CertoraParser::MethodSpecContext *ctx)
    {
        auto method = new Method;
        method->id = ctx->ID()->getText();
        // clog << "    id:" << method->id << endl;
        if (ctx->param_types)
            for (const auto &param_type : ctx->param_types->type())
                method->param_types.push_back(param_type->getText());
        if (ctx->ret_types)
            for (const auto &ret_type : ctx->ret_types->type())
                method->ret_types.push_back(ret_type->getText());
        return method;
    }

    Rule *Visitor::visitRuleSpec(CertoraParser::RulespecContext *ctx)
    {
        // clog << "visitRule" << endl;
        auto rule = new Rule;
        rule->id = ctx->ID()->getText();
        if (ctx->params())
            for (const auto &param : ctx->params()->param())
                rule->params.push_back(visitParam(param));
        for (const auto &stmt : ctx->stmt())
            rule->stmts.push_back(visitStmt(stmt));
        return rule;
    }

    Param *Visitor::visitParam(CertoraParser::ParamContext *ctx)
    {
        return new Param(ctx->type()->getText(), ctx->ID()->getText());
    }

    Stmt *Visitor::visitStmt(CertoraParser::StmtContext *ctx)
    {
        // clog << "visitStmt" << endl;
        if (ctx->type() && ctx->ID())
        {
            auto expr = ctx->expr() ? visitExpr(ctx->expr()) : nullptr;
            return new DeclStmt(ctx->type()->getText(), ctx->ID()->getText(), expr);
        }
        else if (ctx->REQUIRE())
            return new RequireStmt(visitExpr(ctx->expr()));
        else if (ctx->ASSERT())
            return new AssertStmt(visitExpr(ctx->expr()));
        else if (ctx->lhs())
        {
            vector<string> ids;
            for (const auto &id : ctx->lhs()->ID())
                ids.push_back(id->getText());
            return new AssignStmt(ids, visitExpr(ctx->expr()));
        }
        else if (ctx->fnCall())
            return new FnCallStmt(visitFnCall(ctx->fnCall()));
        else
            assert(false);
    }

    Expr *Visitor::visitExpr(CertoraParser::ExprContext *ctx)
    {
        if (ctx->CONST())
            return new ConstExpr(ctx->CONST()->getText());
        else if (ctx->ID())
            return new IdentExpr(ctx->ID()->getText());
        else if (ctx->prth_expr)
            return new PrthExpr(visitExpr(ctx->prth_expr));
        else if (ctx->obj_expr)
            return new FieldAccessExpr(visitExpr(ctx->obj_expr), ctx->ID()->getText());
        else if (ctx->fnCall())
            return new FnCallExpr(visitFnCall(ctx->fnCall()));
        else if (ctx->UOP())
            return new UnExpr(ctx->UOP()->getText(), visitExpr(ctx->expr(0)));
        else if (ctx->le && ctx->re)
        {
            string bop;
            if (ctx->OP_POW())
                bop = ctx->OP_POW()->getText();
            else if (ctx->OP_MUL())
                bop = ctx->OP_MUL()->getText();
            else if (ctx->OP_ADD())
                bop = ctx->OP_ADD()->getText();
            else if (ctx->OP_SH())
                bop = ctx->OP_SH()->getText();
            else if (ctx->OP_REL())
                bop = ctx->OP_REL()->getText();
            else if (ctx->OP_EQ())
                bop = ctx->OP_EQ()->getText();
            else if (ctx->OP_BIT())
                bop = ctx->OP_BIT()->getText();
            else if (ctx->OP_LOGIC())
                bop = ctx->OP_LOGIC()->getText();
            else if (ctx->OP_INF())
                bop = ctx->OP_INF()->getText();
            else
                assert(false);
            return new BinExpr(bop, visitExpr(ctx->le), visitExpr(ctx->re));
        }
        else if (ctx->cond)
            return new CondExpr(visitExpr(ctx->cond), visitExpr(ctx->te), visitExpr(ctx->fe));
        else
            assert(false);
    }
    FnCall *Visitor::visitFnCall(CertoraParser::FnCallContext *ctx)
    {
        vector<Expr *> exprs;
        string method_tag = ctx->method_tag ? ctx->method_tag->getText() : "";
        if (ctx->exprs())
            for (const auto &expr : ctx->exprs()->expr())
                exprs.push_back(visitExpr(expr));
        return new FnCall(ctx->ID(0)->getText(), method_tag, exprs);
    }
}