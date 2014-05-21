<?php
// unit-tests/Extension/fixtures/exit_string.php
$argv = $_SERVER['argv'];
if (isset($argv[1])) {
    $t = new \Test\ExitDie();
    $t->testExit($argv[1]);
}
