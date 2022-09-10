#include "V.hpp"

// void VFinishedSpec::Dump(string &s)
// {
//     s += "vars: ";

//     int var_size = var_decs.size();
//     if (var_size > 0)
//     {
//         var_decs[0]->Dump(s);
//         for (int i = 1; i < var_size; ++i)
//         {
//             s += ", ";
//             var_decs[i]->Dump(s);
//         }
//     }

//     s += "\nspec: ";
//     s += pred + "(";
//     tx.Dump(s);
//     s += ", ";
//     int pre_size = pre.size(), post_size = post.size();
//     if (pre_size == 0)
//         s += "true ";
//     else
//     {
//         s += "(", pre[0]->Dump(s), s += ")";
//         for (int i = 1; i < pre_size; ++i)
//         {
//             s += " && ";
//             s += "(", pre[i]->Dump(s), s += ")";
//         }
//     }
//     s += " |=> ";
//     if (post_size == 0)
//         s += "true ";
//     else
//     {
//         s += "(", post[0]->Dump(s), s += ")";
//         for (int i = 1; i < post_size; ++i)
//         {
//             s += " && ";
//             s += "(", post[i]->Dump(s), s += ")";
//         }
//     }
//     s += ")";
// }

void VFinishedSpec::Dump(string &s)
{
    s += "// " + comment + "\n";
    s += "vars: ";

    int var_size = var_decs.size();
    if (var_size > 0)
    {
        var_decs[0]->Dump(s);
        for (int i = 1; i < var_size; ++i)
        {
            s += ", ";
            var_decs[i]->Dump(s);
        }
    }

    s += "\nspec: ";
    s += "finished(";
    tx->Dump(s);
    s += ", ";

    pre->Dump(s);
    s += " |=> ";
    post->Dump(s);
    s += ")";
}

void VRevertedSpec::Dump(string &s)
{
    s += "// " + comment + "\n";
    s += "vars: ";

    int var_size = var_decs.size();
    if (var_size > 0)
    {
        var_decs[0]->Dump(s);
        for (int i = 1; i < var_size; ++i)
        {
            s += ", ";
            var_decs[i]->Dump(s);
        }
    }

    s += "\nspec: ";
    s += "reverted(";
    tx->Dump(s);
    s += ", ";

    con->Dump(s);
    s += ")";
}

void VVarDecl::Dump(string &s)
{
    s += type + " " + id;
}

void VFunction::Dump(string &s)
{
    s += id + "(";
    int arg_size = args.size();
    if (arg_size > 0)
    {
        s += args[0];
        for (int i = 1; i < arg_size; ++i)
            s += ", " + args[i];
    }
    s += ")";
}

void VStrExpr::Dump(string &s)
{
    if (with_parenthesis)
        s += "(";
    s += str;
    if (with_parenthesis)
        s += ")";
}

void VBinExpr::Dump(string &s)
{
    assert(lexpr && rexpr);
    if (with_parenthesis)
        s += "(";
    lexpr->Dump(s);
    s += " " + bop + " ";
    rexpr->Dump(s);
    if (with_parenthesis)
        s += ")";
}

void VUnExpr::Dump(string &s)
{
    assert(expr);
    if (with_parenthesis)
        s += "(";
    s += uop;
    expr->Dump(s);
    if (with_parenthesis)
        s += ")";
}
