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

namespace Zephir\Stubs;

use Zephir\AliasManager;

use function implode;

/**
 * Shared Zephir->PHP type rendering used by the stub generator and its
 * docblock generator. Keeping the mapping in one place is what lets a union
 * type render identically in a method signature and in its `@param`/`@return`
 * docblock, so the two can never drift.
 */
trait TypeRenderer
{
    /**
     * Render a Zephir union type list (the AST `data-types` node) as a PHP
     * union type string such as `int|string|null` or `string|\Foo`. Each
     * member is mapped with mapTypeNode() so the Zephir->PHP coercions and
     * alias resolution stay identical to the single-type renderers. Members
     * that carry no expressible PHP hint (e.g. `var`) are skipped; an
     * all-unmappable list yields an empty string so the caller emits the
     * parameter/property untyped instead of a broken hint.
     */
    private function buildUnionType(array $dataTypes, AliasManager $aliasManager): string
    {
        $members = [];
        foreach ($dataTypes as $member) {
            $php = $this->mapTypeNode($member, $aliasManager);
            if (null !== $php) {
                $members[] = $php;
            }
        }

        return implode('|', $members);
    }

    /**
     * Map a single Zephir type AST node (`{data-type: ...}` or `{cast: ...}`)
     * to its PHP type name, or null when the node carries no expressible hint.
     * Used by buildUnionType() so union members coerce and alias-resolve
     * exactly like the single-type parameter/property renderers.
     */
    private function mapTypeNode(array $node, AliasManager $aliasManager): ?string
    {
        if (isset($node['cast'])) {
            return $aliasManager->isAlias($node['cast']['value'])
                ? '\\' . $aliasManager->getAlias($node['cast']['value'])
                : $node['cast']['value'];
        }

        if (!isset($node['data-type'])) {
            return null;
        }

        switch ($node['data-type']) {
            case 'array':
                return 'array';
            case 'object':
                return 'object';
            case 'bool':
            case 'boolean':
                return 'bool';
            case 'double':
                return 'float';
            case 'int':
            case 'uint':
            case 'long':
            case 'ulong':
            case 'uchar':
                return 'int';
            case 'char':
            case 'string':
                return 'string';
            case 'null':
                return 'null';
            case 'false':
                return 'false';
            case 'true':
                return 'true';
            default:
                return null;
        }
    }
}
