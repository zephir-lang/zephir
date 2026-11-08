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

use ArrayObject as Renamed;
use Countable;
use DateTime;
use PHPUnit\Framework\TestCase;
use ReflectionClass;
use Stub\Constants;
use Stub\Issue2825;

/**
 * Every expected value is PHP's own `::class` for the same spelling.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
final class Issue2825Test extends TestCase
{
    public function testGlobalClassHasNoLeadingBackslash(): void
    {
        $this->assertSame(DateTime::class, Issue2825::imported());
        $this->assertSame(\DateTime::class, Issue2825::qualified());
        $this->assertSame(Countable::class, Issue2825::importedWithBackslash());
    }

    public function testAliasResolvesToTheImportedName(): void
    {
        $this->assertSame(Renamed::class, Issue2825::aliased());
        $this->assertSame(datetime::class, Issue2825::aliasInOtherCase());
    }

    public function testNameIsKeptAsWritten(): void
    {
        $this->assertSame(\datetime::class, Issue2825::keptAsWritten());
    }

    public function testUnknownClassIsResolvedWithoutLoadingIt(): void
    {
        $this->assertSame(\Stub\NotLoaded::class, Issue2825::unknown());
        $this->assertSame(\Foo\bar::class, Issue2825::unknownQualified());
    }

    public function testParentIsTheExtendsName(): void
    {
        $this->assertSame(\ArrayObject::class, (new Issue2825())->parentName());
    }

    public function testConcatenationAndConstantFolding(): void
    {
        $this->assertSame(DateTime::class . '!', Issue2825::concatenated());
        $this->assertSame(DateTime::class . '!', Issue2825::FOLDED_NAME);
    }

    public function testClassConstantsAndPropertyDefaults(): void
    {
        $this->assertSame(DateTime::class, Issue2825::GLOBAL_NAME);
        $this->assertSame(Renamed::class, Issue2825::ALIAS_NAME);
        $this->assertSame(DateTime::class, (new Issue2825())->propDefault);
        $this->assertSame(Renamed::class, Issue2825::$staticDefault);

        $defaults = (new ReflectionClass(Issue2825::class))->getDefaultProperties();
        $this->assertSame(DateTime::class, $defaults['propDefault']);
    }

    public function testCharConstantAsPropertyDefault(): void
    {
        $this->assertSame(Constants::ANNOTATION_REGEX, (new Constants())->getPropertyC7());
        $this->assertSame(Constants::PARAMETER_REGEX, (new Constants())->getPropertyC8());
    }

    public function testNewWithAliasInOtherCase(): void
    {
        $this->assertInstanceOf(DateTime::class, Issue2825::newAliasInOtherCase());
    }
}
