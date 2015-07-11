<?php
/**
 * Temporary file disabling tests not supported on PHP7
 */
$supportedZep = array('assign.zep');
$supportedTest = array('AssignTest.php');

function joinPath($path1, $path2) {
    $path1 = str_replace("\\", '/', $path1);
    $path2 = str_replace("\\", '/', $path2);
    return join('/', array(rtrim($path1, '/'), rtrim($path2, '/')));
}

$path = dirname(__FILE__);
$zepPath = joinPath($path, 'test');
$testPath = joinPath($path, 'unit-tests/Extension');

/* Move all tests away */
rename($zepPath, $zepPath . '2');
rename($testPath, $testPath . '2');
mkdir($zepPath);
mkdir($testPath);

/* Copy whitelisted tests */
foreach ($supportedZep as $zep) {
    rename(joinPath($zepPath . '2', $zep), joinPath($zepPath, $zep));
}

foreach ($supportedTest as $test) {
    rename(joinPath($testPath . '2', $test), joinPath($testPath, $test));
}
