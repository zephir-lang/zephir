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

Requirements:

* [json-c](https://github.com/json-c/json-c)
* [re2c](http://re2c.org/)

Installation on Ubuntu
-----------------------

* apt-get install libjson0 libjson0-dev libjson0-dbg
* apt-get install re2c


Once you have the required packages installed, you can generate the parser as follows:

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
sudo ./install
```

Current Build Status
--------------------
Zephir is built under Travis CI service. Every commit pushed to this repository will queue a build into the continuous
integration service and will run all PHPUnit tests to ensure that everything is going well and the project is stable.
The current build status is:

[![Build Status](https://secure.travis-ci.org/phalcon/zephir.png?branch=master)](http://travis-ci.org/phalcon/zephir)

Donate & Sponsor
----------------
Zephir is driven by the voluntary and altruist efforts of the core developers and contributors.
We invite you to sponsor our work, by [donating](https://www.gittip.com/phalcon/) or contacting us regarding sponsorship, allowing us to invest more time and resources to the project, ensuring it will be ready sooner and with loads of features.

Hopefully, you can take advantage of the features provided by Zephir too!

