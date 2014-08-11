<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Detectors;

/**
 * ForValueUseDetector
 *
 * Detects if the traversed variable is modified within the 'for's block
 *
 */
class ForValueUseDetector
{

    public function passLetStatement($variable, array $statement)
    {
        foreach ($statement['assignments'] as $assigment) {
            if ($assigment['variable'] == $variable) {
                return true;
            }
        }
        return false;
    }

    public function detect($variable, array $statements)
    {
        foreach ($statements as $statement) {
            switch ($statement['type']) {

                case 'let':
                    if ($this->passLetStatement($variable, $statement) === true) {
                        return true;
                    }
                    break;

                case 'do-while':
                case 'while':
                case 'loop':
                    if (isset($statement['statements'])) {
                        if ($this->detect($variable, $statement['statements']) === true) {
                            return true;
                        }
                    }
                    break;

                case 'try-catch':
                    if (isset($statement['statements'])) {
                        if ($this->detect($variable, $statement['statements']) === true) {
                            return true;
                        }
                    }
                    if (isset($statement['catches'])) {
                        foreach ($statement['catches'] as $catch) {
                            if (isset($catch['variable'])) {
                                if ($catch['variable']['value'] == $variable) {
                                    return true;
                                }
                            }
                            if (isset($catch['statements'])) {
                                if ($this->detect($variable, $catch['statements']) === true) {
                                    return true;
                                }
                            }
                        }
                    }
                    break;

                case 'unset':
                    if ($statement['type'] == "array-access") {
                        if ($statement['left']['value'] == $variable) {
                            return true;
                        }
                    } else {
                        if ($statement['type'] == "list") {
                            if ($statement['left']['left']['value'] == $variable) {
                                return true;
                            }
                        }
                    }
                    break;

                case 'if':
                    if (isset($statement['statements'])) {
                        if ($this->detect($variable, $statement['statements']) === true) {
                            return true;
                        }
                    }
                    if (isset($statement['else_statements'])) {
                        if ($this->detect($variable, $statement['else_statements']) === true) {
                            return true;
                        }
                    }
                    break;

                case 'switch':
                    foreach ($statement['clauses'] as $clause) {
                        if (isset($clause['statements'])) {
                            if ($this->detect($variable, $clause['statements']) === true) {
                                return true;
                            }
                        }
                    }
                    break;

                case 'for':
                    if (isset($statement['key'])) {
                        if ($statement['key'] == $variable) {
                            return true;
                        }
                    }
                    if (isset($statement['value'])) {
                        if ($statement['value'] == $variable) {
                            return true;
                        }
                    }
                    if (isset($statement['statements'])) {
                        if ($this->detect($variable, $statement['statements']) === true) {
                            return true;
                        }
                    }
                    break;
            }
        }
        return false;
    }
}
