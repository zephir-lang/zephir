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

namespace Zephir\Test\Stubs;

use PHPUnit\Framework\TestCase;
use Zephir\Stubs\TagRegistry;

final class TagRegistryTest extends TestCase
{
    public function preservedExactProvider(): array
    {
        return [
            ['template'],
            ['template-covariant'],
            ['template-contravariant'],
            ['extends'],
            ['implements'],
        ];
    }

    /**
     * @dataProvider preservedExactProvider
     */
    public function testIsPreservedTagExactNames(string $tag): void
    {
        $this->assertTrue(TagRegistry::isPreservedTag($tag));
    }

    public function preservedPrefixedProvider(): array
    {
        return [
            ['phpstan-return'],
            ['phpstan-param'],
            ['phpstan-type'],
            ['phpstan-import-type'],
            ['phpstan-template'],
            ['phpstan-var'],
            ['psalm-return'],
            ['psalm-param'],
            ['psalm-type'],
            ['psalm-template'],
            ['psalm-var'],
        ];
    }

    /**
     * @dataProvider preservedPrefixedProvider
     */
    public function testIsPreservedTagPrefixed(string $tag): void
    {
        $this->assertTrue(TagRegistry::isPreservedTag($tag));
    }

    public function notPreservedProvider(): array
    {
        return [
            ['param'],
            ['return'],
            ['var'],
            ['throws'],
            ['deprecated'],
            ['author'],
            ['phpstan'],
            ['psalm'],
            ['template-'],
            [''],
            ['totally-made-up'],
        ];
    }

    /**
     * @dataProvider notPreservedProvider
     */
    public function testIsPreservedTagRejectsNonWhitelisted(string $tag): void
    {
        $this->assertFalse(TagRegistry::isPreservedTag($tag));
    }
}
