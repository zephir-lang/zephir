<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Optimizers\FunctionCall;

use Zephir\Call;
use Zephir\CompilationContext;
use Zephir\CompiledExpression;
use Zephir\Exception\CompilerException;
use Zephir\Optimizers\OptimizerAbstract;

/**
 * IsPhpVersionOptimizer.
 *
 * Checks if PHP has a specific version
 */
class IsPhpVersionOptimizer extends OptimizerAbstract
{
    protected $allowedTypes = [
        'string' => true,
        'int' => true,
        'long' => true,
        'double' => true,
        'uint' => true,
        'ulong' => true,
        'istring' => true,
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
            throw new CompilerException('This function requires parameters', $expression);
        }

        if (1 != \count($expression['parameters'])) {
            throw new CompilerException('This function only requires one parameter', $expression);
        }

        $variableType = $expression['parameters'][0]['parameter']['type'];

        if (!isset($this->allowedTypes[$variableType])) {
            throw new CompilerException("This function requires a scalar types parameter, $variableType given", $expression);
        }

        preg_match('/^(?<major>\d+)(?:\.(?<minor>!?\d+))?(?:\.(?<patch>!?\d+))?(?:[^Ee0-9.]+.*)?$/', $expression['parameters'][0]['parameter']['value'], $matches);
        if (!\count($matches)) {
            throw new CompilerException('Could not parse PHP version', $expression);
        }

        $minorVersion = 0;
        $releaseVersion = 0;

        $majorVersion = $matches['major'] * 10000;

        if (isset($matches['minor'])) {
            $minorVersion = $matches['minor'] * 100;
        }

        if (isset($matches['patch'])) {
            $releaseVersion = $matches['patch'];
        }

        $versionId = (int) ($majorVersion + $minorVersion + $releaseVersion);

        return new CompiledExpression('bool', 'zephir_is_php_version('.$versionId.')', $expression);
    }
}
