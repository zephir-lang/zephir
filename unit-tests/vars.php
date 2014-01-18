<?php

$t = new Test\Vars();

$t->testVarDump();

assert($t->testVarExport() == "'hello'");

$t->test88Issue('foo', 'bar');
$t->test88IssueParam2InitString('foo', 'bar');