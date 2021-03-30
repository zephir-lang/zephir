# Change Log
All notable changes to this project will be documented in this file.

The format based on [Keep a Changelog](http://keepachangelog.com)
and this project adheres to [Semantic Versioning](http://semver.org).

## [Unreleased]
### Added
- Added jobs `-j, --jobs` option for `zephir compile` [#2174](https://github.com/zephir-lang/zephir/issues/2174)

### Fixed
- Fixed not used arginfo for interface static method without parameters (PHP `>= 8.0` only) [#2178](https://github.com/zephir-lang/zephir/pull/2178)

## [0.13.0] - 2021-03-25
### Added
- Added support of PHP `8.0` [#2111](https://github.com/zephir-lang/zephir/pull/2111), [#2165](https://github.com/zephir-lang/zephir/pull/2165)

### Changed
- Dropped support of PHP `< 7.4` versions [#2111](https://github.com/zephir-lang/zephir/pull/2111)
- Removed call of `generate` command inside `compile` call [#2150](https://github.com/zephir-lang/zephir/pull/2150)
- Removed call of `compile` command inside `install` call [#2150](https://github.com/zephir-lang/zephir/pull/2150)

### Fixed
- Fixed parameters type detection in methods/functions (PHP `>= 8.0` only)
- Fixed not adding all build directories [#2144](https://github.com/zephir-lang/zephir/pull/2144)

## [0.12.21] - 2021-03-05
### Fixed
- Fixed path separators in generated `config.m4` file on Windows [#2153](https://github.com/zephir-lang/zephir/issues/2153)

## [0.12.20] - 2020-12-16
### Added
- Added supports void type return value for stubs
  [phalcon/ide-stubs#50](https://github.com/phalcon/ide-stubs/pull/50)
  [#1977](https://github.com/zephir-lang/zephir/issues/1977)

### Fixed
- Fixed missing kernel directory at build time [ice/framework#271](https://github.com/ice/framework/issues/271)
- Fixed stubs generation for case with array declaration with square brackets in params
- Fixed parameters positioning for `implode()` php function [#2120](https://github.com/zephir-lang/zephir/issues/2120)

## [0.12.19] - 2020-05-13
### Fixed
- Fixed duplicate definition with GCC 10
  [ice/framework#266](https://github.com/ice/framework/pull/266)
- Fixed initialization of object properties with default values when
  the object is an instance of a child class
  [#2089](https://github.com/zephir-lang/zephir/issues/2089)

### Changed
- Improved stubs generation for methods which may return object or null
  [#2092](https://github.com/zephir-lang/zephir/issues/2092)

## [0.12.18] - 2020-04-25
### Fixed
- In some cases for C "control characters" aren't properly escaped
  [#2065](https://github.com/zephir-lang/zephir/issues/2065)
- Zephir ignored property visibility and has not thrown error when setting
  private/protected properties in scope that shouldn't intend for it
  [#2078](https://github.com/zephir-lang/zephir/pull/2078),
  [phalcon/cphalcon#14810](https://github.com/phalcon/cphalcon/issues/14810),
  [phalcon/cphalcon#14766](https://github.com/phalcon/cphalcon/issues/14766)

## [0.12.17] - 2020-02-14
### Fixed
- On some platforms special alpha characters aren't correctly escaped.
  [#2058](https://github.com/zephir-lang/zephir/pull/2058)

### Changed
- Changed the internal DI environment mode when compile PHAR
  [#2049](https://github.com/zephir-lang/zephir/pull/2049)

## [0.12.16] - 2020-01-16
### Fixed
- Do not dump config file if config was changed.
  Usually we need dump configuration exactly once - at project initialization.
  There are no needs to dump it for every config change. Also, this patch
  removes `Config::$changed` variable that is no longer needed
  [#2035](https://github.com/zephir-lang/zephir/pull/2035)
- Use a different path for the Kernel cache if possible.
  This patch fixes a cache collision issue. The issue is after creating the
  cache and filling it with a project-specific configuration, there is no
  way to invalidate it. Any next project will use the same Kernel cache and
  the same Kernel configuration (if any).
  [#2036](https://github.com/zephir-lang/zephir/pull/2036)
- Fixed `-V` CLI flag purpose. Initially it was designed to disable verbose mode
  on the fly, e.g. to override project configuration for a single Zephir pass.
  This behavior was returned back.
- Fix increment array elements operation
  [#2020](https://github.com/zephir-lang/zephir/issues/2020)
- Fixed compound addition and subtraction assignment operators for static properties
  [#2038](https://github.com/zephir-lang/zephir/issues/2038)

### Changed
- Improved type hint for arrays when generating stubs
  [#2026](https://github.com/zephir-lang/zephir/issues/2026)

## [0.12.15] - 2019-12-12
### Removed
- Removed `uint` typedef usage

## [0.12.14] - 2019-12-11
### Removed
- Removed `zend_uint` typedef usage

## [0.12.13] - 2019-12-08
### Fixed
- Fixed PHP 7.4 support for macOS [phalcon/cphalcon#14577](https://github.com/phalcon/cphalcon/issues/14577)

### Removed
- Removed `uint` and `ulong` typedefs usage

## [0.12.12] - 2019-11-25
### Added
- Option to set banner for stubs generator
  [#1987](https://github.com/zephir-lang/zephir/1987)

### Fixed
- Calling object methods from static context yields segmentation fault when
  `internal-call-transformation` is set to `TRUE`
  [#2000](https://github.com/zephir-lang/zephir/issues/2000)
- Certain method calls fail when called from static context when
  `internal-call-transformation` is set to `TRUE`
  [#2005](https://github.com/zephir-lang/zephir/issues/2005)
- Method context loses track of `this` after calling static method when
  `internal-call-transformation` is set to `TRUE`
  [#2007](https://github.com/zephir-lang/zephir/issues/2007)
- Fixed incorrect stubs generation for return type hint
  [#1990](https://github.com/zephir-lang/zephir/issues/1990)
- Fixed incorrect stubs generation for classes in the same namespace
  [#2016](https://github.com/zephir-lang/zephir/issues/2016)

## [0.12.11] - 2019-11-02
### Fixed
- Fixed arithmetical operations with `zvals` which stores `double` numbers
- Fixed updating static variables in the loop which represents  `double` and
  `integer` data types [#1494](https://github.com/zephir-lang/zephir/issues/1494)
- Fixed casting char into another of a different type
  [#1988](https://github.com/zephir-lang/zephir/issues/1988)
- Fixed `internal` methods definition when `internal-call-transformation` is enabled
  [#1956](https://github.com/zephir-lang/zephir/issues/1956)
- Fixed aliases using in the `use` statement when generating stubs
  [#1986](https://github.com/zephir-lang/zephir/issues/1986)
- Fixed incorrect namespace on type hinted return when generating API docs
  [#1229](https://github.com/zephir-lang/zephir/issues/1229)

## [0.12.10] - 2019-10-19
### Fixed
- Fixed incorrect behavior in `zephir_get_global` if `zval` reference count <= 1
  [#1961](https://github.com/zephir-lang/zephir/issues/1961)

### Removed
- Removed `--vernum` option from the help for regular commands
- Removed `void` from the return type hint in the generated stubs
  [#1977](https://github.com/zephir-lang/zephir/issues/1977)
- Remove no longer supported `TSRMLS_CC` usage
  [#1865](https://github.com/zephir-lang/zephir/issues/1865)

### Changed
- Disabled PHP warnings for PHP >= 7.3.0 to be able correct work with lowest versions of dependencies
  [zendframework/zend-code#160](https://github.com/zendframework/zend-code/issues/160)
- Introduced support of multi line `@param` body for generated stubs
  [#1968](https://github.com/zephir-lang/zephir/issues/1968)

## [0.12.9] - 2019-10-14
### Added
- Added a single hyphen version of `dumpversion` option (just `-dumpversion`)
- Added `--vernum` option to print compiler version as integer

### Fixed
- Create local `.zephir` only when necessary
- Fixed IDE stubs generation [#1778](https://github.com/zephir-lang/zephir/issues/1778)
- Fixed segfault on cast `IS_UNDEF` to array
  [#1941](https://github.com/zephir-lang/zephir/issues/1941)
- Disables some regression changes introduced in the version `0.12.5`
  [#1941 (comment)](https://github.com/zephir-lang/zephir/issues/1941#issuecomment-538654340)
- Fixed memory leak on update array [#1937](https://github.com/zephir-lang/zephir/issues/1937)
- Fixed IDE stubs generation for classes that extends base classes
  [#1907](https://github.com/zephir-lang/zephir/issues/1907)
- Proper escape slashes in strings [#1495](https://github.com/zephir-lang/zephir/issues/1495)

### Changed
- Print warning during the code generation if the `timecop` extension was detected
  [#1950](https://github.com/zephir-lang/zephir/issues/1950)
- Improved error handling to not print PHP stack traces if `ZEPHIR_DEBUG` is not set

### Removed
- Removed no longer used `zephir_dtor` macro

## [0.12.8] - 2019-10-03
### Fixed
- Fixed `zephir_preg_match` to use `ZVAL_NULL` instead of `ZEPHIR_NULL`
  [#1946](https://github.com/zephir-lang/zephir/issues/1946)
- Fixed `Extension\InternalClassesTest` test to be able run full test suite
  without Phalcon [#1949](https://github.com/zephir-lang/zephir/issues/1949)

## [0.12.7] - 2019-10-03
### Fixed
- Fixed regression introduced in `0.12.5` for those users who doesn't use
  bundled `ext/pcre/php_pcre.h`
  [#1944](https://github.com/zephir-lang/zephir/issues/1944)
  [#1940](https://github.com/zephir-lang/zephir/issues/1940)
- Fixed sitemap API generator [#1940](https://github.com/zephir-lang/zephir/issues/1940)

## [0.12.6] - 2019-10-03
### Fixed
- Fixed regression introduced in `0.12.5` for those users who doesn't use
  bundled `ext/json/php_json.h` [#1940](https://github.com/zephir-lang/zephir/issues/1940)

## [0.12.5] - 2019-10-02
### Changed
- Update `zend_update_static_property` to be compatible with PHP >= 7.3
  [#1904](https://github.com/zephir-lang/zephir/issues/1904)
- Improved error handling

### Fixed
- Fixed IDE stubs generation to properly generate return type hint for `var | null`
  [#1922](https://github.com/zephir-lang/zephir/issues/1922)
- Fixed updating Super Globals [#1917](https://github.com/zephir-lang/zephir/issues/1917)
- Fixed casting variables to array [#1923](https://github.com/zephir-lang/zephir/issues/1923)
- Fixed work with constant which are not present
  [#1928](https://github.com/zephir-lang/zephir/issues/1928)
- Fixed access to Super Globals
  [#1934](https://github.com/zephir-lang/zephir/issues/1934),
  [phalcon/cphalcon#14426](https://github.com/phalcon/cphalcon/issues/14426)

## [0.12.4] - 2019-09-22
### Fixed
- Fixed install template

## [0.12.3] - 2019-09-22
### Fixed
- Fixed concatenation support of strings with `double` numbers
  [#1893](https://github.com/zephir-lang/zephir/issues/1893)
- Fixed 'void' return type hint being ignored
  [#1908](https://github.com/zephir-lang/zephir/issues/1908)
- Fixed updating array properties
  [#1915](https://github.com/zephir-lang/zephir/issues/1915)

## [0.12.2] - 2019-08-05
### Added
- Introduced initial ability to generate `zend_module_deps`
  [#1900](https://github.com/zephir-lang/zephir/pull/1900),
  [phalcon/cphalcon#13702](https://github.com/phalcon/cphalcon/issues/13702),
  [phalcon/cphalcon#13794](https://github.com/phalcon/cphalcon/pull/13794)

### Changed
- Write errors compiler to stderr if available

## [0.12.1] - 2019-07-30
### Added
- Added initial bash completion support (see `zephir-autocomplete` file)

### Changed
- Use local memory management
  [#1859](https://github.com/zephir-lang/zephir/pull/1859),
  [#1880](https://github.com/zephir-lang/zephir/pull/1880)
- Rephrase help strings for common compiler options

### Removed
- Remove HAVE_SPL usage
  [phalcon/cphalcon#14215](https://github.com/phalcon/cphalcon/pull/14215)
- Remove not used redundant command line options
- Cleaning up redundant CLI options

### Fixed
- Fixed segfault when auto-loading class with syntax error
  [#1885](https://github.com/zephir-lang/zephir/issues/1885)
- Optimize memory usage [#1882](https://github.com/zephir-lang/zephir/pull/1882)
- Fixed modifying array values in loops
  [#1879](https://github.com/zephir-lang/zephir/issues/1879)

## [0.12.0] - 2019-06-20
### Added
- Added initial support of "use" keyword in closures
  [#888](https://github.com/zephir-lang/zephir/issues/888),
  [#1848](https://github.com/zephir-lang/zephir/issues/1848)
  [#1860](https://github.com/zephir-lang/zephir/issues/1860)

### Removed
- PHP 5.x is no longer supported

### Changed
- The minimal Zephir Parser version is 1.3.0

### Fixed
- Fixed CLI runner for Windows
  [#1857](https://github.com/zephir-lang/zephir/pull/1857)
- Fixed segfault with fetching and opcache
  [#1855](https://github.com/zephir-lang/zephir/issues/1855)
- Extended classes can't access their private variables
  [#1851](https://github.com/zephir-lang/zephir/issues/1851)
- Incorrect usage of `zend_declare_class_constant_ex`
  [phalcon/cphalcon#14160](https://github.com/phalcon/cphalcon/issues/14160),
  https://bugs.php.net/bug.php?id=78121
- Incorrect implementation of ArrayAccess methods
  [#1871](https://github.com/zephir-lang/zephir/pull/1871)
- Fixed exception on call undefined method
  [#1863](https://github.com/zephir-lang/zephir/issues/1863)

## [0.11.12] - 2019-03-24
### Fixed
- Compilation error for instanceof [#1828](https://github.com/zephir-lang/zephir/issues/1828)
- Fixed `array_shift` behavior to mimicry PHP's "change by reference"
  [#1831](https://github.com/zephir-lang/zephir/issues/1831)
- Fixed reference counting while changing object's properties that are arrays
  [#1833](https://github.com/zephir-lang/zephir/pull/1833)

## [0.11.11] - 2019-02-26
### Fixed
- Objects are not traversable with `foreach`
  [#1818](https://github.com/zephir-lang/zephir/issues/1818)
  [#1595](https://github.com/zephir-lang/zephir/issues/1595)
- Recursion for array_push on PHP 7 [#1140](https://github.com/zephir-lang/zephir/issues/1140)
- Invalid array initialization [#1159](https://github.com/zephir-lang/zephir/issues/1159)

## [0.11.10] - 2019-02-23
### Changed
- Moved internal cache and logs to the user's home directory.
  - On macOS Zephir will use `XDG` if it is possible, otherwise `$HOME/Library`
  - On Windows Zephir will use `LOCALAPPDATA` if it is possible, otherwise home dir as a base path
  - In any other cases, e.g. Linux, BSD and so on, Zephir will use `XDG`if it is possible,
    otherwise `$HOME/.local` and `$HOME/.cache`
- Per project cache used for temporary operations was moved to `%CWD%/.zephir/%VERSION%`
  where `%CWD%` is the current working directory and `%VERSION%` is the current Zephir version e.g. `0.11.10-4a825b8`

### Fixed
- Array of object as return type is reported to PHP as type, not array
  [#1779](https://github.com/zephir-lang/zephir/issues/1779)
- Use namespace as a prefix for ini name [#1604](https://github.com/zephir-lang/zephir/issues/1604)
- Fixed calling anonymous functions by resolving context [#1751](https://github.com/zephir-lang/zephir/issues/1751)
- Fixed a bug when accessing super-global variables that do not exist (only for PHP >= 7.0 )
  [#1775](https://github.com/zephir-lang/zephir/issues/1775)
- Fixed incorrect behavior during work with ArrayAccess [#1061](https://github.com/zephir-lang/zephir/issues/1061),
  [#1400](https://github.com/zephir-lang/zephir/issues/1400)

## [0.11.9] - 2019-01-15
- Fixed `zend_closure` declaration to reflect PHP 7.3 changes

## [0.11.8] - 2018-12-01
### Fixed
- Fixed compilation error with inheritance of prototype interfaces
  [#1758](https://github.com/zephir-lang/zephir/issues/1758)
- Fixed compilation error when a new file is added or removed to the project
  [#1776](https://github.com/zephir-lang/zephir/issues/1776)

## [0.11.7] - 2018-11-27
### Changed
- The cache directory, formerly known as `.temp`, used for temporary operations was moved to
  the new `.zephir` directory. The algorithm for calculating cache path is as follows:
  `%CWD%/.zephir/%HASH%/cache/IR` where `%CWD%` is the current working directory and `%HASH%`
  means a hash calculated from the current Zephir version, environment and configuration
- The compiler's messages was divided into streams. Thus, now it is possible to redirect compiler's
  output as follows: `zephir generate 2> errors.log 1> /dev/null`
- Fixed type hints for scalar arguments for PHP < 7.2
  [#1658](https://github.com/zephir-lang/zephir/pull/1658)
- Coloring the compiler messages in the terminal is temporarily disabled

### Fixed
- Fixed incorrect behavior of `func_get_arg` and `func_get_args` functions for PHP 7.3

## [0.11.6] - 2018-11-19
### Fixed
- Fixed incorrect behavior of `require` statement for ZendEngine3
  [#1621](https://github.com/zephir-lang/zephir/issues/1621)
  [#1403](https://github.com/zephir-lang/zephir/issues/1403)
  [#1428](https://github.com/zephir-lang/zephir/pull/1428)

## [0.11.4] - 2018-11-18
### Added
- Introduced a brand new CLI interface
- The preferred method of installation is to use the Zephir PHAR
  which can be downloaded from the most recent Github Release
- Added `--no-dev` option to force building the extension in production mode
  [#1520](https://github.com/zephir-lang/zephir/issues/1520)
- Zephir development mode will be enabled silently if your PHP binary was compiled in
  a debug configuration [#1520](https://github.com/zephir-lang/zephir/issues/1520)
- Added missed CLI option `--export-classes` to flag whether classes must be exported.
  If export-classes is enabled all headers are copied to `include/php/ext`.

### Fixed
- Fixed regression introduced in the 0.10.12 related to `require` file using protocols
  [#1713](https://github.com/zephir-lang/zephir/issues/1713)

## [0.11.3] - 2018-11-13
### Changed
- Remove legacy installers and provide a common way to install Zephir
  [#1714](https://github.com/zephir-lang/zephir/issues/1714). Supported installation strategies are:
  - Install as a global application (using `composer global require`)
  - Install as a PHAR file. (this feature currently in the testing phase and not released officially)
  - Install as a Git clone (using `git clone` and `composer install` inside cloned project)
  - Install as a project's dependency (using `composer require`)

## [0.11.2] - 2018-11-11
### Added
- Introduced an ability to pack project into one `zephir.phar` file (for PHP 7.1 and later)

### Changed
- Composer now is a mandatory dependency
- Improved Zephir's Compiler error reporting

### Removed
- PHP 5.5 no longer supported

### Fixed
- Correct return types hint check

## [0.11.1] - 2018-10-19
### Added
- Initial support of PHP 7.3 (ported from the 0.10.x branch)

## [0.11.0] - 2018-08-05
### Added
- Add type hints for scalar arguments and return values in ZendEngine 3
  [1656](https://github.com/zephir-lang/zephir/issues/1656)

## [0.10.14] - 2018-11-20
### Fixed
- Fixed incorrect behavior of `func_get_arg` and `func_get_args` functions for PHP 7.3
  (backported from the upstream)

## [0.10.13] - 2018-11-18
### Fixed
- Fixed regression introduced in the 0.10.12 related to `require` file using protocols
  [#1713](https://github.com/zephir-lang/zephir/issues/1713) (backported from the upstream)
- Fixed incorrect behavior of `require` statement for ZendEngine3
  [#1621](https://github.com/zephir-lang/zephir/issues/1621)
  [#1403](https://github.com/zephir-lang/zephir/issues/1403)
  [#1428](https://github.com/zephir-lang/zephir/pull/1428)
  (backported from the upstream)

## [0.10.12] - 2018-10-19
### Added
- Initial support of PHP 7.3

## [0.10.11] - 2018-08-05
### Added
- Allow extension to be loaded prior to the tests

### Fixed
- Fixed [Copy-On-Write](https://en.wikipedia.org/wiki/Copy-on-write) violation for arrays zvals
- Fixed some testing settings
  [5deb64a](https://github.com/zephir-lang/zephir/commit/5deb64a8a1c7c18d45ce1a5a55667c499e2c284f)
- Fixed casting resource to int (only ZendEngine 3)
  [#1524](https://github.com/zephir-lang/zephir/issues/1524)

[Unreleased]: https://github.com/zephir-lang/zephir/compare/0.13.0...HEAD
[0.13.0]: https://github.com/zephir-lang/zephir/compare/0.12.21...0.13.0
[0.12.21]: https://github.com/zephir-lang/zephir/compare/0.12.20...0.12.21
[0.12.20]: https://github.com/zephir-lang/zephir/compare/0.12.19...0.12.20
[0.12.19]: https://github.com/zephir-lang/zephir/compare/0.12.18...0.12.19
[0.12.18]: https://github.com/zephir-lang/zephir/compare/0.12.17...0.12.18
[0.12.17]: https://github.com/zephir-lang/zephir/compare/0.12.16...0.12.17
[0.12.16]: https://github.com/zephir-lang/zephir/compare/0.12.15...0.12.16
[0.12.15]: https://github.com/zephir-lang/zephir/compare/0.12.14...0.12.15
[0.12.14]: https://github.com/zephir-lang/zephir/compare/0.12.13...0.12.14
[0.12.13]: https://github.com/zephir-lang/zephir/compare/0.12.12...0.12.13
[0.12.12]: https://github.com/zephir-lang/zephir/compare/0.12.11...0.12.12
[0.12.11]: https://github.com/zephir-lang/zephir/compare/0.12.10...0.12.11
[0.12.10]: https://github.com/zephir-lang/zephir/compare/0.12.9...0.12.10
[0.12.9]: https://github.com/zephir-lang/zephir/compare/0.12.8...0.12.9
[0.12.8]: https://github.com/zephir-lang/zephir/compare/0.12.7...0.12.8
[0.12.7]: https://github.com/zephir-lang/zephir/compare/0.12.6...0.12.7
[0.12.6]: https://github.com/zephir-lang/zephir/compare/0.12.5...0.12.6
[0.12.5]: https://github.com/zephir-lang/zephir/compare/0.12.4...0.12.5
[0.12.4]: https://github.com/zephir-lang/zephir/compare/0.12.3...0.12.4
[0.12.3]: https://github.com/zephir-lang/zephir/compare/0.12.2...0.12.3
[0.12.2]: https://github.com/zephir-lang/zephir/compare/0.12.1...0.12.2
[0.12.1]: https://github.com/zephir-lang/zephir/compare/0.12.0...0.12.1
[0.12.0]: https://github.com/zephir-lang/zephir/compare/0.11.12...0.12.0
[0.11.12]: https://github.com/zephir-lang/zephir/compare/0.11.11...0.11.12
[0.11.11]: https://github.com/zephir-lang/zephir/compare/0.11.10...0.11.11
[0.11.10]: https://github.com/zephir-lang/zephir/compare/0.11.9...0.11.10
[0.11.9]: https://github.com/zephir-lang/zephir/compare/0.11.8...0.11.9
[0.11.8]: https://github.com/zephir-lang/zephir/compare/0.11.7...0.11.8
[0.11.7]: https://github.com/zephir-lang/zephir/compare/0.11.6...0.11.7
[0.11.6]: https://github.com/zephir-lang/zephir/compare/0.11.4...0.11.6
[0.11.4]: https://github.com/zephir-lang/zephir/compare/0.11.3...0.11.4
[0.11.3]: https://github.com/zephir-lang/zephir/compare/0.11.2...0.11.3
[0.11.2]: https://github.com/zephir-lang/zephir/compare/0.11.1...0.11.2
[0.11.1]: https://github.com/zephir-lang/zephir/compare/0.11.0...0.11.1
[0.11.0]: https://github.com/zephir-lang/zephir/compare/0.10.14...0.11.0
[0.10.14]: https://github.com/zephir-lang/zephir/compare/0.10.13...0.10.4
[0.10.13]: https://github.com/zephir-lang/zephir/compare/0.10.12...0.10.13
[0.10.12]: https://github.com/zephir-lang/zephir/compare/0.10.11...0.10.12
[0.10.11]: https://github.com/zephir-lang/zephir/compare/0.10.10...0.10.11
