<?php

$t = new Test\Scall();

assert($t->testMethod1() === "hello");
assert($t::testMethod1() === "hello");
assert(Test\Scall::testMethod1() === "hello");
assert($t->testCall1() === "hello");
