<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

namespace Zephir\Operators\Other;

use Zephir\Operators\BaseOperator;
use Zephir\CompilationContext;
use Zephir\Expression;
use Zephir\CompilerException;
use Zephir\CompiledExpression;
use Zephir\MethodCall;
use Zephir\Utils;

/**
 * NewInstance
 *
 * Creates a new instance of a class
 */
class NewInstanceOperator extends BaseOperator
{
	/**
	 * Creates a new instance
	 *
	 * @param $expression
	 * @param CompilationContext $compilationContext
	 * @return CompiledExpression
	 * @throws CompilerException
	 */
	public function compile($expression, CompilationContext $compilationContext)
	{

		$codePrinter = $compilationContext->codePrinter;

		/**
		 * Resolves the symbol that expects the value
		 */
		$symbolVariable = $this->getExpected($compilationContext, $expression);

		if ($symbolVariable->getType() != 'variable') {
			throw new CompilerException("Objects can only be instantiated into dynamic variables", $expression);
		}

		if ($symbolVariable->hasDifferentDynamicType(array('unknown', 'undefined', 'object', 'null'))) {
			$compilationContext->logger->warning('Possible attempt to use non-object in "new" operator', 'non-valid-new', $expression);
		}

		/**
		 * Mark variables as dynamic objects
		 */
		$symbolVariable->setDynamicTypes('object');

		$dynamic = false;
		if ($expression['class'] == 'self') {
			$className = $compilationContext->classDefinition->getCompleteName();
		} else {
			if (substr($expression['class'], 0, 1) == '\\') {
				$className = substr($expression['class'], 1);
			} else {
				$className = $expression['class'];
				$dynamic = $expression['dynamic'];
			}
		}

		if (!$className) {
			throw new CompilerException("A class name is required to instantiate the object", $expression);
		}

		/**
		 * stdclass doesn't have constructors
		 */
		if (strtolower($className) == 'stdclass') {
			if (isset($expression['parameters']) && count($expression['parameters']) > 0) {
				throw new CompilerException("stdclass does not receive parameters in its constructor", $expression);
			}

			$codePrinter->output('object_init(' . $symbolVariable->getName() . ');');
			$symbolVariable->setClassTypes('stdclass');
		} else {
			$classDefinition = false;

			if ($compilationContext->compiler->isClass($className)) {
				$classDefinition = $compilationContext->compiler->getClassDefinition($className);
			} else {
				if ($expression['class'][0] != '\\') {
					$nsClassName = $compilationContext->classDefinition->getNamespace().'\\'.$className;
					if ($compilationContext->compiler->isClass($nsClassName)) {
						$classDefinition = $compilationContext->compiler->getClassDefinition($nsClassName);
					}
				}
			}

			/**
			 * Classes inside the same extension
			 */
			if ($classDefinition) {
				$codePrinter->output('object_init_ex(' . $symbolVariable->getName() . ', ' . $classDefinition->getClassEntry() . ');');
				$symbolVariable->setClassTypes($className);
			} else {
				/**
				 * Classes inside the same extension
				 */
				if (!class_exists($className, false)) {

					if ($compilationContext->symbolTable->hasVariable($className)) {

						$classNameVariable = $compilationContext->symbolTable->getVariableForRead($className, $compilationContext, $expression);
						if ($classNameVariable->isNotVariableAndString()) {
							throw new CompilerException("Only dynamic/string variables can be used in new operator", $expression);
						}

						unset($classNameVariable);
					} else {
						$compilationContext->logger->warning('Class "' . $className . '" does not exist at compile time ', "nonexistent-class", $expression);
					}
				}

				/**
				 * @TODO, check if the variable is really internal
				 */
				$zendClassEntry = $compilationContext->symbolTable->addTemp('zend_class_entry', $compilationContext);
				$classNameToFetch = $dynamic ? 'Z_STRVAL_P(' . $className . '), Z_STRLEN_P(' . $className . ')' : 'SL("' . Utils::addSlashes($className, true) . '")';
				$codePrinter->output($zendClassEntry->getName() . ' = zend_fetch_class(' . $classNameToFetch . ', ZEND_FETCH_CLASS_AUTO TSRMLS_CC);');
				$codePrinter->output('object_init_ex(' . $symbolVariable->getName() . ', ' . $zendClassEntry->getName() . ');');
				//$symbolVariable->setClassTypes($expression['class']);
			}
		}

		/**
		 * Mark variable initialized
		 */
		$symbolVariable->setIsInitialized(true, $compilationContext, $expression);

		/**
		 * Don't check the constructor for stdclass instances
		 */
		if (strtolower($className) == 'stdclass') {
			return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
		}

		/**
		 * Call the constructor
		 * For classes in the same extension we check if the class does implement a constructor
		 * For external classes we always assume the class does implement a constructor
		 */
		$callConstructor = false;
		if ($compilationContext->compiler->isClass($className)) {

			$classDefinition = $compilationContext->compiler->getClassDefinition($className);

			if ($classDefinition->getType() != 'class') {
				throw new CompilerException("Only classes can be instantiated", $expression);
			}

			$callConstructor = $classDefinition->hasMethod("__construct");

		} else {

			if ($compilationContext->compiler->isInternalClass($className)) {
				$classDefinition = $compilationContext->compiler->getInternalClassDefinition($className);
				$callConstructor = $classDefinition->hasMethod("__construct");
			}
		}

		/* @TODO use the MethodBuilder here */
		if (isset($expression['parameters'])) {
			$callExpr = new Expression(array(
				'variable'   => array('type' => 'variable', 'value' => $symbolVariable->getRealName()),
				'name'       => '__construct',
				'parameters' => $expression['parameters'],
				'call-type'  => MethodCall::CALL_NORMAL,
				'file'       => $expression['file'],
				'line'       => $expression['line'],
				'char'       => $expression['char'],
				'check'      => $callConstructor
			));
		} else {
			$callExpr = new Expression(array(
				'variable'  => array('type' => 'variable', 'value' => $symbolVariable->getRealName()),
				'name'      => '__construct',
				'call-type' => MethodCall::CALL_NORMAL,
				'file'      => $expression['file'],
				'line'      => $expression['line'],
				'char'      => $expression['char'],
				'check'     => $callConstructor
			));
		}

		/**
		 * If we are certain that there is a constructor we call it, otherwise we checked it at runtime.
		 */
		if ($callConstructor) {
			$methodCall = new MethodCall();
			$callExpr->setExpectReturn(false);
			$methodCall->compile($callExpr, $compilationContext);
		} else {

			$compilationContext->headersManager->add('kernel/fcall');

			$codePrinter->output('if (zephir_has_constructor(' . $symbolVariable->getName() . ' TSRMLS_CC)) {');
			$codePrinter->increaseLevel();

			$methodCall = new MethodCall();
			$callExpr->setExpectReturn(false);
			$methodCall->compile($callExpr, $compilationContext);

			$codePrinter->decreaseLevel();
			$codePrinter->output('}');
		}

		return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
	}

}
