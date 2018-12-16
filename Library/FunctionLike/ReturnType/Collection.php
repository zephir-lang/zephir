<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\FunctionLike\ReturnType;

use SplObjectStorage;
use Zephir\Exception\InvalidArgumentException;

final class Collection extends SplObjectStorage
{
    /**
     * Adds a return type to the storage.
     *
     * @param TypeInterface $returnType the return type instance
     * @param array         $definition the definition to associate with the return type instance
     *
     * @return $this
     *
     * @throws InvalidArgumentException
     */
    public function attach($returnType, $definition = null)
    {
        if (!$returnType instanceof TypeInterface) {
            throw new InvalidArgumentException(
                \sprintf(
                    'Expecting an instance of %s, got %s',
                    TypeInterface::class,
                    \is_object($returnType) ? \get_class($returnType) : \gettype($returnType)
                )
            );
        }

        parent::attach($returnType, $definition);

        return $this;
    }

    /**
     * Gets all return types satisfied by specification.
     *
     * @param SpecificationInterface $spec
     *
     * @return TypeInterface[]
     */
    public function getTypesBySpecification(SpecificationInterface $spec)
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
    public function areReturnTypesNullCompatible()
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IsNull());
    }

    /**
     * Checks whether at least one return type hint is integer compatible.
     *
     * @return bool
     */
    public function areReturnTypesIntCompatible()
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IntCompatible());
    }

    /**
     * Checks whether at least one return type hint is double compatible.
     *
     * @return bool
     */
    public function areReturnTypesDoubleCompatible()
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IsDouble());
    }

    /**
     * Checks whether at least one return type hint is bool compatible.
     *
     * @return bool
     */
    public function areReturnTypesBoolCompatible()
    {
        return $this->isSatisfiedByTypeSpec(new Specification\IsBool());
    }

    /**
     * Checks whether at least one return type hint is string compatible.
     *
     * @return bool
     */
    public function areReturnTypesStringCompatible()
    {
        return $this->isSatisfiedByTypeSpec(new Specification\StringCompatible());
    }

    /**
     * Checks whether at least one return type hint is string compatible.
     *
     * @return bool
     */
    public function areReturnTypesArrayCompatible()
    {
        return $this->isSatisfiedByTypeSpec(new Specification\ArrayCompatible());
    }

    /**
     * Checks whether at least one return type hint is object compatible.
     *
     * @return bool
     */
    public function areReturnTypesObjectCompatible()
    {
        return \count($this->getObjectLikeReturnTypes()) > 0;
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
     * Checks whether all return types hint are well known.
     *
     * @return bool
     */
    public function areReturnTypesWellKnown()
    {
        if (0 == $this->count()) {
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
            ->either(new Specification\IsVoid());

        $found = $this->getTypesBySpecification($spec);

        return \count($found) == $this->count();
    }

    /**
     * Checks if the method have compatible return types.
     *
     * This method does not check for special type.
     *
     * {@see \Zephir\FunctionLike\ReturnType\Collection::areReturnTypesWellKnown}
     *
     * @return bool
     */
    public function areReturnTypesCompatible()
    {
        $numberOfReturnTypes = $this->count();

        if (0 == $numberOfReturnTypes || 1 == $numberOfReturnTypes) {
            return true;
        }

        // <Class_1> | <Class_2> | ... | <Class_n>
        $classes = $this->getObjectLikeReturnTypes();
        if (\count($classes) > 1) {
            return false;
        }

        // <Class> | <Class_1[]> | <Class_1[]> | ... | <Class_n[]>
        $collections = $this->getTypesBySpecification(new Specification\IsCollection());
        if (\count($collections) > 0 && \count($classes) > 0) {
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

        // <Class_1> | <T1> | <T2> | ... | <Tn>
        if (\count($classes) > 0 && $summ > 0) {
            return false;
        }

        // T1 | T2 | ... | Tn | null
        // T1 | T2 | ... | Tn
        return 1 == $summ;
    }

    /**
     * Returns an array of cast hinted return types (if any).
     *
     * @return TypeInterface[]
     */
    public function getCastHintedReturnTypes()
    {
        return $this->getTypesBySpecification(new Not(new Specification\IsReal()));
    }

    /**
     * Returns an array of real return types (if any).
     *
     * @return TypeInterface[]
     */
    public function getRealReturnTypes()
    {
        return $this->getTypesBySpecification(new Specification\IsReal());
    }

    /**
     * Checks if the method must not return any value.
     *
     * @return bool
     */
    public function onlyVoid()
    {
        return 1 == $this->count() && $this->isSatisfiedByTypeSpec(new Specification\IsVoid());
    }

    private function isSatisfiedByTypeSpec(SpecificationInterface $spec)
    {
        if (0 == $this->count()) {
            return false;
        }

        $found = $this->getTypesBySpecification($spec);

        return \count($found) > 0;
    }
}
