<?php
// unit-tests/Extension/fixtures/exit.php
if (!extension_loaded('test') && ini_get('enable_dl') == '1') {
    $prefix = (PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '';
    dl($prefix . 'test.' . PHP_SHLIB_SUFFIX);
}
$t = new \Test\ExitDie();
$t->testExit();
