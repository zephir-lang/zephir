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

namespace Zephir;

/**
 * ClassConstant
 *
 * Represents a class constant
 */
class ClassConstant
{
    protected $name;

    protected $value;

    protected $docblock;

    /**
     * ClassConstant constructor
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
     * Get value of constant
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
     * Produce the code to register a class constant
     *
     * @param CompilationContext $compilationContext
     */
    public function compile(CompilationContext $compilationContext)
    {

        switch ($this->value['type']) {

            case 'long':
            case 'int':
                $compilationContext->codePrinter->output("zend_declare_class_constant_long(" . $compilationContext->classDefinition->getClassEntry($compilationContext) . ", SL(\"" . $this->getName() . "\"), " . $this->value['value'] . " TSRMLS_CC);");
                break;

            case 'double':
                $compilationContext->codePrinter->output("zend_declare_class_constant_double(" . $compilationContext->classDefinition->getClassEntry($compilationContext) . ", SL(\"" . $this->getName() . "\"), " . $this->value['value'] . " TSRMLS_CC);");
                break;

            case 'bool':
                if ($this->value['value'] == 'false') {
                    $compilationContext->codePrinter->output("zend_declare_class_constant_bool(" . $compilationContext->classDefinition->getClassEntry($compilationContext) . ", SL(\"" . $this->getName() . "\"), 0 TSRMLS_CC);");
                } else {
                    $compilationContext->codePrinter->output("zend_declare_class_constant_bool(" . $compilationContext->classDefinition->getClassEntry($compilationContext) . ", SL(\"" . $this->getName() . "\"), 1 TSRMLS_CC);");
                }
                break;

            case 'string':
                $compilationContext->codePrinter->output("zend_declare_class_constant_string(" . $compilationContext->classDefinition->getClassEntry($compilationContext) . ", SL(\"" . $this->getName() . "\"), \"" . Utils::addSlashes($this->value['value']) . "\" TSRMLS_CC);");
                break;

            default:
                $compilationContext->codePrinter->output("zend_declare_class_constant_null(" . $compilationContext->classDefinition->getClassEntry($compilationContext) . ", SL(\"" . $this->getName() . "\") TSRMLS_CC);");
        }
    }
}
