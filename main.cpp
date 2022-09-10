#include <iostream>

#include "antlr4-runtime.h"
#include "libs/CertoraLexer.h"
#include "libs/CertoraParser.h"
#include "Visitor.hpp"
// #include "ImageVisitor.h"

using namespace std;
using namespace antlr4;

int main(int argc, const char *argv[])
{
    // parser input_file -o output_file
    assert(argc == 4);
    auto finput = argv[1];
    auto foutput = argv[3];

    std::ifstream istream;
    istream.open(finput);
    clog << "open." << endl;
    ANTLRInputStream input(istream);
    clog << "input." << endl;
    CertoraLexer lexer(&input);
    clog << "lex." << endl;
    CommonTokenStream tokens(&lexer);
    clog << "token." << endl;
    CertoraParser parser(&tokens);
    clog << "parser." << endl;

    CertoraParser::RootContext *tree = parser.root();
    clog << "got root." << endl;
    visitor::Visitor visitor;
    auto root = visitor.visitRoot(tree);
    clog << "visit." << endl;
    auto e = new Env;
    root->toV(e);
    clog << "toV finished." << endl;
    string str;

    for (const auto &vspec : e->vspeclist)
    {
        vspec->Dump(str);
        str += "\n\n";
    }

    std::ofstream ostream;
    ostream.open(foutput);
    ostream << str;

    istream.close();
    ostream.close();

    return 0;
}