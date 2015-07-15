<?php
/**
 * Temporary file disabling tests not supported on PHP7
 */
$supportedZep = array(
    'assign.zep', 'cblock.zep', 'declaretest.zep', 'echoes.zep', 'unknownclass.zep', 'returns.zep', 'references.zep',
    'properties', 'oo/extendpdoclass.zep', 'testinterface.zep', 'constants.zep', 'constantsparent.zep', 'pdostatement.zep', 'vars.zep',
    'oo/abstractclass.zep', 'oo/ooconstruct.zep', 'oo/ooconstructparams.zep', 'oo/param.zep', 'oo/deprecatedmethods.zep', 'oo/oonoconstruct.zep',
    'constantsinterfaceb.zep', 'constantsinterface.zep', 'constantsinterfacea.zep', 'bitwise.zep', 'branchprediction.zep', 'oo.zep', 'closures.zep',
    'diinterface.zep', 'extendedinterface.zep', 'arrayobject.zep', 'arithmetic.zep', 'oo/constantsinterface.zep',
    'oo/ooparams.zep', 'oo/oodynamica.zep', 'oo/oodynamicb.zep',
    'concat.zep', 'requires.zep', 'typeoff.zep', 'mcall.zep', 'mcallchained.zep', 'operator.zep', 'exists.zep', 'cast.zep',
    'ooimpl', 'oo/extend'
);
$supportedTest = array(
    'AssignTest.php', 'CblockTest.php', 'ArrayObjectTest.php', 'ArithmeticTest.php', 'VarsTest.php',
    'ConstantsTest.php', 'ConstantsInterfaceTest.php', 'BitwiseTest.php', 'BranchPredictionTest.php', 'ConcatTest.php', 'RequiresTest.php',
    'TypeoffTest.php', 'MCallTest.php', 'McallChainedTest.php', 'OoTest.php', 'OperatorTest.php', 'ExistsTest.php', 'CastTest.php', 'ExtendedInterfaceTest.php',
    'Properties', 'php', 'Oo'
);

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
    $target = joinPath($zepPath, $zep);
    @mkdir(dirname($target), 0777, true);
    rename(joinPath($zepPath . '2', $zep), $target);
}

foreach ($supportedTest as $test) {
    $target = joinPath($testPath, $test);
    @mkdir(dirname($target), 0777, true);
    rename(joinPath($testPath . '2', $test), $target);
}
