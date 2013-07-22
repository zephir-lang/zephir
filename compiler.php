<?php

error_reporting(E_ALL);

require 'Library/CompilationContext.php';
require 'Library/HeadersManager.php';
require 'Library/ClassDefinition.php';
require 'Library/ClassProperty.php';
require 'Library/ClassMethod.php';
require 'Library/Compiler.php';
require 'Library/CompilerFile.php';
require 'Library/SymbolTable.php';
require 'Library/StatementsBlock.php';
require 'Library/EchoStatement.php';
require 'Library/LetStatement.php';
require 'Library/DeclareStatement.php';
require 'Library/ReturnStatement.php';
require 'Library/IfStatement.php';
require 'Library/LoopStatement.php';
require 'Library/WhileStatement.php';
require 'Library/BreakStatement.php';
require 'Library/ContinueStatement.php';
require 'Library/Variable.php';
require 'Library/ReadDetector.php';
require 'Library/Expression.php';
require 'Library/CompiledExpression.php';
require 'Library/CodePrinter.php';
require 'Library/ClassMethodParameters.php';

$c = new Compiler();

echo $c->compile();
