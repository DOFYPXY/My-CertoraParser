#ifndef CERTORA_PARSER_V_HPP
#define CERTORA_PARSER_V_HPP

#include <string>
#include <vector>
#include <cassert>
using namespace std;

class VVarDecl
{
public:
    VVarDecl(const string &_type, const string &_id) : type(_type), id(_id) {}
    string type;
    string id;
    void Dump(string &s);
};

class VFunction
{
public:
    string id;
    vector<string> args;
    void Dump(string &s);
};

class VExpr
{
public:
    bool with_parenthesis;
    VExpr(bool _wp) : with_parenthesis(_wp) {}
    virtual void Dump(string &s) = 0;
};

class VBinExpr : public VExpr
{
public:
    string bop;
    VExpr *lexpr;
    VExpr *rexpr;
    VBinExpr(string _bop, VExpr *_l, VExpr *_r, bool _wp = 0) : VExpr(_wp), bop(_bop), lexpr(_l), rexpr(_r) {}
    void Dump(string &s) override;
};

class VUnExpr : public VExpr
{
public:
    string uop;
    VExpr *expr;
    VUnExpr(string _uop, VExpr *_e, bool _wp = 0) : VExpr(_wp), uop(_uop), expr(_e) {}
    void Dump(string &s) override;
};

class VStrExpr : public VExpr
{
public:
    string str;
    VStrExpr(string _str, bool _wp = 0) : VExpr(_wp), str(_str) {}
    void Dump(string &s) override;
};

// class VExpr
// {
// public:
//     string str;
//     VExpr(string _str) : str(_str) {}
//     void Dump(string &s);
// };

class VSpec
{
public:
    vector<VVarDecl *> var_decs;
    VFunction *tx;
    string comment;

    virtual void Dump(string &s) = 0;
};

class VFinishedSpec : public VSpec
{
public:
    // vector<VExpr *> ante;
    // vector<VExpr *> pre;
    // vector<VExpr *> post;
    VExpr *pre;
    VExpr *post;
    void Dump(string &s) override;
};

class VRevertedSpec : public VSpec
{
public:
    // vector<VExpr *> ante; // antecedent
    // vector<VExpr *> suff;
    VExpr *con;
    void Dump(string &s) override;
};

#endif
