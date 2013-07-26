<?php

error_reporting(E_ALL);

require 'Library/ParseException.php';
require 'Library/CompilerException.php';
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

try {
	$c = new Compiler();
	echo $c->compile();
} catch (Exception $e) {
	echo 'Exception: ', $e->getMessage(), PHP_EOL;
	if (method_exists($e, 'getExtra')) {
		$extra = $e->getExtra();
		if (is_array($extra)) {
			if (isset($extra['file'])) {
				echo PHP_EOL;
				$lines = file($extra['file']);
				$line = $lines[$extra['line'] - 1];
				echo "\t", str_replace("\t", " ", $line);
				echo "\t", str_repeat("-", $extra['char'] - 1), "^", PHP_EOL;
			}
		}
	}
	echo PHP_EOL;
	$pwd = getcwd();
	echo $e->getFile(), ' ', $e->getLine(), PHP_EOL;
	echo str_replace($pwd, '', $e->getTraceAsString()), PHP_EOL;
	exit(1);
}
