<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

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
require 'Library/ThrowStatement.php';
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
require 'Library/Optimizers/EvalExpression.php';
require 'Library/Expression.php';
require 'Library/CompiledExpression.php';
require 'Library/CodePrinter.php';
require 'Library/ClassMethodParameters.php';
require 'Library/FunctionCall.php';
require 'Library/MethodCall.php';

Compiler::boot();

