<?php

$r = new Test\Requires();

assert($r->requireExternal1(__DIR__ . "/php/require-me-1.php") == array(1, 2, 3));
assert($r->requireExternal1(__DIR__ . "/php/require-me-2.php") && defined('REQUIRE_ME'));