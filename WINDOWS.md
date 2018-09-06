# Zephir Installation/Usage Guide (Windows)

This guide explains how to use zephir using a windows operating system.
Some parts are optional, when you have a specific PHP version.
Parts which are only necessary for a specific PHP version, are marked as such.
PHP-Version requirements are marked using `[ ]`

## Software Requirements [PHP 5.5 or later]

- [Install Visual Studio 2012 Express](http://www.microsoft.com/en-US/download/details.aspx?id=34673)
(You should start it and activate it)

## Software Requirements General

- [Install PHP (NTS)](http://windows.php.net/download/)
    - Download and extract it
    - Make sure it is in the PATH, as for example below:
    ```cmd
    setx path "%path%;c:\path-to-php\"
    ```
- [Install PHP SDK for PHP 5.6](http://windows.php.net/downloads/php-sdk/)(Currently `php-sdk-binary-tools-20110915.zip` is the newest)
- [Install PHP SDK for PHP 7.0+](https://github.com/OSTC/php-sdk-binary-tools) 
```cmd
setx php_sdk "c:\path-to-php-sdk"
```

- [Download PHP Developer Pack(NTS!)](http://windows.php.net/downloads/releases/)
(or build it yourself with `--enable-debug --disable-zts` and `nmake build-devel` or just `nmake snap` by using the PHP-SDK)
```cmd
setx php_devpack "c:\path-to-extracted-devpack"
```

## Installation of Zephir

- Clone/Download the repostiory and set the path as below
```cmd
setx path "%path%;c:\path-to-zephir\bin"
```

## Installation of Zephir Parser

1. Download [Zephir Parser for Windows](https://github.com/phalcon/php-zephir-parser/releases/latest)
2. Extract the DLL file and copy it to your PHP extensions directory
3. Edit your `php.ini` file and add this line:
   ```ini
   [Zephir Parser]
   ; You can use here absolute path to your custom modules dir
   ; or relative to the PHP modules dir
   extension=zephir_parser.dll
   ```

**NOTE**: Also you can [compile it yourself.](https://github.com/phalcon/php-zephir-parser/blob/master/README.WIN32-BUILD-SYSTEM)

## Usage of Zephir

- [**PHP7 or later**] Open the Visual Studio 2015 Command Prompt
(Find it by searching for cmd or just open `"%VS140COMNTOOLS%\VsDevCmd"`)
- [**PHP5.5 or later**] Open the Visual Studio 2012 Command Prompt
(Find it by searching for cmd or just open `"%VS110COMNTOOLS%\VsDevCmd"`)
- Execute `%PHP_SDK%\bin\phpsdk_setvars`
- `CD` to your extension and `zephir build`
- Take the built `.dll` from `your_ext/Release/php_extname.dll` (or `your_ext/Release_TS/php_extname.dll`)

## Additional Links

* Building PHP 5.5 - 7.1 under Windows: https://wiki.php.net/internals/windows/stepbystepbuild
* Building PHP 7.2+ under Windows: https://wiki.php.net/internals/windows/stepbystepbuild_sdk_2
