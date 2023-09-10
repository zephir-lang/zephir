<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation;

use Zephir\Class\ClassDefinition;
use Zephir\CompilerFile;
use Zephir\Config;
use Zephir\Documentation;
use Zephir\Exception;

class Template
{
    protected string $pathToRoot = './';
    protected $themeOptions;
    /**
     * @var Config
     */
    protected $projectConfig;

    /**
     * Template constructor.
     *
     * @param Theme  $theme
     * @param array  $data
     * @param string $template
     * @param int    $nestedLevel
     *
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

    private function getTemplatePath($fileName)
    {
        if ('/' == $fileName[0] || 0 === strpos($fileName, 'phar://')) {
            return $fileName;
        }

        $inputFilename = $this->theme->getThemePathExtendsAware($fileName);

        if (!file_exists($inputFilename)) {
            throw new Exception("Template not found : $inputFilename");
        }

        return $inputFilename;
    }

    /**
     * set the config of the project (it usually wraps the version, the theme config, etc...).
     *
     * @param array $projectConfig
     */
    public function setProjectConfig($projectConfig): void
    {
        $this->projectConfig = $projectConfig;
    }

    /**
     * add theme options to make them available during the render phase.
     *
     * @param array $themeOptions
     */
    public function setThemeOptions($themeOptions): void
    {
        $this->themeOptions = $themeOptions;
    }

    /**
     * the path to root for the hyperlink in the templates.
     *
     * @param string $pathToRoot
     */
    public function setPathToRoot(string $pathToRoot): void
    {
        $this->pathToRoot = $pathToRoot;
    }

    /**
     * Generate an url relative to the current directory.
     *
     * @param string $url the url we want to reach
     *
     * @return string the relative path to the url
     */
    public function url(string $url): string
    {
        if (\is_string($url)) {
            if ('/' == $url[0]) {
                return $this->getPathToRoot().ltrim($url, '/');
            } elseif (\is_string($url)) {
                return $url;
            }
        } elseif ($url instanceof ClassDefinition) {
            return $this->url(Documentation::classUrl($url));
        } elseif ($url instanceof CompilerFile) {
            return $this->url(Documentation::classUrl($url->getClassDefinition()));
        }

        return '';
    }

    /**
     * @return string
     */
    public function getPathToRoot(): string
    {
        return $this->pathToRoot;
    }

    public function write($outputFile): void
    {
        $content = $this->parse();
        file_put_contents($outputFile, $content);
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
}
