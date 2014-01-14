<?php

$t = new Test\Pregmatch();

assert($t->testWithoutReturnAndMatches());

assert($t->testWithoutReturns() == array('def'));

assert($t->testWithoutMatches());


