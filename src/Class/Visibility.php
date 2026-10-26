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

namespace Zephir\Class;

use ReflectionMethod;
use ReflectionProperty;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\CompilationContext;

use function array_key_first;
use function array_keys;
use function array_search;
use function array_slice;
use function class_exists;
use function class_parents;
use function in_array;
use function interface_exists;
use function ltrim;
use function method_exists;
use function property_exists;
use function strtolower;

/**
 * PHP's visibility rules for methods and static properties, applied while
 * compiling.
 *
 * Each check gives the answer the engine gives when the same code runs. The
 * scope is the class the code is written in (for a closure, the class around
 * it), never the class of the receiver. Classes are compared by name, because
 * the compiler can hold more than one definition of the same class.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2734
 */
final class Visibility
{
    /**
     * Whether the code being compiled may call $method, as
     * zend_check_method_accessible() decides it.
     */
    public static function canCallMethod(Method $method, CompilationContext $context): bool
    {
        $scope = self::scopeOf($context);
        if (null !== $scope && $scope->isTrait()) {
            return true;
        }

        return self::isMethodAccessible($method, $scope);
    }

    /**
     * Whether the code being compiled may access the static $property.
     *
     * A protected property is checked against its root class, the rule PHP
     * applies from 8.4 on. Up to 8.3 PHP checks the declaring class, which
     * refuses more, and a generated extension is built for every version.
     */
    public static function canAccessProperty(Property $property, CompilationContext $context): bool
    {
        $scope = self::scopeOf($context);
        if (null !== $scope && $scope->isTrait()) {
            return true;
        }

        if (!$property->isPrivate() && !$property->isProtected()) {
            return true;
        }

        $declaring = $property->getClassDefinition();
        if (null !== $scope && self::nameOf($declaring) === self::nameOf($scope)) {
            return true;
        }

        if ($property->isPrivate() || null === $scope) {
            return false;
        }

        $line = self::lineage($declaring);

        return self::isProtectedCompatible(self::rootOf($line, $property->getName(), false), $line, $scope);
    }

    /**
     * The class the code being compiled runs in, like `EX(func)->op_array.scope`:
     * the class being compiled, or for a closure the class it is written in.
     * Null inside a function, which PHP reports as the global scope. A trait's
     * methods only get a scope in the class that uses them.
     */
    public static function scopeOf(CompilationContext $context): ?Definition
    {
        $definition = $context->classDefinition;
        if (null === $definition) {
            return null;
        }

        return $definition->getEnclosingClassDefinition() ?? $definition;
    }

    /**
     * zend_check_method_accessible(): the scope is the declaring class, or the
     * method is protected and the scope shares a line of inheritance with the
     * method's root class.
     */
    public static function isMethodAccessible(Method $method, ?Definition $scope): bool
    {
        if (!$method->isPrivate() && !$method->isProtected()) {
            return true;
        }

        $declaring = $method->getClassDefinition();
        if (null === $declaring) {
            return true;
        }

        if (null !== $scope && self::nameOf($declaring) === self::nameOf($scope)) {
            return true;
        }

        if ($method->isPrivate() || null === $scope) {
            return false;
        }

        $line = self::lineage($declaring);

        return self::isProtectedCompatible(self::rootOf($line, $method->getName(), true), $line, $scope);
    }

    /**
     * zend_check_protected(): the scope is the root or one of its ancestors,
     * or the root is one of the scope's ancestors.
     *
     * @param array<string, Definition|null> $line Lineage of the declaring class.
     */
    private static function isProtectedCompatible(string $root, array $line, Definition $scope): bool
    {
        $names     = array_keys($line);
        $fromRoot  = array_slice($names, (int) array_search($root, $names, true));
        $scopeName = self::nameOf($scope);

        if (in_array($scopeName, $fromRoot, true)) {
            return true;
        }

        return isset(self::lineage($scope)[$root]);
    }

    /**
     * zend_get_function_root_class(), and its property counterpart: the
     * topmost class in the declaring class's lineage that declares the member
     * without making it private. An override keeps the root of the member it
     * overrides, which is what lets two subclasses reach each other's override
     * of a protected member they both inherit.
     *
     * @param array<string, Definition|null> $line Lineage of the declaring class.
     */
    private static function rootOf(array $line, string $member, bool $isMethod): string
    {
        $root = (string) array_key_first($line);

        foreach (array_slice($line, 1, null, true) as $className => $definition) {
            $isPrivate = $isMethod
                ? self::declaredMethodIsPrivate((string) $className, $definition, $member)
                : self::declaredPropertyIsPrivate((string) $className, $definition, $member);

            if (null === $isPrivate) {
                continue;
            }

            if ($isPrivate) {
                break;
            }

            $root = (string) $className;
        }

        return $root;
    }

    /**
     * The class and its ancestors, nearest first, keyed by lower-cased name.
     *
     * The value is the class's definition while the compiler has one. A class
     * the compiler only knows through reflection carries no parent link, so
     * the ancestors above it come from the running PHP, which has every
     * internal class loaded.
     *
     * @return array<string, Definition|null>
     */
    private static function lineage(Definition $definition): array
    {
        $line    = [];
        $current = $definition;

        while (true) {
            $line[self::nameOf($current)] = $current;

            if ($current->isBundled()) {
                return $line + self::loadedParents($current->getCompleteName());
            }

            $parent = $current->resolveExtendsDefinition();
            if (null === $parent) {
                $extends = (string) $current->getExtendsClass();
                if ('' !== $extends) {
                    $line[strtolower(ltrim($extends, '\\'))] = null;
                    $line += self::loadedParents($extends);
                }

                return $line;
            }

            $current = $parent;
        }
    }

    /**
     * @return array<string, null>
     */
    private static function loadedParents(string $className): array
    {
        $className = ltrim($className, '\\');
        if (!class_exists($className, false) && !interface_exists($className, false)) {
            return [];
        }

        $parents = [];
        foreach (class_parents($className, false) as $parent) {
            $parents[strtolower($parent)] = null;
        }

        return $parents;
    }

    /**
     * Whether the class declares the method as private, or null when it does
     * not have it. Reflected definitions carry no visibility, so classes
     * outside the extension are asked through reflection.
     */
    private static function declaredMethodIsPrivate(string $className, ?Definition $definition, string $name): ?bool
    {
        if (null !== $definition && !$definition->isBundled()) {
            return $definition->getMethod($name, false)?->isPrivate();
        }

        if (!class_exists($className, false) || !method_exists($className, $name)) {
            return null;
        }

        return (new ReflectionMethod($className, $name))->isPrivate();
    }

    private static function declaredPropertyIsPrivate(string $className, ?Definition $definition, string $name): ?bool
    {
        if (null !== $definition && !$definition->isBundled()) {
            $property = $definition->getProperties()[$name] ?? null;

            return $property?->isPrivate();
        }

        if (!class_exists($className, false) || !property_exists($className, $name)) {
            return null;
        }

        return (new ReflectionProperty($className, $name))->isPrivate();
    }

    private static function nameOf(Definition $definition): string
    {
        return strtolower(ltrim($definition->getCompleteName(), '\\'));
    }
}
