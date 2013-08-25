<?php

$t = new Test\Properties\PublicProperties();

assert($t->someNull === null);
assert($t->someNullInitial === null);
assert($t->someFalse === false);
assert($t->someTrue === true);
assert($t->someInteger === 10);
assert($t->someDouble === 10.25);
assert($t->someString === "test");