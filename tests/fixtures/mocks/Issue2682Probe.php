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

/**
 * The value an Issue2682ProbeContainer hands back from offsetGet().
 *
 * It reports its own destruction through the container instead of through a
 * static counter, so a test can name the exact container it is asking about.
 *
 * Reading the freed slot is undefined behaviour and the buggy path does not
 * reliably abort, so "was it released too early" has to be observed from the
 * destructor rather than from whatever the target still points at.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2682
 */
final class Issue2682Probe
{
    private Issue2682ProbeContainer $owner;

    public function __construct(Issue2682ProbeContainer $owner)
    {
        $this->owner = $owner;
    }

    public function __destruct()
    {
        $this->owner->destroyed = true;
    }
}
