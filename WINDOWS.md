Zephir Installation/Usage Guide (Windows)
===================
This guide explains how to use zephir using a windows operating system.  
Some parts are optional, when you have a specific PHP version.  
Parts which are only necessary for a specific PHP version, are marked as such.  
PHP-Version requirements are marked using ``[]``

Software Requirements [PHP 5.5 or later]
-----------------------
- [Install Visual Studio 2012 Express](http://www.microsoft.com/en-US/download/details.aspx?id=34673)
(You should start it and activate it)

Software Requirements [below PHP 5.5]
-----------------------
- [Install Windows SDK 6.1](http://www.microsoft.com/en-us/download/details.aspx?id=24826)   
WARNING: This usually takes very long to install and is very big
- [Install Visual Studio 2008 Express (after SDK 6.1!)](http://go.microsoft.com/fwlink/?LinkId=104679)  
Install C++ Express Edition, (You should start and activate it)

Software Requirements General
-----------------------

- [Install PHP (NTS)](http://windows.php.net/download/) 
    - Download and extract it 
    - Make sure it is in the PATH, as for example below:
    ```
    setx path "%path%;c:\path-to-php\"
    ```
- [Install PHP SDK](http://windows.php.net/downloads/php-sdk/)   
(Currently "php-sdk-binary-tools-20110915.zip" is the newest)
```
setx php_sdk "c:\path-to-php-sdk"
```

- [Download PHP Developer Pack(NTS!)](http://windows.php.net/downloads/releases/)  
(or build it yourself with ``--enable-debug --disable-zts`` by using the PHP-SDK)
```
setx php_devpack "c:\path-to-extracted-devpack"
```

Installation of Zephir
----------------------
- Clone/Download the repostiory and set the path as below  
(TODO: Parser Building instructions)
```
setx path "%path%;c:\path-to-zephir\bin"
```

Usage of Zephir
----------------
- [**PHP5.5 or later**] Open the Visual Studio 2012 Command Prompt  
(Find it by searching for cmd or just open ``"%VS110COMNTOOLS%\VsDevCmd"``)
- [**below PHP5.5**] Open the Visual Studio 2008 Command Prompt  
(Find it by search for cmd or just open ``"%VS90COMNTOOLS%\vsvars32"``)
- Execute ``%PHP_SDK%\bin\phpsdk_setvars``
- ``CD`` to your extension and ``zephir build``
- Take the built ``.dll`` from ``your_ext/Release/php_extname.dll``

Additional Links
------------------
Building PHP under Windows: https://wiki.php.net/internals/windows/stepbystepbuild