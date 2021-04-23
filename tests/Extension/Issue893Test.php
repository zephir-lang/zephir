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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Issue893;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/893
 */
final class Issue893Test extends TestCase
{
    public function testExtendsDOMClassShouldBeInstanceOfDOMNodeList(): void
    {
        $DOM = new Issue893();

        $this->assertInstanceOf(\DOMNodeList::class, $DOM->childNodes);
    }
}
