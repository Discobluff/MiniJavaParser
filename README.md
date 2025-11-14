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
make debugParser [file=<source-file>]
make debugAll [file=<source-file>]
```

**Arguments:**
- `<source-file>` - Source code file to parse (required)
- `--debug-lexer` - Print token stream during lexical analysis
- `--debug-parser` - Print parser stack and AST operations at each step

Arguments can be in any order.

## Examples

```bash
./main program.txt
./main program.txt --debug-lexer --debug-parser
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

**Parser mode** shows each iteration:
```
==================== NEW ITERATION ===================
[Parser stack contents]

Token: NUMBER     at pos  0 (value: 42)

reducing on AST stack
[AST stack before reduction]

produced stack
[AST stack after reduction]
```

## Architecture

- **Lexer**: Tokenizes source code
- **Parser**: Stack-based parser with reduction rules
- **AST**: Abstract Syntax Tree generation

The parser uses two stacks: one for grammar rules/tokens, one for building the AST. Processing continues until the parser stack is empty and a single AST node remains.