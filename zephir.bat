@echo off
REM This file is part of the Zephir.
REM
REM (c) Zephir Team <team@zephir-lang.com>
REM
REM For the full copyright and license information, please view the LICENSE
REM file that was distributed with this source code.
cls

if "%PHP_PEAR_PHP_BIN%" neq "" (
	set PHPBIN=%PHP_PEAR_PHP_BIN%
) else set PHPBIN=php

SET mypath=%~dp0
echo %mypath:~0,-1%
"%PHPBIN%" "%mypath%\zephir" %*
