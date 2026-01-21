# Parser Front-End

A stack-based lexer and parser implementation in C that builds an Abstract Syntax Tree from source files.

## Building

```bash
make              # Build the parser
make clean        # Remove build artifacts
make rebuild      # Clean and rebuild
```

## Usage

```bash
make run [file=<source-file>]           # Run with default file or with source file
make debugLexer [file=<source-file>] 
make showAst [file=<source-file>]
make typeChecker [file=<source-file>]
```

**Arguments:**
- `<source-file>` - Source code file to parse (required)
- `--debug-lexer` - Print token stream during lexical analysis
- `--show-ast` -    Show the generated Abstract Syntax Tree
- `--typechecker` - Run the type checker on the parsed AST

Arguments can be in any order.

## Examples

```bash
./main program.txt
./main program.txt --debug-lexer --show-ast --typechecker
make run                    # Run with default test file
make debugAll              # Run with all debug flags
make successTests          # Run all tests in tests/success/
make failTests             # Run all tests in tests/fail/
```

## Debug Output

**Lexer mode** shows tokens as they're scanned:
```
Token: NUMBER     at pos  0 (value: 42)
Token: PLUS       at pos  3
Token: EOF        at pos  5
```

## Architecture

- **Lexer**: Tokenizes source code
- **Parser**: Stack-based parser with reduction rules
- **AST**: Abstract Syntax Tree generation
- **Type Checker**: Sanity check of the AST