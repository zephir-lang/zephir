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

namespace Zephir\Detectors;

use Zephir\Variable;

/**
 * ReadDetector
 *
 * Detects if a variable is used in a given expression context
 * Since zvals are collected between executions to the same section of code
 * We need to ensure that a variable is not contained in the "right-side" expression
 * used to assign the variable, avoiding premature initializations
 */
class ReadDetector
{
    public function detect($variable, array $expression)
    {
        if (!isset($expression['type'])) {
            return false;
        }

        /* Remove branch from variable name */
        $pos = strpos($variable, Variable::BRANCH_MAGIC);
        if ($pos > -1) {
            $branchId = intval(substr($variable, $pos + strlen(Variable::BRANCH_MAGIC)));
            $variable = substr($variable, 0, $pos);
        }

        if ($expression['type'] == 'variable') {
            if ($variable == $expression['value']) {
                return true;
            }
        }

        if ($expression['type'] == 'fcall' || $expression['type'] == 'mcall' || $expression['type'] == 'scall') {
            if (isset($expression['parameters'])) {
                foreach ($expression['parameters'] as $parameter) {
                    if (is_array($parameter['parameter'])) {
                        if ($parameter['parameter']['type'] == 'variable') {
                            if ($variable == $parameter['parameter']['value']) {
                                return true;
                            }
                        }
                    }
                }
            }
        }

        if (isset($expression['left'])) {
            if (is_array($expression['left'])) {
                if ($this->detect($variable, $expression['left']) === true) {
                    return true;
                }
            }
        }

        if (isset($expression['right'])) {
            if (is_array($expression['right'])) {
                if ($this->detect($variable, $expression['right']) === true) {
                    return true;
                }
            }
        }

        return false;
    }
}
