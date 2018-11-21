<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Optimizers;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Di\ContainerAwareTrait;
use Zephir\Di\InjectionAwareInterface;

/**
 * Class OptimizerAbstract
 */
abstract class OptimizerAbstract implements InjectionAwareInterface
{
    use ContainerAwareTrait {
        ContainerAwareTrait::__construct as protected __DiInject;
    }

    /**
     * @param  array              $expression
     * @param  Call               $call
     * @param  CompilationContext $context
     * @return mixed
     */
    abstract public function optimize(array $expression, Call $call, CompilationContext $context);
}
