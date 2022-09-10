# [Dev] Certora Parser

A Parser form Certora to [V].

It was able to parse and translate all rules in `VAT` except `fallback_revert`, `ilk_getters` and `urn_getters`.

## Usage

Before Cmake, you should set the path of antlr4 git repo to the remote repository (the local one is for my own convenience). To set it you should first remove line 11 in `cmake/ExternalAntlr4Cpp.cmake`:
```CMake
set(ANTLR4_GIT_REPOSITORY ${CMAKE_CURRENT_SOURCE_DIR}/antlr4)
```
And enable the remote path at line 9:
```CMake
set(ANTLR4_GIT_REPOSITORY https://github.com/antlr/antlr4.git)
```

Then build the parser by running following commands:

```
mkdir build
cd build
cmake ../
make
```

You can run the parser like this:
```
./parser ../Vat/Vat_supported.spec -o ../a.out
```

## Rules of translation

- `require`: pre-condition
- `assert`: post-condition
- local variables without initialization: declare them in `vars`
- local variables with initialization: declare them in `vars` and require the equality betweem the variable and its init value
- expression before invoking the contract method: `old(expr)`


### Examples

The rules in MakerDAO's `VAT` file can be roughly divdied into two kinds:

1. Require the pre-condition, invoke a contract function, and finally assert the post-condition.
2. Assert the necessary and sufficient condition for a contract function to revert.

Here is an example which basically illustrates the rules of translation.

The first kind of rules has the following form, 
```
rule f(Typep p) {
    env e;
    Typea a;
    Typeb b = init_val;
    require(A);
    f(p);
    assert(B);
}
```

Its corresponding [V] spec will be like,
```
vars: Typea a, Typeb b
spec: finished(f(p), A |=> (b = old(init_val) ==> B))
```

For the second kind of rules,
```
rule f_revert(Typep p) {
    env e;
    f@withrevert(p);
    assert(lastReverted => A);
    assert(B => lastReverted);
}
```

The parser will generate two spec, standing for necessary condition and sufficient condition.
```
// necessary condition
spec: finished(f(p), ~A |=> true)

// suffcient condition
spec: reverted(f(p), B);
```

## Validator

Using [VInstrumenter](https://github.com/Veridise/VInstrumenter) to validate the output of the parser.