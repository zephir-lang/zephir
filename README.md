Zephir
======

[![Dependency Status](https://www.versioneye.com/package/phalcon:zephir/badge.png)](https://www.versioneye.com/package/phalcon:zephir)
[![Latest Stable Version](https://poser.pugx.org/phalcon/zephir/v/stable.png)](https://packagist.org/packages/phalcon/zephir)
[![Total Downloads](https://poser.pugx.org/phalcon/zephir/downloads.png)](https://packagist.org/packages/phalcon/zephir)
[![License](https://poser.pugx.org/phalcon/zephir/license.svg)](https://packagist.org/packages/phalcon/zephir)
[![Build on Linux](https://secure.travis-ci.org/phalcon/zephir.svg?branch=master)](http://travis-ci.org/phalcon/zephir)
[![Build on Windows](https://ci.appveyor.com/api/projects/status/cxa1810md7v6n095/branch/master?svg=true)](https://ci.appveyor.com/project/sergeyklay/zephir/branch/master)


Zephir - Ze(nd Engine) Ph(p) I(nt)r(mediate) - is a high level language that eases the creation and maintainability
of extensions for PHP. Zephir extensions are exported to C code that can be compiled and optimized by major C compilers
such as gcc/clang/vc++. Functionality is exposed to the PHP language.

Main features:

* Both dynamic/static typing
* Reduced execution overhead compared with full interpretation
* Restricted procedural programming, promoting OOP
* Memory safety
* Ahead-of-time (AOT) compiler to provide predictable performance

Compiler design goals:

* Multi-pass compilation
* Type speculation/inference
* Allow runtime profile-guided optimizations, pseudo-constant propagation and indirect/virtual function inlining

Requirements
------------

To compile zephir-parser:

* [re2c](http://re2c.org/)

To build the PHP extension:

* g++ >= 4.4/clang++ >= 3.x/vc++ 9
* gnu make 3.81 or later
* automake
* php development headers and tools

Installation
------------

First, clone Zephir repository.

Next, you have to get [Zephir Parser](https://github.com/phalcon/php-zephir-parser).
The most easy way to do it is to use `git submodule` command:

```bash
git submodule init
git submodule update
```

Then you have to install it:

```bash
cd modules/php-zephir-parser
sudo ./install
```

Add the extension to your **php.ini**:

```ini
[Zephir Parser]
extension=zephir_parser.so
```

Finally, install Zephir by using command `./install` from project root dir.
For global installation add `-c` flag.

### Easy way pull latest of all submodules

For git 1.6.1 or above you can use something similar to:

```bash
git submodule update --remote --merge
```

See [git-submodule(1)](http://www.kernel.org/pub/software/scm/git/docs/v1.6.1.3/git-submodule.html) for details.

Additional notes on Ubuntu
--------------------------

The following packages are needed in Ubuntu:

* apt-get install re2c libpcre3-dev

Usage
-----

Compile the extension:

```bash
./bin/zephir compile
```

External Links
--------------

* [Documentation](https://zephir-lang.com/)
* [Official Blog](https://blog.zephir-lang.com/)
* [Forum](https://forum.zephir-lang.com/)
* [Twitter](https://twitter.com/zephirlang)

Contributing
------------

See [CONTRIBUTING.md](https://github.com/phalcon/zephir/blob/master/CONTRIBUTING.md) for details about contributions to this repository.

License
-------

Zephir is open-sourced software licensed under the MIT License. See the LICENSE file for more information.
