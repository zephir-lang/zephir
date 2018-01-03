# <img src="https://avatars0.githubusercontent.com/u/6891407" height="28px"/> Zephir

[![Latest Stable Version][badge-zep-version]][zep-packagist]
[![Total Downloads][badge-zep-downloads]][zep-packagist]
[![License][badge-zep-license]][zep-packagist]
[![Build on Linux][badge-zep-travisci]][zep-travisci]
[![Build on Windows][badge-zep-appveyor]][zep-appveyor]

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

* [re2c][re2c] >= 0.13.6
* [Zephir Parser][zephir-parser] >= 1.1.0
* A C compiler such as `gcc` >= 4.4 or an alternative such as `clang` >= 3.0,
  `Visual C++` >= 11 or `Intel C++`. It is recommended to use `gcc` 4.4 or later
* GNU `make` >= 3.81
* `automake`
* PHP development headers and tools
* The `build-essential` package when using `gcc` on Ubuntu (and likely other distros as well)

## Installation

**NOTE:** The `development` branch will always contain the latest **unstable** version.
If you wish to check older versions or formal, tagged release, please switch to the relevant
[branch][zep-branches]/[tag][zep-tags].

### Windows

To install Zephir on Windows [follow this guide][zep-doc-windows].

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

* [Documentation][zep-docs-site]
* [Official Blog][zep-blog-site]
* [Forum][zep-forum]
* [Twitter][zep-twitter]
* [Facebook Group][zep-facebook]

## Contributing

See [CONTRIBUTING.md][zep-doc-contributting] for details about contributions to this repository.

## License

Zephir is open-sourced software licensed under the MIT License.
See the LICENSE file for more information.

[zep-packagist]: https://packagist.org/packages/phalcon/zephir
[zep-travisci]: https://travis-ci.org/phalcon/zephir
[zep-appveyor]: https://ci.appveyor.com/project/sergeyklay/zephir/branch/master
[re2c]: http://re2c.org/
[zephir-parser]: https://github.com/phalcon/php-zephir-parser
[zep-branches]: https://github.com/phalcon/zephir/branches
[zep-tags]: https://github.com/phalcon/zephir/tags
[zep-doc-windows]: https://github.com/phalcon/zephir/blob/master/WINDOWS.md
[zep-doc-contributting]: https://github.com/phalcon/zephir/blob/master/CONTRIBUTING.md
[zep-docs-site]: https://docs.zephir-lang.com/
[zep-blog-site]: https://blog.zephir-lang.com/
[zep-forum]: https://forum.zephir-lang.com/
[zep-twitter]: https://twitter.com/zephirlang
[zep-facebook]: https://www.facebook.com/groups/zephir.language/
[badge-zep-version]: https://poser.pugx.org/phalcon/zephir/v/stable.png
[badge-zep-downloads]: https://poser.pugx.org/phalcon/zephir/downloads.png
[badge-zep-license]: https://poser.pugx.org/phalcon/zephir/license.svg
[badge-zep-travisci]: https://img.shields.io/travis/phalcon/zephir/master.svg
[badge-zep-appveyor]: https://ci.appveyor.com/api/projects/status/cxa1810md7v6n095?svg=true
