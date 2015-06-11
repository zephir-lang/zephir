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

use Zephir\Builder\StatementsBlockBuilder;
use Zephir\Builder\Statements\LetStatementBuilder;
use Zephir\Statements\LetStatement;

/**
 * ClassProperty
 *
 * Represents a property class
 */
class ClassProperty
{
    /**
     * @var ClassDefinition
     */
    protected $classDefinition;

    protected $visibility;

    protected $name;

    protected $defaultValue;

    protected $docblock;

    protected $original;

    /**
     *
     * @param ClassDefinition $classDefinition
     * @param array $visibility
     * @param string $name
     * @param mixed $defaultValue
     * @param string $docBlock
     * @param array $original
     */
    public function __construct(ClassDefinition $classDefinition, $visibility, $name, $defaultValue, $docBlock, $original)
    {
        $this->checkVisibility($visibility, $name, $original);

        $this->classDefinition = $classDefinition;
        $this->visibility = $visibility;
        $this->name = $name;
        $this->defaultValue = $defaultValue;
        $this->docblock = $docBlock;
        $this->original = $original;

        if (!is_array($this->defaultValue)) {
            $this->defaultValue = array();
            $this->defaultValue['type'] = 'null';
            $this->defaultValue['value'] = null;
        }
    }

    /**
     * Returns the class definition where the method was declared
     *
     * @return ClassDefinition
     */
    public function getClassDefinition()
    {
        return $this->classDefinition;
    }

    /**
     * Returns the property name
     *
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * @return mixed
     */
    public function getValue()
    {
        if ($this->defaultValue['type'] == 'array') {
            $result = array();

            foreach ($this->original['default']['left'] as $key) {
                $result[] = $key['value']['value'];
            }

            $this->defaultValue['value'] = $result;
        }

        return $this->defaultValue['value'];
    }

    public function getType()
    {
        return $this->defaultValue['type'];
    }

    /**
     * @return mixed
     */
    public function getOriginal()
    {
        return $this->original;
    }

    /**
     * Checks for visibility congruence
     *
     * @param array $visibility
     * @param string $name
     * @param array $original
     */
    public function checkVisibility($visibility, $name, $original)
    {
        if (in_array('public', $visibility) && in_array('protected', $visibility)) {
            throw new CompilerException("Property '$name' cannot be 'public' and 'protected' at the same time", $original);
        }
        if (in_array('public', $visibility) && in_array('private', $visibility)) {
            throw new CompilerException("Property '$name' cannot be 'public' and 'private' at the same time", $original);
        }
        if (in_array('private', $visibility) && in_array('protected', $visibility)) {
            throw new CompilerException("Property '$name' cannot be 'protected' and 'private' at the same time", $original);
        }
    }

    /**
     * Returns the C-visibility accessors for the model
     *
     * @return string
     */
    public function getVisibilityAccesor()
    {
        $modifiers = array();

        foreach ($this->visibility as $visibility) {
            switch ($visibility) {

                case 'protected':
                    $modifiers['ZEND_ACC_PROTECTED'] = true;
                    break;

                case 'private':
                    $modifiers['ZEND_ACC_PRIVATE'] = true;
                    break;

                case 'public':
                    $modifiers['ZEND_ACC_PUBLIC'] = true;
                    break;

                case 'static':
                    $modifiers['ZEND_ACC_STATIC'] = true;
                    break;

                default:
                    throw new Exception("Unknown modifier " . $visibility);

            }
        }
        return join('|', array_keys($modifiers));
    }

    /**
     * Returns the docblock related to the property
     *
     * @return string
     */
    public function getDocBlock()
    {
        return $this->docblock;
    }

    /**
     * Checks whether the variable is static
     *
     * @return boolean
     */
    public function isStatic()
    {
        return in_array('static', $this->visibility);
    }

    /**
     * Checks whether the variable is public
     *
     * @return boolean
     */
    public function isPublic()
    {
        return in_array('public', $this->visibility);
    }

    /**
     * Checks whether the variable is protected
     *
     * @return boolean
     */
    public function isProtected()
    {
        return in_array('protected', $this->visibility);
    }

    /**
     * Checks whether the variable is private
     *
     * @return boolean
     */
    public function isPrivate()
    {
        return in_array('private', $this->visibility);
    }

    /**
     * Produce the code to register a property
     *
     * @param CompilationContext $compilationContext
     * @throws CompilerException
     */
    public function compile(CompilationContext $compilationContext)
    {
        switch ($this->defaultValue['type']) {
            case 'long':
            case 'int':
            case 'string':
            case 'double':
            case 'bool':
                $this->declareProperty($compilationContext, $this->defaultValue['type'], $this->defaultValue['value']);
                break;

            case 'array':
            case 'empty-array':
                $classDefinition = $this->classDefinition;
                if ($this->isStatic()) {
                    $methodName = 'zephir_init_static_properties';
                    $visibility = array('internal');
                } else {
                    $methodName = 'zephir_init_properties';
                    $visibility = array('internal');
                    
                    /* Make sure a constructor exists, so that properties are initialized */
                    $method = $classDefinition->getMethod('__construct');
                    if (!$method || $method->getClassDefinition() != $classDefinition) {
                        $classDefinition->setIsGeneratedConstructor(true);
                        $classDefinition->getEventsManager()->dispatch('setMethod', array(new ClassMethod(
                            $classDefinition,
                            array('public'),
                            '__construct',
                            null,
                            null
                        ), null));
                    }
                }

                $constructParentMethod = null;
                $constructMethod = $classDefinition->getMethod($methodName);
                /**
                 * Make sure we do not steal the construct method of the parent,
                 * but initialize our own with the inherited statements to ensure
                 * valid property initialization
                 */
                if ($constructMethod && $constructMethod->getClassDefinition() != $classDefinition) {
                    $constructParentMethod = $constructMethod;
                    $constructMethod = null;
                }
                if ($constructMethod) {
                    $statementsBlock = $constructMethod->getStatementsBlock();
                    if ($statementsBlock) {
                        $statements = $statementsBlock->getStatements();
                        $letStatement = $this->getLetStatement()->get();

                        $needLetStatementAdded = true;
                        foreach ($statements as $statement) {
                            if ($statement === $letStatement) {
                                $needLetStatementAdded = false;
                                break;
                            }
                        }

                        if ($needLetStatementAdded) {
                            $newStatements = array();

                            /**
                             * Start from let statement
                             */
                            $newStatements[] = $letStatement;

                            foreach ($statements as $statement) {
                                $newStatements[] = $statement;
                            }

                            $statementsBlock->setStatements($newStatements);
                            $constructMethod->setStatementsBlock($statementsBlock);
                            $classDefinition->getEventsManager()->dispatch('setMethod', array($constructMethod));
                        }
                    } else {
                        $statementsBlockBuilder = new StatementsBlockBuilder(array($this->getLetStatement()), false);
                        $constructMethod->setStatementsBlock(new StatementsBlock($statementsBlockBuilder->get()));
                        $classDefinition->getEventsManager()->dispatch('setMethod', array($constructMethod));
                    }
                } else {
                    $statements = array();
                    if ($constructParentMethod) {
                        $statements = $constructParentMethod->getStatementsBlock()->getStatements();
                    }
                    $statements[] = $this->getLetStatement()->get();
                    $statementsBlock = new StatementsBlock($statements);

                    $classDefinition->getEventsManager()->dispatch('setMethod', array(new ClassMethod(
                        $classDefinition,
                        $visibility,
                        $methodName,
                        null,
                        $statementsBlock
                    ), null));
                    return false;
                }
                //continue
            case 'null':
                $this->declareProperty($compilationContext, $this->defaultValue['type'], null);
                break;

            case 'static-constant-access':
                $expression = new Expression($this->defaultValue);
                $compiledExpression = $expression->compile($compilationContext);

                $this->declareProperty($compilationContext, $compiledExpression->getType(), $compiledExpression->getCode());
                break;

            default:
                throw new CompilerException('Unknown default type: ' . $this->defaultValue['type'], $this->original);
        }
    }

    /**
     * @return LetStatementBuilder
     */
    protected function getLetStatement()
    {
        if (!$this->isStatic()) {
            return new LetStatementBuilder(array(
                'assign-type' => 'object-property',
                'operator'    => 'assign',
                'variable'    => 'this',
                'property'    => $this->name,
                'file'        => $this->original['default']['file'],
                'line'        => $this->original['default']['line'],
                'char'        => $this->original['default']['char'],
            ), $this->original['default']);
        }
        return new LetStatementBuilder(array(
            'assign-type' => 'static-property',
            'operator'    => 'assign',
            'variable'    => '\\'.$this->classDefinition->getCompleteName(),
            'property'    => $this->name,
            'file'        => $this->original['default']['file'],
            'line'        => $this->original['default']['line'],
            'char'        => $this->original['default']['char'],
        ), $this->original['default']);
    }

    /**
     * @param $value
     * @return bool|string
     */
    protected function getBooleanCode($value)
    {
        if ($value && ($value == 'true' || $value === true)) {
            return '1';
        } else {
            if ($value == 'false' || $value === false) {
                return '0';
            }
        }

        return (boolean) $value;
    }

    /**
     * Declare class property with default value
     *
     * @param CompilationContext $compilationContext
     * @param $type
     * @param $value
     * @throws CompilerException
     */
    protected function declareProperty(CompilationContext $compilationContext, $type, $value)
    {
        $codePrinter = $compilationContext->codePrinter;

        if (is_object($value)) {
            //fix this
            return;
        }

        switch ($type) {

            case 'long':
            case 'int':
                $codePrinter->output("zend_declare_property_long(" . $compilationContext->classDefinition->getClassEntry() . ", SL(\"" . $this->getName() . "\"), " . $value . ", " . $this->getVisibilityAccesor() . " TSRMLS_CC);");
                break;

            case 'double':
                $codePrinter->output("zend_declare_property_double(" . $compilationContext->classDefinition->getClassEntry() . ", SL(\"" . $this->getName() . "\"), " . $value . ", " . $this->getVisibilityAccesor() . " TSRMLS_CC);");
                break;

            case 'bool':
                $codePrinter->output("zend_declare_property_bool(" . $compilationContext->classDefinition->getClassEntry() . ", SL(\"" . $this->getName() . "\"), ".$this->getBooleanCode($value).", " . $this->getVisibilityAccesor() . " TSRMLS_CC);");
                break;

            case Types::CHAR:
            case Types::STRING:
                $codePrinter->output("zend_declare_property_string(" . $compilationContext->classDefinition->getClassEntry() . ", SL(\"" . $this->getName() . "\"), \"" . Utils::addSlashes($value, true, $type) . "\", " . $this->getVisibilityAccesor() . " TSRMLS_CC);");
                break;

            case 'array':
            case 'empty-array':
            case 'null':
                $codePrinter->output("zend_declare_property_null(" . $compilationContext->classDefinition->getClassEntry() . ", SL(\"" . $this->getName() . "\"), " . $this->getVisibilityAccesor() . " TSRMLS_CC);");
                break;

            default:
                throw new CompilerException('Unknown default type: ' . $type, $this->original);
        }
    }
}
