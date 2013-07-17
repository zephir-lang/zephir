<?php

error_reporting(E_ALL);

define('EXPR_IDENTIFIER', 307);
define('EXPR_STRING', 303);

define('VAR_TYPE_INT', 309);
define('VAR_TYPE_DOUBLE', 310);

require 'Library/ClassDefinition.php';
require 'Library/ClassProperty.php';
require 'Library/ClassMethod.php';
require 'Library/Compiler.php';
require 'Library/SymbolTable.php';
require 'Library/StatementsBlock.php';
require 'Library/EchoStatement.php';
require 'Library/DeclareStatement.php';
require 'Library/Variable.php';
require 'Library/CodePrinter.php';

$c = new Compiler();
$ir = $c->genIR();

echo $c->compile($ir);
