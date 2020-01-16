@echo off
rem This file is part of the Zephir.
rem
rem (c) Phalcon Team <team@zephir-lang.com>
rem
rem For the full copyright and license information, please view
rem the LICENSE file that was distributed with this source code.
cls

if "%PHP_PEAR_PHP_BIN%" neq "" (
	set PHPBIN=%PHP_PEAR_PHP_BIN%
) else set PHPBIN=php

SET mypath=%~dp0
echo %mypath:~0,-1%
"%PHPBIN%" "%mypath%\zephir" %*
