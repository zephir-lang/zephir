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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

use function file_get_contents;
use function glob;

/**
 * `X::class` is the name PHP resolves at compile time: as written, through
 * `use` aliases (matched case-insensitively), without a leading backslash and
 * without loading the class. `parent::class` is the `extends` name as written.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2825
 */
final class Issue2825Test extends TestCase
{
    use GeneratesStubProject;

    protected function setUp(): void
    {
        $this->setUpStubProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testGlobalClassNameHasNoLeadingBackslash(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            use DateTime;
            use ArrayObject as Renamed;
            use \Countable;

            class Names
            {
                const GLOBAL_NAME = DateTime::class;
                const ALIAS_NAME = Renamed::class;
                const FOLDED_NAME = DateTime::class . "!";

                public propDefault = DateTime::class;
                public static staticDefault = Renamed::class;

                public static function imported() -> string { return DateTime::class; }
                public static function aliased() -> string { return Renamed::class; }
                public static function qualified() -> string { return \DateTime::class; }
                public static function leadingUse() -> string { return Countable::class; }
                public static function concat() -> string { return DateTime::class . "!"; }
            }
            ZEP);

        $this->assertStringNotContainsString('"\\\\', $generated);
        $this->assertStringContainsString('SL("GLOBAL_NAME"), "DateTime")', $generated);
        $this->assertStringContainsString('SL("ALIAS_NAME"), "ArrayObject")', $generated);
        $this->assertStringContainsString('SL("FOLDED_NAME"), "DateTime!")', $generated);
        $this->assertStringContainsString('SL("propDefault"), "DateTime"', $generated);
        $this->assertStringContainsString('SL("staticDefault"), "ArrayObject"', $generated);
        $this->assertStringContainsString('RETURN_STRING("Countable");', $generated);
        $this->assertSame(2, substr_count($generated, 'RETURN_STRING("DateTime");'));
        $this->assertStringContainsString('RETURN_STRING("ArrayObject");', $generated);
    }

    public function testNameIsKeptAsWritten(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names
            {
                public static function lower() -> string { return \datetime::class; }
                public static function own() -> string { return \stub\names::class; }
            }
            ZEP);

        $this->assertStringContainsString('RETURN_STRING("datetime");', $generated);
        $this->assertStringContainsString('RETURN_STRING("stub\\\\names");', $generated);
    }

    public function testUnknownClassNeedsNoDefinition(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names
            {
                public static function relative() -> string { return NotExisting::class; }
                public static function qualified() -> string { return \Foo\bar::class; }
            }
            ZEP);

        $this->assertStringContainsString('RETURN_STRING("Stub\\\\NotExisting");', $generated);
        $this->assertStringContainsString('RETURN_STRING("Foo\\\\bar");', $generated);
    }

    public function testAliasIsMatchedCaseInsensitively(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            use DateTime;

            class Names
            {
                public static function name() -> string { return datetime::class; }
                public static function make() -> var { return new datetime(); }
            }
            ZEP);

        $this->assertStringContainsString('RETURN_STRING("DateTime");', $generated);
        $this->assertStringNotContainsString('Stub\\\\datetime', $generated);
    }

    public function testParentClassIsTheExtendsName(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names extends \Exception
            {
                public function name() -> string { return parent::class; }
            }
            ZEP);

        $this->assertStringContainsString('RETURN_STRING("Exception");', $generated);
    }

    public function testParentClassIsKeptAsWritten(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            use Exception as Base;

            class Names extends Base
            {
                public function name() -> string { return parent::class; }
            }
            ZEP);

        $this->assertStringContainsString('RETURN_STRING("Exception");', $generated);
    }

    public function testParentClassWithoutParentIsAnError(): void
    {
        $result = $this->generate(['names.zep' => <<<'ZEP'
            namespace Stub;

            class Names
            {
                public function name() -> string { return parent::class; }
            }
            ZEP]);

        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString(
            'Cannot use "parent" when current class scope has no parent',
            $result['stdout'] . $result['stderr']
        );
    }

    public function testNameInLowercaseNamespaceSegmentIsEscaped(): void
    {
        $generated = $this->generateOk('nested/thing.zep', <<<'ZEP'
            namespace Stub\nested;

            class Thing
            {
                public static function own() -> string { return self::class; }
                public static function named() -> string { return Thing::class; }
            }
            ZEP);

        $this->assertSame(2, substr_count($generated, 'RETURN_STRING("Stub\\\\nested\\\\Thing");'));
    }

    public function testNewAndInstanceOfParentOnNamespacedInternalParent(): void
    {
        $generated = $this->generateOk('names.zep', <<<'ZEP'
            namespace Stub;

            class Names extends \Random\RandomError
            {
                public function make() -> var { return new parent("x"); }
                public function test(var o) -> bool { return o instanceof parent; }
            }
            ZEP);

        // The class registration, `new parent` and `instanceof parent`.
        $this->assertStringNotContainsString('stub_random_randomerror_ce', $generated);
        $this->assertSame(3, preg_match_all('/SL\("random\\\\\\\\randomerror"\)/i', $generated));
    }

    public function testTraitMemberResolvesNamesInTheTraitFile(): void
    {
        $result = $this->generate([
            'traits/tr.zep' => <<<'ZEP'
                namespace Stub\Traits;

                use ArrayObject as Ao;

                trait Tr
                {
                    const NAME = Ao::class;

                    public prop = Ao::class;

                    public function aliased() -> string { return Ao::class; }
                    public function relative() -> string { return Helper::class; }
                    public function make() -> var { return new Ao(); }
                    public function typed(<Ao> value) -> <Ao> { return value; }
                    public function closure() -> var { return function () { return Ao::class; }; }
                }
                ZEP,
            'user.zep' => <<<'ZEP'
                namespace Stub;

                class User
                {
                    use \Stub\Traits\Tr;
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $user = (string) file_get_contents($this->projectDir . '/ext/stub/user.zep.c');
        $this->assertStringNotContainsString('Stub\\\\Ao', $user);
        $this->assertStringContainsString('SL("NAME"), "ArrayObject")', $user);
        $this->assertStringContainsString('SL("prop"), "ArrayObject"', $user);
        $this->assertStringContainsString('RETURN_STRING("ArrayObject");', $user);
        $this->assertStringContainsString('RETURN_STRING("Stub\\\\Traits\\\\Helper");', $user);
        $this->assertStringContainsString('object_init_ex(return_value, spl_ce_ArrayObject);', $user);

        $header = (string) file_get_contents($this->projectDir . '/ext/stub/user.zep.h');
        $this->assertStringNotContainsString('Stub\\\\Ao', $header);
        $this->assertStringContainsString('ArrayObject', $header);

        $closures = glob($this->projectDir . '/ext/stub/*__closure.zep.c') ?: [];
        $this->assertCount(2, $closures);
        foreach ($closures as $closure) {
            $this->assertStringContainsString('RETURN_STRING("ArrayObject");', (string) file_get_contents($closure));
        }
    }
}
