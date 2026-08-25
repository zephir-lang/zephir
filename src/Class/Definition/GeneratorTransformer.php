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

namespace Zephir\Class\Definition;

use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\CompilationContext;
use Zephir\Exception\CompilerException;

/**
 * Splits every generator method (a method containing `yield`) into the pair
 * the runtime needs (issue #1849):
 *
 *   - the visible method becomes a "creator": it keeps its signature,
 *     arginfo and parameter handling, and its body is replaced by code that
 *     returns a <Ns>\Generator object seeded with the call arguments;
 *   - a new `internal`-visibility "step" method receives the original body
 *     and is compiled as a resumable state machine (see YieldStatement and
 *     Method::compile). Internal methods are plain C functions: invisible
 *     to reflection/stubs, and their function pointer is what the kernel
 *     generator object resumes.
 *
 * PHP's zend_generator only suspends op_array functions and \Generator is
 * final, which is why the split (not a native generator) is used.
 */
final class GeneratorTransformer
{
    /**
     * Name of the step method's only parameter: the generator object.
     * The prefix makes a collision with user variables practically
     * impossible; YieldStatement resolves it from the symbol table.
     */
    public const GEN_PARAM = 'zephir_gen';

    /**
     * Prefix of the hidden step method holding the original body.
     */
    public const STEP_PREFIX = 'zephir_gen_step_';

    public function transform(Definition $definition, CompilationContext $compilationContext): void
    {
        $generators = [];
        foreach ($definition->getMethods() as $method) {
            if (
                $method->isAbstract()
                || $method->isGeneratorStep()
                || $method->isGeneratorCreator()
                || !$method->isGenerator()
            ) {
                continue;
            }
            $generators[] = $method;
        }

        foreach ($generators as $method) {
            $this->split($definition, $method, $compilationContext);
        }
    }

    private function split(Definition $definition, Method $method, CompilationContext $compilationContext): void
    {
        $original = $method->getExpression();

        if ($method->isInternal()) {
            throw new CompilerException(
                "'yield' cannot be used in an 'internal' method: " . $method->getName(),
                $original
            );
        }
        if ($method->isConstructor() || '__destruct' === strtolower($method->getName())) {
            throw new CompilerException(
                'The "' . $method->getName() . '" method cannot be a generator',
                $original
            );
        }
        $this->checkReturnTypes($method, $compilationContext);

        $visibility = ['internal'];
        if ($method->isStatic()) {
            $visibility[] = 'static';
        }

        $genParameter = [
            'type'      => 'parameter',
            'name'      => self::GEN_PARAM,
            'const'     => 0,
            'data-type' => 'variable',
            'mandatory' => 0,
            'reference' => 0,
        ];

        $step = new Method(
            $definition,
            $visibility,
            self::STEP_PREFIX . $method->getName(),
            new Parameters([$genParameter]),
            $method->getStatementsBlock(),
            null,
            null,
            $method->getExpression()
        );

        $step->setDeclaredName($method->getName());
        $step->setGeneratorCreator($method);
        $method->setGeneratorStep($step);

        $definition->addMethod($step, $original);

        $compilationContext->compiler->markGeneratorsInUse(
            explode('\\', $definition->getCompleteName())[0]
        );
    }

    /**
     * A generator method returns a <Ns>\Generator object, so only object-ish
     * return type hints make sense. PHP's \Generator itself is unreachable
     * for extensions (final class), so it is rejected with a pointer to the
     * supported hints.
     */
    private function checkReturnTypes(Method $method, CompilationContext $compilationContext): void
    {
        if (!$method->hasReturnTypes()) {
            return;
        }

        $original = $method->getExpression();

        foreach (array_keys($method->getReturnTypes()) as $type) {
            if (!in_array($type, ['variable', 'mixed', 'object'], true)) {
                throw new CompilerException(
                    'Generator methods return a compiler-provided object implementing \Iterator; '
                    . "the return type hint '" . $type . "' on " . $method->getName()
                    . ' is not compatible. Use -> <\Iterator>, -> <\Traversable> or omit the hint.',
                    $original
                );
            }
        }

        foreach ($method->getReturnClassTypes() as $classType) {
            $resolved = ltrim((string)$classType, '\\');
            if (strcasecmp($resolved, 'Generator') === 0) {
                throw new CompilerException(
                    "PHP's \\Generator is final and cannot be produced by extensions; "
                    . 'declare -> <\Iterator>, -> <\Traversable> or omit the return type on '
                    . $method->getName(),
                    $original
                );
            }
        }
    }
}
