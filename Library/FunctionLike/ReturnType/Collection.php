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

namespace Zephir\FunctionLike\ReturnType;

use SplObjectStorage;
use Zephir\Exception\InvalidArgumentException;

use function count;

final class Collection extends SplObjectStorage
{
    /**
     * {@inheritdoc}
     *
     * @param TypeInterface $returnType
     *
     * @return string
     */
    #[\ReturnTypeWillChange]
    public function getHash($returnType)
    {
        return $returnType->getDataType();
    }

    /**
     * {@inheritdoc}
     *
     * @param TypeInterface $returnType The return type instance
     * @param array         $definition The definition to associate with the return type instance
     *
     * @return $this
     *
     * @throws InvalidArgumentException
     */
    #[\ReturnTypeWillChange]
    public function attach($returnType, $definition = null)
    {
        if (!$returnType instanceof TypeInterface) {
            $wrong = \is_object($returnType) ? 'instance of '.\get_class($returnType) : \gettype($returnType);

            throw new InvalidArgumentException(
                sprintf('Expecting an instance of %s, got %s', TypeInterface::class, $wrong)
            );
        }

        parent::attach($returnType, $definition);

        return $this;
    }

    /**
     * Checks if the collection has return-type or cast hints.
     *
     * @return bool
     */
    public function hasReturnTypes(): bool
    {
        if (0 === $this->count() || $this->onlyVoid()) {
            return false;
        }

        return true;
    }

    /**
     * Gets all return types satisfied by specification.
     *
     * @param SpecificationInterface $spec
     *
     * @return TypeInterface[]
     */
    public function getTypesBySpecification(SpecificationInterface $spec): array
    {
        $types = [];

        foreach ($this as $type) {
            /** @var TypeInterface $type */
            if ($spec->isSatisfiedBy($type)) {
                $types[] = $type;
            }
        }

        return $types;
    }

    /**
     * Checks whether at least one return type hint is null compatible.
     *
     * @return bool
     */
    public function areReturnTypesNullCompatible(): bool
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IsNull());
    }

    /**
     * Checks whether at least one return type hint is integer compatible.
     *
     * @return bool
     */
    public function areReturnTypesIntCompatible(): bool
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IntCompatible());
    }

    /**
     * Checks whether at least one return type hint is double compatible.
     *
     * @return bool
     */
    public function areReturnTypesDoubleCompatible(): bool
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IsDouble());
    }

    /**
     * Checks whether at least one return type hint is bool compatible.
     *
     * @return bool
     */
    public function areReturnTypesBoolCompatible(): bool
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IsBool());
    }

    /**
     * Checks whether at least one return type hint is string compatible.
     *
     * @return bool
     */
    public function areReturnTypesStringCompatible(): bool
    {
        return $this->isSatisfiedByTypeSpec(new Specification\StringCompatible());
    }

    /**
     * Checks whether at least one return type hint is string compatible.
     *
     * @return bool
     */
    public function areReturnTypesArrayCompatible(): bool
    {
        return $this->isSatisfiedByTypeSpec(new Specification\ArrayCompatible());
    }

    /**
     * Checks whether at least one return type hint is object compatible.
     *
     * @return bool
     */
    public function areReturnTypesObjectCompatible(): bool
    {
        return count($this->getObjectLikeReturnTypes()) > 0;
    }

    /**
     * Get object like return types.
     *
     * @return TypeInterface[]
     */
    public function getObjectLikeReturnTypes()
    {
        return $this->getTypesBySpecification(new Specification\ObjectLike());
    }

    /**
     * Checks if the collection has only void.
     */
    public function onlyVoid()
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IsVoid()) && 1 == $this->count();
    }

    /**
     * Checks if the collection has only special types.
     */
    public function onlySpecial()
    {
        if (0 == $this->count()) {
            return false;
        }

        $found = $this->getTypesBySpecification(new Specification\IsSpecial());

        return count($found) == $this->count();
    }

    /**
     * Checks whether all return types hint are well known.
     *
     * @return bool
     */
    public function areReturnTypesWellKnown(): bool
    {
        if (0 === $this->count() || $this->isSatisfiedByTypeSpec(new Specification\IsSpecial())) {
            return false;
        }

        $spec =
            (new Specification\ObjectLike())
            ->either(new Specification\ArrayCompatible())
            ->either(new Specification\IntCompatible())
            ->either(new Specification\StringCompatible())
            ->either(new Specification\IsBool())
            ->either(new Specification\IsDouble())
            ->either(new Specification\IsNull())
            // TODO: Please double check this
            ->either(new Specification\IsVoid());

        $found = $this->getTypesBySpecification($spec);

        return count($found) == $this->count();
    }

    /**
     * Checks if the collection have compatible return types.
     *
     * @return bool
     */
    public function areReturnTypesCompatible(): bool
    {
        $numberOfReturnTypes = $this->count();

        if (0 == $numberOfReturnTypes || 1 == $numberOfReturnTypes) {
            return true;
        }

        // <Class_1> | <Class_2> | ... | <Class_n>
        $classes = $this->getObjectLikeReturnTypes();

        // <Class> | <Class_1[]> | <Class_1[]> | ... | <Class_n[]>
        $collections = $this->getTypesBySpecification(new Specification\IsCollection());
        if (count($collections) > 0 && $this->areReturnTypesObjectCompatible()) {
            return false;
        }

        $summ =
            // array | <Class_1[]> | <Class_2[]> | ... | <Class_n[]>
            (int) $this->areReturnTypesArrayCompatible() +

            // char | uchar | int | uint | long | ulong
            (int) $this->areReturnTypesIntCompatible() +

            // bool
            (int) $this->areReturnTypesBoolCompatible() +

            // istring | string
            (int) $this->areReturnTypesStringCompatible() +

            // double
            (int) $this->areReturnTypesDoubleCompatible();

        // T1 | T2 | ... | Tn
        if ($summ > 1) {
            return false;
        }

        // <Class_1> | <T1>
        if ($this->areReturnTypesObjectCompatible() && $summ > 0) {
            return false;
        }

        // null
        $null = (int) $this->areReturnTypesNullCompatible();

        // <T1> | null
        // <Class_1> | null
        // <Class_1[]> | null
        if (1 == $null && 2 == $this->count()) {
            return true;
        }

        // T1 | T2 | ... | Tn | null
        if (1 == $null && $null + $summ != $this->count()) {
            return false;
        }

        return 1 == $summ;
    }

    /**
     * Returns an array of cast hinted return types (if any).
     *
     * @return TypeInterface[]
     */
    public function getCastHintedReturnTypes(): array
    {
        return $this->getTypesBySpecification(new Not(new Specification\IsReal()));
    }

    /**
     * Returns an array of real return types (if any).
     *
     * @return TypeInterface[]
     */
    public function getRealReturnTypes(): array
    {
        return $this->getTypesBySpecification(new Specification\IsReal());
    }

    private function isSatisfiedByTypeSpec(SpecificationInterface $spec): bool
    {
        if (0 === $this->count()) {
            return false;
        }

        $found = $this->getTypesBySpecification($spec);

        return count($found) > 0;
    }
}
