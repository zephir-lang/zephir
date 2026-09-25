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

namespace Zephir\Documentation;

use Zephir\Class\Definition\Definition;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\Documentation;
use Zephir\Exception;
use Zephir\Os;

use function array_merge;
use function file_exists;
use function file_put_contents;
use function is_string;
use function ltrim;
use function ob_get_clean;
use function ob_start;
use function rtrim;

/**
 * Renders one theme template. The public helpers below (projectConfig(),
 * themeOption(), getAssets(), url(), asset()) are called only from the
 * theme's .phtml files, which is why a code search finds no caller for them.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2431
 */
class Template
{
    protected string $pathToRoot = './';
    /**
     * @var Config
     */
    protected $projectConfig;
    protected $themeOptions;

    /**
     * @throws Exception
     */
    public function __construct(
        protected Theme $theme,
        protected array $data,
        protected string $template,
        protected int $nestedLevel = 0,
    ) {
        // todo clean buffer before exception
        if ($nestedLevel > 800) {
            throw new Exception('Recursive inclusion detected in theme creation');
        }
    }

    /**
     * The path of a theme asset, relative to the current page.
     */
    public function asset(string $name): string
    {
        return $this->getPathToRoot() . 'asset/' . rtrim($name);
    }

    /**
     * The css and javascript files the theme, or a theme it extends, lists
     * in its theme.json.
     */
    public function getAssets(): array
    {
        return [
            'css'        => $this->theme->getThemeInfoExtendAware('css'),
            'javascript' => $this->theme->getThemeInfoExtendAware('javascript'),
        ];
    }

    public function getPathToRoot(): string
    {
        return $this->pathToRoot;
    }

    public function parse()
    {
        foreach ($this->data as $name => $value) {
            $$name = $value;
        }
        $path = $this->getTemplatePath($this->template);
        ob_start();
        include $path;
        $content = ob_get_clean();

        return $content;
    }

    public function partial(string $fileName, array $data = [])
    {
        $newLevel = $this->nestedLevel + 1;

        $template = new self($this->theme, array_merge($this->data, $data), $fileName, $newLevel);
        $template->setPathToRoot($this->getPathToRoot());
        $template->setThemeOptions($this->themeOptions);
        $template->setProjectConfig($this->projectConfig);

        return $template->parse();
    }

    /**
     * A setting of the project config, or null when no config was given.
     */
    public function projectConfig(string $name): mixed
    {
        return isset($this->projectConfig) ? $this->projectConfig->get($name) : null;
    }

    /**
     * the path to root for the hyperlink in the templates.
     */
    public function setPathToRoot(string $pathToRoot): void
    {
        $this->pathToRoot = $pathToRoot;
    }

    /**
     * set the config of the project (it usually wraps the version, the theme config, etc...).
     *
     * @param Config $projectConfig
     */
    public function setProjectConfig($projectConfig): void
    {
        $this->projectConfig = $projectConfig;
    }

    /**
     * add theme options to make them available during the render phase.
     */
    public function setThemeOptions(array $themeOptions): void
    {
        $this->themeOptions = $themeOptions;
    }

    /**
     * An option of the theme, or null when it is not set.
     */
    public function themeOption(string $name): mixed
    {
        return $this->themeOptions[$name] ?? null;
    }

    /**
     * An url relative to the current page. A leading "/" is resolved from the
     * output root, a class or its file resolves to the class page, and any
     * other string is returned as given.
     */
    public function url(string | Definition | CompilerFile $url): string
    {
        if ($url instanceof CompilerFile) {
            $url = $url->getClassDefinition();
        }

        if ($url instanceof Definition) {
            $url = Documentation::classUrl($url);
        }

        if (is_string($url) && '' !== $url && '/' === $url[0]) {
            return $this->getPathToRoot() . ltrim($url, '/');
        }

        return $url;
    }

    public function write($outputFile): void
    {
        $content = $this->parse();
        file_put_contents($outputFile, $content);
    }

    private function getTemplatePath($fileName)
    {
        if (Os::isAbsolutePath($fileName)) {
            return $fileName;
        }

        $inputFilename = $this->theme->getThemePathExtendsAware($fileName);

        if (!file_exists($inputFilename)) {
            throw new Exception("Template not found : $inputFilename");
        }

        return $inputFilename;
    }
}
