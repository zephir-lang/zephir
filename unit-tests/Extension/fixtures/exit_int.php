<?php
// unit-tests/Extension/fixtures/exit_int.php
$argv = $_SERVER['argv'];
if (isset($argv[1])) {
    $t = new \Test\ExitDie();
    $v = intval($argv[1]);
    $t->testExit($v);
}
