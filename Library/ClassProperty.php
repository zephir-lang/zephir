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
use Zephir\Expression\Builder\Statements\LetStatement as ExpressionLetStatement;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\BinaryOperator;

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
     *
     * @throws CompilerException
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
     * @throws Exception
     */
    public function getVisibilityAccessor()
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

    private function initializeArray($compilationContext)
    {
        $classDefinition = $this->classDefinition;
        $parentClassDefinition = $classDefinition->getExtendsClassDefinition();

        if (!$this->isStatic()) {
            $constructParentMethod = $parentClassDefinition ? $parentClassDefinition->getInitMethod() : null;
            $constructMethod = $classDefinition->getInitMethod();
        } else {
            $constructParentMethod = $parentClassDefinition ? $parentClassDefinition->getStaticInitMethod() : null;
            $constructMethod = $classDefinition->getStaticInitMethod();
        }

        if ($constructMethod) {
            $statementsBlock = $constructMethod->getStatementsBlock();
            if ($statementsBlock) {
                $statements = $statementsBlock->getStatements();
                $letStatement = $this->getLetStatement()->build();

                $needLetStatementAdded = true;
                foreach ($statements as $statement) {
                    if ($statement === $letStatement) {
                        $needLetStatementAdded = false;
                        break;
                    }
                }

                $this->removeInitializationStatements($statements);
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
                    $classDefinition->updateMethod($constructMethod);
                }
            } else {
                $statementsBlockBuilder = BuilderFactory::getInstance()->statements()
                    ->block(array($this->getLetStatement()));
                $constructMethod->setStatementsBlock(new StatementsBlock($statementsBlockBuilder->build()));
                $classDefinition->updateMethod($constructMethod);
            }
        } else {
            $statements = array();
            if ($constructParentMethod) {
                $statements = $constructParentMethod->getStatementsBlock()->getStatements();
            }
            $this->removeInitializationStatements($statements);
            $statements[] = $this->getLetStatement()->build();
            $statementsBlock = new StatementsBlock($statements);

            if ($this->isStatic()) {
                $classDefinition->addStaticInitMethod($statementsBlock);
            } else {
                $classDefinition->addInitMethod($statementsBlock);
            }
        }
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
                $this->initializeArray($compilationContext);
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
     * Removes all initialization statements related to this property
     */
    protected function removeInitializationStatements(&$statements)
    {
        foreach ($statements as $index => $statement) {
            if (!$this->isStatic()) {
                if ($statement['expr']['left']['right']['value'] == $this->name) {
                    unset($statements[$index]);
                }
            } else {
                if ($statement['assignments'][0]['property'] == $this->name) {
                    unset($statements[$index]);
                }
            }
        }
    }

    /**
     * @return $this|ExpressionLetStatement
     */
    protected function getLetStatement()
    {
        $exprBuilder = BuilderFactory::getInstance();

        if ($this->isStatic()) {
            $className = '\\' . $this->classDefinition->getCompleteName();
            $expr      = $exprBuilder->raw($this->original['default']);
            return $exprBuilder->statements()->let(array(
                $exprBuilder->operators()
                    ->assignStaticProperty($className, $this->name, $expr)
                    ->setFile($this->original['default']['file'])
                    ->setLine($this->original['default']['line'])
                    ->setChar($this->original['default']['char'])
            ));
        }

        $lsb = $exprBuilder->statements()->let(array(
            $exprBuilder->operators()
                ->assignProperty('this', $this->name, $exprBuilder->raw($this->original['default']))
                ->setFile($this->original['default']['file'])
                ->setLine($this->original['default']['line'])
                ->setChar($this->original['default']['char'])
        ));

        return $exprBuilder->statements()->ifX()
            ->setCondition(
                $exprBuilder->operators()->binary(
                    BinaryOperator::OPERATOR_EQUALS,
                    $exprBuilder->operators()->binary(
                        BinaryOperator::OPERATOR_ACCESS_PROPERTY,
                        $exprBuilder->variable('this'),
                        $exprBuilder->literal(Types::STRING, $this->name)
                    ),
                    $exprBuilder->literal(Types::NULL_)
                )
            )
            ->setStatements($exprBuilder->statements()->block(array($lsb)));
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
     * @param string $type
     * @param $value
     * @throws CompilerException
     */
    protected function declareProperty(CompilationContext $compilationContext, $type, $value)
    {
        $codePrinter = $compilationContext->codePrinter;

        if (is_object($value)) {
            return;
        }

        $classEntry = $compilationContext->classDefinition->getClassEntry();

        switch ($type) {
            case 'long':
            case 'int':
                $codePrinter->output("zend_declare_property_long(" . $classEntry . ", SL(\"" . $this->getName() . "\"), " . $value . ", " . $this->getVisibilityAccessor() . " TSRMLS_CC);");
                break;

            case 'double':
                $codePrinter->output("zend_declare_property_double(" . $classEntry . ", SL(\"" . $this->getName() . "\"), " . $value . ", " . $this->getVisibilityAccessor() . " TSRMLS_CC);");
                break;

            case 'bool':
                $codePrinter->output("zend_declare_property_bool(" . $classEntry . ", SL(\"" . $this->getName() . "\"), ".$this->getBooleanCode($value).", " . $this->getVisibilityAccessor() . " TSRMLS_CC);");
                break;

            case Types::CHAR:
            case Types::STRING:
                $codePrinter->output("zend_declare_property_string(" . $classEntry . ", SL(\"" . $this->getName() . "\"), \"" . Utils::addSlashes($value, true, $type) . "\", " . $this->getVisibilityAccessor() . " TSRMLS_CC);");
                break;

            case 'array':
            case 'empty-array':
            case 'null':
                $codePrinter->output("zend_declare_property_null(" . $classEntry . ", SL(\"" . $this->getName() . "\"), " . $this->getVisibilityAccessor() . " TSRMLS_CC);");
                break;

            default:
                throw new CompilerException('Unknown default type: ' . $type, $this->original);
        }
    }
}
