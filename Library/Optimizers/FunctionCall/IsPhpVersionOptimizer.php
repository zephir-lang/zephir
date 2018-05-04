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

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\Compiler\CompilerException;
use Zephir\CompiledExpression;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * IsPhpVersionOptimizer
 *
 * Checks if PHP has a specific version
 */
class IsPhpVersionOptimizer extends OptimizerAbstract
{
    protected $allowedTypes = [
        'string'    => true,
        'int'       => true,
        'long'      => true,
        'double'    => true,
        'uint'      => true,
        'ulong'     => true,
        'istring'   => true
    ];

    /**
     * @param array              $expression
     * @param Call               $call
     * @param CompilationContext $context
     *
     * @return CompiledExpression
     */
    public function optimize(array $expression, Call $call, CompilationContext $context)
    {
        if (!isset($expression['parameters'])) {
            throw new CompilerException("This function requires parameters", $expression);
        }

        if (count($expression['parameters']) != 1) {
            throw new CompilerException("This function only requires one parameter", $expression);
        }

        $variableType = $expression['parameters'][0]['parameter']['type'];

        if (!isset($this->allowedTypes[$variableType])) {
            throw new CompilerException("This function requires a scalar types parameter, $variableType given", $expression);
        }

        preg_match('/^(?<major>\d+)(?:\.(?<minor>!?\d+))?(?:\.(?<patch>!?\d+))?$/', $expression['parameters'][0]['parameter']['value'], $matches);
        if (!count($matches)) {
            throw new CompilerException("Could not parse PHP version", $expression);
        }

        $minorVersion   = 0;
        $releaseVersion = 0;

        $majorVersion = $matches['major'] * 10000;

        if (isset($matches['minor'])) {
            $minorVersion = $matches['minor'] * 100;
        }

        if (isset($matches['patch'])) {
            $releaseVersion = $matches['patch'];
        }

        $versionId = intval($majorVersion + $minorVersion + $releaseVersion);

        return new CompiledExpression('bool', 'zephir_is_php_version('.$versionId.')', $expression);
    }
}
