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

namespace Extension\Oo;

use PHPUnit\Framework\TestCase;
use Stub\Issue882;

final class Issue882Test extends TestCase
{
    /**
     * @see https://github.com/zephir-lang/zephir/issues/882
     */
    public function testShouldNotInstantiateClassWithProtectedConstructor(): void
    {
        $this->expectException(\Error::class);
        $this->expectExceptionMessage(
            'Call to protected Issue882ProtectedCtor::__construct() from scope Stub\Issue882'
        );

        Issue882::initClass(\Issue882ProtectedCtor::class);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/882
     */
    public function testShouldNotInstantiateClassWithPrivateConstructor(): void
    {
        $this->expectException(\Error::class);
        $this->expectExceptionMessage(
            'Call to private Issue882PrivateCtor::__construct() from scope Stub\Issue882'
        );

        Issue882::initClass(\Issue882PrivateCtor::class);
    }

    /**
     * @see https://github.com/zephir-lang/zephir/issues/882
     */
    public function testShouldInstantiateClassWithPublicConstructor(): void
    {
        $object = Issue882::initClass(\Issue882PublicCtor::class);

        $this->assertInstanceOf(\Issue882PublicCtor::class, $object);
        $this->assertTrue($object->built);
    }
}
