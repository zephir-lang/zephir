<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Detectors;

/**
 * ForValueUseDetector.
 *
 * Detects whether the traversed variable is modified within the 'for's block
 */
class ForValueUseDetector extends WriteDetector
{
    /**
     * ForValueUseDetector constructor.
     *
     * Initialize detector with safe defaults
     */
    public function __construct()
    {
        $this->setDetectionFlags(self::DETECT_NONE);
    }
}
