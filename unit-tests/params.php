<?php

$t = new Test\Oo\OoParams();

assert($t->setAge(17) === 17);
assert($t->setAge("17") === 17);
assert($t->setAge(17.0) === 17);

assert($t->setAverage(17.1) === 17.1);
assert($t->setAverage(17) === 17.0);
assert($t->setAverage("17.5") === 17.5);

assert($t->setName("peter") === "peter");
assert($t->setName(12.5) === "12.5");
assert($t->setName(17) === "17");

assert($t->setEnabled(true) === true);
assert($t->setEnabled(false) === false);
assert($t->setEnabled(1) === true);
assert($t->setEnabled(0) === false);
assert($t->setEnabled("1") === true);
assert($t->setEnabled("0") === false);

assert($t->setStrictAge(17) === 17);
try { $t->setStrictAge(17.0); assert(false); } catch (InvalidArgumentException $e) { assert(true); }
try { $t->setStrictAge("17"); assert(false); } catch (InvalidArgumentException $e) { assert(true); }

assert($t->setStrictAverage(17.1) === 17.1);
try { $t->setStrictAverage(17); assert(false); } catch (InvalidArgumentException $e) { assert(true); }
try { $t->setStrictAverage("17"); assert(false); } catch (InvalidArgumentException $e) { assert(true); }

assert($t->setStrictName("peter") === "peter");
try { $t->setStrictName(1234); assert(false); } catch (InvalidArgumentException $e) { assert(true); }
