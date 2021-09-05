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

namespace Zephir\Detectors;

use Zephir\Variable;

use function in_array;
use function is_array;

/**
 * ReadDetector.
 *
 * Detects if a variable is used in a given expression context
 * Since zvals are collected between executions to the same section of code
 * We need to ensure that a variable is not contained in the "right-side" expression
 * used to assign the variable, avoiding premature initializations
 */
class ReadDetector
{
    // TODO: Move params from detect() to constructor()

    /**
     * @param string $variable
     * @param array $expression
     * @return bool
     */
    public function detect(string $variable, array $expression): bool
    {
        if (!isset($expression['type'])) {
            return false;
        }

        /**
         * Remove branch from variable name
         */
        $pos = strpos($variable, Variable::BRANCH_MAGIC);
        if ($pos > -1) {
            $variable = substr($variable, 0, $pos);
        }

        if ('variable' === $expression['type'] && $variable === $expression['value']) {
            return true;
        }

        if (in_array($expression['type'], ['fcall', 'mcall', 'scall']) && isset($expression['parameters'])) {
            foreach ($expression['parameters'] as $parameter) {
                if (is_array($parameter['parameter'])) {
                    if ('variable' === $parameter['parameter']['type']) {
                        if ($variable === $parameter['parameter']['value']) {
                            return true;
                        }
                    }
                }
            }
        }

        if (isset($expression['left']) && is_array($expression['left']) && $this->detect($variable, $expression['left'])) {
            return true;
        }

        if (isset($expression['right']) && is_array($expression['right']) && $this->detect($variable, $expression['right'])) {
            return true;
        }

        return false;
    }
}
