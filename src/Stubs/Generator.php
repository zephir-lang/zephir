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
use Zephir\Class\Constant;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\Class\Method\Parameters;
use Zephir\Class\Property;
use Zephir\CompilerFile;
use Zephir\Exception;

use function addslashes;
use function array_diff;
use function array_key_exists;
use function array_map;
use function file_put_contents;
use function implode;
use function in_array;
use function is_dir;
use function key;
use function mkdir;
use function preg_match;
use function realpath;
use function sprintf;
use function str_ireplace;
use function str_replace;
use function strtolower;
use function trim;
use function ucfirst;

use const DIRECTORY_SEPARATOR;
use const PHP_EOL;
use const PHP_VERSION_ID;

class Generator
{
    use TypeRenderer;

    /**
     * Not php visible style variants.
     */
    protected array $ignoreModifiers = [
        'inline',
        'internal',
        'scoped',
        'deprecated',
    ];

    /**
     * @param CompilerFile[] $files
     */
    public function __construct(protected array $files)
    {
    }

    /**
     * Generates stubs.
     *
     * @throws Exception\LogicException
     */
    public function generate(string $namespace, string $path, string $indent, string $banner): void
    {
        if (empty($path)) {
            throw new Exception\LogicException(
                'The stubs path must not be empty.'
            );
        }

        $indent = 'tabs' === $indent ? "\t" : '    ';

        foreach ($this->files as $file) {
            $class  = $file->getClassDefinition();
            $source = $this->buildClass($class, $indent, $banner);

            $filename = ucfirst($class->getName()) . '.php';
            $filePath = $path
                . str_ireplace(
                    $namespace,
                    '',
                    str_replace(
                        $namespace . '\\\\',
                        DIRECTORY_SEPARATOR,
                        $class->getNamespace()
                    )
                );
            $filePath = str_replace('\\', DIRECTORY_SEPARATOR, $filePath);
            $filePath = str_replace(DIRECTORY_SEPARATOR . DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR, $filePath);

            if (!is_dir($filePath)) {
                mkdir($filePath, 0777, true);
            }

            $filePath = realpath($filePath) . '/';
            file_put_contents($filePath . $filename, $source);
        }
    }

    /**
     * Build class.
     *
     * @throws Exception\RuntimeException
     */
    protected function buildClass(Definition $class, string $indent, string $banner): string
    {
        $source = '<?php' . PHP_EOL . PHP_EOL;
        $source .= '' === $banner ? '' : $banner . PHP_EOL;
        $source .= "namespace {$class->getNamespace()};" . PHP_EOL;

        /** @var AliasManager $aliasManager */
        $aliasManager = $class->getAliasManager();
        $aliases      = $aliasManager->getAliases();

        if (!empty($aliases)) {
            $source .= PHP_EOL;

            foreach ($aliases as $alias => $fqn) {
                $isAliased = $aliasManager->isUseStatementAliased($alias);
                $asAlias   = $isAliased ? ' as ' . $alias : '';
                $source    .= 'use ' . $fqn . $asAlias . ';' . PHP_EOL;
            }
        }

        $source .= PHP_EOL;

        $docBlock = (new DocBlock($class->getDocBlock(), ''));

        if ('' !== (string)$docBlock) {
            $source .= $docBlock . PHP_EOL;
        }

        if ($class->isFinal()) {
            $source .= 'final ';
        } elseif ($class->isAbstract()) {
            $source .= 'abstract ';
        }

        $source .= $class->getType() . ' ' . $class->getName();

        if ($class->getExtendsClass()) {
            $extendsClassDefinition = $class->getExtendsClassDefinition();
            if (!$extendsClassDefinition) {
                throw new Exception\RuntimeException(
                    sprintf(
                        'Class "%s" does not have a extendsClassDefinition',
                        $class->getName()
                    )
                );
            }

            $hasAliasForExtends = $aliasManager->isAlias($extendsClassDefinition->getShortName());

            $source .= ' extends ' . ($hasAliasForExtends || $extendsClassDefinition->isBundled() ? '' : '\\');
            $source .= $hasAliasForExtends ? $extendsClassDefinition->getShortName(
            ) : $extendsClassDefinition->getCompleteName();
        }

        if ($interfaces = $class->getImplementedInterfaces()) {
            foreach ($interfaces as $key => $interface) {
                $interfaces[$key] = '\\' . trim($interface, '\\');

                if ($aliasManager->isAliasPresentFor($interface)) {
                    $interfaces[$key] = $aliasManager->getAliasForClassName($interface);
                }
            }

            $keyword = Definition::TYPE_INTERFACE === $class->getType() ? ' extends ' : ' implements ';
            $source  .= $keyword . implode(', ', $interfaces);
        }

        $source .= PHP_EOL . '{' . PHP_EOL;

        /**
         * Trait `use` lines. Members merged in from those traits are skipped
         * below — the stub reads like the original source, with the trait
         * providing them.
         */
        if ($usedTraits = $class->getUsedTraits()) {
            foreach ($usedTraits as $traitName) {
                $source .= $indent . 'use \\' . trim($traitName, '\\') . ';' . PHP_EOL;
            }

            $source .= PHP_EOL;
        }

        $mergedMembers = $class->getMergedMemberNames();
        $mergedMethods = array_map('strtolower', $mergedMembers['methods']);

        /**
         * Build Class constants
         */
        $constants = [];
        foreach ($class->getConstants() as $constant) {
            if (in_array($constant->getName(), $mergedMembers['constants'], true)) {
                continue;
            }

            $constants[] = $this->buildConstant($constant, $indent) . PHP_EOL;
        }

        $source .= implode(PHP_EOL, $constants) . PHP_EOL;
        unset($constants);

        /**
         * Build Class properties
         */
        $properties = [];
        foreach ($class->getProperties() as $property) {
            if (in_array($property->getName(), $mergedMembers['properties'], true)) {
                continue;
            }

            $properties[] = $this->buildProperty($property, $indent) . PHP_EOL;
        }

        $source .= implode(PHP_EOL, $properties) . PHP_EOL;
        unset($properties);

        /**
         * Build Class methods
         */
        $methods = [];
        foreach ($class->getMethods() as $method) {
            if ($method->isInternal()) {
                continue;
            }

            if (in_array(strtolower($method->getName()), $mergedMethods, true)) {
                continue;
            }

            $methods[] = $this->buildMethod($method, Definition::TYPE_INTERFACE === $class->getType(), $indent) . PHP_EOL;
        }

        $source .= implode(PHP_EOL, $methods);
        unset($methods);

        return $source . '}' . PHP_EOL;
    }

    protected function buildConstant(Constant $constant, string $indent): string
    {
        $type   = PHP_VERSION_ID >= 80300 ? $this->extractVarTypeFromDocBlock($constant->getDocBlock()) : '';
        $source = 'const ' . ($type !== '' ? $type . ' ' : '') . $constant->getName();

        $value = $this->wrapPHPValue([
            'default' => $constant->getValue(),
        ]);

        return $this->fetchDocBlock($constant->getDocBlock(), $indent) . $indent . $source . ' = ' . $value . ';';
    }

    protected function buildMethod(Method $method, bool $isInterface, string $indent): string
    {
        $modifier = implode(' ', array_diff($method->getVisibility(), $this->ignoreModifiers));

        $methodParameters = $method->getParameters();
        $aliasManager     = $method->getClassDefinition()->getAliasManager();
        $docBlock         = new MethodDocBlock($method, $aliasManager, $indent);

        $parameters = [];

        if ($methodParameters instanceof Parameters) {
            foreach ($methodParameters->getParameters() as $parameter) {
                /**
                 * A `= null` default on a typed parameter means we MUST
                 * emit a nullable type hint PHP 8.4 deprecates the
                 * implicit-nullable form `Foo $x = null`. Prepend `?` to
                 * any concrete type hint when the default value is null.
                 * Variable/mixed parameters emit no type hint at all so
                 * the `?` doesn't apply there.
                 *
                 * @see https://github.com/zephir-lang/zephir/issues/2426.
                 */
                $nullable = isset($parameter['default']['type'])
                    && 'null' === $parameter['default']['type'];

                $paramStr = '';
                if (!empty($parameter['data-types'])) {
                    /**
                     * Union parameter types (#2613) arrive as an ordered
                     * `data-types` list (with `data-type` set to `variable`),
                     * so they must be handled before the single-type branches
                     * or the hint is silently dropped.
                     */
                    $union = $this->buildUnionType($parameter['data-types'], $aliasManager);
                    if ('' !== $union) {
                        $paramStr .= $union . ' ';
                    }
                } elseif (isset($parameter['cast'])) {
                    if ($aliasManager->isAlias($parameter['cast']['value'])) {
                        $cast = '\\' . $aliasManager->getAlias($parameter['cast']['value']);
                    } else {
                        $cast = $parameter['cast']['value'];
                    }

                    $paramStr .= ($nullable ? '?' : '') . $cast . ' ';
                } elseif (isset($parameter['data-type']) && 'array' === $parameter['data-type']) {
                    $paramStr .= ($nullable ? '?' : '') . 'array ';
                } elseif (isset($parameter['data-type'])) {
                    if (in_array($parameter['data-type'], ['bool', 'boolean'])) {
                        $paramStr .= ($nullable ? '?' : '') . 'bool ';
                    } elseif ('double' == $parameter['data-type']) {
                        $paramStr .= ($nullable ? '?' : '') . 'float ';
                    } elseif (in_array($parameter['data-type'], ['int', 'uint', 'long', 'ulong', 'uchar'])) {
                        $paramStr .= ($nullable ? '?' : '') . 'int ';
                    } elseif (in_array($parameter['data-type'], ['char', 'string'])) {
                        $paramStr .= ($nullable ? '?' : '') . 'string ';
                    }
                }

                $paramStr .= (!empty($parameter['variadic']) ? '...' : '') . '$' . $parameter['name'];

                if (isset($parameter['default'])) {
                    $paramStr .= ' = ' . $this->wrapPHPValue($parameter);
                }

                $parameters[] = $paramStr;
            }
        }

        $return = '';
        $returnTypes = [];
        $methodReturnTypes = $method->getReturnTypes();
        if ($method->hasReturnTypes()) {
            if (array_key_exists('variable', $methodReturnTypes) || array_key_exists('mixed', $methodReturnTypes)) {
                $returnTypes[] = 'mixed';
            } else {
                if (array_key_exists('object', $methodReturnTypes)) {
                    /**
                     * A return type may union several classes
                     * (`-> <Model> | <Row> | null`). Emit every class, not just
                     * the first one — using `key()` dropped all but the first
                     * and produced an invalid hint such as `Model|null`.
                     *
                     * @see https://github.com/zephir-lang/zephir/issues/2428
                     */
                    $classTypes = array_keys($method->getReturnClassTypes());
                    array_push($returnTypes, ...($classTypes ?: ['object']));
                }

                if ($method->areReturnTypesIntCompatible()) {
                    $returnTypes[] = 'int';
                }

                if ($method->areReturnTypesDoubleCompatible()) {
                    $returnTypes[] = 'float';
                }

                if ($method->areReturnTypesFalseCompatible()) {
                    $returnTypes[] = 'false';
                } elseif ($method->areReturnTypesBoolCompatible()) {
                    $returnTypes[] = 'bool';
                }

                if ($method->areReturnTypesStringCompatible()) {
                    $returnTypes[] = 'string';
                }

                if (array_key_exists('array', $methodReturnTypes)) {
                    $returnTypes[] = 'array';
                }

                if ($method->areReturnTypesNullCompatible()) {
                    $returnTypes[] = 'null';
                }
            }

            $returnTypes = array_filter($returnTypes);
            $return = join('|', $returnTypes);
        } elseif ($method->isVoid()) {
            $return = 'void';
        }

        if (!empty($return)) {
            $return = ': ' . $return;
        }

        $function   = trim($modifier . ' function', ' ') . ' ';
        $methodBody = $indent . $function . $method->getName() . '(' . implode(', ', $parameters) . ')' . $return;

        if ($isInterface || $method->isAbstract()) {
            $methodBody .= ';';
        } else {
            $methodBody .= PHP_EOL . $indent . '{' . PHP_EOL . $indent . '}';
        }

        $docs = $docBlock->processMethodDocBlock();
        $docs = $docs ? $docs . PHP_EOL : '';

        return $docs . $methodBody;
    }

    /**
     * Build property.
     */
    protected function buildProperty(Property $property, string $indent): string
    {
        $visibility = 'public';

        if (false === $property->isPublic()) {
            $visibility = $property->isProtected() ? 'protected' : 'private';
        }

        if ($property->isStatic()) {
            $visibility = 'static ' . $visibility;
        }

        /**
         * Readonly properties (#2614) carry the modifier in the property's
         * visibility list; PHP spells it after the visibility keyword, e.g.
         * `public readonly int $id`. Without this the stub drops `readonly`
         * and mis-describes a write-once property as writable.
         */
        if ($property->isReadOnly()) {
            $visibility .= ' readonly';
        }

        $original = $property->getOriginal() ?? [];

        /**
         * Emit the PHP type hint for typed properties (#2608), mirroring the
         * parameter renderer's Zephir->PHP coercions and alias resolution.
         * Unlike a parameter, a property states nullability explicitly with a
         * leading `?`, so it is read from the AST `nullable` flag rather than
         * inferred from a `= null` default. Union properties (#2613) carry an
         * ordered `data-types` list instead of a single `data-type`/`cast`;
         * `null` is a union member there, so the `?` shorthand does not apply.
         */
        $nullable   = !empty($original['nullable']);
        $typePrefix = '';
        if (!empty($original['data-types'])) {
            $union      = $this->buildUnionType(
                $original['data-types'],
                $property->getClassDefinition()->getAliasManager()
            );
            $typePrefix = '' === $union ? '' : $union . ' ';
        } elseif (isset($original['cast'])) {
            $aliasManager = $property->getClassDefinition()->getAliasManager();
            $cast         = $aliasManager->isAlias($original['cast']['value'])
                ? '\\' . $aliasManager->getAlias($original['cast']['value'])
                : $original['cast']['value'];
            $typePrefix   = ($nullable ? '?' : '') . $cast . ' ';
        } elseif (isset($original['data-type']) && 'array' === $original['data-type']) {
            $typePrefix = ($nullable ? '?' : '') . 'array ';
        } elseif (isset($original['data-type'])) {
            if (in_array($original['data-type'], ['bool', 'boolean'])) {
                $typePrefix = ($nullable ? '?' : '') . 'bool ';
            } elseif ('double' === $original['data-type']) {
                $typePrefix = ($nullable ? '?' : '') . 'float ';
            } elseif (in_array($original['data-type'], ['int', 'uint', 'long', 'ulong', 'uchar'])) {
                $typePrefix = ($nullable ? '?' : '') . 'int ';
            } elseif (in_array($original['data-type'], ['char', 'string'])) {
                $typePrefix = ($nullable ? '?' : '') . 'string ';
            }
        }

        $source = $indent . $visibility . ' ' . $typePrefix . '$' . $property->getName();

        /**
         * Whether there *is* a default is only visible on the original AST (a
         * typed property with no default is uninitialized, not null), but the
         * value to render is the folded one — an expression default such as
         * `public size = 1024 * 8;` has no PHP spelling as an AST node [#2061].
         */
        if (isset($original['default'])) {
            $source .= ' = ' . $this->wrapPHPValue([
                    'default' => $property->getDefaultValue(),
                ]);
        }

        return $this->fetchDocBlock($property->getDocBlock(), $indent) . $source . ';';
    }

    /**
     * Prepare AST default value to PHP code print.
     *
     * @throws Exception\NotImplementedException
     */
    protected function wrapPHPValue(array $parameter): string
    {
        switch ($parameter['default']['type']) {
            case 'null':
                $returnValue = 'null';
                break;

            case 'string':
            case 'char':
                $returnValue = '\'' . addslashes($parameter['default']['value']) . '\'';
                break;

            case 'empty-array':
                $returnValue = '[]';
                break;

            case 'array':
                $parameters = [];

                foreach ($parameter['default']['left'] as $value) {
                    $source = '';

                    if (isset($value['key'])) {
                        $source .= $this->wrapPHPValue([
                                'default' => $value['key'],
                                'type'    => $value['key']['type'],
                            ]) . ' => ';
                    }

                    $parameters[] = $source . $this->wrapPHPValue([
                            'default' => $value['value'],
                            'type'    => $value['value']['type'],
                        ]);
                }

                $returnValue = '[' . implode(', ', $parameters) . ']';
                break;

            case 'static-constant-access':
                $returnValue = $parameter['default']['left']['value'] . '::' . $parameter['default']['right']['value'];
                break;

            case 'int':
            case 'double':
            case 'bool':
                $returnValue = $parameter['default']['value'];
                break;

            default:
                throw new Exception\LogicException(
                    sprintf(
                        'Stubs - value with type: %s is not supported',
                        $parameter['default']['type']
                    )
                );
        }

        return (string)$returnValue;
    }

    private function extractVarTypeFromDocBlock(?string $docBlock): string
    {
        if ($docBlock === null) {
            return '';
        }

        if (preg_match('/@var\s+([\w\\\\|]+)/', $docBlock, $matches)) {
            return $matches[1];
        }

        return '';
    }

    private function fetchDocBlock(?string $docBlock, string $indent): string
    {
        $docBlock = (new DocBlock($docBlock, $indent))->__toString();

        return $docBlock ? $docBlock . PHP_EOL : '';
    }
}
