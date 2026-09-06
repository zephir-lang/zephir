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

namespace Zephir\Class\Method;

use ReflectionException;
use Zephir\Branch;
use Zephir\BranchManager;
use Zephir\Cache\Manager;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Definition\GeneratorTransformer;
use Zephir\Class\Entry as ClassEntry;
use Zephir\Code\Printer;
use Zephir\CompilationContext;
use Zephir\Detectors\WriteDetector;
use Zephir\Detectors\YieldDetector;
use Zephir\Documentation\Docblock;
use Zephir\Documentation\DocblockParser;
use Zephir\Exception;
use Zephir\Exception\CompilerException;
use Zephir\Expression;
use Zephir\Name;
use Zephir\Passes\CallGathererPass;
use Zephir\Passes\DefiniteAssignmentPass;
use Zephir\Passes\LocalContextPass;
use Zephir\Passes\NativeArrayPass;
use Zephir\Passes\StaticTypeInference;
use Zephir\StatementsBlock;
use Zephir\SymbolTable;
use Zephir\Variable\Variable;

use function array_diff;
use function array_key_exists;
use function array_keys;
use function array_values;
use function count;
use function implode;
use function in_array;
use function is_array;
use function is_object;
use function json_decode;
use function json_encode;
use function preg_replace;
use function sprintf;
use function str_replace;
use function strtolower;

use const PHP_EOL;

/**
 * Represents a class method
 */
class Method
{
    /**
     * Control runs off the end of the statement list and carries on after it.
     */
    private const OUTCOME_FALLS = 'falls';
    /**
     * Control leaves the enclosing `switch`/loop without leaving the method
     * (a `break` or a `continue`).
     */
    private const OUTCOME_JUMPS = 'jumps';
    /**
     * Control leaves the method (a `return` or a `throw`).
     */
    private const OUTCOME_RETURNS = 'returns';
    /**
     * Statement types that loop, and so can capture a `break`/`continue`.
     */
    private const LOOP_TYPES = ['while', 'do-while', 'for', 'loop'];

    public bool $optimizable = true;

    /**
     * Call Gatherer Pass.
     */
    protected ?CallGathererPass $callGathererPass = null;

    /**
     * Whether the method is abstract or not.
     */
    protected bool $isAbstract = false;
    /**
     * Whether the method is bundled with PHP or not.
     */
    protected bool $isBundled = false;
    /**
     * Whether the method is final or not.
     */
    protected bool $isFinal = false;
    /**
     * Whether the method is an initializer or not.
     */
    protected bool $isInitializer = false;
    /**
     * Whether the method is internal or not.
     */
    protected bool $isInternal = false;
    /**
     * Whether the method is public or not.
     */
    protected bool $isPublic = true;

    /**
     * Whether the method is static or not.
     */
    protected bool              $isStatic     = false;
    protected ?DefiniteAssignmentPass $definiteAssignment = null;
    protected ?LocalContextPass       $localContext       = null;
    protected ?NativeArrayPass        $nativeArray        = null;
    /**
     * Maps a Zephir return-type name to its Zend `MAY_BE_*` type-mask bit.
     *
     * Used both to decide whether a union return type can be expressed as an
     * arginfo type mask (see areReturnTypesCompatible()) and to build that
     * mask (see ArgInfoDefinition). Only non-null, non-class scalar types live
     * here; `null` and class/object types are handled separately.
     */
    protected array $mayBeArgTypes = [
        'int'    => 'MAY_BE_LONG',
        'uint'   => 'MAY_BE_LONG',
        'long'   => 'MAY_BE_LONG',
        'ulong'  => 'MAY_BE_LONG',
        'char'   => 'MAY_BE_LONG',
        'uchar'  => 'MAY_BE_LONG',
        'double' => 'MAY_BE_DOUBLE',
        'bool'   => 'MAY_BE_BOOL',
        'false'  => 'MAY_BE_FALSE',
        'string' => 'MAY_BE_STRING',
        'array'  => 'MAY_BE_ARRAY',
    ];
    /**
     * Whether the variable is mixed.
     *
     * Only for PHP >= 8.0
     */
    protected bool      $mixed          = false;
    protected ?Docblock $parsedDocblock = null;
    /**
     * Class type hints returned by the method.
     */
    protected array $returnClassTypes = [];
    /**
     * Types returned by the method.
     */
    protected array $returnTypes = [];
    /**
     * Raw-types returned by the method.
     */
    protected ?array $returnTypesRaw  = null;
    /**
     * Carrier property holding the enclosing object of a capturing closure.
     *
     * A capturing closure spends its only per-instance slot - the bound
     * `$this` - on the capture carrier, so the enclosing object travels as one
     * more capture. `$` cannot appear in a Zephir identifier, so no user
     * capture can collide with this name.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2652
     */
    public const CLOSURE_BOUND_THIS = '__$zephir_this';

    /**
     * Closure `use (...)` captures, keyed by name.
     */
    protected array  $staticVariables = [];
    /**
     * Captures read off the carrier with a ZVAL_COPY, keyed by name. They are
     * the ones the memory frame has to release again.
     */
    private array    $capturesToRelease = [];
    /**
     * Static Type Inference Pass.
     */
    protected ?StaticTypeInference $typeInference = null;
    /**
     * Whether the variable is void.
     */
    protected bool $void = false;

    /**
     * Cached generator-detection result. Populated lazily by isGenerator().
     */
    protected ?bool $isGenerator = null;

    /**
     * Generator split roles (issue #1849). A generator method is split by
     * GeneratorTransformer into a visible "creator" (returns the kernel
     * <Ns>\Generator object) and a hidden internal "step" method holding the
     * original body compiled as a resumable state machine.
     */
    protected ?Method $generatorStep    = null;
    protected ?Method $generatorCreator = null;
    /**
     * Number of yield suspension points allocated while compiling a step body.
     */
    protected int $yieldPoints = 0;

    /**
     * The method name as written in the .zep source.
     *
     * The compiler emits copies of a method under a mangled name — the
     * `internal-call-transformation` twin (`<name>_zephir_internal_call`) and a
     * generator's step (`zephir_gen_step_<name>`). Those copies are compiled as
     * methods in their own right, so anything user-visible that reads the
     * method name — `__FUNCTION__`, `__METHOD__`, compile diagnostics — must
     * read the declared name instead of the mangled one.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2643
     */
    protected ?string $declaredName = null;

    public function __construct(
        protected ?Definition $classDefinition = null,
        protected array $visibility = [],
        protected string $name = '',
        protected ?Parameters $parameters = null,
        protected ?StatementsBlock $statements = null,
        protected ?string $docblock = null,
        ?array $returnType = null,
        protected ?array $expression = [],
        array $staticVariables = [],
    ) {
        $this->staticVariables = $staticVariables;

        $this->checkVisibility($visibility, $name, $expression);
        $this->setReturnTypes($returnType);
    }

    /**
     * Checks whether at least one return type hint is bool compatible.
     */
    public function areReturnTypesBoolCompatible(): bool
    {
        return isset($this->returnTypes['bool']);
    }

    /**
     * Checks if the method's return type(s) can be expressed in arginfo.
     *
     * A return type is renderable when every declared member is something the
     * engine can describe: `void`, `null`, `mixed`, a class/object, or a scalar
     * that has a `MAY_BE_*` mask bit. Any other member (e.g. `variable`,
     * `callable`, `resource`) makes the whole return type undeterminable, so we
     * emit no return-type arginfo at all. Unions of renderable members — of any
     * arity, scalar and/or class — are fully supported and enforced by PHP,
     * just like a hand-written union return type.
     */
    public function areReturnTypesCompatible(): bool
    {
        if ($this->isVoid()) {
            return true;
        }

        foreach (array_keys($this->returnTypes) as $type) {
            if (in_array($type, ['object', 'null', 'mixed'], true)) {
                continue;
            }

            if (!isset($this->mayBeArgTypes[$type])) {
                return false;
            }
        }

        return true;
    }

    /**
     * Checks whether at least one return type hint is double compatible.
     */
    public function areReturnTypesDoubleCompatible(): bool
    {
        return isset($this->returnTypes['double']);
    }

    /**
     * Checks whether at least one return type hint is false compatible.
     */
    public function areReturnTypesFalseCompatible(): bool
    {
        return isset($this->returnTypes['false']);
    }

    /**
     * Checks whether at least one return type hint is integer compatible.
     */
    public function areReturnTypesIntCompatible(): bool
    {
        $types = ['int', 'uint', 'char', 'uchar', 'long', 'ulong'];

        foreach ($this->returnTypes as $returnType => $definition) {
            if (in_array($returnType, $types)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Checks whether at least one return type hint is null compatible.
     */
    public function areReturnTypesNullCompatible(): bool
    {
        return isset($this->returnTypes['null']);
    }

    public function areReturnTypesObjectCompatible(): bool
    {
        return isset($this->returnTypes['object']);
    }

    /**
     * Checks whether at least one return type hint is string compatible.
     */
    public function areReturnTypesStringCompatible(): bool
    {
        return isset($this->returnTypes['string']);
    }

    /**
     * Assigns a default value.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function assignDefaultValue(array $parameter, CompilationContext $compilationContext): string
    {
        /**
         * Class-Hinted parameters only can be null?
         */
        if (isset($parameter['cast'])) {
            if ('null' !== $parameter['default']['type']) {
                throw new CompilerException(
                    'Class-Hinted parameters only can have "null" as default parameter',
                    $parameter
                );
            }
        }

        $oldCodePrinter = $compilationContext->codePrinter;
        $codePrinter    = new Printer();
        $codePrinter->increaseLevel();
        $codePrinter->increaseLevel();
        $compilationContext->codePrinter = $codePrinter;

        $paramVariable = $compilationContext->symbolTable->getVariableForWrite($parameter['name'], $compilationContext);

        /**
         * TODO: Refactoring this place, move to one - static-constant-access
         */
        $dataType = $this->getParamDataType($parameter);
        switch ($dataType) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(.
                         *
                         * TODO: Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite(
                            $parameter['name'],
                            $compilationContext,
                            $parameter['default']
                        );
                        $expression     = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);

                        if ('int' != $compiledExpression->getType()) {
                            throw new CompilerException(
                                'Default parameter value type: '
                                . $compiledExpression->getType()
                                . ' cannot be assigned to variable(int)',
                                $parameter
                            );
                        }

                        $parameter['default']['type']  = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;

                        return $this->assignDefaultValue($parameter, $compilationContext);

                    case 'null':
                        $codePrinter->output($parameter['name'] . ' = 0;');
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $codePrinter->output($parameter['name'] . ' = ' . $parameter['default']['value'] . ';');
                        break;

                    case 'double':
                        $codePrinter->output($parameter['name'] . ' = (int) ' . $parameter['default']['value'] . ';');
                        break;

                    default:
                        throw new CompilerException(
                            'Default parameter value type: ' . $parameter['default']['type'] . ' cannot be assigned to variable(int)',
                            $parameter
                        );
                }
                break;

            case 'double':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        return $this->processStaticConstantAccess(
                            $compilationContext,
                            $parameter,
                            $oldCodePrinter,
                            'double'
                        );

                    case 'null':
                        $codePrinter->output($parameter['name'] . ' = 0;');
                        break;

                    case 'int':
                    case 'uint':
                    case 'long':
                        $codePrinter->output(
                            $parameter['name'] . ' = (double) ' . $parameter['default']['value'] . ';'
                        );
                        break;

                    case 'double':
                        $codePrinter->output($parameter['name'] . ' = ' . $parameter['default']['value'] . ';');
                        break;

                    default:
                        throw new CompilerException(
                            'Default parameter value type: ' . $parameter['default']['type'] . ' cannot be assigned to variable(double)',
                            $parameter
                        );
                }
                break;

            case 'bool':
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(.
                         *
                         * TODO: Rewrite all to codePrinter
                         */
                        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite(
                            $parameter['name'],
                            $compilationContext,
                            $parameter['default']
                        );
                        $expression     = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);

                        if ('bool' !== $compiledExpression->getType()) {
                            throw new CompilerException(
                                'Default parameter value type: '
                                . $compiledExpression->getType()
                                . ' cannot be assigned to variable(bool)',
                                $parameter
                            );
                        }

                        $parameter['default']['type']  = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;

                        return $this->assignDefaultValue($parameter, $compilationContext);

                    case 'null':
                        $codePrinter->output($parameter['name'] . ' = 0;');
                        break;

                    case 'bool':
                        if ('true' === $parameter['default']['value']) {
                            $codePrinter->output($parameter['name'] . ' = 1;');
                        } else {
                            $codePrinter->output($parameter['name'] . ' = 0;');
                        }
                        break;

                    default:
                        throw new CompilerException(
                            'Default parameter value type: '
                            . $parameter['default']['type']
                            . ' cannot be assigned to variable(bool)',
                            $parameter
                        );
                }
                break;

            case 'string':
                /**
                 * Native zend_string * params: emit zend_string_init() for string
                 * defaults and ZVAL_STR() to hand ownership to the companion zval.
                 * mustGrownStack was already set in Round 1 for this path.
                 */
                if ($paramVariable->isNativeString()) {
                    switch ($parameter['default']['type']) {
                        case 'string':
                            $codePrinter->output(
                                $parameter['name'] . ' = zend_string_init(ZEND_STRL("'
                                . Name::addSlashes($parameter['default']['value'])
                                . '"), 0);'
                            );
                            // Register the companion zval in the memory frame so that
                            // ZEPHIR_MM_RESTORE() decrements the refcount of the freshly
                            // allocated zend_string on return, balancing zend_string_init().
                            $codePrinter->output(
                                'zephir_memory_observe(&' . $parameter['name'] . '_zv);'
                            );
                            $codePrinter->output(
                                'ZVAL_STR(&' . $parameter['name'] . '_zv, '
                                . $parameter['name'] . ');'
                            );
                            break;

                        case 'null':
                            /**
                             * zend_string * stays NULL (Z_PARAM_STR_OR_NULL
                             * didn't set it). Set the companion zval to IS_NULL
                             * so downstream ZEPHIR_IS_NULL() checks work.
                             * ZEPHIR_INIT_VAR registers it in the memory frame.
                             */
                            $codePrinter->output(
                                'ZEPHIR_INIT_VAR(&' . $parameter['name'] . '_zv);'
                            );
                            break;

                        default:
                            throw new CompilerException(
                                sprintf(
                                    'Default parameter value type: %s cannot be assigned to native string variable',
                                    $parameter['default']['type']
                                ),
                                $parameter
                            );
                    }
                    break;
                }

                $compilationContext->symbolTable->mustGrownStack(true);
                $compilationContext->headersManager->add('kernel/memory');

                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        return $this->processStaticConstantAccess(
                            $compilationContext,
                            $parameter,
                            $oldCodePrinter,
                            'string'
                        );

                    case 'null':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        $compilationContext->backend->assignString($paramVariable, null, $compilationContext);
                        break;

                    case 'string':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        $compilationContext->backend->assignString(
                            $paramVariable,
                            Name::addSlashes($parameter['default']['value']),
                            $compilationContext
                        );
                        break;

                    default:
                        throw new CompilerException(
                            sprintf(
                                'Default parameter value type: %s cannot be assigned to variable(string)',
                                $parameter['default']['type']
                            ),
                            $parameter
                        );
                }
                break;

            case 'array':
                $compilationContext->symbolTable->mustGrownStack(true);
                $compilationContext->headersManager->add('kernel/memory');
                switch ($parameter['default']['type']) {
                    case 'null':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        break;

                    case 'empty-array':
                    case 'array':
                        $compilationContext->backend->initVar($paramVariable, $compilationContext);
                        $compilationContext->backend->initArray($paramVariable, $compilationContext);
                        break;

                    default:
                        throw new CompilerException(
                            'Default parameter value type: '
                            . $parameter['default']['type']
                            . ' cannot be assigned to variable(array)',
                            $parameter
                        );
                }
                break;

            case 'variable':
                $symbolVariable = $compilationContext->symbolTable->getVariableForWrite(
                    $parameter['name'],
                    $compilationContext,
                    $parameter['default']
                );
                switch ($parameter['default']['type']) {
                    case 'static-constant-access':
                        /**
                         * Now I can write code for easy use on Expression because code in this method don't write with codePrinter ;(.
                         *
                         * TODO: Rewrite all to codePrinter
                         */
                        $expression = new Expression($parameter['default']);
                        $expression->setExpectReturn(true, $symbolVariable);
                        $compiledExpression = $expression->compile($compilationContext);

                        $parameter['default']['type']  = $compiledExpression->getType();
                        $parameter['default']['value'] = $compiledExpression->getCode();

                        $compilationContext->codePrinter = $oldCodePrinter;

                        return $this->assignDefaultValue($parameter, $compilationContext);

                    case 'int':
                    case 'uint':
                    case 'long':
                    case 'ulong':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->assignLong(
                            $symbolVariable,
                            $parameter['default']['value'],
                            $compilationContext
                        );
                        break;

                    case 'double':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->assignDouble(
                            $symbolVariable,
                            $parameter['default']['value'],
                            $compilationContext
                        );
                        break;

                    case 'string':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->assignString(
                            $paramVariable,
                            Name::addSlashes($parameter['default']['value']),
                            $compilationContext
                        );
                        break;

                    case 'bool':
                        $expectedMutations = $compilationContext->symbolTable->getExpectedMutations($parameter['name']);
                        if ($expectedMutations < 2) {
                            if ('true' == $parameter['default']['value']) {
                                $compilationContext->backend->assignZval(
                                    $paramVariable,
                                    $compilationContext->backend->resolveValue('true', $compilationContext),
                                    $compilationContext
                                );
                            } else {
                                $compilationContext->backend->assignZval(
                                    $paramVariable,
                                    $compilationContext->backend->resolveValue('false', $compilationContext),
                                    $compilationContext
                                );
                            }
                        } else {
                            $compilationContext->symbolTable->mustGrownStack(true);
                            $compilationContext->headersManager->add('kernel/memory');
                            if ('true' == $parameter['default']['value']) {
                                $compilationContext->backend->copyOnWrite(
                                    $paramVariable,
                                    $compilationContext->backend->resolveValue('true', $compilationContext),
                                    $compilationContext
                                );
                            } else {
                                $compilationContext->backend->copyOnWrite(
                                    $paramVariable,
                                    $compilationContext->backend->resolveValue('false', $compilationContext),
                                    $compilationContext
                                );
                            }
                        }
                        break;

                    case 'null':
                        $expectedMutations = $compilationContext->symbolTable->getExpectedMutations($parameter['name']);
                        if ($expectedMutations < 2) {
                            $compilationContext->backend->assignZval(
                                $symbolVariable,
                                $compilationContext->backend->resolveValue('null', $compilationContext),
                                $compilationContext
                            );
                        } else {
                            $compilationContext->symbolTable->mustGrownStack(true);
                            $compilationContext->headersManager->add('kernel/memory');
                            $compilationContext->backend->copyOnWrite(
                                $paramVariable,
                                $compilationContext->backend->resolveValue('null', $compilationContext),
                                $compilationContext
                            );
                        }
                        break;

                    case 'empty-array':
                        $compilationContext->symbolTable->mustGrownStack(true);
                        $compilationContext->headersManager->add('kernel/memory');
                        $compilationContext->backend->initVar($symbolVariable, $compilationContext);
                        $compilationContext->backend->initArray($symbolVariable, $compilationContext);
                        break;

                    default:
                        throw new CompilerException(
                            'Default parameter value type: ' . $parameter['default']['type'] . ' cannot be assigned to variable(variable)',
                            $parameter
                        );
                }
                break;

            default:
                throw new CompilerException('Default parameter type: ' . $dataType, $parameter);
        }

        $compilationContext->codePrinter = $oldCodePrinter;

        return $codePrinter->getOutput();
    }

    /**
     * Assigns a zval value to a static low-level type.
     *
     * Types that ZEND_PARSE_PARAMETERS populates directly into the native local
     * (`int`, `long`, `bool`) normally need nothing emitted here. Internal
     * methods are the exception: they run no ZPP block at all, because they are
     * called C-to-C with the *caller's* execute_data and receive their own
     * arguments through the trailing `_ext` pointers instead (see #2021). For
     * those, the conversion has to be emitted explicitly.
     *
     * @throws CompilerException
     */
    public function assignZvalValue(array $parameter, CompilationContext $compilationContext): string
    {
        $dataType = $this->getParamDataType($parameter);

        if (in_array($dataType, ['variable', 'callable', 'object', 'resource', 'mixed'])) {
            return '';
        }

        /**
         * Native zend_string * params: Z_PARAM_STR already populated the
         * value. Just populate the companion zval. Must check before the
         * _param variable lookup since native strings no longer have one.
         */
        if ($dataType === 'string') {
            $inputParamVar = $compilationContext->symbolTable->getVariableForWrite(
                $parameter['name'],
                $compilationContext
            );
            if ($inputParamVar->isNativeString()) {
                // Populate the companion zval from the native zend_string *.
                // When memory-grow is active, take an owning reference via
                // ZVAL_STR_COPY and register the companion in the memory frame
                // so ZEPHIR_MM_RESTORE() decrements the refcount on return.
                // Without the matching observe, the refcount incremented by
                // ZVAL_STR_COPY would never be released (memory leak, #2500).
                // When memory-grow is NOT active, the caller's zend_string is
                // alive for the entire function duration, so a non-owning
                // ZVAL_STR view is sufficient and needs no cleanup.
                if ($compilationContext->symbolTable->getMustGrownStack()) {
                    return "\t" . 'zephir_memory_observe(&' . $parameter['name'] . '_zv);' . PHP_EOL
                        . "\t" . 'ZVAL_STR_COPY(&' . $parameter['name'] . '_zv, ' . $parameter['name'] . ');' . PHP_EOL;
                }

                return "\t" . 'ZVAL_STR(&' . $parameter['name'] . '_zv, ' . $parameter['name'] . ');' . PHP_EOL;
            }
        }

        $compilationContext->headersManager->add('kernel/operators');
        $parameterVariable = $compilationContext->symbolTable->getVariableForWrite(
            $parameter['name'] . '_param',
            $compilationContext
        );
        $parameterCode     = $compilationContext->backend->getVariableCode($parameterVariable);
        $inputParamVar     = $compilationContext->symbolTable->getVariableForWrite(
            $parameter['name'],
            $compilationContext
        );
        $inputParamCode    = $compilationContext->backend->getVariableCode($inputParamVar);

        switch ($dataType) {
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                if ($this->isInternal()) {
                    return "\t" . $parameter['name'] . ' = zephir_get_intval(' . $parameterCode . ');' . PHP_EOL;
                }

                // Value already passed in `Z_PARAM_LONG()`
                return '';

            case 'char':
            case 'uchar':
                // zephir_get_charval() yields a zend_long; the declared C
                // type (char / unsigned char) narrows it to the byte. See #1629.
                return "\t" . $parameter['name'] . ' = zephir_get_charval(' . $parameterCode . ');' . PHP_EOL;

            case 'bool':
                if ($this->isInternal()) {
                    return "\t" . $parameter['name'] . ' = zephir_get_boolval(' . $parameterCode . ');' . PHP_EOL;
                }

                // Value already passed in `Z_PARAM_BOOL()`
                return '';

            case 'double':
                return "\t" . $parameter['name'] . ' = zephir_get_doubleval(' . $parameterCode . ');' . PHP_EOL;

            case 'string':
                $compilationContext->symbolTable->mustGrownStack(true);

                return "\t" . 'zephir_get_strval(' . $inputParamCode . ', ' . $parameterCode . ');' . PHP_EOL;

            case 'array':
                $compilationContext->symbolTable->mustGrownStack(true);

                return "\t" . 'zephir_get_arrval(' . $inputParamCode . ', ' . $parameterCode . ');' . PHP_EOL;

            default:
                throw new CompilerException('Parameter type: ' . $dataType, $parameter);
        }
    }

    /**
     * Assigns a zval value to a static low-level type.
     *
     * TODO: rewrite this to build ifs and throw from builders
     *
     * @throws CompilerException
     */
    public function checkStrictType(array $parameter, CompilationContext $compilationContext): string
    {
        $dataType = $this->getParamDataType($parameter);

        // Z_PARAM_STR already validates string type for native string params,
        // but we still need to populate the companion zval. See assignZvalValue()
        // for why memory-grow active requires observe + ZVAL_STR_COPY to avoid
        // the refcount leak reported in #2500.
        if ($dataType === 'string') {
            $variable = $compilationContext->symbolTable->getVariable($parameter['name']);
            if ($variable->isNativeString()) {
                if ($compilationContext->symbolTable->getMustGrownStack()) {
                    return "\t" . 'zephir_memory_observe(&' . $parameter['name'] . '_zv);' . PHP_EOL
                        . "\t" . 'ZVAL_STR_COPY(&' . $parameter['name'] . '_zv, ' . $parameter['name'] . ');' . PHP_EOL;
                }

                return "\t" . 'ZVAL_STR(&' . $parameter['name'] . '_zv, ' . $parameter['name'] . ');' . PHP_EOL;
            }
        }

        $compilationContext->headersManager->add('ext/spl/spl_exceptions');
        $compilationContext->headersManager->add('kernel/exception');

        $codePrinter = new Printer();
        $codePrinter->increaseLevel();
        $oldCodePrinter                  = $compilationContext->codePrinter;
        $compilationContext->codePrinter = $codePrinter;
        $compilationContext->backend->checkStrictType($dataType, $parameter, $compilationContext);

        $compilationContext->codePrinter = $oldCodePrinter;

        return $codePrinter->getOutput();
    }

    /**
     * Checks for visibility congruence.
     *
     * @throws CompilerException
     */
    public function checkVisibility(array $visibility, string $name, ?array $original = null): void
    {
        if (count($visibility) > 1) {
            if (in_array('public', $visibility) && in_array('protected', $visibility)) {
                throw new CompilerException(
                    "Method '$name' cannot be 'public' and 'protected' at the same time",
                    $original
                );
            }

            if (in_array('public', $visibility) && in_array('private', $visibility)) {
                throw new CompilerException(
                    "Method '$name' cannot be 'public' and 'private' at the same time",
                    $original
                );
            }

            if (in_array('private', $visibility) && in_array('protected', $visibility)) {
                throw new CompilerException(
                    "Method '$name' cannot be 'protected' and 'private' at the same time",
                    $original
                );
            }

            if (in_array('private', $visibility) && in_array('internal', $visibility)) {
                throw new CompilerException(
                    "Method '$name' cannot be 'internal' and 'private' at the same time",
                    $original
                );
            }

            if (in_array('protected', $visibility) && in_array('internal', $visibility)) {
                throw new CompilerException(
                    "Method '$name' cannot be 'internal' and 'protected' at the same time",
                    $original
                );
            }

            if (in_array('public', $visibility) && in_array('internal', $visibility)) {
                throw new CompilerException(
                    "Method '$name' cannot be 'internal' and 'public' at the same time",
                    $original
                );
            }
        }

        if ('__construct' === $name) {
            if (in_array('static', $visibility)) {
                throw new CompilerException("Constructors cannot be 'static'", $original);
            }
        } elseif ('__destruct' === $name) {
            if (in_array('static', $visibility)) {
                throw new CompilerException("Destructors cannot be 'static'", $original);
            }
        }

        $this->isAbstract = in_array('abstract', $visibility);
        $this->isStatic   = in_array('static', $visibility);
        $this->isFinal    = in_array('final', $visibility);
        $this->isPublic   = in_array('public', $visibility);
        $this->isInternal = in_array('internal', $visibility);
    }

    /**
     * Compiles the method.
     *
     * @throws Exception
     * @throws ReflectionException
     */
    public function compile(CompilationContext $compilationContext): void
    {
        /**
         * Set the method currently being compiled
         */
        $compilationContext->currentMethod = $this;

        /**
         * Assign pre-made compilation passes.
         */
        $typeInference    = $this->typeInference;
        $callGathererPass = $this->callGathererPass;

        /**
         * Initialization of parameters happens in a fictitious external branch.
         */
        $branch = new Branch();
        $branch->setType(Branch::TYPE_EXTERNAL);

        /**
         * BranchManager helps to create graphs of conditional/loop/root/jump branches.
         */
        $branchManager = new BranchManager();
        $branchManager->addBranch($branch);

        $compilationContext->branchManager = $branchManager;

        /**
         * Every method has its own symbol table.
         */
        $symbolTable = new SymbolTable($compilationContext);
        if ($this->localContext instanceof LocalContextPass) {
            $symbolTable->setLocalContext($this->localContext);
        }

        if ($this->definiteAssignment instanceof DefiniteAssignmentPass) {
            $symbolTable->setDefiniteAssignment($this->definiteAssignment);
        }

        if ($this->nativeArray instanceof NativeArrayPass) {
            $symbolTable->setNativeArray($this->nativeArray);
        }

        /**
         * `use (&x)` makes one storage slot shared between this method and the
         * closure, so the local has to be a reference from its very first use.
         * That means knowing about the capture before the body is compiled.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2652
         */
        $byRefCaptures = self::astByRefCaptures($this->statements?->getStatements() ?? []);
        $this->assertByRefCapturesAreLocals($byRefCaptures);
        $symbolTable->setClosureReferences($byRefCaptures);

        $this->capturesToRelease = [];
        foreach ($this->staticVariables as $var) {
            /**
             * A refcountable capture is read off the carrier with a ZVAL_COPY,
             * so the closure needs a memory frame to release it again -
             * without one, every invocation leaked one reference of the
             * captured value. The enclosing object is excluded: it is borrowed
             * with PH_READONLY, pinned by the carrier for the whole call.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2652
             */
            if (
                self::CLOSURE_BOUND_THIS !== $var->getName()
                && !$var->isClosureReference()
                && in_array(
                    $var->getType(),
                    ['variable', 'string', 'array', 'resource', 'callable', 'object', 'mixed'],
                    true
                )
            ) {
                $this->capturesToRelease[$var->getName()] = true;
            }

            $localVar = clone $var;
            $localVar->setIsExternal(true);
            $localVar->setLocalOnly(true);
            $localVar->setDynamicTypes($localVar->getType());
            $localVar->setType('variable');
            $localVar->setIsDoublePointer(false);
            // Captured string params are zend_string * outside, but inside the
            // closure they live as a zval on the carrier. See #2562.
            $localVar->setIsNativeString(false);
            $symbolTable->addRawVariable($localVar);
        }

        /**
         * Set here and not next to the read itself: ReturnStatement consults
         * this flag while the body is compiled, to choose between the MM and
         * non-MM form of its return macros.
         */
        if ([] !== $this->capturesToRelease) {
            $symbolTable->mustGrownStack(true);
        }

        /**
         * Generator step bodies always use a memory frame, and the creator's
         * parameters become plain locals restored from the generator object's
         * slots (the creator seeds slot i with parameter i).
         */
        if ($this->isGeneratorStep()) {
            $symbolTable->mustGrownStack(true);
            $compilationContext->headersManager->add('kernel/generator');

            foreach ($this->getGeneratorCreator()->getCreatorParametersForStep() as $parameter) {
                $paramVar = new Variable(
                    $this->generatorParamLocalType($parameter),
                    $parameter['name'],
                    $compilationContext->branchManager->getCurrentBranch()
                );
                $paramVar->setIsInitialized(true, $compilationContext);
                /* external => declared even when unused; do NOT combine with
                 * setLocalOnly() — external+localOnly means "local static". */
                $paramVar->setIsExternal(true);

                if (isset($parameter['cast'])) {
                    $paramVar->setDynamicTypes('object');
                    $paramVar->setClassTypes($compilationContext->getFullName($parameter['cast']['value']));
                } elseif ('variable' === $paramVar->getType()) {
                    $paramVar->setDynamicTypes('undefined');
                } else {
                    $paramVar->setDynamicTypes($paramVar->getType());
                }

                $symbolTable->addRawVariable($paramVar);
            }
        }

        /**
         * Parameters has an additional extra mutation.
         */
        if ($this->localContext instanceof LocalContextPass && $this->parameters instanceof Parameters) {
            foreach ($this->parameters->getParameters() as $parameter) {
                $this->localContext->increaseMutations($parameter['name']);
            }
        }

        /**
         * Cache Manager manages function calls, method calls and class entries caches.
         */
        $cacheManager = new Manager();
        $cacheManager->setGatherer($callGathererPass);

        $compilationContext->cacheManager  = $cacheManager;
        $compilationContext->typeInference = $typeInference;
        $compilationContext->symbolTable   = $symbolTable;

        $oldCodePrinter = $compilationContext->codePrinter;

        /**
         * Change the code printer to a single method instance.
         */
        $codePrinter                     = new Printer();
        $compilationContext->codePrinter = $codePrinter;

        /**
         * Set an empty function cache
         */
        $compilationContext->functionCache = null;

        /**
         * Reset try/catch and loop counter
         */
        $compilationContext->insideCycle     = 0;
        $compilationContext->switchTargets   = [];
        $compilationContext->switchLabelId   = 0;
        $compilationContext->insideTryCatch  = 0;
        $compilationContext->currentTryCatch = 0;

        /**
         * Reset per-method isset() interned-string cache.
         */
        $compilationContext->issetPropertyCache        = [];
        $compilationContext->issetPropertyCacheCounter = 0;

        /**
         * Reset per-method cached-property-access interned-string cache.
         */
        $compilationContext->propertyNameCache        = [];
        $compilationContext->propertyNameCacheCounter = 0;

        if ($this->parameters instanceof Parameters) {
            /**
             * Round 1. Create variables in parameters in the symbol table.
             */
            $substituteVars = [];
            foreach ($this->parameters->getParameters() as $parameter) {
                $symbolParam = null;

                /**
                 * Variadic parameter: it is not fetched from the call frame
                 * like a normal parameter. Instead it is a local array that we
                 * fill with the trailing arguments. Register it as a plain zval
                 * variable and skip the regular parameter wiring.
                 */
                if (!empty($parameter['variadic'])) {
                    $symbol = $symbolTable->addVariable('variable', $parameter['name'], $compilationContext);
                    $symbol->setMustInitNull(true);
                    $symbol->setIsExternal(true);
                    $symbol->setIsInitialized(true, $compilationContext);
                    $symbol->setDynamicTypes('array');
                    $symbol->setOriginal($parameter);
                    $symbol->increaseUses();
                    $symbolTable->mustGrownStack(true);
                    $compilationContext->headersManager->add('kernel/memory');
                    continue;
                }

                if (isset($parameter['data-type'])) {
                    switch ($parameter['data-type']) {
                        case 'object':
                        case 'callable':
                        case 'resource':
                        case 'variable':
                        case 'mixed':
                            $symbol = $symbolTable->addVariable(
                                $parameter['data-type'],
                                $parameter['name'],
                                $compilationContext
                            );
                            /* TODO: Move this to the respective backend, which requires refactoring how this works */
                            $symbol->setIsDoublePointer(true);
                            $substituteVars[$parameter['name']] = $symbolTable->addVariable(
                                'variable',
                                $parameter['name'] . '_sub',
                                $compilationContext
                            );
                            break;

                        case 'string':
                            /**
                             * Only use native zend_string * for string parameters
                             * that are NOT re-assigned inside the method body.
                             * Mutation count of 1 = the parameter itself; >1 = has
                             * let-assignments in the body.
                             *
                             * Eligible default types: no default, or string literal.
                             * Null defaults → nullable semantics.
                             * static-constant-access defaults → follow-up.
                             */
                            $mutations = ($this->localContext instanceof LocalContextPass)
                                ? $this->localContext->getNumberOfMutations($parameter['name'])
                                : PHP_INT_MAX;

                            $defaultType = $parameter['default']['type'] ?? null;

                            /**
                             * Internal methods are excluded: the native-string
                             * strategy sources the zend_string * from the
                             * caller's frame (Z_PARAM_STR, or ZEND_CALL_ARG for
                             * mixed methods), which an internal method does not
                             * own. They take the zval path so the trailing _ext
                             * pointer stays the single source of truth (#2021).
                             */
                            $canUseNativeString = !$this->isInternal()
                                && $mutations <= 1
                                && (!isset($parameter['default']) || $defaultType === 'string' || $defaultType === 'null');

                            $symbol = $symbolTable->addVariable(
                                'string',
                                $parameter['name'],
                                $compilationContext
                            );

                            if ($canUseNativeString) {
                                $symbol->setIsNativeString(true);
                                $symbol->setMustInitNull(true);

                                if ($defaultType === 'null') {
                                    $symbol->setIsNullableNativeString(true);
                                }

                                /**
                                 * Companion zval used transparently by getVariableCode()
                                 * so that all existing zval-expecting operations work
                                 * without any bridge patches.
                                 */
                                $companion = $symbolTable->addVariable(
                                    'variable',
                                    $parameter['name'] . '_zv',
                                    $compilationContext
                                );
                                $companion->setIsInitialized(true, $compilationContext);
                                $companion->increaseUses();

                                /**
                                 * Optional param with non-null string default allocates
                                 * via zend_string_init(). The companion zval takes
                                 * ownership, so memory-grow is required for the memory
                                 * manager to free it on return.
                                 *
                                 * Null defaults also need memory-grow because
                                 * ZEPHIR_INIT_VAR registers the companion zval in the
                                 * memory frame for cleanup.
                                 *
                                 * Set mustGrownStack EARLY (Round 1, before statement
                                 * compilation) so ReturnStatement sees it and emits
                                 * RETURN_MM_STR instead of RETURN_STR.
                                 */
                                if ($defaultType === 'string' || $defaultType === 'null') {
                                    $symbolTable->mustGrownStack(true);
                                    $compilationContext->headersManager->add('kernel/memory');
                                }
                            } else {
                                /* Mutated or unsupported-default string param: fall back to zval approach */
                                $symbol->setMustInitNull(true);
                            }

                            /**
                             * Only create the _param companion variable for
                             * non-native-string params. Native-string params
                             * are populated directly by Z_PARAM_STR and use
                             * per-param ZEND_CALL_ARG in mixed methods, so
                             * no _param sink is needed.
                             */
                            if (!$canUseNativeString) {
                                $symbolParam = $symbolTable->addVariable(
                                    'variable',
                                    $parameter['name'] . '_param',
                                    $compilationContext
                                );
                                $symbolParam->setIsDoublePointer(true);
                            }
                            break;

                        default:
                            $symbol      = $symbolTable->addVariable(
                                $parameter['data-type'],
                                $parameter['name'],
                                $compilationContext
                            );
                            $symbolParam = $symbolTable->addVariable(
                                'variable',
                                $parameter['name'] . '_param',
                                $compilationContext
                            );
                            $symbolParam->setIsDoublePointer(true);

                            if ('array' == $parameter['data-type']) {
                                $symbol->setMustInitNull(true);
                            }
                            break;
                    }
                } else {
                    $symbol = $symbolTable->addVariable('variable', $parameter['name'], $compilationContext);
                }

                /**
                 * ZE3 only
                 */
                if (isset($substituteVars[$parameter['name']])) {
                    $substituteVar = $substituteVars[$parameter['name']];
                    $substituteVar->increaseUses();
                }

                /**
                 * Some parameters can be read-only
                 */
                if (!empty($parameter['const'])) {
                    $symbol->setReadOnly(true);
                    if (is_object($symbolParam)) {
                        $symbolParam->setReadOnly(true);
                    }
                }

                if (is_object($symbolParam)) {
                    /**
                     * Parameters are marked as 'external'
                     */
                    $symbolParam->setIsExternal(true);

                    /**
                     * Assuming they're initialized
                     */
                    $symbolParam->setIsInitialized(true, $compilationContext);

                    /**
                     * Initialize auxiliary parameter zvals to null
                     */
                    $symbolParam->setMustInitNull(true);

                    /**
                     * Increase uses
                     */
                    $symbolParam->increaseUses();
                } else {
                    if (isset($parameter['default'])) {
                        if (isset($parameter['data-type'])) {
                            if ('variable' === $parameter['data-type']) {
                                $symbol->setMustInitNull(true);
                            }
                        } else {
                            $symbol->setMustInitNull(true);
                        }
                    }
                }

                /**
                 * Original node where the variable was declared
                 */
                $symbol->setOriginal($parameter);

                /**
                 * Parameters are marked as 'external'
                 */
                $symbol->setIsExternal(true);

                /**
                 * Assuming they're initialized
                 */
                $symbol->setIsInitialized(true, $compilationContext);

                /**
                 * Variables with class/type must be objects across the execution
                 */
                if (isset($parameter['cast'])) {
                    $symbol->setDynamicTypes('object');
                    $symbol->setClassTypes($compilationContext->getFullName($parameter['cast']['value']));
                } else {
                    if (isset($parameter['data-type'])) {
                        if ('variable' === $parameter['data-type']) {
                            $symbol->setDynamicTypes('undefined');
                        }
                    } else {
                        $symbol->setDynamicTypes('undefined');
                    }
                }
            }
        }

        $compilationContext->backend->onPreCompile($this, $compilationContext);

        /**
         * Compile the block of statements if any
         */
        if (is_object($this->statements)) {
            $compilationContext->staticContext = $this->hasModifier('static');

            /**
             * Compile the statements block as a 'root' branch
             */
            $this->statements->compile($compilationContext, false, Branch::TYPE_ROOT);
        }

        /**
         * Generator step: prepend the resume dispatch (restore suspended
         * locals from the generator object, then jump to the yield point).
         * preOutput() prepends, so this lands right before the body and
         * after every later-preOutput'ed preamble section.
         */
        if ($this->isGeneratorStep()) {
            $codePrinter->preOutput($this->generatorDispatchCode($compilationContext));
        }

        /**
         * Generator creator: the visible method's body is replaced by
         * "create the <Ns>\Generator object, seed it with the parameters,
         * return it". Parameter fetching/coercion above runs unchanged, so
         * argument errors surface at call time exactly like PHP generators.
         */
        if ($this->isGeneratorCreator()) {
            $this->generatorCreatorCode($compilationContext);
        }

        /**
         * Initialize variable default values.
         */
        $initVarCode = $compilationContext->backend->initializeVariableDefaults(
            $symbolTable->getVariables(),
            $compilationContext
        );

        /**
         * Fetch parameters from vm-top.
         */
        $initCode       = '';
        $code           = '';
        $requiredParams = [];
        $optionalParams = [];
        if ($this->parameters instanceof Parameters) {
            /**
             * Round 2. Fetch the parameters in the method.
             */
            $params               = $this->parameters->fetchParameters($this->isInternal);
            $numberRequiredParams = $this->parameters->countRequiredParameters();
            $numberOptionalParams = $this->parameters->countOptionalParameters();
            $requiredParams       = $this->parameters->getRequiredParameters();
            $optionalParams       = $this->parameters->getOptionalParameters();

            /**
             * Count native-string params to determine fetch strategy.
             * - All native: skip fetching entirely (Phase 5).
             * - Mixed (some native, some zval): emit per-param
             *   ZEND_CALL_ARG for non-native params (Phase 5b).
             * - No native: use zephir_fetch_params as before.
             */
            $nativeStringCount = 0;
            $totalParamCount = 0;
            foreach ($this->parameters->getParameters() as $parameter) {
                if (!empty($parameter['variadic'])) {
                    continue;
                }
                $totalParamCount++;
                $name = $parameter['name'];
                $variable = $compilationContext->symbolTable->getVariable($name);
                if ($variable && $variable->isNativeString()) {
                    $nativeStringCount++;
                }
            }

            $perParamFetchCode = '';
            if ($nativeStringCount === $totalParamCount && $totalParamCount > 0) {
                // All params are native strings — skip zephir_fetch_params
                $params = [];
            } elseif ($nativeStringCount > 0) {
                // Mixed method — replace zephir_fetch_params with per-param
                // ZEND_CALL_ARG() calls for non-native params.
                $params = [];
                $allParams = $this->parameters->getParameters();
                foreach ($allParams as $index => $parameter) {
                    if (!empty($parameter['variadic'])) {
                        continue;
                    }
                    $position = $index + 1; // 1-based
                    $pName = $parameter['name'];
                    $pVariable = $compilationContext->symbolTable->getVariable($pName);
                    if ($pVariable && $pVariable->isNativeString()) {
                        continue; // Z_PARAM_STR already populates it
                    }

                    $pDataType = $parameter['data-type'] ?? 'variable';
                    $target = match ($pDataType) {
                        'object', 'callable', 'resource', 'variable', 'mixed' => $pName,
                        default => $pName . '_param',
                    };

                    if ($position <= $numberRequiredParams) {
                        $perParamFetchCode .= "\t"
                            . $target . ' = ZEND_CALL_ARG(execute_data, ' . $position . ');'
                            . PHP_EOL;
                    } else {
                        $perParamFetchCode .= "\t"
                            . 'if (ZEND_NUM_ARGS() > ' . ($position - 1) . ') {'
                            . PHP_EOL
                            . "\t\t" . $target . ' = ZEND_CALL_ARG(execute_data, ' . $position . ');'
                            . PHP_EOL
                            . "\t" . '}'
                            . PHP_EOL;
                    }
                }
            }

            /**
             * Pass the write detector to the method statement block to check if the parameter
             * variable is modified so as do the proper separation.
             */
            $parametersToSeparate = [];
            if (is_object($this->statements)) {
                if (!$this->localContext instanceof LocalContextPass) {
                    $writeDetector = new WriteDetector();
                }

                foreach ($this->parameters->getParameters() as $parameter) {
                    if (!empty($parameter['variadic'])) {
                        continue;
                    }
                    $dataType = $parameter['data-type'] ?? 'variable';

                    switch ($dataType) {
                        case 'variable':
                        case 'string':
                        case 'array':
                        case 'resource':
                        case 'object':
                        case 'callable':
                            $name = $parameter['name'];
                            if (!$this->localContext instanceof LocalContextPass) {
                                if ($writeDetector->detect($name, $this->statements->getStatements())) {
                                    $parametersToSeparate[$name] = true;
                                }
                            } else {
                                if ($this->localContext->getNumberOfMutations($name) > 1) {
                                    $parametersToSeparate[$name] = true;
                                }
                            }
                            break;
                    }
                }
            }

            /**
             * Emit a deprecation notice for every `!` (strict type) parameter.
             * PHP now enforces scalar argument types itself, so the `!` modifier
             * is redundant and will be removed; a future parser will no longer
             * recognize it (see #2274, precursor to #2275).
             */
            foreach ($this->parameters->getParameters() as $parameter) {
                if (!empty($parameter['variadic'])) {
                    continue;
                }
                if ($parameter['mandatory'] ?? 0) {
                    $compilationContext->logger->warning(
                        sprintf(
                            "The '!' (strict type) modifier on parameter '%s' (%s) is deprecated "
                            . "and will be removed; the parser will no longer recognize it in a "
                            . "future version. Remove the '!' in %s::%s",
                            $parameter['name'],
                            $this->getParamDataType($parameter),
                            $this->getClassDefinition()?->getCompleteName() ?? '[unknown]',
                            $this->getDeclaredName()
                        ),
                        ['deprecated-strict-type', $parameter]
                    );
                }
            }

            /**
             * Initialize required parameters
             */
            foreach ($requiredParams as $parameter) {
                $mandatory = $parameter['mandatory'] ?? 0;
                $dataType  = $this->getParamDataType($parameter);

                if ('variable' !== $dataType) {
                    /**
                     * Assign value from zval to low level type
                     */
                    if ($mandatory) {
                        $initCode .= $this->checkStrictType($parameter, $compilationContext);
                    } else {
                        $initCode .= $this->assignZvalValue($parameter, $compilationContext);
                    }
                }

                switch ($dataType) {
                    case 'variable':
                    case 'resource':
                    case 'object':
                    case 'callable':
                        if (isset($parametersToSeparate[$parameter['name']])) {
                            $symbolTable->mustGrownStack(true);
                            $initCode .= "\t" . 'ZEPHIR_SEPARATE_PARAM(' . $parameter['name'] . ');' . PHP_EOL;
                        }
                        break;
                }
            }

            /**
             * Initialize optional parameters
             */
            foreach ($optionalParams as $parameter) {
                $mandatory = $parameter['mandatory'] ?? 0;
                $dataType  = $this->getParamDataType($parameter);

                $name = match ($dataType) {
                    'object',
                    'callable',
                    'resource',
                    'variable',
                    'mixed' => $parameter['name'],
                    default => $parameter['name'] . '_param',
                };

                /**
                 * Native zend_string * optional params use the bare variable
                 * name — the null-pointer check `if (!name)` replaces the
                 * double-pointer check `if (!name_param)`.
                 */
                if ($dataType === 'string') {
                    $variable = $compilationContext->symbolTable->getVariable($parameter['name']);
                    if ($variable && $variable->isNativeString()) {
                        $name = $parameter['name'];
                    }
                }

                /**
                 * Assign the default value according to the variable's type.
                 */
                $targetVar = $compilationContext->symbolTable->getVariableForWrite($name, $compilationContext);
                $initCode  .= "\t" . $compilationContext->backend->ifVariableValueUndefined(
                    $targetVar,
                    $compilationContext,
                    false,
                    false
                ) . PHP_EOL;

                if ($targetVar->isDoublePointer() && isset($substituteVars[$parameter['name']])) {
                    $substituteVar = $substituteVars[$parameter['name']];
                    $initCode      .= "\t\t"
                        . $targetVar->getName()
                        . ' = &'
                        . $substituteVar->getName()
                        . ';'
                        . PHP_EOL;
                }
                $initCode .= $this->assignDefaultValue($parameter, $compilationContext);

                if (isset($parametersToSeparate[$name]) || 'variable' !== $dataType) {
                    $initCode .= "\t" . '} else {' . PHP_EOL;

                    if (isset($parametersToSeparate[$name])) {
                        $initCode .= "\t\t" . 'ZEPHIR_SEPARATE_PARAM(' . $name . ');' . PHP_EOL;
                    } else {
                        if ($mandatory) {
                            $initCode .= $this->checkStrictType($parameter, $compilationContext);
                        } else {
                            $initCode .= "\t" . $this->assignZvalValue($parameter, $compilationContext);
                        }
                    }
                }

                $initCode .= "\t" . '}' . PHP_EOL;
            }

            /**
             * Fetch the parameters to zval pointers
             */
            $codePrinter->preOutputBlankLine();

            if (!$this->isInternal()) {
                /**
                 * Skip zephir_fetch_params entirely when all params are native
                 * strings (populated directly by Z_PARAM_STR / Z_PARAM_STR_OR_NULL)
                 * or when using per-param ZEND_CALL_ARG (mixed methods).
                 */
                if (!empty($perParamFetchCode)) {
                    $code .= $perParamFetchCode;
                } elseif ($this->parameters->hasVariadicParameter() && !empty($params)) {
                    /**
                     * Fetch only the fixed (leading) parameters; the variadic
                     * fetch tolerates calls with more arguments than declared.
                     */
                    $compilationContext->headersManager->add('kernel/memory');
                    $code .= "\t"
                        . 'zephir_fetch_params_variadic(1, '
                        . $numberRequiredParams
                        . ', '
                        . $numberOptionalParams
                        . ', '
                        . implode(', ', $params)
                        . ');'
                        . PHP_EOL;
                } elseif (!empty($params)) {
                    $compilationContext->headersManager->add('kernel/memory');
                    if ($symbolTable->getMustGrownStack()) {
                        $code .= "\t"
                            . 'zephir_fetch_params(1, '
                            . $numberRequiredParams
                            . ', '
                            . $numberOptionalParams
                            . ', '
                            . implode(', ', $params)
                            . ');'
                            . PHP_EOL;
                    } else {
                        $code .= "\t"
                            . 'zephir_fetch_params_without_memory_grow('
                            . $numberRequiredParams
                            . ', '
                            . $numberOptionalParams
                            . ', '
                            . implode(', ', $params)
                            . ');'
                            . PHP_EOL;
                    }
                }
            } else {
                foreach ($params as $param) {
                    /* TODO: Migrate all this code to codeprinter, get rid of temp code printer */
                    $tempCodePrinter                 = new Printer();
                    $realCodePrinter                 = $compilationContext->codePrinter;
                    $compilationContext->codePrinter = $tempCodePrinter;
                    $paramVar                        = $compilationContext->symbolTable->getVariableForRead(
                        $param,
                        $compilationContext
                    );
                    $compilationContext->backend->assignZval($paramVar, $param . '_ext', $compilationContext);
                    $code                            .= "\t" . $tempCodePrinter->getOutput() . PHP_EOL;
                    $compilationContext->codePrinter = $realCodePrinter;
                }
            }
        }

        $code .= $initCode . $initVarCode;

        /**
         * Populate the variadic parameter with the trailing arguments.
         */
        if ($this->parameters instanceof Parameters && $this->parameters->hasVariadicParameter()) {
            $variadicParameter = $this->parameters->getVariadicParameter();
            $variadicName      = $variadicParameter['name'];
            $fixedCount        = 0;
            foreach ($this->parameters->getParameters() as $parameter) {
                if (empty($parameter['variadic'])) {
                    $fixedCount++;
                }
            }

            $compilationContext->headersManager->add('kernel/memory');
            $compilationContext->headersManager->add('kernel/main');
            $code .= "\t" . 'ZEPHIR_INIT_VAR(&' . $variadicName . ');' . PHP_EOL;
            $code .= "\t" . 'zephir_get_args_from(&' . $variadicName . ', ' . $fixedCount . ');' . PHP_EOL;
        }

        $codePrinter->preOutput($code);

        $compilationContext->headersManager->add('kernel/object');

        /**
         * Promote every `use (&x)` local to a PHP reference before the body
         * runs, so the closure and this method share one storage slot.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2652
         */
        foreach ($symbolTable->getVariables() as $variable) {
            if ($variable->isClosureReference() && !$variable->isClosureCapture()) {
                $codePrinter->preOutput(
                    "\t" . 'zephir_make_local_reference(&' . $variable->getName() . ');'
                );
                $codePrinter->preOutput(
                    "\t" . 'zephir_memory_observe(&' . $variable->getName() . ');'
                );
            }
        }

        /**
         * A capturing closure binds its capture carrier as `$this`, so the
         * captures are read off it below and `this_ptr` is then re-pointed at
         * the enclosing object that rode along on the carrier. `preOutput()`
         * prepends, so the re-point is emitted first in order to land last.
         *
         * Resolving the carrier through getVariableCode() instead of writing
         * `this_ptr` literally is load-bearing: it marks `this` as used, and
         * otherwise the strip in compile() deletes the
         * `zval *this_ptr = getThis();` line these reads need, for any closure
         * whose body never mentions `this`.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2652
         */
        $carrierCode = null;
        if ($this->hasCaptures()) {
            $carrierCode = $compilationContext->backend->getVariableCode($symbolTable->getVariable('this'));

            if ($symbolTable->hasVariable(self::CLOSURE_BOUND_THIS)) {
                $codePrinter->preOutput("\t" . 'this_ptr = &' . self::CLOSURE_BOUND_THIS . ';');
            }
        }

        /**
         * Fetch used superglobals and closure captures
         */
        foreach ($symbolTable->getVariables() as $name => $variable) {
            if ($variable->isSuperGlobal()) {
                $globalVar = $symbolTable->getVariable($name);
                $codePrinter->preOutput(
                    "\t" . $compilationContext->backend->fetchGlobal($globalVar, $compilationContext, false)
                );
            }

            if ($variable->isClosureCapture()) {
                $capture     = $symbolTable->getVariable($name);
                $captureName = $capture->getName();
                $mustRelease = isset($this->capturesToRelease[$captureName]);

                $codePrinter->preOutput(
                    sprintf(
                        "\t" . 'zephir_read_property(&%s, %s, SL("%s"), %s);',
                        $captureName,
                        $carrierCode,
                        $captureName,
                        $mustRelease ? 'PH_NOISY_CC' : 'PH_NOISY_CC | PH_READONLY'
                    )
                );

                if ($mustRelease) {
                    $codePrinter->preOutput("\t" . 'zephir_memory_observe(&' . $captureName . ');');
                }
            }
        }

        /**
         * Grow the stack if needed
         */
        if ($symbolTable->getMustGrownStack()) {
            $compilationContext->headersManager->add('kernel/memory');
            if (!$compilationContext->symbolTable->hasVariable('ZEPHIR_METHOD_GLOBALS_PTR')) {
                $methodGlobals = new Variable('zephir_method_globals', 'ZEPHIR_METHOD_GLOBALS_PTR', $compilationContext->branchManager->getCurrentBranch());
                $methodGlobals->setMustInitNull(true);
                $methodGlobals->increaseUses();
                $methodGlobals->setReusable(false);
                $methodGlobals->setReadOnly(true);
                $methodGlobals->setUsed(true);
                $compilationContext->symbolTable->addRawVariable($methodGlobals);
            }

            // #define ZEPHIR_MM_GROW()
            $codePrinter->preOutput("\t" . 'zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);');
            $codePrinter->preOutput("\t" . 'ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);');
        }

        /**
         * Check if there are unused variables.
         */
        $usedVariables = [];
        $completeName  = $this->getClassDefinition()?->getCompleteName() ?: '[unknown]';
        $thisUnused = false;

        foreach ($symbolTable->getVariables() as $variable) {
            if ($variable->getNumberUses() <= 0) {
                if (!$variable->isExternal()) {
                    $compilationContext->logger->warning(
                        'Variable "'
                        . $variable->getName()
                        . '" declared but not used in '
                        . $completeName
                        . '::'
                        . $this->getDeclaredName(),
                        ['unused-variable', $variable->getOriginal()]
                    );
                    continue;
                }

                $compilationContext->logger->warning(
                    'Variable "'
                    . $variable->getName()
                    . '" declared but not used in '
                    . $completeName
                    . '::'
                    . $this->getDeclaredName(),
                    ['unused-variable-external', $variable->getOriginal()]
                );
            }

            if (
                'this_ptr' !== $variable->getName() &&
                'return_value' !== $variable->getName() &&
                'return_value_ptr' !== $variable->getName()
            ) {
                $type = $variable->getType();
                // Native string params use zend_string * instead of zval for declaration
                if ($type === 'string' && $variable->isNativeString()) {
                    $type = 'zend_string';
                }
                if (!isset($usedVariables[$type])) {
                    $usedVariables[$type] = [];
                }

                $usedVariables[$type][] = $variable;
            }

            if ('this_ptr' === $variable->getName() && !$variable->isUsed()) {
                $thisUnused = true;
            }
        }

        /**
         * Check if there are assigned but not used variables
         * Warn whenever a variable is unused aside from its declaration.
         */
        foreach ($symbolTable->getVariables() as $variable) {
            if ($variable->isExternal() || $variable->isTemporal()) {
                continue;
            }

            if (
                'this_ptr' === $variable->getName() ||
                'return_value' === $variable->getName() ||
                'return_value_ptr' === $variable->getName() ||
                'ZEPHIR_LAST_CALL_STATUS' === $variable->getName()
            ) {
                continue;
            }

            if (!$variable->isUsed()) {
                $node = $variable->getLastUsedNode();
                if (is_array($node)) {
                    $expression = $node['expr'] ?? $node;
                    $compilationContext->logger->warning(
                        'Variable "'
                        . $variable->getName()
                        . '" assigned but not used in '
                        . $completeName
                        . '::'
                        . $this->getDeclaredName(),
                        ['unused-variable', $expression]
                    );
                } else {
                    $compilationContext->logger->warning(
                        'Variable "'
                        . $variable->getName()
                        . '" assigned but not used in '
                        . $completeName
                        . '::'
                        . $this->getDeclaredName(),
                        ['unused-variable', $variable->getOriginal()]
                    );
                }
            }

            /**
             * The counterpart of the check above: read, but nothing ever wrote
             * to it. The value now reads as null, which is what PHP evaluates
             * an unset variable to, and PHP says so at runtime. Zephir knows it
             * at compile time, so it says so here.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2654
             */
            if ($variable->getNumberUses() > 0 && $variable->isNeverAssigned()) {
                $compilationContext->logger->warning(
                    'Variable "'
                    . $variable->getName()
                    . '" read but never assigned in '
                    . $completeName
                    . '::'
                    . $this->getDeclaredName(),
                    ['unassigned-variable', $variable->getOriginal()]
                );
            }

            /**
             * The middle case between the two above: something does write to
             * it, but not on every path that reaches a read. PHP evaluates such
             * a read as null and says so at runtime; the compiler now makes the
             * value null too and says so here.
             *
             * Never both this and `unassigned-variable`: registering the slot
             * leaves a variant init behind, which is what isNeverAssigned()
             * rules out.
             *
             * @see Variable::isReadBeforeAssignment()
             * @see https://github.com/zephir-lang/zephir/issues/2679
             */
            if ($variable->isReadBeforeAssignment()) {
                $compilationContext->logger->warning(
                    'Variable "'
                    . $variable->getName()
                    . '" may be read before it is assigned in '
                    . $completeName
                    . '::'
                    . $this->getDeclaredName()
                    . ', consider initializing it at its declaration',
                    ['conditional-initialization', $variable->getOriginal()]
                );
            }
        }

        if (count($usedVariables)) {
            $codePrinter->preOutputBlankLine();
        }

        /**
         * ZEND_PARSE_PARAMETERS
         *
         * Generator steps skip it: they are invoked C-to-C by the kernel with
         * the resuming caller's execute_data, whose argument frame has no
         * relation to the step's own parameter (the generator object arrives
         * via the trailing _ext pointer).
         *
         * Internal methods skip it for exactly the same reason: the
         * ZEPHIR_CALL_INTERNAL_METHOD_P* macros pass the caller's execute_data
         * straight through and hand the real arguments over as trailing _ext
         * pointers. Parsing that frame read the caller's arguments instead of
         * the callee's, and crashed outright whenever the two arities differed
         * (#2021).
         */
        $tempCodePrinter = new Printer();
        if (
            $this->parameters instanceof Parameters
            && $this->parameters->count() > 0
            && !$this->isGeneratorStep()
            && !$this->isInternal()
        ) {
            // Do not declare variable when it is not needed.
            if ($this->parameters->hasNullableParameters()) {
                $tempCodePrinter->output("\t" . 'bool is_null_true = 1;');
            }

            $maxParameters = $this->parameters->hasVariadicParameter()
                ? '-1'
                : (string) $this->parameters->count();

            $tempCodePrinter->output(
                sprintf(
                    "\t" . 'ZEND_PARSE_PARAMETERS_START(%d, %s)',
                    $this->parameters->countRequiredParameters(),
                    $maxParameters
                )
            );

            foreach ($requiredParams as $requiredParam) {
                $tempCodePrinter->output("\t\t" . $this->detectParam($requiredParam, $compilationContext));
            }

            if (!empty($optionalParams)) {
                $tempCodePrinter->output("\t\t" . 'Z_PARAM_OPTIONAL');

                foreach ($optionalParams as $optionalParam) {
                    $tempCodePrinter->output("\t\t" . $this->detectParam($optionalParam, $compilationContext));
                }
            }

            $tempCodePrinter->output("\t" . 'ZEND_PARSE_PARAMETERS_END();');
        }

        $codePrinter->preOutput($tempCodePrinter->getOutput());

        /**
         * Emit method-scope interned zend_string slots for cached property
         * access (see Backend::internedPropertyName). Declared at function
         * scope — all `static` decls first, then the lazy inits — so a
         * reference from any nested block is valid. Prepended here so it sits
         * just below the variable declarations added just after this.
         */
        if (!empty($compilationContext->propertyNameCache)) {
            $slotPrinter = new Printer();
            foreach ($compilationContext->propertyNameCache as $slotVar) {
                $slotPrinter->output("\t" . 'static zend_string *' . $slotVar . ' = NULL;');
            }
            foreach ($compilationContext->propertyNameCache as $slotName => $slotVar) {
                $slotPrinter->output("\t" . 'if (UNEXPECTED(!' . $slotVar . ')) {');
                $slotPrinter->output(
                    "\t\t" . $slotVar . ' = zend_string_init("' . addslashes($slotName) . '", '
                    . strlen($slotName) . ', 1);'
                );
                $slotPrinter->output("\t" . '}');
            }
            $codePrinter->preOutput($slotPrinter->getOutput());
        }

        /**
         * Generate the variable definition for variables used.
         */
        $initCode = sprintf(
            "\t%s",
            implode(
                PHP_EOL . "\t",
                $compilationContext->backend->declareVariables(
                    $this,
                    $usedVariables
                )
            )
        );

        $codePrinter->preOutput($initCode);

        /**
         * Finalize the method compilation
         */
        if (is_object($this->statements) && !empty($this->statements->getLastStatement())) {
            /**
             * When control cannot reach the end of the body there is nothing to
             * restore and no `return` missing. Otherwise the memory stack has to
             * be restored, and a declared return type is left unsatisfied.
             */
            if (self::OUTCOME_RETURNS !== $this->statementsOutcome($this->statements->getStatements())) {
                if ($this->isGeneratorStep()) {
                    /* Falling off the end finishes the generator (getReturn() => NULL). */
                    $genVariable = $compilationContext->symbolTable->getVariableForRead(
                        GeneratorTransformer::GEN_PARAM,
                        $compilationContext
                    );
                    $codePrinter->output(
                        "\t" . 'zephir_generator_finish('
                        . $compilationContext->backend->getVariableCode($genVariable)
                        . ', NULL);'
                    );
                }

                if ($symbolTable->getMustGrownStack()) {
                    $compilationContext->headersManager->add('kernel/memory');
                    $codePrinter->output("\t" . 'ZEPHIR_MM_RESTORE();');
                }

                /**
                 * If a method has return-type hints we need to ensure the last
                 * statement is a 'return' statement
                 */
                if ($this->hasReturnTypes()) {
                    throw new CompilerException(
                        'Reached end of the method without returning a valid type specified in the return-type hints',
                        $this->expression['return-type']
                    );
                }
            }
        }

        $compilationContext->backend->onPostCompile($this, $compilationContext);

        /**
         * Remove macros that grow/restore the memory frame stack if it wasn't used.
         */
        $code = $this->removeMemoryStackReferences($symbolTable, $codePrinter->getOutput());

        /**
         * Generator step post-processing:
         *  - expand every yield-point save marker with the per-slot save
         *    sequence (the full local set is only known now);
         *  - make first-init macros resume-safe: restored locals are already
         *    observed in the fresh frame, so plain INIT/observe would
         *    double-observe (debug builds abort) or leak.
         */
        if ($this->isGeneratorStep()) {
            $code = str_replace(
                '//%ZEPHIR_GEN_SAVE%',
                $this->generatorSaveCode($compilationContext),
                $code
            );
            $code = str_replace('ZEPHIR_INIT_VAR(', 'ZEPHIR_INIT_NVAR(', $code);
            $code = str_replace('zephir_memory_observe(', 'ZEPHIR_OBS_NVAR(', $code);
        }

        /**
         * Remove unused this_ptr variable.
         */
        if ($thisUnused) {
            $code = str_replace("\t" . 'zval *this_ptr = getThis();' . PHP_EOL, '', $code);
        }

        $code = preg_replace("/(\R){3,}/", "$1", $code);

        /**
         * Restore the compilation context
         */
        $oldCodePrinter->output($code, false);
        $compilationContext->codePrinter = $oldCodePrinter;

        $compilationContext->branchManager = null;
        $compilationContext->cacheManager  = null;
        $compilationContext->typeInference = null;

        $codePrinter->clear();
    }

    /**
     * Determine Z_PARAM_*
     *
     * @throws Exception
     */
    public function detectParam(array $parameter, CompilationContext $compilationContext): string
    {
        $name = $parameter['name'];
        if (!isset($parameter['data-type'])) {
            return sprintf('Z_PARAM_ZVAL(%s)', $name);
        }

        /**
         * In case of unknown type, just return generic param type.
         */
        $hasDefaultNull = isset($parameter['default']['type']) && $parameter['default']['type'] === 'null';
        if ($hasDefaultNull) {
            $param = sprintf('Z_PARAM_ZVAL_OR_NULL(%s)', $name);
        } else {
            $param = sprintf('Z_PARAM_ZVAL(%s)', $name);
        }

        switch ($parameter['data-type']) {
            case 'array':
                if ($hasDefaultNull) {
                    $param = sprintf('ZEPHIR_Z_PARAM_ARRAY_OR_NULL(%s, %s_param)', $name, $name);
                } else {
                    $param = sprintf('ZEPHIR_Z_PARAM_ARRAY(%s, %s_param)', $name, $name);
                }

                break;

            case 'bool':
                if ($hasDefaultNull) {
                    $param = sprintf('Z_PARAM_BOOL_OR_NULL(%s, is_null_true)', $name);
                } else {
                    $param = sprintf('Z_PARAM_BOOL(%s)', $name);
                }

                break;

            case 'double':
                if ($hasDefaultNull) {
                    $param = sprintf('Z_PARAM_ZVAL_OR_NULL(%s_param)', $name);
                } else {
                    $param = sprintf('Z_PARAM_ZVAL(%s_param)', $name);
                }

                break;

            case 'float':
                if ($hasDefaultNull) {
                    $param = sprintf('Z_PARAM_DOUBLE_OR_NULL(%s, is_null_true)', $name);
                } else {
                    $param = sprintf('Z_PARAM_DOUBLE(%s)', $name);
                }

                break;

            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
                if ($hasDefaultNull) {
                    $param = sprintf('Z_PARAM_LONG_OR_NULL(%s, is_null_true)', $name);
                } else {
                    $param = sprintf('Z_PARAM_LONG(%s)', $name);
                }

                break;

            case 'char':
            case 'uchar':
                if ($hasDefaultNull) {
                    $param = sprintf('Z_PARAM_ZVAL_OR_NULL(%s_param)', $name);
                } else {
                    $param = sprintf('Z_PARAM_ZVAL(%s_param)', $name);
                }

                break;

            case 'object':
                if ($hasDefaultNull) {
                    $param = sprintf('Z_PARAM_OBJECT_OF_CLASS_OR_NULL(%s)', $name);
                } else {
                    $param = sprintf('Z_PARAM_OBJECT(%s)', $name);
                }

                break;

            case 'resource':
                if ($hasDefaultNull) {
                    $param = sprintf('Z_PARAM_RESOURCE_OR_NULL(%s)', $name);
                } else {
                    $param = sprintf('Z_PARAM_RESOURCE(%s)', $name);
                }

                break;

            case 'string':
                $variable = $compilationContext->symbolTable->getVariable($name);
                if ($variable && $variable->isNativeString()) {
                    if ($hasDefaultNull) {
                        $param = sprintf('Z_PARAM_STR_OR_NULL(%s)', $name);
                    } else {
                        $param = sprintf('Z_PARAM_STR(%s)', $name);
                    }
                } else {
                    if ($hasDefaultNull) {
                        $param = sprintf('Z_PARAM_ZVAL_OR_NULL(%s_param)', $name);
                    } else {
                        $param = sprintf('Z_PARAM_ZVAL(%s_param)', $name);
                    }
                }

                break;

            case 'variable':
                if (
                    isset($parameter['cast']) &&
                    $parameter['cast']['type'] === 'variable' &&
                    $parameter['cast']['value'] &&
                    $this->classDefinition !== null
                ) {
                    $classEntry = (new ClassEntry($parameter['cast']['value'], $compilationContext))->get();
                    if ($hasDefaultNull) {
                        $param = sprintf('Z_PARAM_OBJECT_OF_CLASS_OR_NULL(%s, %s)', $name, $classEntry);
                    } else {
                        $param = sprintf('Z_PARAM_OBJECT_OF_CLASS(%s, %s)', $name, $classEntry);
                    }
                }

                break;
        }

        return $param;
    }

    /**
     * Returns arginfo name for current method.
     */
    public function getArgInfoName(?Definition $classDefinition = null): string
    {
        if ($classDefinition instanceof Definition) {
            return sprintf(
                'arginfo_%s_%s_%s',
                strtolower($classDefinition->getCNamespace()),
                strtolower($classDefinition->getName()),
                strtolower($this->getName())
            );
        }

        return sprintf('arginfo_%s', strtolower($this->getInternalName()));
    }

    /**
     * Returns the call gatherer pass information.
     */
    public function getCallGathererPass(): CallGathererPass
    {
        return $this->callGathererPass;
    }

    /**
     * Returns the class definition where the method was declared.
     */
    public function getClassDefinition(): ?Definition
    {
        return $this->classDefinition;
    }

    /**
     * Returns the raw docblock.
     */
    public function getDocBlock(): ?string
    {
        return $this->docblock;
    }

    public function getInternalName(): string
    {
        $classDefinition = $this->getClassDefinition();

        return 'zep_' . $classDefinition->getCNamespace() . '_' . $classDefinition->getName() . '_' . $this->getName();
    }

    /**
     * the starting line of the method in the source file.
     */
    public function getLine(): mixed
    {
        return $this->expression['line'] ?? null;
    }

    public function getMayBeArgTypes(): array
    {
        return $this->mayBeArgTypes;
    }

    /**
     * Returns the C-modifier flags.
     *
     * @throws Exception
     */
    public function getModifiers(): string
    {
        $modifiers = [];
        foreach ($this->visibility as $visibility) {
            switch ($visibility) {
                case 'public':
                    $modifiers['ZEND_ACC_PUBLIC'] = $visibility;
                    break;

                case 'protected':
                    $modifiers['ZEND_ACC_PROTECTED'] = $visibility;
                    break;

                case 'private':
                    $modifiers['ZEND_ACC_PRIVATE'] = $visibility;
                    break;

                case 'static':
                    $modifiers['ZEND_ACC_STATIC'] = $visibility;
                    break;

                case 'final':
                    $modifiers['ZEND_ACC_FINAL'] = $visibility;
                    break;

                case 'abstract':
                    $modifiers['ZEND_ACC_ABSTRACT'] = $visibility;
                    break;

                case 'deprecated':
                    $modifiers['ZEND_ACC_DEPRECATED'] = $visibility;
                    break;

                case 'inline':
                case 'scoped':
                case 'internal':
                    break;

                default:
                    throw new Exception('Unknown modifier "' . $visibility . '"');
            }
        }

        if ('__construct' === $this->name) {
            $modifiers['ZEND_ACC_CTOR'] = true;
        } elseif ('__destruct' === $this->name) {
            $modifiers['ZEND_ACC_DTOR'] = true;
        }

        return implode('|', array_keys($modifiers));
    }

    /**
     * Returns the method name as written in the .zep source.
     *
     * Same as {@see getName()} unless this method is a compiler-generated copy
     * of another one, in which case it is the name of the method it copies.
     */
    public function getDeclaredName(): string
    {
        return $this->declaredName ?? $this->name;
    }

    /**
     * Returns the method name.
     */
    public function getName(): string
    {
        return $this->name;
    }

    /**
     * Returns the number of parameters the method has.
     */
    public function getNumberOfParameters(): int
    {
        if ($this->parameters instanceof Parameters) {
            return $this->parameters->count();
        }

        return 0;
    }

    /**
     * Whether the method declares a variadic parameter (e.g. `...rest`) and
     * therefore accepts an unbounded number of trailing arguments.
     */
    public function isVariadic(): bool
    {
        return $this->parameters instanceof Parameters
            && $this->parameters->hasVariadicParameter();
    }

    /**
     * Returns the number of required parameters the method has.
     */
    public function getNumberOfRequiredParameters(): int
    {
        if ($this->parameters === null) {
            return 0;
        }

        $required = 0;
        foreach ($this->parameters->getParameters() as $parameter) {
            if (!empty($parameter['variadic'])) {
                continue;
            }
            if (!isset($parameter['default'])) {
                ++$required;
            }
        }

        return $required;
    }

    public function getOptimizedMethod(): self
    {
        $optimizedName   = $this->getName() . '_zephir_internal_call';
        $optimizedMethod = $this->classDefinition->getMethod($optimizedName, false);
        if (!$optimizedMethod || !$this->optimizable) {
            return $this;
        }

        return $optimizedMethod;
    }

    /**
     * Returns the parameters.
     */
    public function getParameters(): ?Parameters
    {
        return $this->parameters;
    }

    /**
     * Returns the parsed docblock.
     */
    public function getParsedDocBlock(): ?Docblock
    {
        if ($this->parsedDocblock) {
            return $this->parsedDocblock;
        }

        if ($this->docblock === null) {
            return null;
        }

        $parser               = new DocblockParser('/' . $this->docblock . '/');
        $this->parsedDocblock = $parser->parse();

        return $this->parsedDocblock;
    }

    /**
     * Returned class-type hints by the method.
     */
    public function getReturnClassTypes(): array
    {
        return $this->returnClassTypes;
    }

    /**
     * Returned type hints by the method.
     */
    public function getReturnTypes(): array
    {
        return $this->returnTypes;
    }

    public function getReturnTypesRaw(): array
    {
        return $this->returnTypesRaw;
    }

    /**
     * Return shortcut method name.
     */
    public function getShortcutName(): mixed
    {
        return $this->expression['name'];
    }

    /**
     * Getter for statements block.
     */
    public function getStatementsBlock(): StatementsBlock
    {
        return $this->statements;
    }

    /**
     * Returns method visibility modifiers.
     */
    public function getVisibility(): array
    {
        return $this->visibility;
    }

    /**
     * Issue #1706: tells whether a `switch` statement is guaranteed to return
     * (or throw) on every path.
     *
     * Every clause has to end up returning, because any of them can be the one
     * that matches. A clause that neither returns nor jumps away falls through
     * into the clause written after it (issue #1704), so it returns exactly
     * when that next clause does - which leaves the last clause having to
     * return on its own. Without a `default` clause a non-matching value skips
     * the whole `switch`, so it is never exhaustive.
     */
    private function switchAlwaysReturns(array $statement): bool
    {
        if (empty($statement['clauses']) || !is_array($statement['clauses'])) {
            return false;
        }

        $clauses    = array_values($statement['clauses']);
        $hasDefault = false;
        foreach ($clauses as $clause) {
            if ('default' === ($clause['type'] ?? null)) {
                $hasDefault = true;
                break;
            }
        }

        if (!$hasDefault) {
            return false;
        }

        /**
         * Walk backwards so that each clause can be answered against the one
         * it falls into. Past the last clause control leaves the `switch`
         * without returning, hence the initial false.
         */
        $nextClauseReturns = false;
        for ($index = count($clauses) - 1; $index >= 0; --$index) {
            $outcome = $this->statementsOutcome($clauses[$index]['statements'] ?? []);

            if (self::OUTCOME_JUMPS === $outcome) {
                /**
                 * A `break`/`continue` leaves the `switch` without returning,
                 * so the method can still fall off its end.
                 */
                return false;
            }

            if (self::OUTCOME_FALLS === $outcome && !$nextClauseReturns) {
                return false;
            }

            $nextClauseReturns = true;
        }

        return true;
    }

    /**
     * How control leaves a list of statements, as far as the enclosing method
     * is concerned.
     *
     * The scan stops at the first statement that transfers control
     * unconditionally, so a dead `break` written after a `return` as padding is
     * never reached and cannot mask the `return` in front of it.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1704
     */
    private function statementsOutcome(array $statements): string
    {
        foreach ($statements as $statement) {
            $outcome = $this->statementOutcome($statement);

            if (self::OUTCOME_FALLS !== $outcome) {
                return $outcome;
            }
        }

        return self::OUTCOME_FALLS;
    }

    /**
     * How control leaves a single statement. Anything that does not transfer
     * control of its own accord just falls through to the next one.
     */
    private function statementOutcome(array $statement): string
    {
        $type = $statement['type'] ?? null;

        if ('return' === $type || 'throw' === $type) {
            return self::OUTCOME_RETURNS;
        }

        if ('break' === $type || 'continue' === $type) {
            return self::OUTCOME_JUMPS;
        }

        if ('if' === $type) {
            return $this->ifOutcome($statement);
        }

        if ('try-catch' === $type) {
            return $this->tryCatchOutcome($statement);
        }

        if (in_array($type, self::LOOP_TYPES, true)) {
            return $this->loopOutcome($statement);
        }

        if ('switch' === $type) {
            /**
             * A `switch` swallows its own `break`s, so it either returns on
             * every path or control continues after it.
             */
            return $this->switchAlwaysReturns($statement)
                ? self::OUTCOME_RETURNS
                : self::OUTCOME_FALLS;
        }

        return self::OUTCOME_FALLS;
    }

    /**
     * How control leaves a `try`/`catch`.
     *
     * Without `catch` clauses normal flow simply continues with the outcome of
     * the `try` body. With them every body has to leave the method for the
     * statement as a whole to do so, because any of them can be the one that
     * runs.
     */
    private function tryCatchOutcome(array $statement): string
    {
        $outcomes = [$this->statementsOutcome($statement['statements'] ?? [])];

        foreach ($statement['catches'] ?? [] as $catch) {
            $outcomes[] = $this->statementsOutcome($catch['statements'] ?? []);
        }

        if (in_array(self::OUTCOME_JUMPS, $outcomes, true)) {
            return self::OUTCOME_JUMPS;
        }

        return in_array(self::OUTCOME_FALLS, $outcomes, true)
            ? self::OUTCOME_FALLS
            : self::OUTCOME_RETURNS;
    }

    /**
     * How control leaves a loop.
     *
     * A loop whose condition is not a constant truth - and every `for`, whose
     * iterand can be empty - may run zero times, so control just continues
     * after it. An infinite loop is left only by a `break` that targets it;
     * without one the code after the loop is unreachable and the sole way out
     * is a `return`/`throw`.
     */
    private function loopOutcome(array $statement): string
    {
        if (!$this->isInfiniteLoop($statement)) {
            return self::OUTCOME_FALLS;
        }

        return $this->containsLoopBreak($statement['statements'] ?? [])
            ? self::OUTCOME_FALLS
            : self::OUTCOME_RETURNS;
    }

    /**
     * Tells whether a loop can only be left by jumping out of it: a `loop`, or
     * a `while`/`do-while` whose condition is a literal truth.
     */
    private function isInfiniteLoop(array $statement): bool
    {
        $type = $statement['type'] ?? null;

        if ('loop' === $type) {
            return true;
        }

        if ('while' !== $type && 'do-while' !== $type) {
            return false;
        }

        $expr     = $statement['expr'] ?? [];
        $exprType = $expr['type'] ?? null;

        if ('bool' === $exprType) {
            return 'true' === ($expr['value'] ?? null);
        }

        return 'int' === $exprType && 0 !== (int) ($expr['value'] ?? 0);
    }

    /**
     * Tells whether a loop body holds a `break` that targets that loop.
     *
     * The walk enters `if` arms and `try`/`catch` bodies, but stops at a nested
     * loop or `switch`, each of which captures a `break` of its own.
     */
    private function containsLoopBreak(array $statements): bool
    {
        foreach ($statements as $statement) {
            $type = $statement['type'] ?? null;

            if ('break' === $type) {
                return true;
            }

            if ('if' === $type) {
                $bodies = [
                    $statement['statements'] ?? [],
                    $statement['else_statements'] ?? [],
                ];

                foreach ($statement['elseif_statements'] ?? [] as $elseIf) {
                    $bodies[] = $elseIf['statements'] ?? [];
                }
            } elseif ('try-catch' === $type) {
                $bodies = [$statement['statements'] ?? []];

                foreach ($statement['catches'] ?? [] as $catch) {
                    $bodies[] = $catch['statements'] ?? [];
                }
            } else {
                continue;
            }

            foreach ($bodies as $body) {
                if ($this->containsLoopBreak($body)) {
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * How control leaves an `if` statement, taking every arm into account: the
     * `if` body, each `elseif` body, and either the `else` body or - when there
     * is none - the implicit arm on which execution simply continues.
     *
     * An arm that jumps out makes the whole statement able to jump out, which
     * is what stops a `break` hidden in a branch from being mistaken for an
     * exhaustive clause.
     */
    private function ifOutcome(array $statement): string
    {
        $arms = [$this->statementsOutcome($statement['statements'] ?? [])];

        foreach ($statement['elseif_statements'] ?? [] as $elseIf) {
            $arms[] = $this->statementsOutcome($elseIf['statements'] ?? []);
        }

        $arms[] = isset($statement['else_statements'])
            ? $this->statementsOutcome($statement['else_statements'])
            : self::OUTCOME_FALLS;

        if (in_array(self::OUTCOME_JUMPS, $arms, true)) {
            return self::OUTCOME_JUMPS;
        }

        return in_array(self::OUTCOME_FALLS, $arms, true)
            ? self::OUTCOME_FALLS
            : self::OUTCOME_RETURNS;
    }

    /**
     * Checks whether the method has a specific modifier.
     */
    public function hasModifier(string $modifier): bool
    {
        return in_array($modifier, $this->visibility);
    }

    /**
     * Returns the number of parameters the method has.
     */
    public function hasParameters(): bool
    {
        return $this->parameters instanceof Parameters && $this->parameters->count() > 0;
    }

    /**
     * Checks if the method has return-type or cast hints.
     */
    public function hasReturnTypes(): bool
    {
        return count($this->returnTypes) || count($this->returnClassTypes);
    }

    public function hasReturnTypesRaw(): bool
    {
        return !empty($this->returnTypesRaw);
    }

    /**
     * Checks is abstract method?
     */
    public function isAbstract(): bool
    {
        return $this->isAbstract;
    }

    /**
     * Checks whether the method is bundled.
     */
    public function isBundled(): bool
    {
        return $this->isBundled;
    }

    /**
     * Check whether the current method is a constructor.
     */
    public function isConstructor(): bool
    {
        return '__construct' === $this->name;
    }

    public function isDeprecated(): bool
    {
        return !empty($this->visibility) && in_array('deprecated', $this->visibility, true);
    }

    /**
     * Checks whether the method is empty.
     */
    public function isEmpty(): bool
    {
        return $this->statements->isEmpty();
    }

    /**
     * Checks whether the method is final.
     */
    public function isFinal(): bool
    {
        return $this->isFinal;
    }

    /**
     * Checks whether the method body contains a `yield` statement, which
     * means the method is a PHP generator. Result is cached; the underlying
     * AST walk runs at most once per method instance. Returns false when the
     * method has no statements block (abstract/external methods).
     *
     * Code-generation of generator bodies is not yet implemented;
     * The API is exposed now so passes and future codegen can branch on it cleanly.
     *
     * @see https://github.com/zephir-lang/zephir/issues/1849
     */
    public function isGenerator(): bool
    {
        if ($this->isGenerator !== null) {
            return $this->isGenerator;
        }

        if (!$this->statements instanceof StatementsBlock) {
            return $this->isGenerator = false;
        }

        return $this->isGenerator = (new YieldDetector())->detect(
            $this->statements->getStatements()
        );
    }

    /**
     * Raw method AST node (used by generator diagnostics for file/line info).
     */
    public function getExpression(): array
    {
        return $this->expression ?: [];
    }

    /**
     * Marks this method as a generator creator and wires its hidden step
     * method. The creator keeps signature/arginfo but compiles to
     * "return new <Ns>\Generator(...)": its statements are detached (they
     * move to the step method).
     */
    public function setGeneratorStep(Method $step): void
    {
        $this->generatorStep = $step;
        $this->statements    = null;
    }

    public function setGeneratorCreator(Method $creator): void
    {
        $this->generatorCreator = $creator;
    }

    public function isGeneratorCreator(): bool
    {
        return $this->generatorStep instanceof Method;
    }

    public function isGeneratorStep(): bool
    {
        return $this->generatorCreator instanceof Method;
    }

    public function getGeneratorStep(): ?Method
    {
        return $this->generatorStep;
    }

    public function getGeneratorCreator(): ?Method
    {
        return $this->generatorCreator;
    }

    /**
     * Allocates the next 1-based yield suspension point while compiling a
     * generator step body (called by YieldStatement).
     */
    public function allocateYieldPoint(): int
    {
        return ++$this->yieldPoints;
    }

    public function getYieldPoints(): int
    {
        return $this->yieldPoints;
    }

    /**
     * Parameters of the creator method in declaration order: they occupy the
     * generator object's slots [0..P-1] (seeded by the creator, restored by
     * the step's dispatch block).
     */
    public function getCreatorParametersForStep(): array
    {
        return $this->parameters instanceof Parameters ? $this->parameters->getParameters() : [];
    }

    /**
     * Local variable type used inside the step body for a creator parameter.
     * The creator boxes every argument into a zval slot, so native-string
     * and cast parameters become plain zvals here.
     */
    private function generatorParamLocalType(array $parameter): string
    {
        if (!empty($parameter['variadic']) || isset($parameter['cast'])) {
            return 'variable';
        }

        return match ($parameter['data-type'] ?? 'variable') {
            'int', 'uint', 'long', 'ulong', 'char', 'uchar',
            'double', 'bool', 'array', 'string' => $parameter['data-type'],
            default                             => 'variable',
        };
    }

    /**
     * How a local is preserved across a suspension: boxed zval copy, boxed
     * scalar, or not at all (null). Cache pointers (fcall/ce/function/property
     * caches) are deliberately not saved — their `= NULL` declaration
     * initializers rerun on every step invocation, costing one re-lookup.
     */
    private function generatorSlotClass(Variable $variable): ?string
    {
        static $skipNames = [
            'this_ptr',
            'return_value',
            'return_value_ptr',
            'ZEPHIR_LAST_CALL_STATUS',
            'ZEPHIR_METHOD_GLOBALS_PTR',
            GeneratorTransformer::GEN_PARAM,
            GeneratorTransformer::GEN_PARAM . '_sub',
        ];

        if (in_array($variable->getName(), $skipNames, true)) {
            return null;
        }
        if (
            $variable->isSuperGlobal()
            || $variable->isClosureCapture()
            || $variable->isDoublePointer()
            || $variable->isNativeString()
            /* Non-tracked temps are statement-scoped by construction and are
             * (re)initialized with plain ZVAL_* macros: restoring a value
             * into them would leak it on the next in-place overwrite. */
            || !$variable->isMemoryTracked()
        ) {
            return null;
        }

        return match ($variable->getType()) {
            'variable', 'string', 'array', 'null', 'mixed'                   => 'zval',
            'int', 'uint', 'long', 'ulong', 'char', 'uchar', 'zend_ulong',
            'HashPosition'                                                   => 'long',
            'double'                                                         => 'double',
            'bool', 'zephir_ce_guard'                                        => 'bool',
            default                                                          => null,
        };
    }

    /**
     * Slot index map for the step's symbol table: creator parameters keep
     * their declaration index, every other suspendable local follows in
     * name order (deterministic regeneration).
     *
     * @var array<string, array{index: int, class: ?string, var: ?Variable}>
     */
    private array $generatorSlots = [];

    private function generatorBuildSlotMap(CompilationContext $compilationContext): void
    {
        $this->generatorSlots = [];
        $symbolTable          = $compilationContext->symbolTable;
        $index                = 0;

        foreach ($this->getGeneratorCreator()->getCreatorParametersForStep() as $parameter) {
            $variable = $symbolTable->getVariable($parameter['name']);

            $this->generatorSlots[$parameter['name']] = [
                'index' => $index++,
                'class' => $variable instanceof Variable ? $this->generatorSlotClass($variable) : null,
                'var'   => $variable,
            ];
        }

        $variables = $symbolTable->getVariables();
        ksort($variables, SORT_STRING);
        foreach ($variables as $name => $variable) {
            if (!$variable instanceof Variable || isset($this->generatorSlots[$name])) {
                continue;
            }
            if ($variable->getNumberUses() <= 0 && !$variable->isExternal()) {
                continue; // never declared
            }
            $class = $this->generatorSlotClass($variable);
            if (null === $class) {
                continue;
            }
            $this->generatorSlots[$name] = ['index' => $index++, 'class' => $class, 'var' => $variable];
        }
    }

    /**
     * The resume dispatch block injected between the method preamble and the
     * body: restore every suspendable local from its slot (UNDEF slots are
     * skipped), then jump to the suspension point recorded in the generator.
     */
    private function generatorDispatchCode(CompilationContext $compilationContext): string
    {
        $this->generatorBuildSlotMap($compilationContext);

        $genVariable = $compilationContext->symbolTable->getVariableForRead(
            GeneratorTransformer::GEN_PARAM,
            $compilationContext
        );
        $gen = $compilationContext->backend->getVariableCode($genVariable);

        $lines   = [];
        $lines[] = "\t" . '/* Generator resume dispatch (issue #1849) */';
        $lines[] = "\t" . 'zephir_generator_slots_ensure(' . $gen . ', ' . count($this->generatorSlots) . ');';

        foreach ($this->generatorSlots as $slot) {
            if (null === $slot['class'] || !$slot['var'] instanceof Variable) {
                continue;
            }
            $lines[] = "\t" . $this->generatorRestoreLine($slot, $gen, $compilationContext);
        }

        if ($this->yieldPoints > 0) {
            $lines[] = "\t" . 'switch (zephir_generator_get_state(' . $gen . ')) {';
            for ($n = 1; $n <= $this->yieldPoints; ++$n) {
                $lines[] = "\t\t" . 'case ' . $n . ': goto zephir_yield_resume_' . $n . ';';
            }
            $lines[] = "\t\t" . 'default: break;';
            $lines[] = "\t" . '}';
        }

        return implode(PHP_EOL, $lines);
    }

    private function generatorRestoreLine(array $slot, string $gen, CompilationContext $compilationContext): string
    {
        /** @var Variable $variable */
        $variable = $slot['var'];
        $index    = $slot['index'];
        $slotExpr = 'zephir_generator_slot(' . $gen . ', ' . $index . ')';

        switch ($slot['class']) {
            case 'zval':
                return 'ZEPHIR_GEN_RESTORE_ZVAL('
                    . $gen . ', ' . $index . ', '
                    . $compilationContext->backend->getVariableCode($variable) . ');';

            case 'long':
                [, $cType] = $compilationContext->backend->getTypeDefinition($variable->getType());

                return 'if (Z_TYPE_P(' . $slotExpr . ') != IS_UNDEF) { '
                    . $variable->getName() . ' = (' . $cType . ') Z_LVAL_P(' . $slotExpr . '); }';

            case 'double':
                return 'if (Z_TYPE_P(' . $slotExpr . ') != IS_UNDEF) { '
                    . $variable->getName() . ' = Z_DVAL_P(' . $slotExpr . '); }';

            case 'bool':
                return 'if (Z_TYPE_P(' . $slotExpr . ') != IS_UNDEF) { '
                    . $variable->getName() . ' = (Z_TYPE_P(' . $slotExpr . ') == IS_TRUE); }';
        }

        return '';
    }

    /**
     * The per-slot save sequence substituted for every yield point's
     * //%ZEPHIR_GEN_SAVE% marker once the full symbol table is known.
     * zval saves are copies: the frame's reference is released by the
     * ZEPHIR_MM_RESTORE() that follows the marker, leaving the slot as the
     * single owner while suspended.
     */
    private function generatorSaveCode(CompilationContext $compilationContext): string
    {
        $genVariable = $compilationContext->symbolTable->getVariableForRead(
            GeneratorTransformer::GEN_PARAM,
            $compilationContext
        );
        $gen = $compilationContext->backend->getVariableCode($genVariable);

        $lines = [];
        foreach ($this->generatorSlots as $slot) {
            if (null === $slot['class'] || !$slot['var'] instanceof Variable) {
                continue;
            }
            /** @var Variable $variable */
            $variable = $slot['var'];
            $index    = $slot['index'];

            $lines[] = match ($slot['class']) {
                'zval'   => 'zephir_generator_slot_set(' . $gen . ', ' . $index . ', '
                    . $compilationContext->backend->getVariableCode($variable) . ');',
                'long'   => 'zephir_generator_slot_set_long(' . $gen . ', ' . $index . ', (zend_long) '
                    . $variable->getName() . ');',
                'double' => 'zephir_generator_slot_set_double(' . $gen . ', ' . $index . ', '
                    . $variable->getName() . ');',
                'bool'   => 'zephir_generator_slot_set_bool(' . $gen . ', ' . $index . ', (int) '
                    . $variable->getName() . ');',
            };
        }

        return implode(PHP_EOL . "\t\t", $lines);
    }

    /**
     * Body of a generator creator: parameter fetching/coercion above runs
     * unchanged; here the <Ns>\Generator object is created, seeded with the
     * (already coerced) arguments and returned.
     */
    private function generatorCreatorCode(CompilationContext $compilationContext): void
    {
        $codePrinter = $compilationContext->codePrinter;
        $symbolTable = $compilationContext->symbolTable;

        $compilationContext->headersManager->add('kernel/generator');

        $thisArg = 'NULL';
        if (!$this->isStatic()) {
            /* The symbol is named 'this' (low name this_ptr); reading it marks
             * the getThis() preamble line as required. */
            $thisVariable = $symbolTable->getVariableForRead('this', $compilationContext);
            $thisVariable->setUsed(true);
            $thisArg = 'this_ptr';
        }

        $parameters = $this->getCreatorParametersForStep();

        $codePrinter->output(
            "\t" . 'zephir_generator_create(return_value, ' . $thisArg . ', '
            . $this->classDefinition->getClassEntry($compilationContext) . ', '
            . $this->generatorStep->getInternalName() . ', '
            . count($parameters) . ');'
        );

        foreach ($parameters as $index => $parameter) {
            $variable = $symbolTable->getVariableForRead($parameter['name'], $compilationContext);

            $codePrinter->output("\t" . $this->generatorSeedLine($variable, (int)$index, $compilationContext));
        }

        $codePrinter->output("\t" . 'ZEPHIR_MM_RESTORE();');
        $codePrinter->output("\t" . 'return;');
    }

    private function generatorSeedLine(Variable $variable, int $index, CompilationContext $compilationContext): string
    {
        $name = $variable->getName();

        if ($variable->isNativeString()) {
            return 'zephir_generator_slot_set_str(return_value, ' . $index . ', ' . $name . ');';
        }

        return match ($variable->getType()) {
            'int', 'uint', 'long', 'ulong', 'char', 'uchar', 'zend_ulong'
                     => 'zephir_generator_slot_set_long(return_value, ' . $index . ', (zend_long) ' . $name . ');',
            'double' => 'zephir_generator_slot_set_double(return_value, ' . $index . ', ' . $name . ');',
            'bool'   => 'zephir_generator_slot_set_bool(return_value, ' . $index . ', (int) ' . $name . ');',
            default  => 'zephir_generator_slot_set(return_value, ' . $index . ', '
                . $compilationContext->backend->getVariableCode($variable) . ');',
        };
    }

    /**
     * Checks whether the method is an initializer.
     */
    public function isInitializer(): bool
    {
        return $this->isInitializer;
    }

    /**
     * Checks whether the method is internal.
     */
    public function isInternal(): bool
    {
        return $this->isInternal;
    }

    /**
     * Checks if the methods return type is `mixed`.
     */
    public function isMixed(): bool
    {
        return $this->mixed;
    }

    /**
     * Checks if the method is private.
     */
    public function isPrivate(): bool
    {
        return in_array('private', $this->visibility);
    }

    /**
     * Checks if the method is protected.
     */
    public function isProtected(): bool
    {
        return in_array('protected', $this->visibility);
    }

    /**
     * Checks if the method is public.
     */
    public function isPublic(): bool
    {
        return $this->isPublic;
    }

    /**
     * Checks if method's return type is nullable object `?object`.
     */
    public function isReturnTypeNullableObject(): bool
    {
        return count($this->returnTypes) === 2
            && isset($this->returnTypes['object'])
            && isset($this->returnTypes['null']);
    }

    /**
     * Checks if method's return type is object `object`.
     */
    public function isReturnTypeObject(): bool
    {
        return count($this->returnTypes) === 1 && isset($this->returnTypes['object']);
    }

    /**
     * Is method have determined return type hint.
     *
     * This method is used to generate:
     *
     * - ZEND_BEGIN_ARG_INFO_EX
     * - ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX
     *
     * Examples:
     *
     * - TRUE: function foo() -> void;
     * - TRUE: function foo() -> null;
     * - TRUE: function foo() -> bool|string|...;
     * - TRUE: function foo() -> <\stdClass>;
     * - FALSE: function foo();
     * - FALSE: function foo() -> var;
     * - FALSE: function foo() -> resource|callable;
     */
    public function isReturnTypesHintDetermined(): bool
    {
        if ($this->isVoid()) {
            return true;
        }

        if (0 === count($this->returnTypes)) {
            return false;
        }

        foreach ($this->returnTypes as $returnType => $definition) {
            switch ($returnType) {
                case 'variable':
                case 'callable':
                case 'resource':
                    return false;
            }

            if (isset($definition['type']) && 'return-type-annotation' === $definition['type']) {
                if (
                    $this->areReturnTypesBoolCompatible() ||
                    $this->areReturnTypesDoubleCompatible() ||
                    $this->areReturnTypesIntCompatible() ||
                    $this->areReturnTypesNullCompatible() ||
                    $this->areReturnTypesStringCompatible() ||
                    $this->areReturnTypesFalseCompatible() ||
                    $this->areReturnTypesObjectCompatible() ||
                    array_key_exists('array', $this->getReturnTypes())
                ) {
                    continue;
                }

                /**
                 * TODO: Probably we should detect return type more more carefully.
                 * It is hard to process return type from the annotations at this time.
                 * Thus we just return false here.
                 */
                return false;
            }
        }

        return true;
    }

    /**
     * Checks if method is a shortcut.
     */
    public function isShortcut(): bool
    {
        return $this->expression && 'shortcut' === $this->expression['type'];
    }

    /**
     * Checks whether the method is static.
     */
    public function isStatic(): bool
    {
        return $this->isStatic;
    }

    /**
     * Checks if the method must not return any value.
     */
    public function isVoid(): bool
    {
        return $this->void;
    }

    /**
     * Pre-compiles the method making compilation pass data (static inference, local-context-pass) available to other
     * methods.
     *
     * @throws CompilerException
     */
    public function preCompile(CompilationContext $compilationContext): void
    {
        $definiteAssignment = null;
        $localContext       = null;
        $typeInference      = null;
        $callGathererPass   = null;
        $nativeArray        = null;

        if (is_object($this->statements)) {
            $compilationContext->currentMethod = $this;

            /**
             * Which locals the user declared without a value are read before
             * anything assigns them. Not an optimization and so not switchable:
             * it decides whether such a read produces null, as PHP does, or
             * hands userland the IS_UNDEF the declaration left behind.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2679
             */
            $definiteAssignment = new DefiniteAssignmentPass();
            $definiteAssignment->pass($this->statements);

            /**
             * Which locals can only hold a native array, so a subscript read
             * of one may borrow the value the container owns instead of taking
             * a reference to it. Not an optimization either: an ArrayAccess
             * container owns nothing after offsetGet() returns, so borrowing
             * from one leaks the value or frees it under its own target.
             *
             * @see https://github.com/zephir-lang/zephir/issues/2682
             */
            $nativeArray = new NativeArrayPass();
            if ($this->parameters instanceof Parameters) {
                $nativeArray->passParameters($this->parameters->getParameters());
            }
            $nativeArray->pass($this->statements);

            /**
             * This pass checks for zval variables than can be potentially
             * used without allocating memory and track it
             * these variables are stored in the stack
             *
             * Generator step bodies skip it: suspension moves locals into the
             * generator object, so stack-only allocation assumptions no
             * longer hold across yield points.
             */
            if (!$this->isGeneratorStep() && $compilationContext->config->get('local-context-pass', 'optimizations')) {
                $localContext = new LocalContextPass();
                $localContext->pass($this->statements);
            }

            /**
             * This pass tries to infer types for dynamic variables
             * replacing them by low level variables
             *
             * Also skipped for generator step bodies: values that cross a
             * yield point are boxed/unboxed through the generator object, so
             * conservative dynamic typing is the safe default.
             */
            if (!$this->isGeneratorStep() && $compilationContext->config->get('static-type-inference', 'optimizations')) {
                $typeInference = new StaticTypeInference();
                $typeInference->pass($this->statements);
                if ($compilationContext->config->get('static-type-inference-second-pass', 'optimizations')) {
                    $typeInference->reduce();
                    $typeInference->pass($this->statements);
                }
            }

            /**
             * This pass counts how many times a specific
             */
            if ($compilationContext->config->get('call-gatherer-pass', 'optimizations')) {
                $callGathererPass = new CallGathererPass($compilationContext);
                $callGathererPass->pass($this->statements);
            }
        }

        $this->definiteAssignment = $definiteAssignment;
        $this->localContext       = $localContext;
        $this->nativeArray        = $nativeArray;
        $this->typeInference      = $typeInference;
        $this->callGathererPass   = $callGathererPass;
    }

    /**
     * Replace macros.
     *
     * @deprecated
     */
    public function removeMemoryStackReferences(SymbolTable $symbolTable, string $containerCode): string
    {
        if ($symbolTable->getMustGrownStack()) {
            return $containerCode;
        }

        $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_STR', 'ZEPHIR_THROW_EXCEPTION_STRW', $containerCode);
        $containerCode = str_replace(
            'ZEPHIR_THROW_EXCEPTION_DEBUG_STR',
            'ZEPHIR_THROW_EXCEPTION_DEBUG_STRW',
            $containerCode
        );
        $containerCode = str_replace('ZEPHIR_THROW_EXCEPTION_ZVAL', 'ZEPHIR_THROW_EXCEPTION_ZVALW', $containerCode);
        $containerCode = str_replace('RETURN_THIS', 'RETURN_THISW', $containerCode);
        $containerCode = str_replace('RETURN_LCTOR', 'RETURN_LCTORW', $containerCode);
        $containerCode = str_replace('RETURN_CTOR', 'RETURN_CTORW', $containerCode);
        $containerCode = str_replace('RETURN_NCTOR', 'RETURN_NCTORW', $containerCode);
        $containerCode = str_replace('RETURN_CCTOR', 'RETURN_CCTORW', $containerCode);
        $containerCode = str_replace('RETURN_MM_NULL', 'RETURN_NULL', $containerCode);
        $containerCode = str_replace('RETURN_MM_BOOL', 'RETURN_BOOL', $containerCode);
        $containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
        $containerCode = str_replace('RETURN_MM_TRUE', 'RETURN_TRUE', $containerCode);
        $containerCode = str_replace('RETURN_MM_STRING', 'RETURN_STRING', $containerCode);
        $containerCode = str_replace('RETURN_MM_LONG', 'RETURN_LONG', $containerCode);
        $containerCode = str_replace('RETURN_MM_DOUBLE', 'RETURN_DOUBLE', $containerCode);
        $containerCode = str_replace('RETURN_MM_FALSE', 'RETURN_FALSE', $containerCode);
        $containerCode = str_replace('RETURN_MM_EMPTY_STRING', 'RETURN_MM_EMPTY_STRING', $containerCode);
        $containerCode = str_replace('RETURN_MM_EMPTY_ARRAY', 'RETURN_EMPTY_ARRAY', $containerCode);
        $containerCode = str_replace('RETURN_MM_MEMBER', 'RETURN_MEMBER', $containerCode);
        $containerCode = str_replace('RETURN_MM()', 'return', $containerCode);

        return preg_replace('/[ \t]+ZEPHIR_MM_RESTORE\(\);' . PHP_EOL . '/s', '', $containerCode);
    }

    /**
     * A `use (&x)` capture needs `x` to be a plain zval local it can turn into
     * a reference. A parameter arrives in a shape that cannot be promoted - a
     * `zval *` borrowed from the caller, or an unboxed C scalar - so say so
     * rather than emit C that does not compile.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2652
     *
     * @param string[] $names
     */
    private function assertByRefCapturesAreLocals(array $names): void
    {
        if ([] === $names || null === $this->parameters) {
            return;
        }

        foreach ($this->parameters->getParameters() as $parameter) {
            if (in_array($parameter['name'], $names, true)) {
                throw new CompilerException(
                    "Cannot capture parameter '" . $parameter['name'] . "' by reference in "
                    . $this->getDeclaredName() . '(); copy it into a local variable first',
                    $parameter
                );
            }
        }
    }

    /**
     * Names captured by reference by any closure literal in this AST.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2652
     *
     * @return string[]
     */
    private static function astByRefCaptures(mixed $node): array
    {
        if (!is_array($node)) {
            return [];
        }

        $names = [];

        if (($node['type'] ?? null) === 'closure' && is_array($node['use'] ?? null)) {
            foreach ($node['use'] as $parameter) {
                if (!empty($parameter['reference']) && isset($parameter['name'])) {
                    $names[] = $parameter['name'];
                }
            }
        }

        foreach ($node as $child) {
            if (is_array($child)) {
                $names = array_merge($names, self::astByRefCaptures($child));
            }
        }

        return array_values(array_unique($names));
    }

    /**
     * Whether this method is a closure `__invoke` whose `$this` is a capture
     * carrier rather than the enclosing object.
     *
     * @see https://github.com/zephir-lang/zephir/issues/2652
     */
    public function hasCaptures(): bool
    {
        return [] !== $this->staticVariables;
    }

    /**
     * Sets if the method is bundled or not.
     */
    public function setIsBundled(bool $bundled): void
    {
        $this->isBundled = $bundled;
    }

    /**
     * Sets if the method is an initializer or not.
     */
    public function setIsInitializer(bool $initializer): void
    {
        $this->isInitializer = $initializer;
    }

    /**
     * Sets if the method is internal or not.
     */
    public function setIsStatic(bool $static): void
    {
        $this->isStatic = $static;
    }

    /**
     * Records the name of the method this one is a generated copy of.
     */
    public function setDeclaredName(string $name): void
    {
        $this->declaredName = $name;
    }

    /**
     * Sets the method name.
     */
    public function setName(string $name): void
    {
        $this->name = $name;
    }

    /**
     * Process RAW return types structure.
     *
     * Example:
     *
     * ```
     * $returnType = [
     *  'type' => 'return-type',
     *  'list' => [
     *      [
     *          'type' => 'return-type-parameter',
     *          'cast' => [
     *              'type' => 'variable',
     *              'value' => '\StdClass',
     *              'file' => './stubs.zep',
     *              'line' => 21,
     *              'char' => 48
     *          ],
     *          'collection' => 1,
     *          'file' => './stubs.zep',
     *          'line' => 21,
     *          'char' => 48
     *      ],
     *      [
     *          'type' => 'return-type-parameter',
     *          'data-type' => 'bool',
     *          'mandatory' => 0,
     *          'file' => './stubs.zep',
     *          'line' => 22,
     *          'char' => 5
     *      ]
     *  ],
     *  'void' => 0,
     *  'file' => './stubs.zep',
     *  'line' => 22,
     *  'char' => 5
     * ];
     * ```
     *
     * @param array|null $returnType
     */
    public function setReturnTypes(?array $returnType = null): void
    {
        $this->returnTypesRaw = $returnType;
        if (null === $returnType) {
            return;
        }

        if (isset($returnType['void']) && $returnType['void']) {
            $this->void = true;

            return;
        }

        if (!isset($returnType['list'])) {
            return;
        }

        $types     = [];
        $castTypes = [];

        foreach ($returnType['list'] as $returnTypeItem) {
            /**
             * We continue the loop, because it only works for PHP >= 8.0.
             */
            if (isset($returnTypeItem['data-type']) && $returnTypeItem['data-type'] === 'mixed') {
                $this->mixed = true;
            }

            if (!isset($returnTypeItem['cast'])) {
                $types[$returnTypeItem['data-type']] = $returnTypeItem;
                continue;
            }

            if (isset($returnTypeItem['cast']['collection'])) {
                continue;
            }

            if (isset($returnTypeItem['collection']) && $returnTypeItem['collection']) {
                $types['array'] = [
                    'type'      => 'return-type-parameter',
                    'data-type' => 'array',
                    'mandatory' => 0,
                    'file'      => $returnTypeItem['cast']['file'],
                    'line'      => $returnTypeItem['cast']['line'],
                    'char'      => $returnTypeItem['cast']['char'],
                ];
            } else {
                $castTypes[$returnTypeItem['cast']['value']] = $returnTypeItem['cast']['value'];
            }
        }

        if (count($castTypes) > 0) {
            $types['object']        = [];
            $this->returnClassTypes = $castTypes;
        }

        if (count($types) > 0) {
            $this->returnTypes = $types;
        }
    }

    /**
     * Setter for statements block.
     */
    public function setStatementsBlock(StatementsBlock $statementsBlock): void
    {
        $this->statements = $statementsBlock;
    }

    /**
     * Generate internal method's based on the equivalent PHP methods,
     * allowing bypassing php userspace for internal method calls.
     */
    public function setupOptimized(CompilationContext $compilationContext): self
    {
        if (!$compilationContext->config->get('internal-call-transformation', 'optimizations')) {
            return $this;
        }

        $classDefinition = $this->getClassDefinition();

        /**
         * Skip for closures
         */
        if ('__invoke' === $this->getName() || $classDefinition->isInterface()) {
            return $this;
        }

        if (!$this->isInternal() && !$classDefinition->isBundled()) {
            /* Not supported for now */
            if ($this->getNumberOfRequiredParameters() != $this->getNumberOfParameters()) {
                return $this;
            }

            if ($this->isConstructor()) {
                return $this;
            }

            /**
             * A method with no body of its own cannot be turned into a C
             * function. The twin's visibility is just `internal`, so it does not
             * inherit `isAbstract()` and would be emitted as a body-less
             * function that silently returns nothing. Generator creators are the
             * same shape: GeneratorTransformer moves the body into a step and
             * nulls the statements before this runs, and the twin carries no
             * generator step, so it would return null instead of a Generator.
             */
            if ($this->isAbstract() || null === $this->statements || $this->isGeneratorCreator()) {
                return $this;
            }

            $optimizedName = $this->getName() . '_zephir_internal_call';

            /**
             * `static` has to be carried over: the twin's body is compiled with
             * a static context iff the method it copies had one. Visibility
             * itself is irrelevant to an internal method (it is deliberately
             * left out of the method_entry) but `static` is not.
             */
            $visibility = ['internal'];
            if ($this->isStatic()) {
                $visibility[] = 'static';
            }

            $statements = new StatementsBlock(
                json_decode(json_encode($this->statements->getStatements()), true)
            );

            /**
             * The twin gets its own Parameters instance: fetchParameters()
             * appends to the required/optional lists without ever resetting
             * them, so sharing the original's instance made the twin see every
             * parameter twice and emit each conversion twice — a leak for
             * `string` and `array` parameters, which allocate.
             */
            $parameters = null;
            if ($this->parameters instanceof Parameters) {
                $parameters = new Parameters($this->parameters->getParameters());
            }

            $optimizedMethod                = new self(
                $classDefinition,
                $visibility,
                $optimizedName,
                $parameters,
                $statements,
                $this->docblock,
                null,
                $this->expression
            );
            $optimizedMethod->typeInference = $this->typeInference;
            $optimizedMethod->setReturnTypes($this->returnTypes);
            $optimizedMethod->setDeclaredName($this->getName());
            $classDefinition->addMethod($optimizedMethod);
        }

        return $this;
    }

    /**
     * Get data type of method's parameter
     */
    private function getParamDataType(array $parameter): string
    {
        return $parameter['data-type'] ?? 'variable';
    }

    /**
     * @param CompilationContext $compilationContext
     * @param array              $parameter
     * @param Printer            $oldCodePrinter
     * @param string             $type
     *
     * @return string
     * @throws Exception
     * @throws ReflectionException
     */
    private function processStaticConstantAccess(
        CompilationContext $compilationContext,
        array $parameter,
        Printer $oldCodePrinter,
        string $type
    ): string {
        /**
         * Now I can write code for easy use on Expression because
         * code in this method don't write with codePrinter ;(.
         *
         * TODO: Rewrite all to codePrinter
         */
        $symbolVariable = $compilationContext->symbolTable->getVariableForWrite(
            $parameter['name'],
            $compilationContext,
            $parameter['default']
        );
        $expression     = new Expression($parameter['default']);
        $expression->setExpectReturn(true, $symbolVariable);
        $compiledExpression = $expression->compile($compilationContext);

        if ($type !== $compiledExpression->getType()) {
            throw new CompilerException(
                'Default parameter value type: '
                . $compiledExpression->getType()
                . ' cannot be assigned to variable(' . $type . ')',
                $parameter
            );
        }

        $parameter['default']['type']  = $compiledExpression->getType();
        $parameter['default']['value'] = $compiledExpression->getCode();

        $compilationContext->codePrinter = $oldCodePrinter;

        return $this->assignDefaultValue($parameter, $compilationContext);
    }
}
