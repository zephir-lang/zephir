Zephir
======

[![Dependency Status](https://www.versioneye.com/package/phalcon:zephir/badge.png)](https://www.versioneye.com/package/phalcon:zephir)
[![Latest Stable Version](https://poser.pugx.org/phalcon/zephir/v/stable.png)](https://packagist.org/packages/phalcon/zephir)
[![Total Downloads](https://poser.pugx.org/phalcon/zephir/downloads.png)](https://packagist.org/packages/phalcon/zephir)
[![License](https://poser.pugx.org/phalcon/zephir/license.svg)](https://packagist.org/packages/phalcon/zephir)
[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/phalcon/zephir?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

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

* [json-c](https://github.com/phalcon/json-c) (Please install this one from Github)
* [re2c](http://re2c.org/)

To build the PHP extension:

* g++ >= 4.4/clang++ >= 3.x/vc++ 9
* gnu make 3.81 or later
* php development headers and tools

Installation
------------
You can install zephir using composer.
Run `composer require phalcon/zephir`, run `./install` and then run `zephir`
from your `bin-dir`. By default it is `./vendor/bin/zephir`.
You can read more about composer binaries
in it's [documentation](https://getcomposer.org/doc/articles/vendor-binaries.md).

For global installation via composer you can use `composer global require`.
Do not forget add `~/.composer/vendor/bin` into your `$PATH`.

Also you can just clone zephir repository and run `./install`.
For global installation add `-c` flag.

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
* [Documentation](http://zephir-lang.com/)
* [Official Blog](http://blog.zephir-lang.com/)
* [Forum](http://forum.zephir-lang.com/)
* [Twitter](http://twitter.com/zephirlang)

License
-------
Zephir is open-sourced software licensed under the MIT License. See the LICENSE file for more information.

Contributing
------------

See [CONTRIBUTING.md](https://github.com/phalcon/zephir/blob/master/CONTRIBUTING.md) for details about contributions to this repository.

Current Build Status
--------------------
Zephir is built under Travis CI service. Every commit pushed to this repository will queue a build into the continuous integration service and will run all PHPUnit tests to ensure that everything is going well and the project is stable. The current build status is:

* Linux: [![Build Status](https://secure.travis-ci.org/phalcon/zephir.svg?branch=master)](http://travis-ci.org/phalcon/zephir)
* Windows: [![Build Status](https://ci.appveyor.com/api/projects/status/github/gruntjs/grunt?branch=master&svg=true)](https://ci.appveyor.com/project/phalcon/zephir)
