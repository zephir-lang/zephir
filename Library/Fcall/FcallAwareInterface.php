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

namespace Zephir\Fcall;

use Zephir\Fcall\FcallManagerInterface;

/**
 * Zephir\Fcall\FcallAwareInterface
 *
 * @package Zephir\Fcall
 */
interface FcallAwareInterface
{
    /**
     * Sets the FcallManager.
     *
     * @param FcallManagerInterface $fcallManager
     */
    public function setFcallManager(FcallManagerInterface $fcallManager);

    /**
     * Returns currently used FcallManager.
     *
     * @return FcallManagerInterface
     */
    public function getFcallManager();
}
