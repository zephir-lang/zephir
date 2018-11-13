<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir;

/**
 * Zephir\Version
 *
 * @package Zephir
 */
final class Version
{
    const VERSION = '0.11.3';

    /** @var string */
    private $currentVersion;

    /**
     * Returns the current version + the ident if available.
     *
     * @return string
     */
    public function get()
    {
        $version = '$Id$';
        if (strlen($version) !== 4) {
            return self::VERSION . '-' . substr($version, 0, 10);
        }

        if ($this->currentVersion === null) {
            if (file_exists(__DIR__ . '/../.git')) {
                $command = sprintf('git --git-dir=%s/../.git log --format="%%H" -n 1', __DIR__);
                exec($command, $xversion);
                if (isset($xversion[0]) && strlen($xversion[0]) > 10) {
                    $this->currentVersion = substr($xversion[0], 0, 10);
                } else {
                    $this->currentVersion = false;
                }
            }
        }

        if ($this->currentVersion) {
            return self::VERSION . '-' . $this->currentVersion;
        }

        return self::VERSION;
    }

    /**
     * The alias for {@see get}.
     *
     * @return string
     */
    public function __toString()
    {
        return $this->get();
    }
}
