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

namespace Zephir;

use ReflectionException;
use Zephir\Detectors\ReadDetector;
use Zephir\Exception\CompilerException;
use Zephir\Variable\Variable;

use function array_filter;
use function array_keys;
use function array_merge;
use function array_slice;
use function array_values;
use function count;
use function implode;
use function in_array;
use function is_array;
use function is_object;
use function preg_match;
use function sprintf;
use function str_starts_with;

/**
 * Base class for common functionality in functions/calls
 */
class Call
{
    /**
     * Call expression.
     */
    protected ?Expression $expression = null;

    protected bool $isExpecting = false;

    protected array $mustCheckForCopy = [];

    protected bool $mustInit = false;

    /**
     * @var mixed|null
     */
    protected $reflection;
    /**
     * Positions of the callee's by-reference parameters, when the callee is
     * known. Populated before the arguments are resolved, because a subscript
     * argument has to be fetched differently for one.
     *
     * @var array<int, true>
     */
    protected array $byReferenceParameters = [];

    /**
     * The name of each argument in the order written, null for a positional
     * one.
     *
     * @var array<int, ?string>
     */
    protected array $argumentNames = [];

    /**
     * The array the named arguments are passed in, and the variables made
     * references for them, both released after the call.
     */
    protected ?Variable $namedArguments = null;

    /**
     * @var string[]
     */
    protected array $namedReferences = [];

    protected array $resolvedDynamicTypes = [];

    protected array $resolvedParams = [];

    protected array $resolvedTypes = [];

    protected ?Variable $symbolVariable = null;

    protected array $temporalVariables = [];

    /**
     * Add the last-call-status flag to the current symbol table.
     */
    public function addCallStatusFlag(CompilationContext $compilationContext): void
    {
        if (!$compilationContext->symbolTable->hasVariable('ZEPHIR_LAST_CALL_STATUS')) {
            $callStatus = new Variable(
                'int',
                'ZEPHIR_LAST_CALL_STATUS',
                $compilationContext->branchManager->getCurrentBranch()
            );
            $callStatus->setIsInitialized(true, $compilationContext);
            $callStatus->increaseUses();
            $callStatus->setReadOnly(true);
            $compilationContext->symbolTable->addRawVariable($callStatus);
        }
    }

    /**
     * Checks the last call status or make a label jump to the next catch block.
     *
     * Both zephir_check_call_status() and zephir_check_call_status_or_jump()
     * use ZEPHIR_MM_RESTORE() in their failure path, which requires
     * ZEPHIR_METHOD_GLOBALS_PTR to be declared. Force memory-grow so the
     * variable is always present.
     */
    public function addCallStatusOrJump(CompilationContext $compilationContext): void
    {
        $compilationContext->headersManager->add('kernel/fcall');
        $compilationContext->symbolTable->mustGrownStack(true);

        if ($compilationContext->insideTryCatch) {
            $compilationContext->codePrinter->output(
                'zephir_check_call_status_or_jump(try_end_' . $compilationContext->currentTryCatch . ');'
            );

            return;
        }

        $compilationContext->codePrinter->output('zephir_check_call_status();');
    }

    /**
     * Checks if temporary parameters must be copied or not.
     */
    public function checkTempParameters(CompilationContext $compilationContext): void
    {
        $compilationContext->headersManager->add('kernel/fcall');
        foreach ($this->getMustCheckForCopyVariables() as $checkVariable) {
            $compilationContext->codePrinter->output('zephir_check_temp_parameter(' . $checkVariable . ');');
        }
    }

    /**
     * Parameters to check if they must be copied.
     */
    public function getMustCheckForCopyVariables(): array
    {
        return $this->mustCheckForCopy;
    }

    /**
     * Resolve parameters using zvals in the stack and without allocating memory for constants.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function getReadOnlyResolvedParams(
        array $parameters,
        CompilationContext $compilationContext,
        array $expression,
    ): array {
        $codePrinter = $compilationContext->codePrinter;
        $exprParams  = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression, true);

        $params       = [];
        $types        = [];
        $dynamicTypes = [];

        foreach ($exprParams as $compiledExpression) {
            $expression = $compiledExpression->getOriginal();
            switch ($compiledExpression->getType()) {
                case 'null':
                    $params[]       = $compilationContext->backend->resolveValue('null', $compilationContext);
                    $types[]        = 'null';
                    $dynamicTypes[] = 'null';
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable(
                        'variable',
                        $compilationContext
                    );
                    $compilationContext->backend->assignLong(
                        $parameterVariable,
                        $compiledExpression->getCode(),
                        $compilationContext
                    );
                    $this->temporalVariables[] = $parameterVariable;
                    $params[]                  = '&' . $parameterVariable->getName();
                    $types[]                   = $parameterVariable->getType();
                    $dynamicTypes[]            = $parameterVariable->getType();
                    break;

                case 'char':
                case 'uchar':
                case 'ulong':
                case 'string':
                case 'istring':
                    $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );

                    $compilationContext->backend->assignString(
                        $parameterVariable,
                        Name::addSlashes($compiledExpression->getCode()),
                        $compilationContext
                    );

                    $this->temporalVariables[] = $parameterVariable;
                    $params[]                  = '&' . $parameterVariable->getName();
                    $types[]                   = $parameterVariable->getType();
                    $dynamicTypes[]            = $parameterVariable->getType();
                    break;

                case 'double':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable(
                        'variable',
                        $compilationContext
                    );
                    $codePrinter->output(
                        'ZVAL_DOUBLE(&' . $parameterVariable->getName() . ', ' . $compiledExpression->getCode() . ');'
                    );
                    $this->temporalVariables[] = $parameterVariable;
                    $params[]                  = '&' . $parameterVariable->getName();
                    $types[]                   = $parameterVariable->getType();
                    $dynamicTypes[]            = $parameterVariable->getType();
                    break;

                case 'bool':
                    $booleanCode = $compiledExpression->getBooleanCode();
                    if ('1' === $booleanCode || '0' === $booleanCode) {
                        $params[] = $compilationContext->backend->resolveValue(
                            '1' === $booleanCode ? 'true' : 'false',
                            $compilationContext
                        );
                    } else {
                        // A computed bool, such as a comparison, is boxed like any other scalar (#2771).
                        $parameterVariable = $compilationContext->backend->getScalarTempVariable(
                            'variable',
                            $compilationContext
                        );
                        $compilationContext->backend->assignBool(
                            $parameterVariable,
                            $booleanCode,
                            $compilationContext
                        );
                        $this->temporalVariables[] = $parameterVariable;
                        $params[]                  = '&' . $parameterVariable->getName();
                    }
                    $types[]        = 'bool';
                    $dynamicTypes[] = 'bool';
                    break;

                case 'array':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead(
                        $compiledExpression->getCode(),
                        $compilationContext,
                        $expression
                    );
                    $params[]          = $compilationContext->backend->getVariableCode($parameterVariable);
                    $types[]           = $parameterVariable->getType();
                    $dynamicTypes[]    = $parameterVariable->getType();
                    break;

                case 'variable':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead(
                        $compiledExpression->getCode(),
                        $compilationContext,
                        $expression
                    );
                    switch ($parameterVariable->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                        case 'ulong':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable(
                                'variable',
                                $compilationContext
                            );
                            $codePrinter->output(
                                'ZVAL_LONG(&' . $parameterTempVariable->getName() . ', ' . $compiledExpression->getCode(
                                ) . ');'
                            );
                            $params[]                  = '&' . $parameterTempVariable->getName();
                            $types[]                   = $parameterTempVariable->getType();
                            $dynamicTypes[]            = $parameterTempVariable->getType();
                            $this->temporalVariables[] = $parameterTempVariable;
                            break;

                        case 'char':
                        case 'uchar':
                            $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                'variable',
                                $compilationContext,
                                $expression
                            );
                            $compilationContext->backend->assignChar(
                                $parameterVariable,
                                $compiledExpression->getCode(),
                                $compilationContext
                            );

                            $this->temporalVariables[] = $parameterVariable;
                            $params[]                  = '&' . $parameterVariable->getName();
                            $types[]                   = $parameterVariable->getType();
                            $dynamicTypes[]            = $parameterVariable->getType();
                            break;

                        case 'double':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable(
                                'variable',
                                $compilationContext
                            );
                            $codePrinter->output(
                                'ZVAL_DOUBLE(&' . $parameterTempVariable->getName(
                                ) . ', ' . $compiledExpression->getCode() . ');'
                            );
                            $params[]                  = '&' . $parameterTempVariable->getName();
                            $types[]                   = $parameterTempVariable->getType();
                            $dynamicTypes[]            = $parameterTempVariable->getType();
                            $this->temporalVariables[] = $parameterTempVariable;
                            break;

                        case 'bool':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable(
                                'variable',
                                $compilationContext
                            );
                            $compilationContext->backend->assignBool(
                                $parameterTempVariable,
                                '(' . $parameterVariable->getName() . ' ? 1 : 0)',
                                $compilationContext
                            );
                            $params[]       = $compilationContext->backend->getVariableCode($parameterTempVariable);
                            $dynamicTypes[] = $parameterTempVariable->getType();
                            $types[]        = $parameterTempVariable->getType();
                            break;

                        case 'string':
                        case 'variable':
                        case 'mixed':
                        case 'array':
                            $params[]       = $compilationContext->backend->getVariableCode($parameterVariable);
                            $dynamicTypes[] = $parameterVariable->getType();
                            $types[]        = $parameterVariable->getType();
                            break;

                        default:
                            throw CompilerException::cannotUseVariableTypeAs(
                                $parameterVariable,
                                'as parameter',
                                $expression
                            );
                    }
                    break;

                default:
                    throw CompilerException::cannotUseValueTypeAs(
                        $compiledExpression,
                        'parameter',
                        $expression
                    );
            }
        }

        $this->resolvedTypes        = $types;
        $this->resolvedDynamicTypes = $dynamicTypes;

        return $params;
    }

    /**
     * Return resolved parameter dynamic types.
     */
    public function getResolvedDynamicTypes(): array
    {
        return $this->resolvedDynamicTypes;
    }

    /**
     * Resolve parameters getting aware that the target function/method could retain or change
     * the parameters.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function getResolvedParams(
        array $parameters,
        CompilationContext $compilationContext,
        array $expression,
    ): array {
        $codePrinter = $compilationContext->codePrinter;
        $exprParams  = $this->getResolvedParamsAsExpr($parameters, $compilationContext, $expression);

        $params       = [];
        $types        = [];
        $dynamicTypes = [];
        $mustCheck    = [];
        foreach ($exprParams as $compiledExpression) {
            $expression = $compiledExpression->getOriginal();
            switch ($compiledExpression->getType()) {
                case 'null':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable(
                        'variable',
                        $compilationContext
                    );
                    $params[]          = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignNull($parameterVariable, $compilationContext);
                    $this->temporalVariables[] = $parameterVariable;
                    $types[]                   = $compiledExpression->getType();
                    $dynamicTypes[]            = $compiledExpression->getType();
                    break;

                case 'int':
                case 'uint':
                case 'long':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable(
                        'variable',
                        $compilationContext
                    );
                    $params[]          = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignLong(
                        $parameterVariable,
                        $compiledExpression->getCode(),
                        $compilationContext
                    );
                    $this->temporalVariables[] = $parameterVariable;
                    $types[]                   = $compiledExpression->getType();
                    $dynamicTypes[]            = $compiledExpression->getType();
                    break;

                case 'double':
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable(
                        'variable',
                        $compilationContext
                    );
                    $params[]          = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignDouble(
                        $parameterVariable,
                        $compiledExpression->getCode(),
                        $compilationContext
                    );
                    $this->temporalVariables[] = $parameterVariable;
                    $types[]                   = $compiledExpression->getType();
                    break;

                case 'bool':
                    $value = $compiledExpression->getCode();
                    if ('true' == $value) {
                        $value = '1';
                    } elseif ('false' == $value) {
                        $value = '0';
                    }
                    $parameterVariable = $compilationContext->backend->getScalarTempVariable(
                        'variable',
                        $compilationContext
                    );
                    $params[]          = $compilationContext->backend->getVariableCode($parameterVariable);
                    $compilationContext->backend->assignBool($parameterVariable, $value, $compilationContext);

                    $this->temporalVariables[] = $parameterVariable;
                    $types[]                   = $compiledExpression->getType();
                    $dynamicTypes[]            = $compiledExpression->getType();
                    break;

                case 'char':
                case 'uchar':
                case 'ulong':
                case 'string':
                case 'istring':
                    $parameterVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );

                    $compilationContext->backend->assignString(
                        $parameterVariable,
                        Name::addSlashes($compiledExpression->getCode()),
                        $compilationContext
                    );

                    $this->temporalVariables[] = $parameterVariable;
                    $params[]                  = $compilationContext->backend->getVariableCode($parameterVariable);
                    $types[]                   = $compiledExpression->getType();
                    $dynamicTypes[]            = $compiledExpression->getType();
                    break;

                case 'array':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead(
                        $compiledExpression->getCode(),
                        $compilationContext,
                        $expression
                    );
                    $params[]          = $compilationContext->backend->getVariableCode($parameterVariable);
                    $types[]           = $compiledExpression->getType();
                    $dynamicTypes[]    = $compiledExpression->getType();
                    break;

                case 'variable':
                    $parameterVariable = $compilationContext->symbolTable->getVariableForRead(
                        $compiledExpression->getCode(),
                        $compilationContext,
                        $expression
                    );
                    switch ($parameterVariable->getType()) {
                        case 'int':
                        case 'uint':
                        case 'long':
                            /* ulong must be stored in string */
                        case 'ulong':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable(
                                'variable',
                                $compilationContext
                            );
                            $params[]              = $compilationContext->backend->getVariableCode(
                                $parameterTempVariable
                            );
                            $compilationContext->backend->assignLong(
                                $parameterTempVariable,
                                $parameterVariable,
                                $compilationContext
                            );
                            $this->temporalVariables[] = $parameterTempVariable;
                            $types[]                   = $parameterVariable->getType();
                            $dynamicTypes[]            = $parameterVariable->getType();
                            break;

                        case 'double':
                            $parameterTempVariable = $compilationContext->backend->getScalarTempVariable(
                                'variable',
                                $compilationContext
                            );
                            $compilationContext->backend->assignDouble(
                                $parameterTempVariable,
                                $parameterVariable,
                                $compilationContext
                            );
                            $params[]                  = $compilationContext->backend->getVariableCode(
                                $parameterTempVariable
                            );
                            $this->temporalVariables[] = $parameterTempVariable;
                            $types[]                   = $parameterVariable->getType();
                            $dynamicTypes[]            = $parameterVariable->getType();
                            break;

                        case 'char':
                        case 'uchar':
                            $parameterTempVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                                'variable',
                                $compilationContext,
                                $expression
                            );
                            // A C char is a single byte; wrap it into a length-1 zend_string
                            // so the call site receives a zval and the callee can re-extract
                            // the byte via zephir_get_charval().
                            $compilationContext->backend->assignChar(
                                $parameterTempVariable,
                                $parameterVariable,
                                $compilationContext
                            );

                            $this->temporalVariables[] = $parameterTempVariable;
                            $params[]                  = '&' . $parameterTempVariable->getName();
                            $types[]                   = $parameterVariable->getType();
                            $dynamicTypes[]            = $parameterVariable->getType();
                            break;

                        case 'bool':
                            $tempVariable = $compilationContext->backend->getScalarTempVariable(
                                'variable',
                                $compilationContext
                            );
                            $codePrinter->output('if (' . $parameterVariable->getName() . ') {');
                            $codePrinter->increaseLevel();
                            $compilationContext->backend->assignBool($tempVariable, '1', $compilationContext);
                            $codePrinter->decreaseLevel();
                            $codePrinter->output('} else {');
                            $codePrinter->increaseLevel();
                            $compilationContext->backend->assignBool($tempVariable, '0', $compilationContext);
                            $codePrinter->decreaseLevel();
                            $codePrinter->output('}');
                            $params[]       = $compilationContext->backend->getVariableCode($tempVariable);
                            $types[]        = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'string':
                        case 'array':
                            $params[]       = $compilationContext->backend->getVariableCode($parameterVariable);
                            $types[]        = $parameterVariable->getType();
                            $dynamicTypes[] = $parameterVariable->getType();
                            break;

                        case 'variable':
                        case 'mixed':
                            $params[]     = $compilationContext->backend->getVariableCode($parameterVariable);
                            $types[]      = $parameterVariable->getType();
                            $dynamicTypes = array_merge(
                                $dynamicTypes,
                                array_keys($parameterVariable->getDynamicTypes())
                            );
                            break;

                        default:
                            throw CompilerException::cannotUseVariableTypeAs(
                                $parameterVariable,
                                'as parameter',
                                $expression
                            );
                    }
                    break;

                default:
                    throw CompilerException::cannotUseValueTypeAs(
                        $compiledExpression,
                        'parameter',
                        $expression
                    );
            }
        }

        $this->resolvedTypes        = $types;
        $this->resolvedDynamicTypes = $dynamicTypes;
        $this->mustCheckForCopy     = $mustCheck;

        return $params;
    }

    /**
     * Resolves parameters.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function getResolvedParamsAsExpr(
        array $parameters,
        CompilationContext $compilationContext,
        array $expression,
        bool $readOnly = false
    ): ?array {
        if ($this->resolvedParams) {
            return $this->resolvedParams;
        }

        $parameters          = array_values($parameters);
        $this->argumentNames = $this->checkArgumentNames($parameters);
        $positions           = $this->calleeParameterPositions();

        $params = [];
        foreach ($parameters as $written => $parameter) {
            /**
             * A named argument stands for the parameter of its name, wherever
             * it is written.
             */
            $name     = $this->argumentNames[$written];
            $position = null === $name ? $written : ($positions[$name] ?? -1);

            if (is_array($parameter['parameter'])) {
                $paramExpr = new Expression($parameter['parameter']);

                switch ($parameter['parameter']['type']) {
                    case 'property-access':
                    case 'array-access':
                    case 'static-property-access':
                        $paramExpr->setReadOnly(true);
                        /**
                         * A by-reference parameter is written by the callee, so
                         * the fetch runs in write context and the element itself
                         * becomes a reference, exactly as PHP does it. Never a
                         * borrow, whatever the container is.
                         *
                         * @see \Zephir\Expression\NativeArrayAccess::setWriteThrough()
                         */
                        $paramExpr->setWriteThrough(isset($this->byReferenceParameters[$position]));
                        break;

                    case 'property-string-access':
                    case 'property-dynamic-access':
                        /**
                         * A property named at runtime reaches the same write
                         * context. Its read is left as it was: unlike the three
                         * above it never borrowed, so forcing read-only here
                         * would be a change with nothing behind it.
                         *
                         * @see \Zephir\Expression\PropertyDynamicAccess::setWriteThrough()
                         */
                        $paramExpr->setReadOnly($readOnly);
                        $paramExpr->setWriteThrough(isset($this->byReferenceParameters[$position]));
                        break;

                    default:
                        $paramExpr->setReadOnly($readOnly);
                        break;
                }

                $params[] = $paramExpr->compile($compilationContext);
                continue;
            }

            if ($parameter['parameter'] instanceof CompiledExpression) {
                $params[] = $parameter['parameter'];
                continue;
            }

            throw new CompilerException('Invalid expression ', $expression);
        }

        $this->resolvedParams = $params;

        return $this->resolvedParams;
    }

    /**
     * A local made a reference for a by-reference parameter is assigned by the
     * callee alone, so the memory frame has to know it before the call, or what
     * the callee stores there is never released. A parameter is not the
     * method's to release.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2824
     */
    protected function observeLocal(string $code, CompilationContext $compilationContext): void
    {
        if (str_starts_with($code, '&')) {
            $compilationContext->codePrinter->output('ZEPHIR_OBSERVE_IF_UNDEF(' . $code . ');');
        }
    }

    /**
     * Whether any argument of the call is passed by name.
     */
    public function hasNamedArguments(): bool
    {
        return [] !== array_filter($this->argumentNames, 'is_string');
    }

    /**
     * The argument names, in the order written, null for a positional one.
     *
     * @return array<int, ?string>
     */
    public function getArgumentNames(): array
    {
        return $this->argumentNames;
    }

    /**
     * Splits resolved arguments into the positional ones and an array of the
     * named ones, which the engine binds by the parameter names of the method
     * or function actually called, as PHP does. Returns the positional ones and
     * the C expression of that array's HashTable, or null without names.
     *
     * A parameter the known callee takes by reference gets the variable as a
     * reference, released by releaseNamedArguments() after the call.
     *
     * @param string[] $params as resolved, in the order written
     *
     * @return array{0: string[], 1: ?string}
     *
     * @see https://github.com/zephir-lang/zephir/issues/2824
     */
    public function splitNamedArguments(array $params, CompilationContext $compilationContext): array
    {
        if (!$this->hasNamedArguments()) {
            return [$params, null];
        }

        $positional = [];
        $named      = [];
        foreach ($params as $written => $code) {
            $name = $this->argumentNames[$written];
            if (null === $name) {
                $positional[] = $code;
                continue;
            }

            $named[$name] = $this->isByReferenceParameter($name)
                ? [$this->namedReference($code, $written, $compilationContext), true]
                : [$code, false];
        }

        $this->resolvedTypes        = array_slice($this->resolvedTypes, 0, count($positional));
        $this->resolvedDynamicTypes = array_slice($this->resolvedDynamicTypes, 0, count($positional));

        $compilationContext->headersManager->add('kernel/array');

        $backend = $compilationContext->backend;
        $array   = $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
        $backend->initArray($array, $compilationContext, count($named));

        /**
         * An assignment would store a reference's value; the entry for a
         * by-reference parameter has to be the reference itself.
         */
        $arrayCode = $backend->getVariableCode($array);
        foreach ($named as $name => [$code, $byReference]) {
            $compilationContext->codePrinter->output(
                $byReference
                    ? 'Z_TRY_ADDREF_P(' . $code . '); zend_hash_str_update(Z_ARRVAL_P(' . $arrayCode . '), SL("' . $name . '"), ' . $code . ');'
                    : 'zephir_array_update_string(' . $arrayCode . ', SL("' . $name . '"), ' . $code . ', PH_COPY);'
            );
        }

        $this->namedArguments      = $array;
        $this->temporalVariables[] = $array;

        return [$positional, 'Z_ARRVAL_P(' . $arrayCode . ')'];
    }

    /**
     * Releases what splitNamedArguments() made for the call, once it is made,
     * as PHP frees the arguments when the call returns: the array first, so a
     * variable made a reference for the callee is the reference's only holder
     * again when it is unwrapped.
     */
    public function releaseNamedArguments(CompilationContext $compilationContext): void
    {
        if (null === $this->namedArguments) {
            return;
        }

        $codePrinter = $compilationContext->codePrinter;
        $codePrinter->output(
            'ZEPHIR_INIT_NVAR(' . $compilationContext->backend->getVariableCode($this->namedArguments) . ');'
        );
        foreach ($this->namedReferences as $reference) {
            $codePrinter->output('ZEPHIR_UNREF_WRITE(' . $reference . ');');
        }
    }

    /**
     * Return resolved parameter types.
     */
    public function getResolvedTypes(): array
    {
        return $this->resolvedTypes;
    }

    /**
     * Returns the symbol variable that must be returned by the call.
     */
    public function getSymbolVariable(bool $useTemp = false, ?CompilationContext $compilationContext = null): ?Variable
    {
        $symbolVariable = $this->symbolVariable;

        if ($useTemp && !is_object($symbolVariable)) {
            return $compilationContext->symbolTable->getTempVariableForWrite('variable', $compilationContext);
        }

        return $symbolVariable;
    }

    /**
     * Returns the temporal variables generated during the parameter resolving.
     *
     * @return Variable[]
     */
    public function getTemporalVariables(): array
    {
        return $this->temporalVariables;
    }

    /**
     * Check if an external expression is expecting the call return a value.
     */
    public function isExpectingReturn(): bool
    {
        return $this->isExpecting;
    }

    /**
     * Returns if the symbol to be returned by the call must be initialized.
     */
    public function mustInitSymbolVariable(): bool
    {
        return $this->mustInit;
    }

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call.
     */
    public function processExpectedObservedReturn(CompilationContext $compilationContext): void
    {
        $expr       = $this->expression;
        $expression = $expr->getExpression();

        /**
         * Create temporary variable if needed.
         */
        $mustInit       = false;
        $symbolVariable = null;
        $isExpecting    = $expr->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $expr->getExpectingVariable();
            if (is_object($symbolVariable)) {
                $readDetector = new ReadDetector();
                if ($readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                        'variable',
                        $compilationContext
                    );
                } else {
                    $mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForObserveOrNullify(
                    'variable',
                    $compilationContext
                );
            }
        }

        $this->mustInit       = $mustInit;
        $this->symbolVariable = $symbolVariable;
        $this->isExpecting    = $isExpecting;
    }

    /**
     * Processes the symbol variable that will be used to return
     * the result of the symbol call.
     */
    public function processExpectedReturn(CompilationContext $compilationContext): void
    {
        $expression = $this->expression->getExpression();

        /**
         * Create temporary variable if needed.
         */
        $symbolVariable = null;
        $isExpecting    = $this->expression->isExpectingReturn();
        if ($isExpecting) {
            $symbolVariable = $this->expression->getExpectingVariable();
            if (is_object($symbolVariable)) {
                $readDetector = new ReadDetector();
                if ($readDetector->detect($symbolVariable->getName(), $expression)) {
                    $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                        'variable',
                        $compilationContext,
                        $expression
                    );
                } else {
                    $this->mustInit = true;
                }
            } else {
                $symbolVariable = $compilationContext->symbolTable->getTempVariableForWrite(
                    'variable',
                    $compilationContext,
                    $expression
                );
            }
        }

        $this->symbolVariable = $symbolVariable;
        $this->isExpecting    = $isExpecting;
    }

    /**
     * PHP's compile time rules for named arguments, plus the parameter names
     * of a callee known at compile time unless it collects extra names in a
     * variadic parameter.
     *
     * @return array<int, ?string>
     */
    private function checkArgumentNames(array $parameters): array
    {
        $names     = [];
        $seen      = [];
        $positions = null;
        foreach ($parameters as $parameter) {
            $name = $parameter['name'] ?? null;
            if (null === $name) {
                if ([] !== $seen) {
                    throw new CompilerException('Cannot use positional argument after named argument', $parameter);
                }

                $names[] = null;
                continue;
            }

            if (isset($seen[$name])) {
                throw new CompilerException(
                    'Named parameter $' . $name . ' overwrites previous argument',
                    $parameter['parameter']
                );
            }

            $seen[$name] = true;
            $names[]     = $name;

            $positions ??= $this->calleeParameterPositions();
            if ($this->reflection && !$this->isVariadicCallee() && !isset($positions[$name])) {
                throw new CompilerException(
                    'Named parameter "'
                    . $name
                    . '" is not a valid parameter name, available: '
                    . implode(', ', array_keys($positions)),
                    $parameter['parameter']
                );
            }
        }

        return $names;
    }

    /**
     * The known callee's parameter positions by name.
     *
     * @return array<string, int>
     */
    private function calleeParameterPositions(): array
    {
        $positions = [];
        foreach ($this->reflection?->getParameters() ?? [] as $position => $parameter) {
            $positions[is_object($parameter) ? $parameter->getName() : $parameter['name']] = $position;
        }

        return $positions;
    }

    private function isVariadicCallee(): bool
    {
        return (bool) $this->reflection?->isVariadic();
    }

    private function isByReferenceParameter(string $name): bool
    {
        $position = $this->calleeParameterPositions()[$name] ?? null;

        return null !== $position && isset($this->byReferenceParameters[$position]);
    }

    /**
     * The variable a by-reference named argument passes, made a reference.
     * A closure's `use (&x)` capture is one already.
     */
    private function namedReference(string $code, int $written, CompilationContext $compilationContext): string
    {
        if (preg_match('/^Z_REFVAL_P\\((&[a-zA-Z0-9_]+)\\)$/', $code, $slot)) {
            return $slot[1];
        }

        $parameter = $this->resolvedParams[$written]->getOriginal();
        $variable  = 'variable' === ($parameter['type'] ?? null)
            ? $compilationContext->symbolTable->getVariable($parameter['value'])
            : null;
        if (
            null === $variable
            || !in_array($variable->getType(), ['variable', 'mixed'], true)
            || $code !== $compilationContext->backend->getVariableCode($variable)
        ) {
            throw new CompilerException('Only variables can be passed by reference', $parameter);
        }

        /**
         * The callee writes through the reference, as for a positional one
         * (#2654): without the record the variable looks never assigned.
         */
        $variable->increaseMutates();
        $variable->setDynamicTypes('undefined');

        $this->observeLocal($code, $compilationContext);
        $compilationContext->codePrinter->output('ZEPHIR_MAKE_REF(' . $code . ');');
        $this->namedReferences[] = $code;

        return $code;
    }
}
