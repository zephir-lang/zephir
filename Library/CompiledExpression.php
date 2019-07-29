<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

/**
 * CompiledExpression.
 *
 * This represent a compiled expression, the object can be used to check
 * if the expression type is able to be used in certain types of the application
 */
class CompiledExpression implements TypeAwareInterface
{
    protected $type;

    protected $code;

    protected $originalExpr;

    /**
     * @param string $type
     * @param string $code
     * @param array  $originalExpr
     */
    public function __construct($type, $code, $originalExpr)
    {
        $this->type = $type;
        $this->code = $code;
        $this->originalExpr = $originalExpr;
    }

    /**
     * Returns the type of the compiled expression.
     *
     * @return string
     */
    public function getType()
    {
        return $this->type;
    }

    /**
     * Returns the code produced by the compiled expression.
     *
     * @return string
     */
    public function getCode()
    {
        return $this->code;
    }

    /**
     * Original AST code that produced the code.
     *
     * @return array
     */
    public function getOriginal()
    {
        return $this->originalExpr;
    }

    /**
     * Returns a C representation for a boolean constant.
     *
     * @return string
     */
    public function getBooleanCode()
    {
        if ($this->code && ('true' == $this->code || true === $this->code)) {
            return '1';
        } else {
            if ('false' == $this->code || false === $this->code) {
                return '0';
            }
        }

        return $this->code;
    }

    /**
     * Checks if the compiled expression is an integer or compatible type.
     *
     * @return bool
     */
    public function isIntCompatibleType()
    {
        switch ($this->type) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'char':
            case 'uchar':
                return true;
        }

        return false;
    }

    /**
     * Checks if the compiled expression is a char or compatible type.
     *
     * @return bool
     */
    public function isCharCompatibleType()
    {
        switch ($this->type) {
            case 'char':
            case 'uchar':
                return true;
        }

        return false;
    }

    /**
     * Resolves an expression
     * Some code cannot be directly pushed into the generated source
     * because it's missing some bound parts, this method resolves the missing parts
     * returning the generated code.
     *
     * @param string             $result
     * @param CompilationContext $compilationContext
     *
     * @return string
     */
    public function resolve($result, CompilationContext $compilationContext)
    {
        if ($this->code instanceof \Closure) {
            $code = $this->code;
            if (!$result) {
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext
                );
                $compilationContext->codePrinter->output($code($tempVariable->getName()));
                $tempVariable->setIsInitialized(true, $compilationContext);

                return $tempVariable->getName();
            }

            return $code($result);
        }

        return $this->code;
    }
}
