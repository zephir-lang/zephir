<?php

namespace Stub\PhpStan;

/**
 * Showcase class for PHPStan/Psalm/template tag pass-through.
 *
 * @template T of \Phalcon\Mvc\ModelInterface
 * @extends \Phalcon\Mvc\Model<T>
 * @implements \Foo\BarInterface<T>
 * @phpstan-type FindParams array{
 *     conditions?: string,
 *     bind?: array<string, mixed>,
 *     limit?: int,
 * }
 */
class PhpStanShowcase
{


    /**
     * Find records matching parameters.
     *
     * @phpstan-return \Phalcon\Mvc\Model\Resultset\Simple<array-key, static>
     * @psalm-param array<string, mixed> $parameters
     * @param array $parameters
     * @return array
     */
    public static function find(array $parameters): array
    {
    }

    /**
     * Find the first record matching parameters.
     *
     * @phpstan-return static|null
     * @param array $parameters
     */
    public static function findFirst(array $parameters)
    {
    }
}
