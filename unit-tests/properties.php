<?php

$t = new Test\Properties\PublicProperties();

assert($t->someNull === null);
assert($t->someNullInitial === null);
assert($t->someFalse === false);
assert($t->someTrue === true);
assert($t->someInteger === 10);
assert($t->someDouble === 10.25);
assert($t->someString === "test");

$t = new Test\Properties\ProtectedProperties();

assert($t->getSomeNull() === null);
assert($t->getSomeNullInitial() === null);
assert($t->getSomeFalse() === false);
assert($t->getSomeTrue() === true);
assert($t->getSomeInteger() === 10);
assert($t->getSomeDouble() === 10.25);
assert($t->getSomeString() === "test");

$t->setSomeVar(($rand = rand(1, 1000)*100));
assert($t->getSomeVar() === $rand);

$t = new Test\Properties\PrivateProperties();

assert($t->getSomeNull() === null);
assert($t->getSomeNullInitial() === null);
assert($t->getSomeFalse() === false);
assert($t->getSomeTrue() === true);
assert($t->getSomeInteger() === 10);
assert($t->getSomeDouble() === 10.25);
assert($t->getSomeString() === "test");

assert(Test\Properties\StaticPublicProperties::$someNull === null);
assert(Test\Properties\StaticPublicProperties::$someNullInitial === null);
assert(Test\Properties\StaticPublicProperties::$someFalse === false);
assert(Test\Properties\StaticPublicProperties::$someTrue === true);
assert(Test\Properties\StaticPublicProperties::$someInteger === 10);
assert(Test\Properties\StaticPublicProperties::$someDouble === 10.25);
assert(Test\Properties\StaticPublicProperties::$someString === "test");
