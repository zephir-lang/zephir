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
use Zephir\Class\Method\Method;
use Zephir\Types\Types;

/**
 * Stubs Generator.
 *
 * TODO: Merge class with documentation generator
 */
class MethodDocBlock extends DocBlock
{
    private array $parameters = [];

    /**
     * Parameters which are described by User into docblock
     *
     * @var array
     */
    private array $predefinedParams = [];

    /**
     * @var array
     */
    private array $return = [];

    /**
     * @var string|mixed
     */
    private string $shortcutName;

    /**
     * @var bool
     */
    private bool $deprecated;

    /**
     * @var AliasManager
     */
    private AliasManager $aliasManager;

    /**
     * @var Method
     */
    private Method $classMethod;

    /**
     * @var Types
     */
    private Types $types;

    public function __construct(
        Method       $method,
        AliasManager $aliasManager,
        string       $indent = '    ',
        ?Types       $types = null
    ) {
        parent::__construct($method->getDocBlock(), $indent);

        $this->deprecated = $method->isDeprecated();
        $this->aliasManager = $aliasManager;
        $this->shortcutName = $method->isShortcut() ? $method->getShortcutName() : '';
        $this->classMethod = $method;
        $this->types = $types ?? new Types();
    }

    /**
     * Process DocBlock and Method arguments.
     *
     * @return string
     */
    public function processMethodDocBlock(): string
    {
        $this->parseMethodParameters($this->classMethod);
        $this->parseLines();
        $this->parseMethodReturnType($this->classMethod);
        $this->appendParametersLines();

        if (!empty($this->return)) {
            $this->appendReturnLine();
        }

        return $this->__toString();
    }

    protected function parseMethodReturnType(Method $method): void
    {
        $return = [];
        foreach ($method->getReturnTypes() as $type) {
            if (isset($type['data-type'])) {
                $dataType = 'variable' === $type['data-type'] ? 'mixed' : $type['data-type'];
                $return[$dataType] = $dataType;
            }
        }

        $returnClassTypes = $method->getReturnClassTypes();
        if ($returnClassTypes) {
            foreach ($returnClassTypes as $key => $returnClassType) {
                if ($this->aliasManager->isAlias($returnClassType)) {
                    $returnClassTypes[$key] = '\\'.$this->aliasManager->getAlias($returnClassType);
                }
            }

            $return = array_merge($return, $returnClassTypes);
        }

        /**
         * Prepare return types for Collections compatible types.
         * Only for complex types.
         */
        $collections = [];
        if ($method->hasReturnTypesRaw()) {
            $returnClassTypes = $method->getReturnTypesRaw();

            if (!empty($returnClassTypes['list'])) {
                foreach ($returnClassTypes['list'] as $returnType) {
                    if (empty($returnType['cast']) || !$returnType['collection']) {
                        continue;
                    }

                    $key = $returnType['cast']['value'];
                    $type = $key;

                    if ($this->aliasManager->isAlias($type)) {
                        $type = '\\'.$this->aliasManager->getAlias($type);
                    }

                    $return[$key] = $type.'[]';
                    $collections[$type.'[]'] = $returnType;
                }
            }
        }

        $processedTypes = !empty($method->getReturnClassTypes()) ? $return : null;
        $returnType = $this->types->getReturnTypeAnnotation(
            $this->classMethod,
            $processedTypes ?: array_merge($method->getReturnTypes(), $collections)
        );

        if (!empty($returnType)) {
            // Empty line in array - it's an empty description. Don't remove it!
            $this->return = [$returnType, ''];
        }
    }

    /**
     * Parse DocBlock and returns extracted groups.
     */
    protected function parseDocBlockParam(string $line): array
    {
        $pattern = '~
            @(?P<doctype>param|return|var)\s+
            (?P<type>[\\\\\w]+(:?\s*\|\s*[\\\\\w]+|\s*\[]+)*)\s*
            (?P<dollar>\$)?
            (?P<name>[a-z_][a-z0-9_]*)?\s*
            (?P<description>(.|\s)*)?
            ~xi';

        preg_match($pattern, $line, $matched);

        return $matched;
    }

    protected function parseLines(): void
    {
        $lines = [];

        foreach ($this->lines as $line) {
            $parsedLine = $this->parseDocBlockParam($line);
            $docType = $parsedLine['doctype'] ?? null;

            $dollar = $parsedLine['dollar'] ?? '';
            $identifier = $parsedLine['name'] ?? false;
            $description = $parsedLine['description'] ?? '';
            $type = $parsedLine['type'] ?? '';

            // Remember docblock @param to avoid param duplication when parse input args.
            if ($identifier) {
                $this->predefinedParams[$identifier] = true;
            }

            /**
             * Remember docblock `@return` to avoid duplication.
             * Also replace `@var` to `@mixed` for PHP docblock.
             */
            if ('return' === $docType) {
                $this->predefinedParams['return'] = true;

                $mixed = str_replace('var', 'mixed', $type);
                $line = str_replace($type, $mixed, $line);
            }

            if ('$' !== $dollar && $identifier && 'return' !== $docType) {
                $line = '@'.$docType.' '.trim($type).' $'.trim($identifier.' '.$description);
            }

            if ('var' === $docType) {
                $line = str_replace('@var', '@param', $line);
            }

            if ('var' === $docType && 'set' === $this->shortcutName) {
                $docType = 'param';
                $name = array_keys($this->parameters);
                $name = $name[0];
            } elseif ('var' == $docType && 'get' == $this->shortcutName) {
                $docType = 'return';
            } else {
                $name = $identifier ? '$'.trim($identifier, '$') : '';
            }

            switch ($docType) {
                case 'param':
                    $this->parameters[$name] = [$type, $description];
                    break;
                case 'return':
                    $this->return = [$type, $description];
                    break;
            }

            $lines[] = $line;
        }

        $this->lines = $lines;
    }

    private function appendReturnLine(): void
    {
        if (!isset($this->predefinedParams['return'])) {
            list($type, $description) = $this->return;

            $return = $this->aliasManager->getAliasForClassName($type).' '.$description;
            $this->lines[] = '@return '.trim($return, ' ');
        }
    }

    private function parseMethodParameters(Method $method): void
    {
        $parameters = $method->getParameters();
        $aliasManager = $method->getClassDefinition()->getAliasManager();

        if ($parameters === null) {
            return;
        }

        foreach ($parameters as $parameter) {
            if (isset($parameter['cast'])) {
                if ($aliasManager->isAlias($parameter['cast']['value'])) {
                    $type = '\\'.$aliasManager->getAlias($parameter['cast']['value']);
                } else {
                    $type = $parameter['cast']['value'];
                }
            } elseif (isset($parameter['data-type'])) {
                $type = 'variable' === $parameter['data-type'] ? 'mixed' : $parameter['data-type'];
            } else {
                $type = 'mixed';
            }

            $this->parameters['$'.trim($parameter['name'], '$')] = [$type, ''];
        }
    }

    private function appendParametersLines(): void
    {
        foreach ($this->parameters as $name => $parameter) {
            if (empty($name)) {
                continue;
            }

            if (!isset($this->predefinedParams[trim($name, '$')])) {
                list($type, $description) = $parameter;

                $param = $type.' '.$name.' '.$description;
                $this->lines[] = '@param '.trim($param, ' ');
            }
        }

        if ($this->deprecated) {
            $this->lines[] = '@deprecated';
        }

        $this->lines = array_unique($this->lines);
    }
}
