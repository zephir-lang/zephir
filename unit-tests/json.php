<?php

$t = new Test\Json();

assert($t->testEncodeObject() == '{"a":"hello","b":"world","c":128}');

assert($t->testEncodeArray() == '[1,2,3]');
assert($t->testEncodeOptions() == '["\\u003Cfoo\\u003E","\'bar\'","&blong&","\\u00e9"]');

$obj1 = $t->testDecodeObject();
assert($obj1->a == 'hello');

$arr1 = $t->testDecodeArray();
assert($arr1[1] == 2);

$obj2 = $t->testDecodeObject2();
assert($obj2['a'] == 'hello');

