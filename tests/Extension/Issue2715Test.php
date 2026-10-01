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
use Stub\Issue2715;

final class Issue2715Magic
{
    public function __call($name, $args)
    {
        return $name;
    }

    public static function __callStatic($name, $args)
    {
        return $name;
    }
}

final class Issue2715Container
{
    private array $services = ['myService' => 'the real service'];

    public function __call($method, $args)
    {
        if (strpos($method, 'get') === 0) {
            $name = lcfirst(substr($method, 3));

            return $this->services[$name] ?? 'NO SERVICE NAMED ' . $name;
        }

        return 'NOT A GETTER';
    }
}

final class Issue2715Plain
{
    public function declaredMethod(): string
    {
        return 'declared';
    }
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2715
 */
final class Issue2715Test extends TestCase
{
    public function testDeclaredMethodIsReachedWhateverTheSpelling(): void
    {
        $test = new Issue2715();

        $this->assertSame('declared', $test->callDeclaredMethod(new Issue2715Plain()));
        $this->assertSame('self', $test->callSelfMixedCase());
    }

    public function testMagicCallReceivesTheNameAsWritten(): void
    {
        $test  = new Issue2715();
        $magic = new Issue2715Magic();

        $this->assertSame('NoPeCaSe', $test->callMixedCase($magic));
        $this->assertSame('NoPeCaSe', $test->callDynamicStringName($magic));
        $this->assertSame('NoPeCaSe', $test->callDynamicVariableName($magic, 'NoPeCaSe'));
    }

    public function testMagicStaticCallReceivesTheNameAsWritten(): void
    {
        $test = new Issue2715();

        $this->assertSame('NoPeCaSe', $test->callStaticMixedCase(Issue2715Magic::class));
        $this->assertSame('NoPeCaSe', $test->callStaticDynamicName(Issue2715Magic::class));
    }

    public function testServiceGetterResolvesTheServiceItNames(): void
    {
        $test = new Issue2715();

        $this->assertSame('the real service', $test->callServiceGetter(new Issue2715Container()));
    }
}
