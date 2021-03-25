<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation;

interface FileInterface
{
    /**
     * @return string
     */
    public function getTemplateName(): string;

    /**
     * @return string
     */
    public function getOutputFile(): string;

    /**
     * @return array
     */
    public function getData(): array;
}
