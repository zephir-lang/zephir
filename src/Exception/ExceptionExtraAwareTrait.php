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

namespace Zephir\Exception;

use function file;
use function file_exists;
use function sprintf;
use function str_repeat;
use function str_replace;

trait ExceptionExtraAwareTrait
{
    /**
     * Extra info.
     */
    protected ?array $extra = [];

    /**
     * @return string
     */
    public function getErrorRegion(): string
    {
        $region = '';
        $extra  = $this->getExtra();

        if (isset($extra['file']) && file_exists($extra['file'])) {
            $lines = file($extra['file']);

            if (isset($lines[$extra['line'] - 1])) {
                $line   = $lines[$extra['line'] - 1];
                $region .= sprintf("\t%s", str_replace("\t", ' ', $line));

                if (($extra['char'] - 1) > 0) {
                    $region .= sprintf("\t%s^\n", str_repeat('-', $extra['char'] - 1));
                }
            }
        }

        return $region;
    }

    /**
     * @return array
     */
    public function getExtra(): array
    {
        return $this->extra;
    }
}
