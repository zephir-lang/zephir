<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Class;

use Zephir\Class\Definition\Definition;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Expression\Builder\BuilderFactory;
use Zephir\Expression\Builder\Operators\BinaryOperator;
use Zephir\StatementsBlock;
use Zephir\Types\Types;

use function Zephir\add_slashes;

/**
 * Represents a property class
 */
class Property
{
    public function __construct(
        protected Definition $classDefinition,
        protected array $visibility,
        protected string $name,
        protected ?array $defaultValue,
        protected ?string $docBlock = null,
        protected ?array $original = null,
    ) {
        $this->checkVisibility($visibility, $name, $original);

        if (!\is_array($this->defaultValue)) {
            $this->defaultValue = [];
            $this->defaultValue['type'] = 'null';
            $this->defaultValue['value'] = null;
        }
    }

    /**
     * Returns the class definition where the method was declared.
     */
    public function getClassDefinition(): Definition
    {
        return $this->classDefinition;
    }

    /**
     * Returns the property name.
     */
    public function getName(): string
    {
        return $this->name;
    }

    public function getValue(): mixed
    {
        if ('array' == $this->defaultValue['type']) {
            $result = [];

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

    public function getOriginal(): ?array
    {
        return $this->original;
    }

    /**
     * Checks for visibility congruence.
     */
    public function checkVisibility(array $visibility, string $name, array $original = null): void
    {
        if (\in_array('public', $visibility) && \in_array('protected', $visibility)) {
            throw new CompilerException("Property '$name' cannot be 'public' and 'protected' at the same time", $original);
        }
        if (\in_array('public', $visibility) && \in_array('private', $visibility)) {
            throw new CompilerException("Property '$name' cannot be 'public' and 'private' at the same time", $original);
        }
        if (\in_array('private', $visibility) && \in_array('protected', $visibility)) {
            throw new CompilerException("Property '$name' cannot be 'protected' and 'private' at the same time", $original);
        }
    }

    /**
     * Returns the C-visibility accessors for the model.
     *
     * @throws Exception
     */
    public function getVisibilityAccessor(): string
    {
        $modifiers = [];

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
                    throw new Exception('Unknown modifier '.$visibility);
            }
        }

        return implode('|', array_keys($modifiers));
    }

    /**
     * Returns the docblock related to the property.
     */
    public function getDocBlock(): ?string
    {
        return $this->docBlock;
    }

    /**
     * Checks whether the variable is static.
     */
    public function isStatic(): bool
    {
        return \in_array('static', $this->visibility);
    }

    /**
     * Checks whether the variable is public.
     */
    public function isPublic(): bool
    {
        return \in_array('public', $this->visibility);
    }

    /**
     * Checks whether the variable is protected.
     */
    public function isProtected(): bool
    {
        return \in_array('protected', $this->visibility);
    }

    /**
     * Checks whether the variable is private.
     */
    public function isPrivate(): bool
    {
        return \in_array('private', $this->visibility);
    }

    /**
     * Produce the code to register a property.
     *
     * @throws Exception
     * @throws \ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
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
                $this->initializeArray();
                // no break
            case 'null':
                $this->declareProperty($compilationContext, $this->defaultValue['type'], null);
                break;

            case 'static-constant-access':
                $expression = new Expression($this->defaultValue);
                $compiledExpression = $expression->compile($compilationContext);

                $this->declareProperty($compilationContext, $compiledExpression->getType(), $compiledExpression->getCode());
                break;

            default:
                throw new CompilerException('Unknown default type: '.$this->defaultValue['type'], $this->original);
        }
    }

    /**
     * Removes all initialization statements related to this property.
     */
    protected function removeInitializationStatements(array &$statements): void
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

    protected function getLetStatement()
    {
        $exprBuilder = BuilderFactory::getInstance();

        if ($this->isStatic()) {
            $className = '\\'.$this->classDefinition->getCompleteName();
            $expr = $exprBuilder->raw($this->original['default']);

            return $exprBuilder->statements()->let([
                $exprBuilder->operators()
                    ->assignStaticProperty($className, $this->name, $expr)
                    ->setFile($this->original['default']['file'])
                    ->setLine($this->original['default']['line'])
                    ->setChar($this->original['default']['char']),
            ]);
        }

        $lsb = $exprBuilder->statements()->let([
            $exprBuilder->operators()
                ->assignProperty('this', $this->name, $exprBuilder->raw($this->original['default']))
                ->setFile($this->original['default']['file'])
                ->setLine($this->original['default']['line'])
                ->setChar($this->original['default']['char']),
        ]);

        return $exprBuilder->statements()->ifX()
            ->setCondition(
                $exprBuilder->operators()->binary(
                    BinaryOperator::OPERATOR_EQUALS,
                    $exprBuilder->operators()->binary(
                        BinaryOperator::OPERATOR_ACCESS_PROPERTY,
                        $exprBuilder->variable('this'),
                        $exprBuilder->literal(Types::T_STRING, $this->name)
                    ),
                    $exprBuilder->literal(Types::T_NULL)
                )
            )
            ->setStatements($exprBuilder->statements()->block([$lsb]));
    }

    protected function getBooleanCode($value): bool | string
    {
        if ('true' == $value || true === $value) {
            return '1';
        }

        if ('false' == $value || false === $value) {
            return '0';
        }

        return (bool) $value;
    }

    /**
     * Declare class property with default value.
     *
     * @param CompilationContext $compilationContext
     * @param string             $type
     * @param                    $value
     *
     * @throws Exception
     * @throws CompilerException
     */
    protected function declareProperty(CompilationContext $compilationContext, $type, $value): void
    {
        $codePrinter = $compilationContext->codePrinter;

        if (\is_object($value)) {
            return;
        }

        $classEntry = $compilationContext->classDefinition->getClassEntry();

        switch ($type) {
            case Types::T_INT:
            case Types::T_LONG:
                $codePrinter->output(
                    sprintf(
                        'zend_declare_property_long(%s, SL("%s"), %s, %s);',
                        $classEntry,
                        $this->getName(),
                        $value,
                        $this->getVisibilityAccessor()
                    ),
                    false
                );
                break;

            case Types::T_DOUBLE:
                $codePrinter->output(
                    sprintf(
                        'zend_declare_property_double(%s, SL("%s"), %s, %s);',
                        $classEntry,
                        $this->getName(),
                        $value,
                        $this->getVisibilityAccessor()
                    ),
                    false
                );
                break;

            case Types::T_BOOL:
                $codePrinter->output(
                    sprintf(
                        'zend_declare_property_bool(%s, SL("%s"), %s, %s);',
                        $classEntry,
                        $this->getName(),
                        $this->getBooleanCode($value),
                        $this->getVisibilityAccessor()
                    ),
                    false
                );
                break;

            case Types::T_CHAR:
            case Types::T_STRING:
                $codePrinter->output(
                    sprintf(
                        'zend_declare_property_string(%s, SL("%s"), "%s", %s);',
                        $classEntry,
                        $this->getName(),
                        add_slashes($value),
                        $this->getVisibilityAccessor()
                    ),
                    false
                );
                break;

            case 'array':
            case 'empty-array':
            case 'null':
                $codePrinter->output(
                    sprintf(
                        'zend_declare_property_null(%s, SL("%s"), %s);',
                        $classEntry,
                        $this->getName(),
                        $this->getVisibilityAccessor()
                    ),
                    false
                );
                break;

            default:
                throw new CompilerException('Unknown default type: '.$type, $this->original);
        }
    }

    private function initializeArray(): void
    {
        $classDefinition = $this->classDefinition;
        $parentClassDefinition = $classDefinition->getExtendsClassDefinition();

        if (!$this->isStatic()) {
            $constructParentMethod = $parentClassDefinition?->getInitMethod();
            $constructMethod = $classDefinition->getInitMethod();
        } else {
            $constructParentMethod = $parentClassDefinition?->getStaticInitMethod();
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
                    $newStatements = [$letStatement];
                    foreach ($statements as $statement) {
                        $newStatements[] = $statement;
                    }

                    $statementsBlock->setStatements($newStatements);
                    $constructMethod->setStatementsBlock($statementsBlock);
                    $classDefinition->updateMethod($constructMethod);
                }
            } else {
                $statementsBlockBuilder = BuilderFactory::getInstance()->statements()
                    ->block([$this->getLetStatement()]);
                $constructMethod->setStatementsBlock(new StatementsBlock($statementsBlockBuilder->build()));
                $classDefinition->updateMethod($constructMethod);
            }
        } else {
            $statements = [];
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
}
