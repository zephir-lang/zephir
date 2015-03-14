<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

namespace Zephir\Expression;

use Zephir\Types;
use Zephir\Variable;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\CompilerException;
use Zephir\Expression;
use Zephir\LiteralCompiledExpression;
use Zephir\Utils;

/**
 * Constants
 *
 * Resolves PHP or Zephir constants into C-Code
 */
class Constants
{
    /**
     * @var bool
     */
    protected $_expecting = true;

    /**
     * @var bool
     */
    protected $_readOnly = false;

    /**
     * @var Variable
     */
    protected $_expectingVariable;

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value
     *
     * @param boolean $expecting
     * @param Variable $expectingVariable
     */
    public function setExpectReturn($expecting, Variable $expectingVariable = null)
    {
        $this->_expecting = $expecting;
        $this->_expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only
     *
     * @param boolean $readOnly
     */
    public function setReadOnly($readOnly)
    {
        $this->_readOnly = $readOnly;
    }

    /**
     * Reserved ENV Constants
     * @link http://www.php.net/manual/ru/reserved.constants.php
     * @var array
     */
    protected $envConstans = array(
        'PHP_VERSION',
        'PHP_MAJOR_VERSION',
        'PHP_MINOR_VERSION',
        'PHP_RELEASE_VERSION',
        'PHP_VERSION_ID',
        'PHP_EXTRA_VERSION',
        'PHP_ZTS',
        'PHP_DEBUG',
        'PHP_MAXPATHLEN',
        'PHP_OS',
        'PHP_SAPI',
        'PHP_EOL',
        'PHP_INT_MAX',
        'PHP_INT_SIZE',
        'DEFAULT_INCLUDE_PATH',
        'PHP_BINDIR',
        'PHP_LIBDIR',
        'PHP_DATADIR',
    );

    /**
     * Magick constants
     * @link http://php.net/manual/en/language.constants.predefined.php
     * @var array
     */
    protected $magickConstants = array(
        '__LINE__',
        '__FILE__',
        '__DIR__',
        '__FUNCTION__',
        '__CLASS__',
        '__TRAIT__',
        '__METHOD__',
        '__NAMESPACE__'
    );

    protected $resources = array(
        'STDIN',
        'STDOUT',
        'STDERR'
    );

    /**
     * Resolves a PHP constant value into C-code
     *
     * @param array $expression
     * @param CompilationContext $compilationContext
     * @return CompiledExpression
     * @throws \Zephir\CompilerException
     */
    public function compile(array $expression, CompilationContext $compilationContext)
    {
        $isPhpConstant = false;
        $isZephirConstant = false;

        $constantName    = $expression['value'];

        $mergedConstants = array_merge($this->envConstans, $this->magickConstants, $this->resources);
        if (!defined($expression['value']) && !in_array($constantName, $mergedConstants)) {
            if (!$compilationContext->compiler->isConstant($constantName)) {
                $compilationContext->logger->warning("Constant '" . $constantName . "' does not exist at compile time", 'nonexistent-constant', $expression);
            } else {
                $isZephirConstant = true;
            }
        } else {
            if (strpos($constantName, 'VERSION') !== false) {
                $isPhpConstant = false;
            } else {
                $isPhpConstant = true;
            }
        }

        if ($isZephirConstant && !in_array($constantName, $this->resources)) {
            $constant = $compilationContext->compiler->getConstant($constantName);
            return new LiteralCompiledExpression($constant[0], $constant[1], $expression);
        }

        if ($isPhpConstant && !in_array($constantName, $mergedConstants)) {
            $constantName = constant($constantName);
            $type = strtolower(gettype($constantName));

            switch ($type) {

                case 'integer':
                    return new LiteralCompiledExpression('int', $constantName, $expression);

                case 'double':
                    return new LiteralCompiledExpression('double', $constantName, $expression);

                case 'string':
                    return new LiteralCompiledExpression('string', Utils::addSlashes($constantName, true, Types::STRING), $expression);

                case 'object':
                    throw new CompilerException('?');

                default:
                    return new LiteralCompiledExpression($type, $constantName, $expression);
            }
        }

        if (in_array($constantName, $this->magickConstants)) {
            switch ($constantName) {
                case '__CLASS__':
                    return new CompiledExpression(
                        'string',
                        $compilationContext->classDefinition->getCompleteName(),
                        $expression
                    );
                    //no break
                case '__NAMESPACE__':
                    return new CompiledExpression(
                        'string',
                        $compilationContext->classDefinition->getNamespace(),
                        $expression
                    );
                    //no break
                case '__METHOD__':
                    return new CompiledExpression(
                        'string',
                        $compilationContext->classDefinition->getName() . ':' . $compilationContext->currentMethod->getName(),
                        $expression
                    );
                    //no break
                case '__FUNCTION__':
                    return new CompiledExpression(
                        'string',
                        $compilationContext->currentMethod->getName(),
                        $expression
                    );
                    //no break
            }

            $compilationContext->logger->warning("Magic constant '" . $constantName . "' is not supported", 'not-supported-magic-constant', $expression);
            return new LiteralCompiledExpression('null', null, $expression);
        }

        if ($this->_expecting && $this->_expectingVariable) {
            $symbolVariable = $this->_expectingVariable;

            $symbolVariable->setLocalOnly(false);
            $symbolVariable->setMustInitNull(true);
            $symbolVariable->initVariant($compilationContext);
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext, $expression);
        }

        if (!$symbolVariable->isVariable()) {
            throw new CompilerException('Cannot use variable: ' . $symbolVariable->getType() . ' to assign property value', $expression);
        }

        $compilationContext->codePrinter->output('ZEPHIR_GET_CONSTANT(' . $symbolVariable->getName() . ', "' . $expression['value'] . '");');
        return new CompiledExpression('variable', $symbolVariable->getName(), $expression);
    }
}
