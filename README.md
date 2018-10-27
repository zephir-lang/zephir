<h1 align="center"><img src="https://zephir-lang.com/images/banner-232x80.png" alt="Zephir"/></h1>

<p align="center">
<a href="https://travis-ci.org/phalcon/zephir"><img src="https://travis-ci.org/phalcon/zephir.svg" alt="Build on Linux"></a>
<a href="https://ci.appveyor.com/project/sergeyklay/zephir"><img src="https://ci.appveyor.com/api/projects/status/cxa1810md7v6n095?svg=true" alt="BBuild on Windows"></a>
<a href="https://packagist.org/packages/phalcon/zephir"><img src="https://poser.pugx.org/phalcon/zephir/d/total.svg" alt="Total Downloads"></a>
<a href="https://packagist.org/packages/phalcon/zephir"><img src="https://poser.pugx.org/phalcon/zephir/v/stable.svg" alt="Latest Stable Version"></a>
<a href="https://packagist.org/packages/phalcon/zephir"><img src="https://poser.pugx.org/phalcon/zephir/license.svg" alt="License"></a>
</p>

## Introduction

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

Official documentation [is located here](https://docs.zephir-lang.com).

## Contributing

See [CONTRIBUTING.md](https://github.com/phalcon/zephir/blob/master/CONTRIBUTING.md) for details about contributions to this repository.

## License

Zephir is open-sourced software licensed under the MIT License.
See the [LICENSE](https://github.com/phalcon/zephir/blob/master/LICENSE) file for more information.
