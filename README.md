Zephir Language
===============

Zephir - Ze(nd Engine) Ph(p) I(nt)r(mediate) - is a high level language that eases the creation and maintainability
of C-extensions for PHP. Zephir extensions are exported to C code that can be compiled and optimized by major C compilers
such as gcc/clang/vc++. Functionality is exposed to the PHP language. Zephir aims for near full
interoperability with existing PHP code.

Main features:

* Both dynamic/static typing
* Reduced execution overhead compared with full interpretation
* Restricted procedural programming, promoting OOP
* Memory safety
* Ahead-of-time compiler

Compiler design goals:

* Multi-pass compilation
* Type speculation/inference
* Allow runtime profile-guided optimizations, pseudo-constant propagation and indirect/virtual function inlining

Installation
------------

xx is the parser for the language:

You will need json-c installed on your machine:

* [json-c](https://github.com/json-c/json-c)

Compilation:

```bash
./generate
```

Usage
-----

Compile the extension:

```bash
./zephir
```

The code produced is placed in ext/, there you can perform the standard compilation:

```bash
cd ext/
phpize
./configure --enable-test
make && sudo make install
```
