# LISP-Style Prefix Arithmetic Interpreter in C++

A lightweight command-line interpreter built from scratch in C++ that evaluates simple arithmetic expressions written in LISP-style prefix notation (S-expressions).

## Features
* **Lexical Analysis (Tokenizer):** Breaks raw source strings into typed tokens (Numbers, Operators, Parentheses), with support for negative numbers.
* **Syntactic Analysis (Parser):** Implements a recursive descent parser to build an Abstract Syntax Tree (AST).
* **Evaluation Engine:** Utilizes object-oriented polymorphism (`std::unique_ptr`, virtual nodes) to recursively evaluate expressions.
* **Interactive REPL:** Continuous command-line loop to evaluate expressions on the fly.

## Example Usage

```
> (+ 1 2 (* 1 33))
Output: 36

> (- 10 2 3)
Output: 5
```
