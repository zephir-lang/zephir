# Change Log
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](http://keepachangelog.com/)
and this project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]

## [0.11.5] - 2018-11-19
### Fixed
- Fixed incorrect behavior of `require` statement for ZendEngine3
  [#1621](https://github.com/phalcon/zephir/issues/1621)
  [#1403](https://github.com/phalcon/zephir/issues/1403)
  [#1428](https://github.com/phalcon/zephir/pull/1428)

## [0.11.4] - 2018-11-18
### Added
- Introduced a brand new CLI interface
- The preferred method of installation is to use the Zephir PHAR
  which can be downloaded from the most recent Github Release
- Added `--no-dev` option to force building the extension in production mode
  [#1520](https://github.com/phalcon/zephir/issues/1520)
- Zephir development mode will be enabled silently if your PHP binary was compiled in
  a debug configuration [#1520](https://github.com/phalcon/zephir/issues/1520)
- Added missed CLI option `--export-classes` to flag whether classes must be exported.
  If export-classes is enabled all headers are copied to `include/php/ext`.

### Fixed
- Fixed regression introduced in the 0.10.12 related to `require` file using protocols
  [#1713](https://github.com/phalcon/zephir/issues/1713)

[Unreleased]: https://github.com/phalcon/zephir/compare/0.11.5...HEAD
[0.11.5]: https://github.com/phalcon/zephir/compare/0.11.4...0.11.5
[0.11.4]: https://github.com/phalcon/zephir/compare/0.11.3...0.11.4
