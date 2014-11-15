Zephir Installation/Usage Guide (Windows)
===================

Software Requirements
-----------------------

- [Install Visual Studio 2012 Express](http://www.microsoft.com/en-US/download/details.aspx?id=34673)
(You should start it and activate it)
- [Install PHP (NTS)](http://windows.php.net/download/) 
    - Download and extract it 
    - Make sure it is in the PATH, as for example below:
    ```
    setx path "%path%;c:\path-to-php\"
    ```
- [Install PHP SDK](http://windows.php.net/downloads/php-sdk/) (Currently "php-sdk-binary-tools-20110915.zip" is the newest)
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
- Clone/Download the repostiory and set the path as below (TODO: Parser Building instructions)
```
setx path "%path%;c:\path-to-zephir\bin"
```

Usage of Zephir
----------------
- Open the Visual Studio 2012 Command Prompt 
(Find it by searching for cmd or just open ``%VS110COMNTOOLS%\VsDevCmd``)
- ``CD`` to your extension and ``zephir build```
- Take the built ``.dll`` from ``your_ext/Release/php_extname.dll``

Additional Links
------------------
Building PHP under Windows: https://wiki.php.net/internals/windows/stepbystepbuild