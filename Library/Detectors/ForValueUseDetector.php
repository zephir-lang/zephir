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

/**
 * ForValueUseDetector
 *
 * Detects whether the traversed variable is modified within the 'for's block
 */
class ForValueUseDetector extends WriteDetector
{
    /**
     * ForValueUseDetector constructor
     *
     * Initialize detector with safe defaults
     */
    public function __construct()
    {
        $this->setDetectionFlags(self::DETECT_NONE);
    }
}
