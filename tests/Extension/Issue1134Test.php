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
use ReflectionClass;
use ReflectionException;
use Stub\Issue1134;

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1134
 */
final class Issue1134Test extends TestCase
{
    /**
     * @throws ReflectionException
     */
    public function testIssue914ReflectionParamDefaultValueShouldReturnTrue(): void
    {
        $ref = new ReflectionClass(Issue1134::class);
        $constructor = $ref->getConstructor();
        $params = $constructor->getParameters();

        $this->assertSame('config', $params[0]->getName());
        $this->assertTrue($params[0]->isOptional());
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->assertTrue($params[0]->isDefaultValueAvailable());
        }

        $arrayWithFilledDefaultValueParam = $ref->getMethod('arrayWithFilledDefaultValue');
        $params2 = $arrayWithFilledDefaultValueParam->getParameters();

        $this->assertSame('someDefaultData', $params2[0]->getName());
        $this->assertTrue($params2[0]->isOptional());
        if (version_compare(PHP_VERSION, '8.0.0', '>=')) {
            $this->assertTrue($params2[0]->isDefaultValueAvailable());
        }

        $class = new Issue1134();
        $this->assertSame([], $class->arrayWithFilledDefaultValue());
    }
}
