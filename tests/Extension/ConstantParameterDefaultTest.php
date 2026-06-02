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
use Stub\ConstantParameterDefault;

/**
 * Follow-up to #2564 / #2566: a class constant used as a parameter default
 * (e.g. `string cipher = self::DEFAULT_CIPHER`) must be folded into the
 * arg_info so its value is readable through Reflection. Previously the
 * `static-constant-access` default was not folded and the arg_info stored
 * `"null"`, so `getDefaultValue()` wrongly returned `null`.
 *
 * @see https://github.com/zephir-lang/zephir/pull/2566
 */
final class ConstantParameterDefaultTest extends TestCase
{
    public function testClassConstantParameterDefaultsAreExposedViaReflection(): void
    {
        $ctor = (new ReflectionClass(ConstantParameterDefault::class))->getConstructor();

        $params = [];
        foreach ($ctor->getParameters() as $p) {
            $params[$p->getName()] = $p;
        }

        // string class-constant default (self::DEFAULT_CIPHER)
        $this->assertTrue($params['cipher']->isDefaultValueAvailable());
        $this->assertSame('aes-256-cfb', $params['cipher']->getDefaultValue());

        // int class-constant default (self::DEFAULT_ROUNDS)
        $this->assertTrue($params['rounds']->isDefaultValueAvailable());
        $this->assertSame(12, $params['rounds']->getDefaultValue());
    }
}
