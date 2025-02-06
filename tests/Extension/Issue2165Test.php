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
use Stub\Issue2165\Issue as Issue2165Class;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2165
 */
final class Issue2165Test extends TestCase
{
    public function testIssue2165(): void
    {
        if (version_compare(PHP_VERSION, '8.2.0', '>=')) {
            $this->markTestSkipped('Creation of dynamic property is deprecated');
        }

        $class = Issue2165Class::build([
            [22, -17, 12],
            [4, 11, -2],
            [20, -6, -9],
        ]);

        $this->assertInstanceOf(Issue2165Class::class, $class);
        $this->assertInstanceOf(Issue2165Class::class, $class->reciprocal());
        $this->assertInstanceOf(Issue2165Class::class, Issue2165Class::ones(4, 2));
    }
}
