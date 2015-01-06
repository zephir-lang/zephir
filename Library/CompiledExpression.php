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

namespace Zephir;

/**
 * CompiledExpression
 *
 * This represent a compiled expression, the object can be used to check
 * if the expression type is able to be used in certain types of the application
 */
class CompiledExpression
{
    protected $_type;

    protected $_code;

    protected $_originalExpr;

    /**
     *
     * @param string $type
     * @param string $code
     * @param array $originalExpr
     */
    public function __construct($type, $code, $originalExpr)
    {
        $this->_type = $type;
        $this->_code = $code;
        $this->_originalExpr = $originalExpr;
    }

    /**
     * Returns the type of the compiled expression
     *
     * @return string
     */
    public function getType()
    {
        return $this->_type;
    }

    /**
     * Returns the code produced by the compiled expression
     *
     * @return string
     */
    public function getCode()
    {
        return $this->_code;
    }

    /**
     * Original AST code that produced the code
     *
     * @return array
     */
    public function getOriginal()
    {
        return $this->_originalExpr;
    }

    /**
     * Returns a C representation for a boolean constant
     *
     * @return string
     */
    public function getBooleanCode()
    {
        if ($this->_code && ($this->_code == 'true' || $this->_code === true)) {
            return '1';
        } else {
            if ($this->_code == 'false' || $this->_code === false) {
                return '0';
            }
        }

        return $this->_code;
    }

    /**
     * Checks if the compiled expression is an integer or compatible type
     *
     * @return boolean
     */
    public function isIntCompatibleType()
    {
        switch ($this->_type) {
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
     * Checks if the compiled expression is a char or compatible type
     *
     * @return boolean
     */
    public function isCharCompatibleType()
    {
        switch ($this->_type) {
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
     * returning the generated code
     *
     * @param string $result
     * @param CompilationContext $compilationContext
     * @return string
     */
    public function resolve($result, CompilationContext $compilationContext)
    {
        if ($this->_code instanceof \Closure) {
            $code = $this->_code;
            if (!$result) {
                $tempVariable = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
                $compilationContext->codePrinter->output($code($tempVariable->getName()));
                $tempVariable->setIsInitialized(true, $compilationContext, array());
                return $tempVariable->getName();
            } else {
                return $code($result);
            }
        }
        return $this->_code;
    }
}
