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

namespace Extension\Properties;

use PHPUnit\Framework\TestCase;
use Stub\Properties\GetObjectVars;

final class GetObjectVarsTest extends TestCase
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1245
     */
    public function testIssue1245ShouldReturnAllVisibilityProperties(): void
    {
        $test = new GetObjectVars();

        $this->assertSame(['a' => 1, 'b' => 2, 'c' => 3], $test->issue1245());
    }
}
