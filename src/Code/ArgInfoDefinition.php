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

namespace Zephir\Code;

use Zephir\Class\Entry;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\CompilationContext;
use Zephir\Exception;

use function array_key_exists;
use function array_merge;
use function count;
use function implode;
use function is_array;
use function key;
use function sprintf;
use function str_replace;

class ArgInfoDefinition
{
    private string      $booleanDefinition = '_IS_BOOL';
    private ?Parameters $parameters;
    private bool        $richFormat        = true;

    public function __construct(
        private string $name,
        private Method $functionLike,
        private Printer $codePrinter,
        private CompilationContext $compilationContext,
        private bool $returnByRef = false
    ) {
        $this->parameters = $this->functionLike->getParameters();
    }

    /**
     * Render argument information.
     *
     * @throws Exception
     */
    public function render(): void
    {
        if ($this->renderPhalconCompatible()) {
            $this->codePrinter->outputBlankLine();

            return;
        }

        if (
            $this->richFormat &&
            $this->functionLike->isReturnTypesHintDetermined() &&
            $this->functionLike->areReturnTypesCompatible()
        ) {
            $this->richRenderStart();

            if (!$this->hasParameters() && !$this->functionLike->isVoid()) {
                $this->codePrinter->output('ZEND_END_ARG_INFO()');
                $this->codePrinter->outputBlankLine();
            }
        } elseif ($this->hasParameters()) {
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_INFO_EX(%s, 0, %d, %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters()
                )
            );
        } else {
            if ($this->functionLike->getName() === '__toString') {
                $this->codePrinter->output(
                    sprintf(
                        'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(%s, %d, %d, IS_STRING, %d)',
                        $this->name,
                        (int)$this->returnByRef,
                        $this->functionLike->getNumberOfRequiredParameters(),
                        (int)$this->functionLike->areReturnTypesNullCompatible()
                    )
                );
            } else {
                $this->codePrinter->output(
                    sprintf('ZEND_BEGIN_ARG_INFO_EX(%s, 0, 0, 0)', $this->name)
                );
            }

            $this->codePrinter->output('ZEND_END_ARG_INFO()');
            $this->codePrinter->outputBlankLine();
        }

        if ($this->hasParameters()) {
            $this->renderEnd();

            $this->codePrinter->output('ZEND_END_ARG_INFO()');
            $this->codePrinter->outputBlankLine();
        }
    }

    public function setBooleanDefinition(string $definition): void
    {
        $this->booleanDefinition = $definition;
    }

    public function setRichFormat(bool $flag): void
    {
        $this->richFormat = $flag;
    }

    private function allowNull(array $parameter): bool
    {
        if (!isset($parameter['default']) || !is_array($parameter['default'])) {
            return false;
        }

        if ('null' === $parameter['default']['type']) {
            return true;
        }

        return false;
    }

    private function defaultArrayValue(array $parameter): string
    {
        if ($parameter['default']['type'] === 'empty-array') {
            return '"[]"';
        }

        // TODO: Come back later
        /**
         * It seems that it is impossible to pass default array with some data inside.
         * Only empty array, even if manually specify not empty array - it will be ignored,
         * for example:
         *
         * `ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, someDefaultData, IS_ARRAY, 0, "[\"key\" => \"value\"]")`
         *
         * Output of default value will be `[]` during method call.
         */
        return '"[]"';
    }

    private function getReturnType(): string
    {
        // TODO: Come back here when PHP7.4 is deprecated.
        /*if (array_key_exists('mixed', $this->functionLike->getReturnTypes())) {
            return 'IS_MIXED';
        }*/

        if ($this->functionLike->areReturnTypesIntCompatible()) {
            return 'IS_LONG';
        }

        if ($this->functionLike->areReturnTypesDoubleCompatible()) {
            return 'IS_DOUBLE';
        }

        if ($this->functionLike->areReturnTypesBoolCompatible()) {
            return '_IS_BOOL';
        }

        if ($this->functionLike->areReturnTypesStringCompatible()) {
            return 'IS_STRING';
        }

        if ($this->functionLike->isVoid()) {
            return 'IS_VOID';
        }

        if (array_key_exists('array', $this->functionLike->getReturnTypes())) {
            return 'IS_ARRAY';
        }

        return 'IS_NULL';
    }

    private function hasParameters(): bool
    {
        return null !== $this->parameters && count($this->parameters->getParameters()) > 0;
    }

    private function passByReference(array $parameter)
    {
        return $parameter['reference'] ?? 0;
    }

    private function renderEnd(): void
    {
        $flag = $this->richFormat ? '1' : '0';

        foreach ($this->parameters->getParameters() as $parameter) {
            if (!empty($parameter['variadic'])) {
                $this->emitVariadicArgInfo($parameter);
                continue;
            }

            switch ("$flag:" . $parameter['data-type']) {
                case '0:array':
                case '1:array':
                    if (!isset($parameter['default'])) {
                        $this->codePrinter->output(
                            sprintf(
                                "\tZEND_ARG_ARRAY_INFO(%d, %s, %d)",
                                $this->passByReference($parameter),
                                $parameter['name'],
                                (int)$this->allowNull($parameter)
                            )
                        );
                    } else {
                        $this->codePrinter->output(
                            sprintf(
                                'ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(%d, %s, IS_ARRAY, %d, %s)',
                                $this->passByReference($parameter),
                                $parameter['name'],
                                (int)$this->allowNull($parameter),
                                $this->defaultArrayValue($parameter),
                            )
                        );
                    }
                    break;
                case '0:variable':
                case '1:variable':
                    if (isset($parameter['cast'])) {
                        if ($parameter['cast']['type'] !== 'variable') {
                            throw new Exception('Unexpected exception');
                        }

                        $className = Entry::escape(
                            $this->compilationContext->getFullName($parameter['cast']['value'])
                        );

                        if (isset($parameter['default'])) {
                            /**
                             * Parameter that has a class type and a default value, usually null.
                             * ZEND_ARG_OBJ_INFO does not keep a default, so reflection makes
                             * getDefaultValue() throw and isDefaultValueAvailable() return false.
                             * ZEND_ARG_OBJ_TYPE_MASK keeps the default value.
                             *
                             * @see https://github.com/zephir-lang/zephir/issues/2564
                             */
                            $this->codePrinter->output(
                                sprintf(
                                    "\tZEND_ARG_OBJ_TYPE_MASK(%d, %s, %s, %s, \"%s\")",
                                    $this->passByReference($parameter),
                                    $parameter['name'],
                                    $className,
                                    $this->allowNull($parameter) ? 'MAY_BE_NULL' : '0',
                                    $parameter['default']['value'] ?? 'null'
                                )
                            );
                        } else {
                            $this->codePrinter->output(
                                sprintf(
                                    "\tZEND_ARG_OBJ_INFO(%d, %s, %s, %d)",
                                    $this->passByReference($parameter),
                                    $parameter['name'],
                                    $className,
                                    (int)$this->allowNull($parameter)
                                )
                            );
                        }
                    } else {
                        $this->codePrinter->output(
                            sprintf(
                                "\tZEND_ARG_INFO(%d, %s)",
                                $this->passByReference($parameter),
                                $parameter['name']
                            )
                        );
                    }
                    break;

                case '1:bool':
                case '1:boolean':
                    $this->emitTypedArgInfo($parameter, $this->booleanDefinition);
                    break;
                case '1:uchar':
                case '1:int':
                case '1:uint':
                case '1:long':
                case '1:ulong':
                    $this->emitTypedArgInfo($parameter, 'IS_LONG');
                    break;
                case '1:double':
                    $this->emitTypedArgInfo($parameter, 'IS_DOUBLE');
                    break;
                case '1:char':
                case '1:string':
                    $this->emitTypedArgInfo($parameter, 'IS_STRING');
                    break;
                default:
                    $this->codePrinter->output(
                        sprintf(
                            "\tZEND_ARG_INFO(%d, %s)",
                            $this->passByReference($parameter),
                            $parameter['name']
                        )
                    );
                    break;
            }
        }
    }

    private function emitVariadicArgInfo(array $parameter): void
    {
        $zendType = match ($parameter['data-type'] ?? 'variable') {
            'int', 'uint', 'long', 'ulong', 'char', 'uchar' => 'IS_LONG',
            'double'        => 'IS_DOUBLE',
            'bool', 'boolean' => '_IS_BOOL',
            'string'        => 'IS_STRING',
            'array'         => 'IS_ARRAY',
            default         => null,
        };

        if ($zendType === null) {
            $this->codePrinter->output(
                sprintf(
                    "\tZEND_ARG_VARIADIC_INFO(%d, %s)",
                    $this->passByReference($parameter),
                    $parameter['name']
                )
            );

            return;
        }

        $this->codePrinter->output(
            sprintf(
                "\tZEND_ARG_VARIADIC_TYPE_INFO(%d, %s, %s, 0)",
                $this->passByReference($parameter),
                $parameter['name'],
                $zendType
            )
        );
    }

    private function emitTypedArgInfo(array $parameter, string $zendType): void
    {
        $gotDefault = isset($parameter['default']);
        $format = $gotDefault
            ? "\tZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(%d, %s, %s, %d, \"%s\")"
            : "\tZEND_ARG_TYPE_INFO(%d, %s, %s, %d)";

        $args = [
            $this->passByReference($parameter),
            $parameter['name'],
            $zendType,
            (int)$this->allowNull($parameter),
        ];

        if ($gotDefault) {
            if (isset($parameter['default']['value']) && $zendType === 'IS_STRING') {
                $args[] = $this->escapeString($parameter['default']['value']);
            } else {
                $args[] = $parameter['default']['value'] ?? 'null';
            }
        }

        $this->codePrinter->output(vsprintf($format, $args));
    }

    private function escapeString(string $value): string
    {
        $escaped = str_replace("'", '\\\'', $value);
        $escaped = Entry::escape($escaped);

        return "'" . $escaped . "'";
    }

    /**
     * Find from $compatibilityClasses and render specific
     * hardcoded arg info for with specific PHP version
     * conditions.
     *
     * This is temporary solution designed specifically for Phalcon project.
     *
     * @return bool
     * @deprecated used as MVP solution for cross PHP versions support
     *
     */
    private function renderPhalconCompatible(): bool
    {
        $compatibilityClasses  = require_once __DIR__ . '/../../config/phalcon-compatibility-headers.php';
        $classDefinition       = $this->functionLike->getClassDefinition();
        $implementedInterfaces = $classDefinition !== null ? $classDefinition->getImplementedInterfaces() : [];
        $extendsClass          = $classDefinition?->getExtendsClass();

        if (empty($implementedInterfaces) && $extendsClass === null) {
            return false;
        }

        $methodName = $this->functionLike->getName();

        if ($extendsClass !== null) {
            $implementedInterfaces = array_merge($implementedInterfaces, [$extendsClass]);
        }

        $found = false;
        foreach ($implementedInterfaces as $implementedInterface) {
            if (isset($compatibilityClasses[$implementedInterface][$methodName])) {
                foreach ($compatibilityClasses[$implementedInterface][$methodName] as $condition => $args) {
                    $this->codePrinter->output($condition);
                    foreach ($args as $arg) {
                        $this->codePrinter->output(
                            str_replace(['__ce__'], [$this->name], $arg)
                        );
                    }
                }

                $this->codePrinter->output('#endif');

                $found = true;
            }
        }

        return $found;
    }

    private function richRenderStart(): void
    {
        $returnTypes      = $this->functionLike->getReturnTypes();
        $returnClassTypes = $this->functionLike->getReturnClassTypes();
        $isNullable       = $this->functionLike->areReturnTypesNullCompatible();

        /**
         * Non-null, non-object scalar members of the return type
         * (`int`, `string`, `bool`, `array`, ...). `object` represents the
         * class set and `null` is handled via the nullable flag/mask bit, so
         * both are excluded here.
         */
        $scalarKeys = array_values(
            array_diff(array_keys($returnTypes), ['object', 'null'])
        );

        /**
         * The bare `object` keyword as one member of a wider union
         * (`array | object`, `object | string`, ...). The dedicated generic
         * `object` / `object | null` branches below only fire when `object` is
         * the *sole* non-null member, so in a union it contributes a
         * MAY_BE_OBJECT bit here. When `object` accompanies named classes the
         * key instead represents those classes (handled via $returnClassTypes),
         * so this is only "generic" when there are no class types.
         */
        $hasGenericObject = array_key_exists('object', $returnTypes)
            && [] === $returnClassTypes;

        /**
         * Single class return (`<Foo>`, `<Foo> | null`, and the reserved
         * `self` / `parent` / `static` keywords). Kept as a dedicated branch so
         * the precise OBJ_INFO / MAY_BE_STATIC forms — and their existing test
         * coverage — stay byte-for-byte identical.
         *
         * `self`, `static`, and `parent` are PHP-reserved return-type names
         * needing different engine handling:
         *
         *   - `self` / `parent` reach the engine as the literal lowercase
         *     keyword. PHP recognizes both as reserved names during arginfo
         *     class-name resolution and reflection reports them verbatim,
         *     preserving covariant-return semantics.
         *   - `static` has no class entry at all, so passing the literal string
         *     to `ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX` makes MINIT abort with
         *     "static must be registered before <Class>". Emit the dedicated
         *     `MAY_BE_STATIC` type-mask bit instead; reflection still reports
         *     the type as `'static'`.
         *
         * See https://github.com/zephir-lang/zephir/issues/2505.
         */
        if (
            array_key_exists('object', $returnTypes)
            && 1 === count($returnClassTypes)
            && [] === $scalarKeys
        ) {
            $class    = key($returnClassTypes);
            $reserved = strtolower($class);

            if ($reserved === 'static') {
                $mask = 'MAY_BE_STATIC';
                if ($isNullable) {
                    $mask = 'MAY_BE_NULL|' . $mask;
                }

                $this->codePrinter->output(
                    sprintf(
                        'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(%s, %d, %d, %s)',
                        $this->name,
                        (int)$this->returnByRef,
                        $this->functionLike->getNumberOfRequiredParameters(),
                        $mask
                    )
                );

                return;
            }

            if ($reserved === 'self' || $reserved === 'parent') {
                $class = $reserved;
            } else {
                $class = Entry::escape($this->compilationContext->getFullName($class));
            }

            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(%s, %d, %d, %s, %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters(),
                    $class,
                    (int)$isNullable
                )
            );

            return;
        }

        if ($this->functionLike->isVoid()) {
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(%s, %d, %d, %s, %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters(),
                    $this->getReturnType(),
                    (int)$isNullable
                )
            );

            if (!$this->hasParameters()) {
                $this->codePrinter->output('ZEND_END_ARG_INFO()');
            }

            $this->codePrinter->outputBlankLine();

            return;
        }

        if ($this->functionLike->isMixed()) {
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(%s, %d, %d, IS_MIXED, %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters(),
                    (int)$isNullable
                )
            );

            return;
        }

        /**
         * Generic `object` keyword (no specific class), optionally nullable.
         */
        if (
            [] === $returnClassTypes
            && [] === $scalarKeys
            && array_key_exists('object', $returnTypes)
        ) {
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(%s, %d, %d, %s)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters(),
                    $isNullable ? 'MAY_BE_NULL|MAY_BE_OBJECT' : 'MAY_BE_OBJECT'
                )
            );

            return;
        }

        /**
         * True union return types: two or more non-null components mixing any
         * number of classes and/or scalar types (`<A> | <B>`, `<A> | <B> | null`,
         * `<A> | int`, `int | string`, `string | bool | null`, ...). Emit a PHP
         * type mask so the engine enforces every member, exactly as a
         * hand-written PHP union return type would — scalar-only unions use
         * TYPE_MASK, unions containing at least one named class use
         * OBJ_TYPE_MASK with the `Class1|Class2` list plus any scalar bits.
         *
         * A single non-null component plus `null` (`<A> | null`, `int | null`)
         * is intentionally NOT handled here: it stays on the precise
         * OBJ_INFO / `IS_*, allow_null=1` paths above/below.
         *
         * @see https://github.com/zephir-lang/zephir/issues/2428
         */
        if (count($scalarKeys) + count($returnClassTypes) + (int) $hasGenericObject >= 2) {
            $classNames = [];
            $maskBits   = [];
            $mayBe      = $this->functionLike->getMayBeArgTypes();

            foreach ($returnClassTypes as $class) {
                $reserved = strtolower($class);
                if ($reserved === 'static') {
                    $maskBits[] = 'MAY_BE_STATIC';
                    continue;
                }

                if ($reserved === 'self' || $reserved === 'parent') {
                    $classNames[] = $reserved;
                    continue;
                }

                $classNames[] = Entry::escape($this->compilationContext->getFullName($class));
            }

            foreach ($scalarKeys as $type) {
                if (isset($mayBe[$type])) {
                    $maskBits[] = $mayBe[$type];
                }
            }

            if ($hasGenericObject) {
                $maskBits[] = 'MAY_BE_OBJECT';
            }

            if ($isNullable) {
                array_unshift($maskBits, 'MAY_BE_NULL');
            }

            $maskBits = array_values(array_unique($maskBits));

            if ([] !== $classNames) {
                $this->codePrinter->output(
                    sprintf(
                        'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_TYPE_MASK_EX(%s, %d, %d, %s, %s)',
                        $this->name,
                        (int)$this->returnByRef,
                        $this->functionLike->getNumberOfRequiredParameters(),
                        implode('|', $classNames),
                        [] === $maskBits ? '0' : implode('|', $maskBits)
                    )
                );

                return;
            }

            if (count($maskBits) > 1) {
                $this->codePrinter->output(
                    sprintf(
                        'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(%s, %d, %d, %s)',
                        $this->name,
                        (int)$this->returnByRef,
                        $this->functionLike->getNumberOfRequiredParameters(),
                        implode('|', $maskBits)
                    )
                );

                return;
            }
        }

        $this->codePrinter->output(
            sprintf(
                'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(%s, %d, %d, %s, %d)',
                $this->name,
                (int)$this->returnByRef,
                $this->functionLike->getNumberOfRequiredParameters(),
                $this->getReturnType(),
                (int)$isNullable
            )
        );
    }
}
