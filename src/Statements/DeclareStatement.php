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

namespace Zephir\Statements;

use ReflectionException;
use Zephir\CompilationContext;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression\Builder\BuilderFactory;

use function is_string;

/**
 * This creates variables in the current symbol table
 */
class DeclareStatement extends StatementAbstract
{
    /**
     * @throws ReflectionException
     * @throws Exception
     */
    public function compile(CompilationContext $compilationContext): void
    {
        if (!isset($this->statement['data-type'])) {
            throw new CompilerException('Data type is required', $this->statement);
        }

        $typeInference = $compilationContext->typeInference;
        $symbolTable   = $compilationContext->symbolTable;

        foreach ($this->statement['variables'] as $variable) {
            $varName = $variable['variable'];
            if ($symbolTable->hasVariableInBranch($varName, $compilationContext->branchManager->getCurrentBranch())) {
                throw new CompilerException("Variable '" . $varName . "' is already defined", $variable);
            }

            /**
             * Parameter symbols live in a synthetic EXTERNAL branch created
             * before the function body branch is opened, so the branch-scoped
             * check above misses them. Without this guard a `var x;` that
             * collides with a parameter named `x` silently overwrites the
             * parameter symbol (losing its isDoublePointer flag) and produces
             * uncompilable C — see https://github.com/zephir-lang/zephir/issues/2009.
             */
            if ($compilationContext?->currentMethod?->getParameters() !== null) {
                foreach ($compilationContext->currentMethod->getParameters()->getParameters() as $parameter) {
                    if ($parameter['name'] === $varName) {
                        throw new CompilerException(
                            "Variable '" . $varName . "' was already declared as a parameter of method '"
                            . $compilationContext->currentMethod->getDeclaredName() . "'",
                            $variable
                        );
                    }
                }
            }

            $currentType = $this->statement['data-type'];

            /**
             * Replace original data type by the pre-processed infered type.
             */
            if ($typeInference) {
                if ('variable' === $currentType) {
                    $type = $typeInference->getInferedType($varName);
                    if (is_string($type)) {
                        $currentType = $type;
                    }
                }
            }

            switch ($currentType) {
                case 'variable':
                case 'array-access':
                case 'property-access':
                case 'static-property-access':
                case 'fcall':
                case 'mcall':
                case 'scall':
                    $currentType = 'variable';
                    break;
            }

            /**
             * A local captured with `use (&x)` becomes a PHP reference shared
             * with the closure, so it has to be a zval however narrow its type
             * looks - whether that type was inferred or written out. `int n`
             * kept its `zend_long` shape and got the zval-only
             * zephir_make_local_reference() emitted against it.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2652
             * @see https://github.com/zephir-lang/zephir/issues/2653
             */
            if ($symbolTable->isClosureReference($varName)) {
                $currentType = 'variable';
            }

            /**
             * Variables are added to the symbol table.
             */
            $symbolVariable = $symbolTable->addVariable($currentType, $varName, $compilationContext);
            $varName        = $symbolVariable->getName();

            /**
             * Set the node where the variable is declared
             */
            $symbolVariable->setOriginal($variable);
            $symbolVariable->setIsInitialized(true, $compilationContext);

            if ('variable' === $currentType) {
                $symbolVariable->setMustInitNull(true);
                $symbolVariable->setLocalOnly(false);
            }

            if (isset($variable['expr'])) {
                $builder    = BuilderFactory::getInstance();
                $letBuilder = $builder->statements()->let([
                    $builder->operators()
                            ->assignVariable($varName, $builder->raw($variable['expr'])),
                ]);

                $letStatement = new LetStatement($letBuilder->build());
                $letStatement->compile($compilationContext);
            } else {
                /**
                 * The only place a user-written declaration with no value is
                 * recorded. The emitter starts such a local at IS_NULL when
                 * nothing ever writes to it, because PHP evaluates an unset
                 * variable as null while an IS_UNDEF zval reaches userland as
                 * `UNKNOWN:0`.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2654
                 */
                $symbolVariable->setDeclaredWithoutValue(true);
                $symbolVariable->enableDefaultAutoInitValue();
            }
        }
    }
}
