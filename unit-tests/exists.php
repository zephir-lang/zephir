<?php

class TestExClass {
}

interface TestExInterface {
}

function TestExFunction() {
}

$t = new Test\Exists();

assert($t->testClassExists("TestExClass") == true);
assert($t->testClassExists("TestExClassx") == false);
assert($t->testClassExists("TestExInterface") == false);
// with namespace
assert($t->testClassExists("\\Test\\Exists") == true);


assert($t->testInterfaceExists("TestExClass") == false); // class not interface
assert($t->testInterfaceExists("TestExInterface") == true);
assert($t->testInterfaceExists("TestExInterfacex") == false);

assert($t->testMethodExists($t, "testMethodExists") == true);

assert($t->testFileExists("./unit-tests/exists.php") == true);
assert($t->testFileExists("./unit-tests/existsxxxx.php") == false);
