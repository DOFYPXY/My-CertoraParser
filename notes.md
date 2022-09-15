I found that some rules written in Certora are translated into multiple specs in Cairo. For example, the rule for contract `file_ilk_revert` is separated to 4 specs in Cairo named `file_ilk_revert_(suff_spec1 | suff_spec2 ｜ suff_spec3 | necessary_spec)`. One possible value combination of `wards`, `live`, and `what` is considered in each specs. I am curious why it is not written together to make it similar to the original one, like

```
func file_ilk_revert_spec():
    let (condition1) = ...
    let (condition2) = ...
    ...
    if condition1:
        verify ...
    elif condition2:
        verify ...
    ...
```

Certora specs: https://github.com/makerdao/xdomain-dss/blob/371315e165a523a07af278511c2a3358aa96bd06/certora/Vat.spec#L210

Cairo specs: https://github.com/Veridise/Medjai-Dev/blob/shankara/jacob/debug/MCD/contracts/vatspec.cairo#L345


----

IR DSL:

stmt ::= def | assume | assert | func_call

def ::= id, type, exp


---

OP Priority:

```
Exp           ::= LOrExp;
LVal          ::= IDENT {"[" Exp "]"};
PrimaryExp    ::= "(" Exp ")" | LVal | Number;
Number        ::= LITERAL;
UnaryExp      ::= PrimaryExp | FunctionCall | UnaryOp UnaryExp;
UnaryOp       ::= "~" | "-" | "!";
MulExp        ::= UnaryExp | MulExp ("*" | "/" | "%") UnaryExp;
AddExp        ::= MulExp | AddExp ("+" | "-") MulExp;
ShExp         ::= AddExp | ShExp ("<<" | ">>" | ">>>") AddExp;
RelExp        ::= ShExp | RelExp ("<" | ">" | "<=" | ">=") ShExp;
EqExp         ::= RelExp | EqExp ("==" | "!=") RelExp;
AndExp        ::= EqExp | AndExp "&" EqExp;
XorExp        ::= AndExp | XorExp "^" AndExp;
OrExp         ::= XorExp | OrExp "|" XorExp; 
LAndExp       ::= OrExp | LAndExp "&&" OrExp;
LOrExp        ::= LAndExp | LOrExp "||" LAndExp;
```


---


I think for translating Certora specs we may need to focus on these points:
- local variables without initialization: define them in `vars`
- local variables with initialization: 
    1. declare them in `vars` and require the equality betweem the variable and its init value in the pre-condition
    2. or not declare them `vars` but replace their occurrence in spec by their init value 
- expression before invoking the contract method: `old(expr)`
- arguments of the rule: seem to always be the same as the arguments of contract function so just ignore them.
- `require`: pre-condition
- `assert`: post-condition

For rules about reverting, I think we still need a new predicate [V] to specify the necessary condition of reverting.


---
 
I tried to find some existing tools to parse Certora specs but couldn't find any for now. So I implemented a parser in C++. Now it can translated some simple Certora specs to [V] like `rely` and `move`. And the results can be regonized by VInstrumenter.

And I plan to deal with the rules with `revert` next days.

I have a question about `finished()` in [V]. The [V-blog](https://www.notion.so/veridise/V-blog-d5a751d20b084818a555c994bd1fc8fa) said that `finished(transaction, Pre |=> Post)` indicates if `Pre` holds the transaction should not reverted. I want to know that whether `old()` should be used when writing pre-condition.

For these two statements,

`finished(withdraw(amt), balance >= amt |=> balance = old(balance) - amt)`

and 

`finished(withdraw(amt), old(balance) >= amt |=> balance = old(balance) - amt)`

which one is right?

---
TODO: 

- more certora examples (most rules of makerDAO have the same pattern)


- `ArtBefore, rateBefore, spotBefore, lineBefore, dustBefore = ilks(i);`

Can user writes equalities like this `ArtBefore, rateBefore, spotBefore, lineBefore, dustBefore = ilks(i)` to decouple a data structrue in [V]?

Since there are many such statements in certora specs, I wonder that whether I can directly copy them when implementing the parser.

Or the parser must refer to the definition of the data structure and decouple it separately like

```
ArtBefore = ilks(i).Art
rateBefore = ilks(i).rate
...
```

- correspondence of types betweem Certora and [V].
    - `bytes32`
    - `mathint`
- sol: no type in [V]


- loop and if statements


----

9.13

- About the initial value of local var, like `uint x = a`. I think the following two are equivalent:
    - `Pre |=> Post(a)`
    - `\forall x, Pre |=> (x==a => Post(x))`

`uint x = a; uint y = x ...;`

- The rules without invocation of contract method, like `ilk_getters()` and `urn_getters()`
```
rule ilk_getters() {
    bytes32 ilk;
    uint256 Art; uint256 rate; uint256 spot; uint256 line; uint256 dust;
    Art, rate, spot, line, dust = ilks(ilk);

    assert(Art == Art(ilk), "Art getter did not return ilk.Art");
    assert(rate == rate(ilk), "rate getter did not return ilk.rate");
    assert(spot == spot(ilk), "spot getter did not return ilk.spot");
    assert(line == line(ilk), "line getter did not return ilk.line");
    assert(dust == dust(ilk), "dust getter did not return ilk.dust");
}
```