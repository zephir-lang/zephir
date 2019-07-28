<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Fcall;

/**
 * Zephir\Fcall\FcallAwareInterface.
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
