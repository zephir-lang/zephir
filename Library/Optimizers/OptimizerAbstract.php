<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Optimizers;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Utils;

/**
 * Class OptimizerAbstract
 */
abstract class OptimizerAbstract
{
	protected $functionName;

	public function __construct() {
		$fqcn = explode('\\', get_called_class());
		$this->functionName = Utils::uncamelize(substr(array_pop($fqcn), 0, strlen('Optimizer')));
	}

    /**
     * @param array $expression
     * @param Call $call
     * @param CompilationContext $context
     * @return mixed
     */
    abstract public function optimize(array $expression, Call $call, CompilationContext $context);
}
