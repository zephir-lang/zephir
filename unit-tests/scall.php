<?php

$t = new Test\Scall();

assert($t->testMethod1() === "hello public");
assert($t::testMethod1() === "hello public");
assert(Test\Scall::testMethod1() === "hello public");
assert($t->testCall1() === "hello public");
assert($t->testCall2() === "hello protected");
