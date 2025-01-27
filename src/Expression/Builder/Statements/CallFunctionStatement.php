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

namespace Zephir\Expression\Builder\Statements;

use Zephir\FunctionCall;

use function is_array;

/**
 * Allows to manually build a function call AST node
 */
class CallFunctionStatement extends AbstractStatement
{
    /**
     * f(x, y, ... , N).
     */
    public const TYPE_CALL_DIRECT = FunctionCall::CALL_NORMAL;

    /**
     * {f}(x, y, ... , N).
     */
    public const TYPE_CALL_DYNAMIC = FunctionCall::CALL_DYNAMIC;

    /**
     * {"f"}(x, y, ... , N).
     */
    public const TYPE_CALL_DYNAMIC_STRING = FunctionCall::CALL_DYNAMIC_STRING;

    private ?array $arguments = null;

    public function __construct(private ?string $name = null, private ?array $parameters = null, private int $typeCall = self::TYPE_CALL_DIRECT)
    {
        if (null !== $name) {
            $this->setName($name);
        }

        if (null !== $parameters) {
            $this->setArguments($parameters);
        }

        if (null !== $typeCall) {
            $this->setTypeCall($typeCall);
        }
    }

    /**
     * @param $argument
     *
     * @return $this
     */
    public function addArgument($argument)
    {
        $this->arguments[] = $argument;

        return $this;
    }

    /**
     * @return array
     */
    public function getArguments()
    {
        return $this->arguments;
    }

    /**
     * @return string
     */
    public function getName()
    {
        return $this->name;
    }

    /**
     * @return int
     */
    public function getTypeCall()
    {
        return $this->typeCall;
    }

    public function setArguments(?array $arguments = null): self
    {
        $this->arguments = $arguments;

        return $this;
    }

    /**
     * @param string $name
     *
     * @return $this
     */
    public function setName($name)
    {
        $this->name = $name;

        return $this;
    }

    /**
     * @param int $typeCall
     *
     * @return $this
     */
    public function setTypeCall($typeCall)
    {
        $this->typeCall = $typeCall;

        return $this;
    }

    /**
     * @return array
     */
    protected function preBuild()
    {
        if ($arguments = $this->getArguments()) {
            foreach ($arguments as &$argument) {
                if (!is_array($argument) || !isset($argument['parameter'])) {
                    $argument = ['parameter' => $argument];
                }
            }
        }

        return [
            'type'       => 'fcall',
            'name'       => $this->getName(),
            'call-type'  => $this->getTypeCall(),
            'parameters' => $arguments,
        ];
    }
}
