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

use Zephir\Documentation;
use Zephir\Exception;

use function array_unshift;
use function closedir;
use function copy;
use function count;
use function explode;
use function file_exists;
use function file_get_contents;
use function file_put_contents;
use function is_array;
use function is_dir;
use function json_decode;
use function json_encode;
use function ltrim;
use function mkdir;
use function opendir;
use function pathinfo;
use function readdir;
use function str_repeat;
use function str_replace;
use function strpos;
use function touch;

class Theme
{
    /**
     * @var Documentation
     */
    protected $documentation;
    /**
     * @var Theme
     */
    protected $extendedTheme;
    protected $options;
    protected $outputDir;
    protected $projectConfig;
    protected $themeConfig;
    protected $themeDir;
    protected $themeInfos = [];

    public function __construct($themeDir, $outputDir, $themeConfig, $config, Documentation $documentation)
    {
        $this->outputDir     = $outputDir;
        $this->themeConfig   = $themeConfig;
        $this->themeDir      = $themeDir;
        $this->options       = $themeConfig['options'];
        $this->projectConfig = $config;
        $this->documentation = $documentation;

        $themeInfosPath = $this->getThemePath('theme.json');
        if ($themeInfosPath) {
            $themeInfos = json_decode(file_get_contents($themeInfosPath), true);
            if (!$themeInfos) {
                throw new Exception("Cant parse file $themeInfosPath");
            } else {
                $this->themeInfos = $themeInfos;
                if (isset($themeInfos['extends'])) {
                    $extThemePath = $documentation->findThemePathByName($themeInfos['extends']);
                    if (!$extThemePath) {
                        throw new Exception('Unable to find extended theme ' . $themeInfos['extends']);
                    }

                    $this->extendedTheme = new self($extThemePath, $outputDir, $themeConfig, $config, $documentation);
                }
            }
        }
    }

    public function buildJsonClassDefinition($classList, NamespaceAccessor $nsA)
    {
        $output = [
            'allClasses'    => [],
            'allNamespaces' => [],
            'classes'       => [],
            'namespaces'    => [],
        ];

        foreach ($classList as $class) {
            $cDef  = $class->getClassDefinition();
            $cName = $cDef->getCompleteName();

            $output['allClasses'][$cName] = [
                'type'      => $cDef->getType(),
                'name'      => $cName,
                'shortname' => $cDef->getName(),
            ];

            if (!strpos($cName, '\\') > 0) {
                $output['classes'][] = $cName;
            }
        }

        $namespaces = $nsA->getByNamespace();

        foreach ($namespaces as $ns) {
            $subclasses    = [];
            $subnamespaces = [];

            foreach ($ns->getClasses() as $scs) {
                $subclasses[] = $scs->getClassDefinition()->getCompleteName();
            }

            foreach ($ns->getNamespaces() as $sns) {
                $subnamespaces[] = $sns->getFullNamespace();
            }

            $output['allNamespaces'][$ns->getFullNamespace()] = [
                'name'       => $ns->getFullNamespace(),
                'shortName'  => $ns->getShortName(),
                'parentName' => $ns->getParentName(),
                'classes'    => $subclasses,
                'namespaces' => $subnamespaces,
            ];

            if (!strpos($ns->getFullNamespace(), '\\') > 0) {
                $output['namespaces'][] = $ns->getFullNamespace();
            }
        }

        return json_encode($output);
    }

    /**
     * copy the static directory of the theme into the output directory.
     */
    public function buildStaticDirectory(): void
    {
        $outputStt = $this->getOutputPath('asset');

        if (!file_exists($outputStt)) {
            mkdir($outputStt, 0777, true);
        }

        if ($this->extendedTheme) {
            $this->extendedTheme->buildStaticDirectory();
        }

        $themeStt = $this->getThemePath('static');

        if ($themeStt) {
            $files = [];

            $this->__copyDir($themeStt, $outputStt . '/static', $files);

            foreach ($files as $f) {
                foreach ($this->options as $optName => $opt) {
                    $fcontent = file_get_contents($f);
                    $fcontent = str_replace('%_' . $optName . '_%', $opt, $fcontent);

                    file_put_contents($f, $fcontent);
                }
            }
        }
    }

    public function createFile($path, $content): void
    {
        $outputFile = $this->getOutputPath($path);
        touch($outputFile);
        file_put_contents($outputFile, $content);
    }

    /**
     * Parse and draw the specified file.
     *
     * @param FileInterface $file
     *
     * @throws Exception
     */
    public function drawFile(FileInterface $file): void
    {
        $outputFile = ltrim($file->getOutputFile(), '/');

        $output         = pathinfo($this->outputDir . '/' . $outputFile);
        $outputDirname  = $output['dirname'];
        $outputBasename = $output['basename'];
        $outputFilename = $outputDirname . '/' . $outputBasename;

        // todo : check if writable
        if (!file_exists($outputDirname)) {
            mkdir($outputDirname, 0777, true);
        }

        $subDirNumber = count(explode('/', $outputFile)) - 1;

        if ($subDirNumber > 0) {
            $pathToRoot = str_repeat('../', $subDirNumber);
        } else {
            $pathToRoot = './';
        }

        $template = new Template($this, $file->getData(), $file->getTemplateName());
        $template->setPathToRoot($pathToRoot);
        $template->setThemeOptions($this->options);
        $template->setProjectConfig($this->projectConfig);

        touch($outputFilename);
        $template->write($outputFilename);
    }

    public function getOutputPath($path)
    {
        $path         = pathinfo($this->outputDir . '/' . $path);
        $pathDirname  = $path['dirname'];
        $pathBasename = $path['basename'];
        $pathFilename = $pathDirname . '/' . $pathBasename;

        return $pathFilename;
    }

    /**
     * Get assets from the theme info (theme.json file placed inside the theme directory).
     *
     * @param string $name
     *
     * @return mixed|null
     */
    public function getThemeInfo($name)
    {
        if (isset($this->themeInfos[$name])) {
            return $this->themeInfos[$name];
        }

        return null;
    }

    /**
     * Similar with getThemeInfo but includes the value for all extended themes, and returns the results as an array.
     *
     * @param string $name
     *
     * @return array
     */
    public function getThemeInfoExtendAware($name)
    {
        if ($this->extendedTheme) {
            $data = $this->extendedTheme->getThemeInfoExtendAware($name);
        } else {
            $data = [];
        }
        $info = $this->getThemeInfo($name);
        array_unshift($data, $info);

        return $data;
    }

    /**
     * find the path to a file in the theme.
     *
     * @param $path
     *
     * @return string
     */
    public function getThemePath($path)
    {
        $path         = pathinfo($this->themeDir . '/' . $path);
        $pathDirname  = $path['dirname'];
        $pathBasename = $path['basename'];
        $pathFilename = $pathDirname . '/' . $pathBasename;

        if (!file_exists($pathFilename)) {
            return null;
        }

        return $pathFilename;
    }

    /**
     * find the path to a file in the theme or from the extended theme.
     *
     * @param $path
     *
     * @return string
     */
    public function getThemePathExtendsAware($path)
    {
        $newPath = $this->getThemePath($path);
        if (!$newPath) {
            if ($this->extendedTheme) {
                return $this->extendedTheme->getThemePathExtendsAware($path);
            }
        }

        return $newPath;
    }

    /**
     * from : https://stackoverflow.com/questions/2050859/copy-entire-contents-of-a-directory-to-another-using-php.
     *
     * @param $src
     * @param $dst
     * @param $files
     */
    private function __copyDir($src, $dst, &$files = null): void
    {
        $dir = opendir($src);
        @mkdir($dst);
        while (false !== ($file = readdir($dir))) {
            if (('.' != $file) && ('..' != $file)) {
                if (is_dir($src . '/' . $file)) {
                    $this->__copyDir($src . '/' . $file, $dst . '/' . $file, $files);
                } else {
                    copy($src . '/' . $file, $dst . '/' . $file);
                    if (is_array($files)) {
                        $files[] = $dst . '/' . $file;
                    }
                }
            }
        }
        closedir($dir);
    }
}
