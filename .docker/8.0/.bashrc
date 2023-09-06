#!/bin/bash

alias test-ext='php -d extension=ext/modules/stub.so vendor/bin/phpunit --bootstrap tests/ext-bootstrap.php --testsuite Extension'
alias test-zephir='php vendor/bin/phpunit --colors=always --testsuite Zephir'
alias test-all='test-ext; test-zephir'
