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

namespace Zephir\Class\Definition;

use Zephir\CompilationContext;
use Zephir\CompilerFile;
use Zephir\Exception\CompilerException;

use function array_key_exists;
use function array_merge;
use function in_array;
use function is_array;
use function sprintf;
use function strtolower;

/**
 * Copies trait members into every class (and trait) that `use`s them, before
 * code generation — the compile-time analog of the engine's link-time trait
 * binding (zephir#504).
 *
 * Members are re-instantiated from the trait's raw AST into the using
 * definition, so `self::`, `static::` and `this->` compile in the using
 * class's context, exactly like PHP's scope rebinding. Merge rules follow
 * PHP: the using class's own members win over trait members; trait members
 * override inherited ones (they land in the class's own method table); two
 * traits contributing the same concrete method is a compile error.
 */
final class TraitMerger
{
    /**
     * @var array<string, CompilerFile> Lowercased complete name => file
     */
    private array $files = [];

    /**
     * @var array<string, bool> Lowercased complete names already merged
     */
    private array $merged = [];

    /**
     * @var array<string, bool> Merge-in-progress set for cycle detection
     */
    private array $inProgress = [];

    /**
     * Post-merge member AST of each trait (its own members plus everything
     * inherited from traits it uses), keyed by lowercased complete name:
     * ['methods' => [...], 'properties' => [...], 'constants' => [...]]
     *
     * @var array<string, array<string, array>>
     */
    private array $effectiveMembers = [];

    /**
     * @param CompilerFile[] $compilerFiles
     */
    public function __construct(array $compilerFiles, private CompilationContext $context)
    {
        foreach ($compilerFiles as $compilerFile) {
            $definition = $compilerFile->getClassDefinition();
            if ($definition instanceof Definition) {
                $this->files[strtolower($definition->getCompleteName())] = $compilerFile;
            }
        }
    }

    /**
     * Merges the used traits' members into the given definition. Recursive:
     * a used trait is itself merged first, so trait-composing-trait chains
     * flatten in dependency order regardless of file order.
     */
    public function merge(Definition $definition): void
    {
        $key = strtolower($definition->getCompleteName());
        if (isset($this->merged[$key])) {
            return;
        }

        if (isset($this->inProgress[$key])) {
            throw new CompilerException(
                sprintf('Cyclic trait use detected involving "%s"', $definition->getCompleteName()),
                $definition->getOriginalNode()
            );
        }

        $usedTraits = $definition->getUsedTraits();
        if ([] === $usedTraits) {
            $this->rememberEffectiveMembers($definition, []);
            $this->merged[$key] = true;

            return;
        }

        $this->inProgress[$key] = true;

        $traitDefinitions = $definition->getUsedTraitDefinitions();

        /**
         * Collect winners across all used traits before registering anything,
         * so two-trait conflicts are decided on the full candidate set.
         * Methods key by lowercased name (Definition does the same);
         * properties and constants key by exact name.
         */
        $methods    = [];
        $properties = [];
        $constants  = [];

        foreach ($usedTraits as $traitName) {
            $traitDefinition = $traitDefinitions[strtolower($traitName)];
            $this->merge($traitDefinition);

            $incoming = $this->effectiveMembers[strtolower($traitDefinition->getCompleteName())];

            foreach ($incoming['methods'] as $member) {
                $this->pickMethod($methods, $member, $traitName, $definition);
            }
            foreach ($incoming['properties'] as $member) {
                $this->pickIdentical($properties, $member, $traitName, $definition, 'Property');
            }
            foreach ($incoming['constants'] as $member) {
                $this->pickIdentical($constants, $member, $traitName, $definition, 'Constant');
            }
        }

        /**
         * The using class's own members always win, silently — as in PHP.
         */
        $ownMethods    = $definition->getMethods();
        $ownProperties = $definition->getProperties();
        $ownConstants  = $definition->getConstants();

        $buckets = ['methods' => [], 'properties' => [], 'constants' => []];

        foreach ($methods as $lowerName => $candidate) {
            if (isset($ownMethods[$lowerName])) {
                continue;
            }

            $this->checkAbstractSatisfiable($candidate, $definition);
            $buckets['methods'][] = $candidate['member'];
            $definition->addMergedMemberName('methods', $candidate['member']['name']);
        }
        foreach ($properties as $name => $candidate) {
            if (isset($ownProperties[$name])) {
                continue;
            }
            $buckets['properties'][] = $candidate['member'];
            $definition->addMergedMemberName('properties', $name);
        }
        foreach ($constants as $name => $candidate) {
            if (isset($ownConstants[$name])) {
                continue;
            }
            $buckets['constants'][] = $candidate['member'];
            $definition->addMergedMemberName('constants', $name);
        }

        /**
         * Re-instantiate the surviving members on the using definition with
         * the exact construction path preCompile uses (including property
         * shortcut expansion).
         */
        $registrar = $this->files[$key] ?? null;
        if (null === $registrar) {
            throw new CompilerException(
                sprintf('Cannot locate the compiled file for "%s" while merging traits', $definition->getCompleteName()),
                $definition->getOriginalNode()
            );
        }

        $methodsBeforeRegistration = $definition->getMethods();

        $registrar->registerDefinitionMembers(
            $definition,
            ['properties' => $buckets['properties'], 'constants' => $buckets['constants'], 'methods' => $buckets['methods']]
        );

        /**
         * Run the per-method optimization pre-passes the merged methods missed
         * (their file's preCompile round ran before the merge). Trait method
         * bodies resolve imports against the USING file's aliases — prefer
         * fully-qualified names inside trait methods.
         */
        $this->context->classDefinition = $definition;
        $this->context->aliasManager    = $definition->getAliasManager();
        foreach ($definition->getMethods() as $methodKey => $method) {
            if (!isset($methodsBeforeRegistration[$methodKey]) && !$method->isAbstract()) {
                $method->preCompile($this->context);
            }
        }

        $this->rememberEffectiveMembers($definition, $buckets);

        unset($this->inProgress[$key]);
        $this->merged[$key] = true;
    }

    /**
     * Records a trait's post-merge member AST so traits composing it can
     * inherit the full flattened set. Classes never act as trait sources,
     * so nothing is recorded for them.
     */
    private function rememberEffectiveMembers(Definition $definition, array $mergedBuckets): void
    {
        if (!$definition->isTrait()) {
            return;
        }

        $own = $definition->getOriginalNode()['definition'] ?? [];

        $this->effectiveMembers[strtolower($definition->getCompleteName())] = [
            'methods'    => array_merge($own['methods'] ?? [], $mergedBuckets['methods'] ?? []),
            'properties' => array_merge($own['properties'] ?? [], $mergedBuckets['properties'] ?? []),
            'constants'  => array_merge($own['constants'] ?? [], $mergedBuckets['constants'] ?? []),
        ];
    }

    /**
     * Applies PHP's two-trait method rule: an abstract and a concrete version
     * pair up (the concrete one wins); two concrete or two abstract versions
     * collide.
     */
    private function pickMethod(array &$picked, array $member, string $traitName, Definition $target): void
    {
        $lowerName = strtolower($member['name']);

        if (!array_key_exists($lowerName, $picked)) {
            $picked[$lowerName] = ['member' => $member, 'trait' => $traitName];

            return;
        }

        $existing         = $picked[$lowerName];
        $existingAbstract = $this->isAbstractMember($existing['member']);
        $incomingAbstract = $this->isAbstractMember($member);

        if ($existingAbstract && !$incomingAbstract) {
            $picked[$lowerName] = ['member' => $member, 'trait' => $traitName];

            return;
        }

        if (!$existingAbstract && $incomingAbstract) {
            return;
        }

        throw new CompilerException(
            sprintf(
                'Trait method collision: "%s::%s" and "%s::%s" collide in "%s"',
                $existing['trait'],
                $existing['member']['name'],
                $traitName,
                $member['name'],
                $target->getCompleteName()
            ),
            $target->getOriginalNode()
        );
    }

    /**
     * Properties and constants from two traits must be identical (ignoring
     * source positions) — as in PHP, where an incompatible collision is fatal.
     */
    private function pickIdentical(array &$picked, array $member, string $traitName, Definition $target, string $kind): void
    {
        $name = $member['name'];

        if (!array_key_exists($name, $picked)) {
            $picked[$name] = ['member' => $member, 'trait' => $traitName];

            return;
        }

        $existing = $picked[$name];
        if ($this->normalize($existing['member']) === $this->normalize($member)) {
            return;
        }

        throw new CompilerException(
            sprintf(
                '%s collision: "%s::%s" and "%s::%s" differ in "%s"',
                $kind,
                $existing['trait'],
                $name,
                $traitName,
                $name,
                $target->getCompleteName()
            ),
            $target->getOriginalNode()
        );
    }

    /**
     * A concrete class must implement (or inherit an implementation of) every
     * abstract method a trait hands it — PHP raises the same error at link
     * time. When the class extends something the parent may provide the
     * implementation, which cannot be checked reliably here, so it passes.
     */
    private function checkAbstractSatisfiable(array $candidate, Definition $target): void
    {
        if (!$this->isAbstractMember($candidate['member'])) {
            return;
        }

        if (
            Definition::TYPE_CLASS === $target->getType()
            && !$target->isAbstract()
            && !$target->getExtendsClass()
        ) {
            throw new CompilerException(
                sprintf(
                    'Class "%s" uses trait "%s" but does not implement its abstract method "%s"',
                    $target->getCompleteName(),
                    $candidate['trait'],
                    $candidate['member']['name']
                ),
                $target->getOriginalNode()
            );
        }
    }

    private function isAbstractMember(array $member): bool
    {
        return in_array('abstract', $member['visibility'] ?? [], true);
    }

    /**
     * Strips source positions so AST identity means "same code", not "same
     * location".
     */
    private function normalize(array $node): array
    {
        unset($node['file'], $node['line'], $node['char']);

        foreach ($node as $key => $value) {
            if (is_array($value)) {
                $node[$key] = $this->normalize($value);
            }
        }

        return $node;
    }
}
