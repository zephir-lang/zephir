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
require 'Library/Variable.php';
require 'Library/Expression.php';
require 'Library/CompiledExpression.php';
require 'Library/CodePrinter.php';
require 'Library/ClassMethodParameters.php';
require 'Library/StatementsBlock.php';
require 'Library/Detectors/ReadDetector.php';
require 'Library/Detectors/WriteDetector.php';
require 'Library/Statements/ThrowStatement.php';
require 'Library/Statements/EchoStatement.php';
require 'Library/Statements/LetStatement.php';
require 'Library/Statements/DeclareStatement.php';
require 'Library/Statements/ReturnStatement.php';
require 'Library/Statements/IfStatement.php';
require 'Library/Statements/LoopStatement.php';
require 'Library/Statements/WhileStatement.php';
require 'Library/Statements/ForStatement.php';
require 'Library/Statements/BreakStatement.php';
require 'Library/Statements/ContinueStatement.php';
require 'Library/Optimizers/EvalExpression.php';
require 'Library/Call.php';
require 'Library/FunctionCall.php';
require 'Library/MethodCall.php';
require 'Library/Passes/LocalContextPass.php';

Compiler::boot();

