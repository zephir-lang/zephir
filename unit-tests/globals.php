<?php

$t = new Test\Globals();

assert($t->getDefaultGlobals1() === true);
assert($t->getDefaultGlobals2() === 100);
assert($t->getDefaultGlobals3() === 7.5);
assert($t->getDefaultGlobals4() === true);
assert($t->getDefaultGlobals5() === 10);
assert($t->getDefaultGlobals6() === 15.2);
