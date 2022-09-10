#include "AST.hpp"

#include <cassert>

using namespace std;

namespace ast
{
    // string Type::toStr()
    // {
    //     switch (kd)
    //     {
    //     case UINT256:
    //         return "uint";
    //     case ADDRESS:
    //         return "address";
    //     case BOOL:
    //         return "bool";
    //     case ENV:
    //         return "env";
    //     default:
    //         assert(false);
    //         break;
    //     }
    // }

    void Root::toV(Env *e)
    {
        for (const auto &method : methods)
            e->method_dict[method->id] = method;
        for (const auto &rule : rules)
        {
            e->start_parsing_a_rule();
            rule->toV(e);
            e->end_parsing_a_rule();
        }
    }

    void Rule::toV(Env *e)
    {
        e->rule_name = id;
        clog << "rule toV: " << id << endl;
        for (auto &stmt : stmts)
            if (stmt->stmtKind() == STMT_FN_CALL)
            {
                auto p = (FnCallStmt *)stmt;
                if (p->fn_call->method_tag == "withrevert")
                    e->with_revert = true;
            }

        for (auto &stmt : stmts)
            stmt->toV(e);
    }

    void DeclStmt::toV(Env *e)
    {
        if (expr) // with init
        {
            if (e->with_revert) // with reverted
            {
                auto exp = new VStrExpr(id + " = (" + expr->toStr() + ")", 1);
                e->rev_ante.push_back(exp);
            }
            else // finished
                if (e->after_invoke)
                {
                    auto exp = new VStrExpr(id + " = (" + expr->toStr() + ")", 1);
                    e->fin_ante.push_back(exp);
                }
                else
                {
                    auto exp = new VStrExpr(id + " = old(" + expr->toStr() + ")", 1);
                    e->fin_ante.push_back(exp);
                }
        }

        if (type != "env")
        {
            auto var_dec = new VVarDecl(type, id);
            e->var_decs.push_back(var_dec);
        }
    }
    void AssertStmt::toV(Env *e)
    {
        assert(e->after_invoke == true); // pred = finished
        if (e->with_revert == false)
        {
            auto vexpr = new VStrExpr(expr->toStr(), 1);
            e->fin_post.push_back(vexpr);
        }
        else
        {
            if (expr->exprKind() == BIN)
            {
                auto p = (BinExpr *)expr;
                assert(p->bin_op == "=>");
                if (p->rexpr->toStr() == "lastReverted")
                    e->rev_suff.push_back(new VStrExpr(p->lexpr->toStr(), 1));
                if (p->lexpr->toStr() == "lastReverted")
                    e->rev_nece.push_back(new VStrExpr(p->rexpr->toStr(), 1));
            }
        }
    }
    void RequireStmt::toV(Env *e)
    {
        assert(e->after_invoke == false); // pred = finished
        auto vexpr = new VStrExpr(expr->toStr(), 1);
        e->fin_pre.push_back(vexpr);
    }
    void AssignStmt::toV(Env *e)
    {
        int ids_size = ids.size();
        if (ids_size == 1)
        {
            if (e->with_revert) // with reverted
            {
                auto exp = new VStrExpr(ids[0] + " = (" + expr->toStr() + ")", 1);
                e->rev_ante.push_back(exp);
            }
            else // finished
                if (e->after_invoke)
                {
                    auto exp = new VStrExpr(ids[0] + " = (" + expr->toStr() + ")", 1);
                    e->fin_ante.push_back(exp);
                }
                else
                {
                    auto exp = new VStrExpr(ids[0] + " = old(" + expr->toStr() + ")", 1);
                    e->fin_ante.push_back(exp);
                }
        }
        else
        {
            assert(expr->exprKind() == EXPR_FN_CALL); // rhs must be an function call
            auto fn_call = ((FnCallExpr *)expr)->fn_call;
            assert(e->method_dict.count(fn_call->id));                         // the function should be declared in method block
            assert(e->method_dict[fn_call->id]->ret_types.size() == ids_size); // match the output
            for (int i = 0; i < ids_size; ++i)
            {
                if (e->with_revert) // with reverted
                {
                    auto exp = new VStrExpr(ids[i] + " = (" + expr->toStr() + "." + to_string(i) + ")", 1);
                    e->rev_ante.push_back(exp);
                }
                else // finished
                    if (e->after_invoke)
                    {
                        auto exp = new VStrExpr(ids[i] + " = (" + expr->toStr() + "." + to_string(i) + ")", 1);
                        e->fin_ante.push_back(exp);
                    }
                    else
                    {
                        auto exp = new VStrExpr(ids[i] + " = old(" + expr->toStr() + "." + to_string(i) + ")", 1);
                        e->fin_ante.push_back(exp);
                    }
            }
        }
    }
    void FnCallStmt::toV(Env *e)
    {
        assert(e->after_invoke == false);
        e->after_invoke = true;
        e->tx = new VFunction;
        e->tx->id = e->contract_name + "." + fn_call->id;
        for (const auto &exp : fn_call->exprs)
            if (exp->toStr() != "e") // not need to pass the env
                e->tx->args.push_back(exp->toStr());
    }
    string ConstExpr::toStr()
    {
        return str;
    }
    string IdentExpr::toStr()
    {
        return id;
    }
    string UnExpr::toStr()
    {
        return un_op + expr->toStr();
    }
    string BinExpr::toStr()
    {
        string vbinop = bin_op;
        if (bin_op == "==")
            vbinop = "=";
        if (bin_op == "=>")
            vbinop = "==>";
        return lexpr->toStr() + " " + vbinop + " " + rexpr->toStr();
    }
    string PrthExpr::toStr()
    {
        return "(" + expr->toStr() + ")";
    }
    string CondExpr::toStr()
    {
        // TODO
        return "";
    }
    string FieldAccessExpr::toStr()
    {
        // TODO
        string tmp = obj->toStr();
        if (tmp == "e")
            return field;
        else
            return tmp + "." + field;
    }
    string FnCallExpr::toStr()
    {
        return fn_call->toStr();
    }
    string FnCall::toStr()
    {
        int len = exprs.size();
        string res = id + "(";
        if (len > 0)
            res += exprs[0]->toStr();
        for (int i = 1; i < len; ++i)
            res += "," + exprs[i]->toStr();
        res += ")";
        return res;
    }

}

void Env::debug_output()
{
    cout << "with_revert: " << with_revert << endl;

    // cout << "rev_ante: " << endl;
    // for (auto &e : rev_ante)
    //     cout << e->str << endl;
    // cout << "rev_suff: " << endl;
    // for (auto &e : rev_suff)
    //     cout << e->str << endl;
    // cout << "rev_nece: " << endl;
    // for (auto &e : rev_nece)
    //     cout << e->str << endl;
}

VExpr *reduce(const vector<VExpr *> &list, string bop)
{
    int size = list.size();
    if (size == 0)
    {
        string init_val;
        if (bop == "&&")
            init_val = "true";
        else if (bop == "||")
            init_val = "false";
        else
            assert(false);
        return new VStrExpr(init_val);
    }
    auto ret = list[0];
    for (int i = 1; i < size; ++i)
        ret = new VBinExpr(bop, ret, list[i]);
    ret->with_parenthesis = 1;
    return ret;
}

void Env::start_parsing_a_rule()
{
    fin_pre.clear();
    fin_post.clear();
    fin_ante.clear();
    start_con.clear();
    rev_ante.clear();
    rev_suff.clear();
    rev_nece.clear();
    var_decs.clear();
    rule_name = "";
    tx = nullptr;
    after_invoke = false;
    with_revert = false;
}

void Env::end_parsing_a_rule()
{
    if (with_revert == false)
    {
        auto vfin = new VFinishedSpec;
        vfin->var_decs = var_decs;
        vfin->tx = tx;

        vfin->pre = reduce(fin_pre, "&&");
        vfin->post = new VBinExpr("==>", reduce(fin_ante, "&&"), reduce(fin_post, "&&"), 1);
        vfin->comment = rule_name;
        vspeclist.push_back(vfin);
    }
    else
    {
        auto vrev = new VRevertedSpec;
        vrev->var_decs = var_decs;
        vrev->tx = tx;
        vrev->con = new VBinExpr("==>", reduce(rev_ante, "&&"), reduce(rev_suff, "&&"), 1);
        vrev->comment = rule_name + "(sufficient condition)";
        vspeclist.push_back(vrev);

        auto vfin = new VFinishedSpec;
        vfin->var_decs = var_decs;
        vfin->tx = tx;

        vfin->pre = new VUnExpr("~", new VBinExpr("==>", reduce(rev_ante, "&&"), reduce(rev_nece, "&&"), 1), 1);
        vfin->post = new VStrExpr("true");
        vfin->comment = rule_name + "(necessary condition)";
        vspeclist.push_back(vfin);
    }
}