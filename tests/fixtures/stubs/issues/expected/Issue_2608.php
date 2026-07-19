<?php

namespace Stubs;

/**
 * Exercises issue #2608 — typed class properties must carry their PHP type
 * hint into the generated stub. Covers every branch of
 * Stubs\Generator::buildProperty(): array, bool, int, double->float, string,
 * an explicit-nullable scalar with a non-null default, a nullable scalar with
 * a null default, a class cast, and a nullable class cast.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2608
 */
class Issue_2608
{

    public array $orgs = [];

    protected bool $valid = false;

    public int $counter = 7;

    public float $ratio = 1.5;

    public string $label = 'hi';

    private ?string $val = '';

    public ?int $maybeNum = null;

    public \Iterator $friend;

    public ?\Iterator $maybeFriend = null;

}
