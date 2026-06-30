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

namespace Zephir\Test\Compiler;

use PHPUnit\Framework\TestCase;
use ReflectionClass;
use ReflectionMethod;

/**
 * Covers the bundled redis prototype: the `redis` stub itself and the
 * `RedisCluster` stub that must live in the same file, because the
 * prototype loader maps the file basename to an extension name and
 * RedisCluster ships with the `redis` extension (see Compiler::generate()).
 *
 * @runTestsInSeparateProcesses
 *
 * @preserveGlobalState disabled
 */
final class PrototypesTest extends TestCase
{
    protected function setUp(): void
    {
        if (extension_loaded('redis')) {
            $this->markTestSkipped('ext-redis is loaded; the redis prototype must not be required.');
        }

        require_once __DIR__ . '/../../../prototypes/redis.php';
    }

    public function testRedisPrototypeDefinesBothClasses(): void
    {
        $this->assertTrue(class_exists('redis', false));
        $this->assertTrue(class_exists('RedisCluster', false));
    }

    public function testRedisClusterConstants(): void
    {
        $constants = (new ReflectionClass('RedisCluster'))->getConstants();

        // Cluster-specific failover constants.
        $this->assertSame(5, $constants['OPT_SLAVE_FAILOVER']);
        $this->assertSame(0, $constants['FAILOVER_NONE']);
        $this->assertSame(1, $constants['FAILOVER_ERROR']);
        $this->assertSame(2, $constants['FAILOVER_DISTRIBUTE']);
        $this->assertSame(3, $constants['FAILOVER_DISTRIBUTE_SLAVES']);

        // Constants shared with Redis.
        $this->assertSame(1, $constants['OPT_SERIALIZER']);
        $this->assertSame(2, $constants['OPT_PREFIX']);
        $this->assertSame(3, $constants['OPT_READ_TIMEOUT']);
        $this->assertSame(4, $constants['OPT_SCAN']);
        $this->assertSame(0, $constants['SERIALIZER_NONE']);
        $this->assertSame(1, $constants['SERIALIZER_PHP']);
        $this->assertSame(2, $constants['SERIALIZER_IGBINARY']);
        $this->assertSame(0, $constants['ATOMIC']);
        $this->assertSame(1, $constants['MULTI']);
        $this->assertSame(0, $constants['SCAN_NORETRY']);
        $this->assertSame(1, $constants['SCAN_RETRY']);

        // RedisCluster does not support pipelining.
        $this->assertArrayNotHasKey('PIPELINE', $constants);
    }

    public function testRedisClusterConstructorSignature(): void
    {
        $constructor = new ReflectionMethod('RedisCluster', '__construct');
        $parameters  = $constructor->getParameters();

        $this->assertCount(7, $parameters);
        $this->assertSame(1, $constructor->getNumberOfRequiredParameters());

        $this->assertSame('name', $parameters[0]->getName());
        $this->assertSame('seeds', $parameters[1]->getName());
        $this->assertSame('timeout', $parameters[2]->getName());
        $this->assertSame('read_timeout', $parameters[3]->getName());
        $this->assertSame('persistent', $parameters[4]->getName());
        $this->assertSame('auth', $parameters[5]->getName());
        $this->assertSame('context', $parameters[6]->getName());

        $this->assertNull($parameters[1]->getDefaultValue());
        $this->assertSame(0, $parameters[2]->getDefaultValue());
        $this->assertSame(0, $parameters[3]->getDefaultValue());
        $this->assertFalse($parameters[4]->getDefaultValue());
        $this->assertNull($parameters[5]->getDefaultValue());
        $this->assertNull($parameters[6]->getDefaultValue());
    }

    /**
     * @dataProvider clusterMethodsProvider
     */
    public function testRedisClusterHasMethod(string $method): void
    {
        $this->assertTrue(
            (new ReflectionClass('RedisCluster'))->hasMethod($method),
            "RedisCluster prototype is missing the {$method}() method"
        );
    }

    public static function clusterMethodsProvider(): array
    {
        return [
            ['get'],
            ['set'],
            ['del'],
            ['exists'],
            ['mget'],
            ['mset'],
            ['setOption'],
            ['getOption'],
            ['close'],
            ['flushDB'],
            ['_masters'],
        ];
    }

    public function testRedisClusterFlushDbRequiresNodeArgument(): void
    {
        // Directed node command: $key_or_address is mandatory in cluster mode.
        $method     = new ReflectionMethod('RedisCluster', 'flushDB');
        $parameters = $method->getParameters();

        $this->assertSame(1, $method->getNumberOfRequiredParameters());
        $this->assertSame('key_or_address', $parameters[0]->getName());
        $this->assertFalse($parameters[1]->getDefaultValue());
    }

    public function testRedisClusterHasNoDeleteAlias(): void
    {
        // delete() never existed on RedisCluster (and was removed from Redis in phpredis 6).
        $this->assertFalse((new ReflectionClass('RedisCluster'))->hasMethod('delete'));
    }

    public function testRedisHasCanonicalDelAndKeepsDeleteAlias(): void
    {
        $redis = new ReflectionClass('redis');

        $this->assertTrue($redis->hasMethod('del'));
        $this->assertTrue($redis->getMethod('del')->isVariadic());
        $this->assertTrue($redis->hasMethod('delete'));
    }

    public function testRedisFlushDbAcceptsAsyncFlag(): void
    {
        $parameters = (new ReflectionMethod('redis', 'flushDB'))->getParameters();

        $this->assertCount(1, $parameters);
        $this->assertSame('async', $parameters[0]->getName());
        $this->assertFalse($parameters[0]->getDefaultValue());
    }
}
