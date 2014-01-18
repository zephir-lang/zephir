<?php

$t = new Test\Vars();

$t->testVarDump();

assert($t->testVarExport() == "'hello'");

$t->test88Issue('foo', 'bar');
$t->test88IssueParam2InitString('foo', 'bar');

$t->testVarDump2param(3.1, true);
$t->testVarDump3param(3.1, true, array(1,2,3));
$t->testVarDump2param(3.1, true) ;
