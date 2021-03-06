<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Builder\Operators;

/**
 * NewInstanceOperatorBuilder.
 *
 * Allows to manually build a 'new instance' operator AST node
 */
class NewInstanceOperatorBuilder extends AbstractOperatorBuilder
{
    protected $type;

    protected $className;

    protected $dynamic;

    protected $file;

    protected $line;

    protected $char;

    public function __construct($className, array $parameters, $dynamic = false, $file = null, $line = 0, $char = 0)
    {
        $this->className = $className;
        $this->parameters = $parameters;
        $this->dynamic = $dynamic;
        $this->file = $file;
        $this->line = $line;
        $this->char = $char;
    }

    /**
     * Returns a builder definition.
     *
     * @return array
     */
    public function get()
    {
        $parameters = [];
        foreach ($this->parameters as $parameter) {
            $parameters[] = $parameter->get();
        }

        return [
            'type' => 'new',
            'class' => $this->className,
            'parameters' => $parameters,
            'dynamic' => $this->dynamic,
            'file' => $this->file,
            'line' => $this->line,
            'char' => $this->char,
        ];
    }
}
