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

namespace Zephir\Expression;

use ReflectionException;
use Zephir\Class\Constant;
use Zephir\Class\Definition\AbstractDefinition;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Entry as ClassEntry;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Variable\Variable;

use function gettype;
use function in_array;
use function sprintf;

/**
 * Resolves class constants
 */
class StaticConstantAccess
{
    protected bool      $expecting         = true;
    protected ?Variable $expectingVariable = null;
    protected bool      $readOnly          = false;
    protected bool      $alwaysFold        = false;

    /**
     * Access a static constant class.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(array $expression, CompilationContext $compilationContext): CompiledExpression
    {
        $className = $expression['left']['value'];
        $constant  = $expression['right']['value'];

        if (!in_array($className, ['this', 'self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
        }

        /**
         * A declaration is a compile-time constant expression, which has no
         * called class. PHP's own compile errors. See #2825.
         */
        if ($this->alwaysFold && 'static' === $className) {
            throw new CompilerException(
                'class' === $constant
                    ? 'static::class cannot be used for compile-time class name resolution'
                    : '"static::" is not allowed in compile-time constants',
                $expression
            );
        }

        if ('class' === $constant) {
            return $this->compileClassName($className, $expression, $compilationContext);
        }

        if (
            in_array($className, ['self', 'static', 'parent'], true)
            && null === $compilationContext->classScope($className, $expression)
        ) {
            return new CompiledExpression('null', null, $expression);
        }

        $classDefinition = $this->resolveClassDefinition($className, $expression, $compilationContext);

        /**
         * A version-shaped engine constant is emitted as its C macro, before
         * the existence check: the PHP running Zephir may not have it at all
         * (`Attribute::TARGET_CONSTANT` before 8.5). See #2738.
         */
        $native = $this->nativeOf($className, $classDefinition, $constant, $compilationContext);
        if (null !== $native) {
            return new CompiledExpression('int', $native['c'], $expression);
        }

        /**
         * Constants are resolved to their values at compile time,
         * so we need to check that they effectively do exist
         */
        if (!$classDefinition->hasConstant($constant)) {
            throw new CompilerException(
                sprintf(
                    "Class '%s' does not have a constant called: '%s'",
                    $classDefinition->getCompleteName(),
                    $constant
                ),
                $expression
            );
        }

        /**
         * We can optimize the reading of constants by avoiding query their value every time.
         *
         * `static::` names the called class, which only exists at run time, so
         * it is never folded: folding gave a subclass's override the parent's
         * value. A declaration default (class constant, property, parameter)
         * is always folded, since it is emitted where no run time read can go.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2711
         */
        $folding = $compilationContext->config->get('static-constant-class-folding', 'optimizations');
        if (!$this->alwaysFold && ('static' === $className || !$folding)) {
            return $this->compileRuntimeRead($className, $classDefinition, $constant, $expression, $compilationContext);
        }

        $constantDefinition = $classDefinition->getConstant($constant);

        /**
         * Array constants cannot be folded to a scalar literal; compile the
         * constant's array value as a fresh array expression at the use site.
         */
        if (
            $constantDefinition instanceof Constant &&
            in_array($constantDefinition->getValueType(), ['array', 'empty-array'], true)
        ) {
            $expression = new Expression($constantDefinition->getValue());
            $expression->setExpectReturn($this->expecting, $this->expectingVariable);
            $expression->setReadOnly($this->readOnly);

            return $expression->compile($compilationContext);
        }

        if ($constantDefinition instanceof Constant) {
            $constantDefinition->processValue($compilationContext);
            $value = $constantDefinition->getValueValue();
            $type  = $constantDefinition->getValueType();
        } else {
            $value = $constantDefinition;
            $type  = gettype($value);
            if ('integer' === $type) {
                $type = 'int';
            }
        }

        /**
         * A single-quoted constant is a `char` or an `istring` to Zephir and a
         * string to PHP. See #2825.
         */
        if ('char' === $type || 'istring' === $type) {
            $type = 'string';
        }

        switch ($type) {
            case 'string':
            case 'int':
            case 'double':
            case 'float':
            case 'bool':
            case 'null':
                break;
            default:
                $compilationContext->logger->warning(
                    "Constant '" . $constantDefinition->getName() . "' does not exist at compile time",
                    ['nonexistent-constant', $expression]
                );

                return new CompiledExpression('null', null, $expression);
        }

        return new CompiledExpression($type, $value, $expression);
    }

    /**
     * The `::class` postfix is not a real constant but the class name PHP
     * resolves at compile time: an explicit name as written (through `use`
     * aliases, never loading the class), `self` the declared name, `parent`
     * the `extends` name as written, and `static` the called class, which only
     * exists at run time.
     *
     * The name goes out escaped with Entry::escape(), not Name::addSlashes(),
     * which would keep a segment such as `\nested` as a C escape.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2527
     * @see https://github.com/zephir-lang/zephir/issues/2825
     */
    private function compileClassName(
        string $className,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        if (
            in_array($className, ['self', 'static', 'parent'], true)
            && null === $compilationContext->classScope($className, $expression, true)
        ) {
            return new CompiledExpression('null', null, $expression);
        }

        if ('static' === $className) {
            if ($this->expecting && $this->expectingVariable) {
                $symbolVariable = $this->expectingVariable;
                $symbolVariable->initVariant($compilationContext);
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    $expression
                );
            }

            $symbolVariable->setDynamicTypes('string');
            $compilationContext->headersManager->add('kernel/object');
            $compilationContext->codePrinter->output(
                'zephir_get_called_class(' . $compilationContext->backend->getVariableCode($symbolVariable) . ');'
            );

            return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
        }

        $name = match ($className) {
            'self', 'this' => $compilationContext->lexicalClassDefinition()->getCompleteName(),
            'parent'       => $compilationContext->lexicalClassDefinition()->getExtendsClass()
                ?: throw new CompilerException(
                    'Cannot use "parent" when current class scope has no parent',
                    $expression
                ),
            default        => $className,
        };

        return new CompiledExpression('string', ClassEntry::escape(ltrim($name, '\\')), $expression);
    }

    /**
     * The array a Zephir class constant holds, when $expression reads one,
     * for a declaration to take in place: a declaration has no symbol table
     * to build it in at run time. Null for anything else.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function arrayConstantValue(array $expression, CompilationContext $compilationContext): ?array
    {
        $className = $expression['left']['value'];
        $constant  = $expression['right']['value'];

        if ('class' === $constant || 'static' === $className) {
            return null;
        }

        if (!in_array($className, ['this', 'self', 'parent'])) {
            $className = $compilationContext->getFullName($className);
        }

        if (!$this->isResolvable($className, $compilationContext)) {
            return null;
        }

        $classDefinition = $this->resolveClassDefinition($className, $expression, $compilationContext);
        if (!$classDefinition->hasConstant($constant)) {
            return null;
        }

        $constantDefinition = $classDefinition->getConstant($constant);
        if (!$constantDefinition instanceof Constant) {
            return null;
        }

        $constantDefinition->processValue($compilationContext);
        $value = $constantDefinition->getValue();

        return in_array($value['type'], ['array', 'empty-array'], true) ? $value : null;
    }

    /**
     * The C and fully-qualified PHP spellings of a constant whose value depends
     * on the PHP version that compiles the C, or null for any other constant.
     *
     * That is an `Attribute::*` flag, or a Zephir class constant initialized
     * from one, so `self::FLAGS` keeps the macro of what it was declared as.
     *
     * @return array{c: string, qualified: string}|null
     *
     * @throws Exception
     * @throws ReflectionException
     *
     * @see https://github.com/zephir-lang/zephir/issues/2738
     */
    public function resolveNative(array $expression, CompilationContext $compilationContext): ?array
    {
        $className = $expression['left']['value'];
        $constant  = $expression['right']['value'];

        if ('class' === $constant) {
            return null;
        }

        if (!in_array($className, ['this', 'self', 'static', 'parent'])) {
            $className = $compilationContext->getFullName($className);
        }

        /**
         * A class this lookup cannot see is simply not native. The leaf may
         * sit in a branch the evaluator never reaches, and compile() reports
         * it wherever it is actually read.
         */
        if (!$this->isResolvable($className, $compilationContext)) {
            return null;
        }

        return $this->nativeOf(
            $className,
            $this->resolveClassDefinition($className, $expression, $compilationContext),
            $constant,
            $compilationContext
        );
    }

    /**
     * @return array{c: string, qualified: string}|null
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function nativeOf(
        string $className,
        AbstractDefinition $classDefinition,
        string $constant,
        CompilationContext $compilationContext
    ): ?array {
        $macro = AttributeConstants::macro($className, $constant);
        if (null !== $macro) {
            return ['c' => $macro, 'qualified' => '\\Attribute::' . $constant];
        }

        if (!$classDefinition->hasConstant($constant)) {
            return null;
        }

        $constantDefinition = $classDefinition->getConstant($constant);
        if (!$constantDefinition instanceof Constant) {
            return null;
        }

        $constantDefinition->processValue($compilationContext);
        $value = $constantDefinition->getValue();

        if (!isset($value['native'])) {
            return null;
        }

        return [
            'c'         => '(' . $value['value'] . ')',
            'qualified' => '\\' . $classDefinition->getCompleteName() . '::' . $constant,
        ];
    }

    /**
     * Whether resolveClassDefinition() would find a definition for $className.
     */
    private function isResolvable(string $className, CompilationContext $compilationContext): bool
    {
        $compiler = $compilationContext->compiler;

        if (in_array($className, ['self', 'static', 'this'])) {
            return null !== $compilationContext->lexicalClassDefinition();
        }

        if ('parent' === $className) {
            return $compilationContext->lexicalClassDefinition()?->getExtendsClassDefinition() instanceof Definition;
        }

        return $compiler->isClass($className)
            || $compiler->isInterface($className)
            || $compiler->isBundledClass($className)
            || $compiler->isBundledInterface($className);
    }

    /**
     * Fetches the class definition according to the class where the constant
     * is supposed to be declared.
     *
     * @param string $className resolved name, or one of this/self/static/parent
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function resolveClassDefinition(
        string $className,
        array $expression,
        CompilationContext $compilationContext
    ): AbstractDefinition {
        $compiler = $compilationContext->compiler;

        if (in_array($className, ['self', 'static', 'this'])) {
            return $compilationContext->lexicalClassDefinition();
        }

        if ('parent' === $className) {
            return $compilationContext->parentClassDefinition(
                sprintf('find constant called "%s"', $expression['right']['value']),
                $expression
            );
        }

        if ($compiler->isClass($className) || $compiler->isInterface($className)) {
            return $compiler->getClassDefinition($className);
        }

        if ($compiler->isBundledClass($className) || $compiler->isBundledInterface($className)) {
            return $compiler->getInternalClassDefinition($className);
        }

        throw new CompilerException("Cannot locate class '" . $className . "'", $expression['left']);
    }

    /**
     * Folds the constant even when `static-constant-class-folding` is off.
     *
     * For a declaration default (class constant, property, parameter), which
     * has to be a literal: PHP evaluates it as a constant expression, so the
     * folded value is the one PHP gives.
     */
    public function setAlwaysFold(bool $alwaysFold): void
    {
        $this->alwaysFold = $alwaysFold;
    }

    /**
     * Reads the constant when the code runs, for `static::` and when folding
     * is off.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    private function compileRuntimeRead(
        string $className,
        AbstractDefinition $classDefinition,
        string $constant,
        array $expression,
        CompilationContext $compilationContext
    ): CompiledExpression {
        /**
         * The value is a zval, so a typed destination (`string s`) receives it
         * through a temporary and the assignment converts it.
         */
        if ($this->expecting && $this->expectingVariable?->isVariable()) {
            $symbolVariable = $this->expectingVariable;
            $symbolVariable->initVariant($compilationContext);
        } else {
            $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                'variable',
                $compilationContext,
                $expression
            );
        }

        /**
         * A PHP class (`\PDO`, or a Zephir class's internal parent) has no
         * Zephir `*_ce` symbol, so it is looked up the way `new` and
         * `instanceof` look it up.
         */
        $classEntry = match (true) {
            'static' === $className => $compilationContext->lateStaticClassEntry(),
            $classDefinition instanceof Definition && !$classDefinition->isBundled()
                => $classDefinition->getClassEntry($compilationContext),
            default => (new ClassEntry('\\' . ltrim($classDefinition->getCompleteName(), '\\'), $compilationContext))->get(),
        };

        $symbolVariable->setDynamicTypes('undefined');
        $compilationContext->headersManager->add('kernel/object');
        /**
         * The destination is a `zval *` parameter, so it goes through
         * getVariableCode() like every other emitter here. A bare name
         * hands over the zval itself and the generated C does not compile.
         */
        $compilationContext->codePrinter->output(
            sprintf(
                'zephir_get_class_constant(%s, %s, SL("%s"));',
                $compilationContext->backend->getVariableCode($symbolVariable),
                $classEntry,
                $constant
            )
        );
        $compilationContext->emitExceptionCheck();

        return new CompiledExpression('variable', $symbolVariable->getRealName(), $expression);
    }

    /**
     * Sets if the variable must be resolved into a direct variable symbol
     * create a temporary value or ignore the return value.
     */
    public function setExpectReturn(bool $expecting, ?Variable $expectingVariable = null): void
    {
        $this->expecting         = $expecting;
        $this->expectingVariable = $expectingVariable;
    }

    /**
     * Sets if the result of the evaluated expression is read only.
     */
    public function setReadOnly(bool $readOnly): void
    {
        $this->readOnly = $readOnly;
    }
}
