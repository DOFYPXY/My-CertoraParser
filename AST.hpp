#ifndef CERTORA_PARSER_AST_HPP
#define CERTORA_PARSER_AST_HPP

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "V.hpp"

using namespace std;

class Env;

namespace ast
{
    class Rule;
    class Param;
    class Stmt;
    class Type;
    class Expr;
    class FnCall;

    class AST
    {
    public:
        virtual ~AST() = default;
    };

    class Rule : public AST
    {
    public:
        string id;
        vector<Param *> params;
        vector<Stmt *> stmts;
        void toV(Env *e);
    };

    class Method : public AST
    {
    public:
        string id;
        vector<string> param_types;
        vector<string> ret_types;
        bool env_free;
        void toV(Env *e);
    };

    class Definition : public AST
    {
        // TODO
    };

    class Root : public AST
    {
    public:
        // Root(const vector<Method *> &_methods, const vector<Definition *> _definitions, vector<Rule *> _rules) : methods(_methods), definitions(_definitions), rules(_rules) {}
        vector<Method *> methods;
        vector<Definition *> definitions;
        vector<Rule *> rules;
        void toV(Env *e);
    };

    class Param : public AST
    {
    public:
        Param(const string &_type, const string &_id) : type(_type), id(_id) {}
        string type;
        string id;
    };

    enum StmtKind
    {
        DECL,
        ASSERT,
        REQUIRE,
        ASSIGN,
        STMT_FN_CALL
    };

    class Stmt : public AST
    {
    public:
        virtual StmtKind stmtKind() = 0;
        virtual void toV(Env *e) = 0;
    };

    class DeclStmt : public Stmt
    {
    public:
        DeclStmt(const string &_type, const string &_id, Expr *_expr) : type(_type), id(_id), expr(_expr) {}
        StmtKind stmtKind() override { return DECL; }
        string type;
        string id;
        Expr *expr;
        void toV(Env *e) override;
    };

    class AssertStmt : public Stmt
    {
    public:
        AssertStmt(Expr *_expr) : expr(_expr) {}
        StmtKind stmtKind() override { return ASSERT; }
        Expr *expr;
        // string str;
        void toV(Env *e) override;
    };

    class RequireStmt : public Stmt
    {
    public:
        RequireStmt(Expr *_expr) : expr(_expr) {}
        StmtKind stmtKind() override { return REQUIRE; }
        Expr *expr;
        void toV(Env *e) override;
    };

    class AssignStmt : public Stmt
    {
    public:
        AssignStmt(const vector<string> &_ids, Expr *_expr) : ids(_ids), expr(_expr) {}
        StmtKind stmtKind() override { return ASSIGN; }
        vector<string> ids;
        Expr *expr;
        void toV(Env *e) override;
    };

    class FnCallStmt : public Stmt
    {
    public:
        FnCallStmt(FnCall *_fn_call) : fn_call(_fn_call) {}
        StmtKind stmtKind() override { return STMT_FN_CALL; }
        FnCall *fn_call;
        void toV(Env *e) override;
    };

    // enum TypeKind
    // {
    //     UINT256,
    //     ADDRESS,
    //     BOOL,
    //     ENV,
    //     BYTES32
    // };

    // class Type : public AST
    // {
    // public:
    //     TypeKind kd;
    //     string toStr();
    // };

    enum ExprKind
    {
        CONST,
        IDENT,
        UN,
        BIN,
        COND,
        PARENTHESIS,
        FIELD_ACCESS,
        EXPR_FN_CALL
    };

    class Expr : public AST
    {
    public:
        virtual ExprKind exprKind() = 0;
        virtual string toStr() = 0;
    };

    class PrthExpr : public Expr
    {
    public:
        PrthExpr(Expr *_expr) : expr(_expr) {}
        ExprKind exprKind() override { return PARENTHESIS; }
        Expr *expr;
        string toStr() override;
    };

    class ConstExpr : public Expr
    {
    public:
        ConstExpr(const string &_str) : str(_str) {}
        ExprKind exprKind() override { return CONST; }
        string str;
        string toStr() override;
    };

    class IdentExpr : public Expr
    {
    public:
        IdentExpr(const string &_id) : id(_id) {}
        ExprKind exprKind() override { return IDENT; }
        string id;
        string toStr() override;
    };

    class UnExpr : public Expr
    {
    public:
        UnExpr(const string &_un_op, Expr *_expr) : un_op(_un_op), expr(_expr) {}
        ExprKind exprKind() override { return UN; }
        Expr *expr;
        string un_op;
        string toStr() override;
    };

    class BinExpr : public Expr
    {
    public:
        BinExpr(const string &_bin_op, Expr *_lexpr, Expr *_rexpr) : bin_op(_bin_op), lexpr(_lexpr), rexpr(_rexpr) {}
        ExprKind exprKind() override { return BIN; }
        Expr *lexpr, *rexpr;
        string bin_op;
        string toStr() override;
    };

    class CondExpr : public Expr
    {
    public:
        CondExpr(Expr *_cond, Expr *_texpr, Expr *_fexpr) : cond(_cond), texpr(_texpr), fexpr(_fexpr) {}
        ExprKind exprKind() override { return COND; }
        Expr *cond, *texpr, *fexpr;
        string toStr() override;
    };

    class FieldAccessExpr : public Expr
    {
    public:
        FieldAccessExpr(Expr *_obj, const string &_field) : obj(_obj), field(_field) {}
        ExprKind exprKind() override { return FIELD_ACCESS; }
        Expr *obj;
        string field;
        string toStr() override;
    };

    class FnCallExpr : public Expr
    {
    public:
        FnCallExpr(FnCall *_fn_call) : fn_call(_fn_call) {}
        ExprKind exprKind() override { return EXPR_FN_CALL; }
        FnCall *fn_call;
        string toStr() override;
    };

    class FnCall : public AST
    {
    public:
        FnCall(const string &_id, const string &_method_tag, const vector<Expr *> &_exprs) : id(_id), method_tag(_method_tag), exprs(_exprs) {}
        string id;
        vector<Expr *> exprs;
        string method_tag;
        string toStr();
    };

}

class Env
{
public:
    vector<VExpr *> fin_pre;
    vector<VExpr *> fin_post;
    vector<VExpr *> fin_ante;
    vector<VExpr *> start_con;
    vector<VExpr *> rev_ante;
    vector<VExpr *> rev_suff;
    vector<VExpr *> rev_nece;
    vector<VVarDecl *> var_decs;
    VFunction *tx;
    string rule_name;
    string contract_name;
    bool after_invoke;
    bool with_revert;

    vector<VSpec *> vspeclist;
    unordered_map<string, ast::Method *> method_dict;

    Env() { contract_name = "Vat"; }
    void start_parsing_a_rule();
    void end_parsing_a_rule();

    void debug_output();
};

#endif
