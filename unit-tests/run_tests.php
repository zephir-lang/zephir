<?php

function assert_failure( $script, $line, $message ) {
	echo 'Assertation failed '.$script.':'.$line."\n";
	exit(1);
}

assert_options(ASSERT_CALLBACK, 'assert_failure');

include_once __DIR__ . '/assign.php';
include_once __DIR__ . '/arithmetic.php';
include_once __DIR__ . '/nativearray.php';
include_once __DIR__ . '/flow.php';
include_once __DIR__ . '/oo.php';
include_once __DIR__ . '/fcall.php';
include_once __DIR__ . '/mcall.php';
include_once __DIR__ . '/mcallchained.php';
include_once __DIR__ . '/scall.php';
include_once __DIR__ . '/properties.php';
include_once __DIR__ . '/constants.php';
include_once __DIR__ . '/constantsinterface.php';
include_once __DIR__ . '/exceptions.php';
include_once __DIR__ . '/params.php';
include_once __DIR__ . '/emptytest.php';
include_once __DIR__ . '/regexdna.php';
include_once __DIR__ . '/fannkuch.php';
include_once __DIR__ . '/scallexternal.php';
include_once __DIR__ . '/cblock.php';
include_once __DIR__ . '/pregmatch.php';
include_once __DIR__ . '/json.php';
include_once __DIR__ . '/strings.php';
include_once __DIR__ . '/typeoff.php';
include_once __DIR__ . '/unset.php';
include_once __DIR__ . '/builtin-methods.php';
include_once __DIR__ . '/instanceof.php';
include_once __DIR__ . '/require.php';
include_once __DIR__ . '/globals.php';
include_once __DIR__ . '/ternary.php';
