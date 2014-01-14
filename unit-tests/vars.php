<?php

$t = new Test\Vars();

$t->testVarDump();

assert($t->testVarExport() == "'hello'");



