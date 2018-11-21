<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

/**
 * Zephir\ArgInfoDefinition
 */
class ArgInfoDefinition
{
    /** @var bool */
    private $returnByRef = false;

    /** @var ClassMethod */
    private $method;

    /** @var string */
    private $name = '';

    /** @var ClassMethodParameters|null */
    private $parameters;

    /** @var CodePrinter */
    private $codePrinter;

    /** @var CompilationContext */
    private $compilationContext;

    /** @var string */
    private $booleanDefinition = '_IS_BOOL';

    /** @var bool */
    private $richFormat = true;

    public function __construct(
        $name,
        ClassMethod $method,
        CodePrinter $codePrinter,
        CompilationContext $compilationContext,
        $returnByRef = false
    ) {
        $this->method = $method;
        $this->codePrinter = $codePrinter;
        $this->compilationContext = $compilationContext;

        $this->name = $name;
        $this->parameters = $this->method->getParameters();

        $this->returnByRef = $returnByRef;
    }

    public function setBooleanDefinition($definition)
    {
        $this->booleanDefinition = (string)$definition;
    }

    public function setRichFormat($flag)
    {
        $this->richFormat = (bool)$flag;
    }

    /**
     * Render argument information.
     *
     * @throws Exception
     */
    public function render()
    {
        if ($this->richFormat &&
            $this->method->isReturnTypesHintDetermined() &&
            $this->method->areReturnTypesCompatible()
        ) {
            $this->richRenderStart();

            if ($this->hasParameters() == false) {
                $this->codePrinter->output('ZEND_END_ARG_INFO()');
                $this->codePrinter->outputBlankLine();
            }
        } elseif ($this->hasParameters() == true) {
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_INFO_EX(%s, 0, %d, %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->method->getNumberOfRequiredParameters()
                )
            );
        }

        if ($this->hasParameters() == true) {
            $this->renderEnd();

            $this->codePrinter->output('ZEND_END_ARG_INFO()');
            $this->codePrinter->outputBlankLine();
        }
    }

    private function richRenderStart()
    {
        if (array_key_exists('object', $this->method->getReturnTypes())) {
            $class = 'NULL';

            if (count($this->method->getReturnClassTypes()) == 1) {
                $class = key($this->method->getReturnClassTypes());
                $class = escape_class($this->compilationContext->getFullName($class));
            }

            $this->codePrinter->output('#if PHP_VERSION_ID >= 70200');
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(%s, %d, %d, %s, %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->method->getNumberOfRequiredParameters(),
                    $class,
                    (int)$this->method->areReturnTypesNullCompatible()
                )
            );
            $this->codePrinter->output('#else');
            $this->codePrinter->output(
                sprintf(
                    'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(%s, %d, %d, IS_OBJECT, "%s", %d)',
                    $this->name,
                    (int)$this->returnByRef,
                    $this->method->getNumberOfRequiredParameters(),
                    $class,
                    (int)$this->method->areReturnTypesNullCompatible()
                )
            );
            $this->codePrinter->output('#endif');

            return;
        }

        $this->codePrinter->output('#if PHP_VERSION_ID >= 70200');
        $this->codePrinter->output(
            sprintf(
                'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(%s, %d, %d, %s, %d)',
                $this->name,
                (int)$this->returnByRef,
                $this->method->getNumberOfRequiredParameters(),
                $this->getReturnType(),
                (int)$this->method->areReturnTypesNullCompatible()
            )
        );

        $this->codePrinter->output('#else');

        $this->codePrinter->output(
            sprintf(
                'ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(%s, %d, %d, %s, NULL, %d)',
                $this->name,
                (int)$this->returnByRef,
                $this->method->getNumberOfRequiredParameters(),
                $this->getReturnType(),
                (int)$this->method->areReturnTypesNullCompatible()
            )
        );

        $this->codePrinter->output('#endif');
    }

    private function renderEnd()
    {
        $flag = $this->richFormat ? '1' : '0';

        foreach ($this->parameters->getParameters() as $parameter) {
            switch ("{$flag}:" . $parameter['data-type']) {
                case '0:array':
                case '1:array':
                    $this->codePrinter->output(
                        sprintf(
                            "\tZEND_ARG_ARRAY_INFO(%d, %s, %d)",
                            $this->passByReference($parameter),
                            $parameter['name'],
                            (int)$this->allowNull($parameter)
                        )
                    );
                    break;
                case '0:variable':
                case '1:variable':
                    if (isset($parameter['cast'])) {
                        switch ($parameter['cast']['type']) {
                            case 'variable':
                                $value = $parameter['cast']['value'];
                                $this->codePrinter->output(
                                    sprintf(
                                        "\tZEND_ARG_OBJ_INFO(%d, %s, %s, %d)",
                                        $this->passByReference($parameter),
                                        $parameter['name'],
                                        escape_class($this->compilationContext->getFullName($value)),
                                        (int)$this->allowNull($parameter)
                                    )
                                );
                                break;

                            default:
                                throw new Exception('Unexpected exception');
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
                    $this->codePrinter->output(
                        sprintf(
                            "\tZEND_ARG_TYPE_INFO(%d, %s, %s, %d)",
                            $this->passByReference($parameter),
                            $parameter['name'],
                            $this->booleanDefinition,
                            (int)$this->allowNull($parameter)
                        )
                    );
                    break;
                case '1:uchar':
                case '1:int':
                case '1:uint':
                case '1:long':
                case '1:ulong':
                    $this->codePrinter->output(
                        sprintf(
                            "\tZEND_ARG_TYPE_INFO(%d, %s, IS_LONG, %d)",
                            $this->passByReference($parameter),
                            $parameter['name'],
                            (int)$this->allowNull($parameter)
                        )
                    );
                    break;
                case '1:double':
                    $this->codePrinter->output(
                        sprintf(
                            "\tZEND_ARG_TYPE_INFO(%d, %s, IS_DOUBLE, %d)",
                            $this->passByReference($parameter),
                            $parameter['name'],
                            (int)$this->allowNull($parameter)
                        )
                    );
                    break;
                case '1:char':
                case '1:string':
                    $this->codePrinter->output(
                        sprintf(
                            "\tZEND_ARG_TYPE_INFO(%d, %s, IS_STRING, %d)",
                            $this->passByReference($parameter),
                            $parameter['name'],
                            (int)$this->allowNull($parameter)
                        )
                    );
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

    private function hasParameters()
    {
        return $this->parameters !== null && count($this->parameters->getParameters()) > 0;
    }

    private function allowNull($parameter)
    {
        if (!isset($parameter['default']) || !is_array($parameter['default'])) {
            return false;
        }

        if ($parameter['default']['type'] == 'null') {
            return true;
        }

        return false;
    }

    private function passByReference($parameter)
    {
        return isset($parameter['reference']) ? $parameter['reference'] : 0;
    }

    private function getReturnType()
    {
        if ($this->method->areReturnTypesIntCompatible()) {
            return 'IS_LONG';
        }

        if ($this->method->areReturnTypesDoubleCompatible()) {
            return 'IS_DOUBLE';
        }

        if ($this->method->areReturnTypesBoolCompatible()) {
            return '_IS_BOOL';
        }

        if ($this->method->areReturnTypesStringCompatible()) {
            return 'IS_STRING';
        }

        if (array_key_exists('array', $this->method->getReturnTypes())) {
            return 'IS_ARRAY';
        }

        return 'IS_NULL';
    }
}
