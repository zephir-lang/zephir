<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir;

use Zephir\Compiler\CompilerException;
use Zephir\Expression\Constants;

/**
 * ClassConstant
 *
 * Represents a class constant
 */
class ClassConstant
{
    protected $name;

    /**
     * @var array
     */
    protected $value;

    protected $docblock;

    /**
     * ClassConstant constructor
     *
     * @param $name
     * @param $value
     * @param $docBlock
     */
    public function __construct($name, $value, $docBlock)
    {
        $this->name = $name;
        $this->value = $value;
        $this->docblock = $docBlock;
    }

    /**
     * Returns the constant's name
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * Returns the constant's value
     *
     * @todo Rewrite name
     *
     * @return array
     */
    public function getValue()
    {
        return $this->value;
    }

    /**
     * Get the type of the value of the constant
     *
     * @return string
     */
    public function getValueType()
    {
        return $this->value['type'];
    }

    /**
     * Get value of the value of the constant
     *
     * @return mixed
     */
    public function getValueValue()
    {
        if (isset($this->value['value'])) {
            return $this->value['value'];
        }

        return false;
    }

    /**
     * Returns the docblock related to the constant
     *
     * @return string
     */
    public function getDocBlock()
    {
        return $this->docblock;
    }

    /**
     * Get type of class constant
     *
     * @return string
     */
    public function getType()
    {
        return $this->value['type'];
    }

    /**
     * Process the value of the class constant if needed
     *
     * @param compilationContext $compilationContext
     */
    public function processValue($compilationContext)
    {
        if ($this->value['type'] == 'constant') {
            $constant = new Constants();
            $compiledExpression = $constant->compile($this->value, $compilationContext);

            $this->value = array(
                'type' => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode()
            );
            return;
        }

        if ($this->value['type'] == 'static-constant-access') {
            $expression = new Expression($this->value);
            $compiledExpression = $expression->compile($compilationContext);

            $this->value = array(
                'type' => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode()
            );
            return;
        }
    }

    /**
     * Produce the code to register a class constant
     *
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext)
    {
        $this->processValue($compilationContext);

        $compilationContext->backend->declareConstant($this->value['type'], $this->getName(), isset($this->value['value']) ? $this->value['value'] : null, $compilationContext);
    }
}
