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

                        $this->codePrinter->output(
                            sprintf(
                                "\tZEND_ARG_OBJ_INFO(%d, %s, %s, %d)",
                                $this->passByReference($parameter),
                                $parameter['name'],
                                Entry::escape($this->compilationContext->getFullName($parameter['cast']['value'])),
                                (int)$this->allowNull($parameter)
                            )
                        );
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
                    $gotDefault = isset($parameter['default']);
                    $format = $gotDefault
                        ? "\tZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(%d, %s, %s, %d, \"%s\")"
                        : "\tZEND_ARG_TYPE_INFO(%d, %s, %s, %d)";

                    $args = [
                        $this->passByReference($parameter),
                        $parameter['name'],
                        $this->booleanDefinition,
                        (int)$this->allowNull($parameter),
                    ];

                    if ($gotDefault) {
                        $args[] = $parameter['default']['value'] ?? 'null';
                    }

                    $this->codePrinter->output(vsprintf($format, $args));
                    break;
                case '1:uchar':
                case '1:int':
                case '1:uint':
                case '1:long':
                case '1:ulong':
                    $gotDefault = isset($parameter['default']);
                    $format = $gotDefault
                        ? "\tZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(%d, %s, IS_LONG, %d, \"%s\")"
                        : "\tZEND_ARG_TYPE_INFO(%d, %s, IS_LONG, %d)";

                    $args = [
                        $this->passByReference($parameter),
                        $parameter['name'],
                        (int)$this->allowNull($parameter),
                    ];

                    if ($gotDefault) {
                        $args[] = $parameter['default']['value'] ?? 'null';
                    }

                    $this->codePrinter->output(vsprintf($format, $args));
                    break;
                case '1:double':
                    $gotDefault = isset($parameter['default']);
                    $format = $gotDefault
                        ? "\tZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(%d, %s, IS_DOUBLE, %d, \"%s\")"
                        : "\tZEND_ARG_TYPE_INFO(%d, %s, IS_DOUBLE, %d)";

                    $args = [
                        $this->passByReference($parameter),
                        $parameter['name'],
                        (int)$this->allowNull($parameter),
                    ];

                    if ($gotDefault) {
                        $args[] = $parameter['default']['value'] ?? 'null';
                    }

                    $this->codePrinter->output(vsprintf($format, $args));
                    break;
                case '1:char':
                case '1:string':
                    $gotDefault = isset($parameter['default']);
                    $format = $gotDefault
                        ? "\tZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(%d, %s, IS_STRING, %d, \"%s\")"
                        : "\tZEND_ARG_TYPE_INFO(%d, %s, IS_STRING, %d)";

                    $args = [
                        $this->passByReference($parameter),
                        $parameter['name'],
                        (int)$this->allowNull($parameter),
                    ];

                    if ($gotDefault) {
                        $args[] = isset($parameter['default']['value'])
                            ? $this->escapeString($parameter['default']['value'])
                            : 'null';
                    }

                    $this->codePrinter->output(vsprintf($format, $args));
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
        if (
            array_key_exists('object', $this->functionLike->getReturnTypes()) &&
            1 === count($this->functionLike->getReturnClassTypes())
        ) {
            $class = key($this->functionLike->getReturnClassTypes());
            $class = Entry::escape($this->compilationContext->getFullName($class));

            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(%s, %d, %d, %s, %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters(),
                    $class,
                    (int)$this->functionLike->areReturnTypesNullCompatible()
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
                    (int)$this->functionLike->areReturnTypesNullCompatible()
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
                    (int)$this->functionLike->areReturnTypesNullCompatible()
                )
            );

            return;
        }

        if ($this->functionLike->isReturnTypeNullableObject()) {
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(%s, %d, %d, %s)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters(),
                    'MAY_BE_NULL|MAY_BE_OBJECT',
                )
            );

            return;
        }

        if ($this->functionLike->isReturnTypeObject()) {
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(%s, %d, %d, %s)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->functionLike->getNumberOfRequiredParameters(),
                    'MAY_BE_OBJECT',
                )
            );

            return;
        }

        if (count($this->functionLike->getReturnTypes()) > 1) {
            $types      = [];
            $mayBeTypes = $this->functionLike->getMayBeArgTypes();
            foreach ($this->functionLike->getReturnTypes() as $type => $typeInfo) {
                if (!isset($mayBeTypes[$type])) {
                    continue;
                }

                $types[] = $mayBeTypes[$type];
            }

            if (count($types) > 1) {
                $this->codePrinter->output(
                    sprintf(
                        'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(%s, %d, %d, %s)',
                        $this->name,
                        (int)$this->returnByRef,
                        $this->functionLike->getNumberOfRequiredParameters(),
                        implode('|', $types)
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
                (int)$this->functionLike->areReturnTypesNullCompatible()
            )
        );
    }
}
