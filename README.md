# Zephir

[![Dependency Status](https://www.versioneye.com/package/phalcon:zephir/badge.png)](https://www.versioneye.com/package/phalcon:zephir)
[![Latest Stable Version](https://poser.pugx.org/phalcon/zephir/v/stable.png)](https://packagist.org/packages/phalcon/zephir)
[![Total Downloads](https://poser.pugx.org/phalcon/zephir/downloads.png)](https://packagist.org/packages/phalcon/zephir)
[![License](https://poser.pugx.org/phalcon/zephir/license.svg)](https://packagist.org/packages/phalcon/zephir)
[![Build on Linux](https://secure.travis-ci.org/phalcon/zephir.svg?branch=master)](http://travis-ci.org/phalcon/zephir)
[![Build on Windows](https://ci.appveyor.com/api/projects/status/cxa1810md7v6n095/branch/master?svg=true)](https://ci.appveyor.com/project/sergeyklay/zephir/branch/master)


**Zephir** - Ze(nd Engine) Ph(p) I(nt)r(mediate) - is a high level language that eases the creation and maintainability
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

## Requirements

* [re2c](http://re2c.org/)
* [Zephir Parser](https://github.com/phalcon/php-zephir-parser) >= 1.1.0
* `g++` >= 4.4 | `clang++` >= 3.x | `vc++` >= 11
* GNU `make` >= 3.81
* `automake`
* PHP development headers and tools

## Installation

**NOTE:** The `development` branch will always contain the latest **unstable** version.
If you wish to check older versions or formal, tagged release, please switch to the relevant
[branch](https://github.com/phalcon/zephir/branches)/[tag](https://github.com/phalcon/zephir/tags).

### Windows

To install Zephir on Windows [follow this guide](https://github.com/phalcon/zephir/blob/master/WINDOWS.md).

### Linux | macOS | BSD | Solaris

#### Git way

Clone Zephir repository.

Then, install Zephir by using command `./install` from project root dir.
For global installation add `-c` flag.

### Composer way

You can install Zephir using composer.

To get Zephir, run `composer require phalcon/zephir`.

### Additional notes on Ubuntu

The following packages are needed in Ubuntu:

* `apt-get install re2c libpcre3-dev`

## Usage

Compile the extension:

```bash
zephir compile
```

## External Links

* [Documentation](https://zephir-lang.com/)
* [Official Blog](https://blog.zephir-lang.com/)
* [Forum](https://forum.zephir-lang.com/)
* [Twitter](https://twitter.com/zephirlang)

## Contributing

See [CONTRIBUTING.md](https://github.com/phalcon/zephir/blob/master/CONTRIBUTING.md) for details about contributions to this repository.

## License

Zephir is open-sourced software licensed under the MIT License.
See the LICENSE file for more information.
