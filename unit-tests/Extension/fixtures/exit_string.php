<?php
// unit-tests/Extension/fixtures/exit_string.php
if (!extension_loaded('test') && ini_get('enable_dl') == '1') {
    $prefix = (PHP_SHLIB_SUFFIX === 'dll') ? 'php_' : '';
    dl($prefix . 'test.' . PHP_SHLIB_SUFFIX);
}
$argv = $_SERVER['argv'];
if (isset($argv[1])) {
    $t = new \Test\ExitDie();
    $t->testExit($argv[1]);
}
