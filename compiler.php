<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

error_reporting(E_ALL);

define('ZEPHIRPATH', __DIR__ . '/');

require ZEPHIRPATH . 'Library/ParseException.php';
require ZEPHIRPATH . 'Library/CompilerException.php';
require ZEPHIRPATH . 'Library/CompilationContext.php';
require ZEPHIRPATH . 'Library/HeadersManager.php';
require ZEPHIRPATH . 'Library/StringsManager.php';
require ZEPHIRPATH . 'Library/ClassDefinition.php';
require ZEPHIRPATH . 'Library/ClassProperty.php';
require ZEPHIRPATH . 'Library/ClassConstant.php';
require ZEPHIRPATH . 'Library/ClassMethod.php';
require ZEPHIRPATH . 'Library/Bootstrap.php';
require ZEPHIRPATH . 'Library/Compiler.php';
require ZEPHIRPATH . 'Library/CompilerFile.php';
require ZEPHIRPATH . 'Library/SymbolTable.php';
require ZEPHIRPATH . 'Library/Variable.php';
require ZEPHIRPATH . 'Library/GlobalConstant.php';
require ZEPHIRPATH . 'Library/Expression.php';
require ZEPHIRPATH . 'Library/CompiledExpression.php';
require ZEPHIRPATH . 'Library/CodePrinter.php';
require ZEPHIRPATH . 'Library/ClassMethodParameters.php';
require ZEPHIRPATH . 'Library/StatementsBlock.php';
require ZEPHIRPATH . 'Library/Detectors/ReadDetector.php';
require ZEPHIRPATH . 'Library/Detectors/WriteDetector.php';
require ZEPHIRPATH . 'Library/Statements/Abstract.php';
require ZEPHIRPATH . 'Library/Statements/ThrowStatement.php';
require ZEPHIRPATH . 'Library/Statements/EchoStatement.php';
require ZEPHIRPATH . 'Library/Statements/LetStatement.php';
require ZEPHIRPATH . 'Library/Statements/DeclareStatement.php';
require ZEPHIRPATH . 'Library/Statements/ReturnStatement.php';
require ZEPHIRPATH . 'Library/Statements/IfStatement.php';
require ZEPHIRPATH . 'Library/Statements/LoopStatement.php';
require ZEPHIRPATH . 'Library/Statements/DoWhileStatement.php';
require ZEPHIRPATH . 'Library/Statements/WhileStatement.php';
require ZEPHIRPATH . 'Library/Statements/SwitchStatement.php';
require ZEPHIRPATH . 'Library/Statements/ForStatement.php';
require ZEPHIRPATH . 'Library/Statements/BreakStatement.php';
require ZEPHIRPATH . 'Library/Statements/ContinueStatement.php';
require ZEPHIRPATH . 'Library/Statements/UnsetStatement.php';
require ZEPHIRPATH . 'Library/Optimizers/EvalExpression.php';
require ZEPHIRPATH . 'Library/Builder/Operators/BinaryOperatorBuilder.php';
require ZEPHIRPATH . 'Library/Builder/VariableBuilder.php';
require ZEPHIRPATH . 'Library/Call.php';
require ZEPHIRPATH . 'Library/FunctionCall.php';
require ZEPHIRPATH . 'Library/MethodCall.php';
require ZEPHIRPATH . 'Library/StaticCall.php';
require ZEPHIRPATH . 'Library/Passes/LocalContextPass.php';
require ZEPHIRPATH . 'Library/Passes/LoopBreakPass.php';
require ZEPHIRPATH . 'Library/Passes/SkipVariantInit.php';
require ZEPHIRPATH . 'Library/Passes/StaticTypeInference.php';
require ZEPHIRPATH . 'Library/Config.php';
require ZEPHIRPATH . 'Library/Logger.php';
require ZEPHIRPATH . 'Library/Color.php';
require ZEPHIRPATH . 'Library/Utils.php';

Bootstrap::boot();
