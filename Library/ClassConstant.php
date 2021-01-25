<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Zephir\Exception\CompilerException;
use Zephir\Expression\Constants;
use Zephir\Expression\StaticConstantAccess;

/**
 * ClassConstant.
 *
 * Represents a class constant
 */
class ClassConstant
{
    /**
     * @var string
     */
    protected $name;

    /**
     * @var array
     */
    protected $value = [];

    /**
     * @var string
     */
    protected $docblock;

    /**
     * ClassConstant constructor.
     *
     * @param string $name
     * @param array  $value
     * @param string $docBlock
     */
    public function __construct($name, array $value, $docBlock)
    {
        $this->name = $name;
        $this->value = $value;
        $this->docblock = $docBlock;
    }

    /**
     * Returns the constant's name.
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * Returns the constant's value.
     *
     * TODO: Rewrite name
     *
     * @return array
     */
    public function getValue()
    {
        return $this->value;
    }

    /**
     * Get the type of the value of the constant.
     *
     * @return string
     */
    public function getValueType()
    {
        return $this->value['type'];
    }

    /**
     * Get value of the value of the constant.
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
     * Returns the docblock related to the constant.
     *
     * @return string
     */
    public function getDocBlock()
    {
        return $this->docblock;
    }

    /**
     * Get type of class constant.
     *
     * @return string
     */
    public function getType()
    {
        return $this->value['type'];
    }

    /**
     * Process the value of the class constant if needed.
     *
     * @param CompilationContext $compilationContext
     *
     * @throws Exception
     */
    public function processValue(CompilationContext $compilationContext)
    {
        if ('constant' == $this->value['type']) {
            $constant = new Constants();
            $compiledExpression = $constant->compile($this->value, $compilationContext);

            $this->value = [
                'type' => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode(),
            ];

            return;
        }

        if ('static-constant-access' == $this->value['type']) {
            $staticConstantAccess = new StaticConstantAccess();
            $compiledExpression = $staticConstantAccess->compile($this->value, $compilationContext);

            $this->value = [
                'type' => $compiledExpression->getType(),
                'value' => $compiledExpression->getCode(),
            ];

            return;
        }
    }

    /**
     * Produce the code to register a class constant.
     *
     * @param CompilationContext $compilationContext
     *
     * @throws CompilerException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext)
    {
        $this->processValue($compilationContext);

        $constanValue = isset($this->value['value']) ? $this->value['value'] : null;

        $compilationContext->backend->declareConstant(
            $this->value['type'],
            $this->getName(),
            $constanValue,
            $compilationContext
        );
    }
}
