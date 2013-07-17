<?php

error_reporting(E_ALL);

require 'Library/ClassDefinition.php';
require 'Library/ClassProperty.php';
require 'Library/ClassMethod.php';
require 'Library/Compiler.php';
require 'Library/StatementsBlock.php';
require 'Library/EchoStatement.php';

$c = new Compiler();
$ir = $c->genIR();

echo $c->compile($ir);
