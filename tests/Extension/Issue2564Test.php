<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension;

use PHPUnit\Framework\TestCase;
use ReflectionClass;
use Stub\Issue2564;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2564
 */
final class Issue2564Test extends TestCase
{
    public function testConstructorDefaultValuesAreExposedViaReflection(): void
    {
        $ctor = (new ReflectionClass(Issue2564::class))->getConstructor();

        $params = [];
        foreach ($ctor->getParameters() as $p) {
            $params[$p->getName()] = $p;
        }

        // string default
        $this->assertTrue($params['cipher']->isDefaultValueAvailable());
        $this->assertSame('aes-256-cfb', $params['cipher']->getDefaultValue());

        // bool default
        $this->assertTrue($params['useSigning']->isDefaultValueAvailable());
        $this->assertTrue($params['useSigning']->getDefaultValue());

        // nullable object default (the real bug): it must be available
        // and return null instead of throwing.
        $this->assertTrue($params['padFactory']->isDefaultValueAvailable());
        $this->assertNull($params['padFactory']->getDefaultValue());
    }
}
