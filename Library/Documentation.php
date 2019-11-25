<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir;

use Psr\Log\LoggerAwareTrait;
use Psr\Log\NullLogger;
use Zephir\Documentation\File;
use Zephir\Documentation\NamespaceAccessor;
use Zephir\Documentation\Theme;
use Zephir\Exception\InvalidArgumentException;

/**
 * Documentation Generator.
 */
class Documentation
{
    use LoggerAwareTrait;

    protected $outputDirectory;

    /** @var Config */
    protected $config;

    /** @var CompilerFile[] */
    protected $classes;

    /** @var Theme */
    protected $theme;

    /** @var array */
    protected $options;

    protected $baseUrl;

    protected $themesDirectories;

    /** @var string|null */
    private $templatesPath;

    /**
     * Documentation constructor.
     *
     * @param CompilerFile[] $classes
     * @param Config         $config
     * @param string         $templatesPath
     * @param array          $options
     *
     * @throws ConfigException
     * @throws Exception
     */
    public function __construct(array $classes, Config $config, $templatesPath, array $options)
    {
        ksort($classes);

        $this->config = $config;
        $this->classes = $classes;
        $this->logger = new NullLogger();
        $this->templatesPath = $templatesPath;
        $this->options = $options;

        $this->initialize();
    }

    /**
     * TODO: options => to ApiOptions object
     * TODO: Validate options.
     *
     * @throws ConfigException
     * @throws Exception
     */
    protected function initialize()
    {
        $themeConfig = $this->config->get('theme', 'api');

        if (!$themeConfig) {
            throw new ConfigException('Theme configuration is not present');
        }

        $themeDir = $this->findThemeDirectory($themeConfig, $this->options['path']);

        if (!file_exists($themeDir)) {
            throw new ConfigException('There is no theme named '.$themeConfig['name']);
        }

        $outputDir = $this->findOutputDirectory($this->options['output']);

        if (!$outputDir) {
            throw new ConfigException('Api path (output directory) is not configured');
        }

        $this->outputDirectory = $outputDir;

        if (!file_exists($outputDir)) {
            if (!mkdir($outputDir, 0777, true)) {
                throw new Exception("Can't write output directory $outputDir");
            }
        }

        if (!is_writable($outputDir)) {
            throw new Exception("Can't write output directory $outputDir");
        }

        $themeConfig['options'] = $this->prepareThemeOptions($themeConfig, $this->options['options']);

        $this->theme = new Theme($themeDir, $outputDir, $themeConfig, $this->config, $this);

        $this->baseUrl = $this->options['url'];
    }

    /**
     * Search a theme by its name.
     *
     * Return the path to it if it exists. Otherwise NULL.
     *
     * @param string $name
     *
     * @return string|null
     */
    public function findThemePathByName($name)
    {
        // check the theme from the config
        $path = null;

        foreach ($this->themesDirectories as $themeDir) {
            $path = rtrim($themeDir, '\\/').\DIRECTORY_SEPARATOR.$name;
            if (0 !== strpos($path, 'phar://')) {
                $path = realpath($path);
            }

            if (is_dir($path)) {
                break;
            }
        }

        return $path;
    }

    /**
     * @throws Exception
     */
    public function build()
    {
        foreach ($this->classes as $class) {
            // class files (class/ns1/n2/class.html)
            $cfile = new File\ClassFile($this->config, $class);
            $this->theme->drawFile($cfile);

            // class source file (source/ns1/n2/class.html)
            $sfile = new File\SourceFile($this->config, $class);
            $this->theme->drawFile($sfile);
        }

        // classes file (classes.html)
        $file = new File\ClassesFile($this->config, $this->classes);
        $this->theme->drawFile($file);

        $namespaceAccessor = new NamespaceAccessor($this->classes);
        $namespaceAccessor->build();

        $byNamespace = $namespaceAccessor->getByNamespace();

        foreach ($byNamespace as $namespaceName => $nh) {
            // namespace files (namespace/ns1/n2/namespace.html)
            $nfile = new File\NamespaceFile($this->config, $nh);
            $this->theme->drawFile($nfile);
        }

        $sitemapFile = new File\Sitemap(
            $this->templatesPath,
            $this->baseUrl,
            $this->classes,
            $byNamespace
        );

        $this->theme->drawFile($sitemapFile);

        // namespaces files (namespaces.html)
        $nsfile = new File\NamespacesFile($this->config, $namespaceAccessor);
        $this->theme->drawFile($nsfile);

        // index (index.html)
        $indexfile = new File\IndexFile($this->config, $namespaceAccessor);
        $this->theme->drawFile($indexfile);

        $this->theme->buildStaticDirectory();

        $JsonClassDef = $this->theme->buildJsonClassDefinition($this->classes, $namespaceAccessor);
        $this->theme->createFile('asset/api_definition.js', "var ZephirApi = $JsonClassDef;");
    }

    public static function classUrl($c)
    {
        $cname = $c;
        if ($c instanceof ClassDefinition) {
            $cname = $c->getCompleteName();
        }

        return '/class/'.trim(str_replace('\\', '/', $cname), '/').'.html';
    }

    public static function namespaceUrl($ns)
    {
        return '/namespace/'.str_replace('\\', '/', $ns).'.html';
    }

    public static function sourceUrl(ClassDefinition $c)
    {
        return '/source/'.str_replace('\\', '/', $c->getCompleteName()).'.html';
    }

    /**
     * get the directory where the doc is going to be generated.
     *
     * @return string
     */
    public function getOutputDirectory()
    {
        return $this->outputDirectory;
    }

    /**
     * Prepare the options by merging the one in the project config with the one in the command line arg "theme-options".
     *
     * command line arg "theme-options" can be either a path to a json file containing the options or a raw json string
     *
     * @param array       $themeConfig
     * @param string|null $options
     *
     * @throws Exception
     *
     * @return array
     */
    private function prepareThemeOptions($themeConfig, $options = null)
    {
        $parsedOptions = null;
        if (!empty($options)) {
            if ('{' == $options[0]) {
                $parsedOptions = json_decode(trim($options), true);
                if (!$parsedOptions || !\is_array($parsedOptions)) {
                    throw new Exception("Unable to parse json from 'theme-options' argument");
                }
            } else {
                if (file_exists($options)) {
                    $unparsed = file_get_contents($options);
                    $parsedOptions = json_decode($unparsed, true);
                    if (!$parsedOptions || !\is_array($parsedOptions)) {
                        throw new Exception(
                            sprintf("Unable to parse json from the file '%s'", $options)
                        );
                    }
                } else {
                    throw new Exception(sprintf("Unable to find file '%s'", $options));
                }
            }
        }

        if (\is_array($parsedOptions)) {
            $options = array_merge($themeConfig['options'], $parsedOptions);
        } else {
            $options = $themeConfig['options'];
        }

        return $options;
    }

    /**
     * Find the directory where the doc is going to be generated depending on the command line options and the config.
     *
     * output directory is checked in this order :
     *  => check if the command line argument --output-directory was given
     *  => if not ; check if config config[api][path] was given
     *
     * @param string $outputDir
     *
     * @return string|null
     */
    private function findOutputDirectory($outputDir)
    {
        $outputDir = str_replace('%version%', $this->config->get('version'), $outputDir);

        if ('/' !== $outputDir[0]) {
            $outputDir = getcwd().'/'.$outputDir;
        }

        return $outputDir;
    }

    /**
     * Find the theme directory depending on the command  line options and the config.
     *
     * theme directory is checked in this order :
     *  => check if the command line argument --theme-path was given
     *  => if not ; find the different theme directories on the config $config['api']['theme-directories']
     *  search the theme from the name ($config['api']['theme']['name'] in the theme directories,
     * if nothing was found, we look in the zephir install dir default themes (templates/Api/themes)
     *
     * @param array       $themeConfig
     * @param string|null $path
     *
     * @throws InvalidArgumentException
     * @throws ConfigException
     *
     * @return string|null
     */
    private function findThemeDirectory($themeConfig, $path = null)
    {
        // check if there are additional theme paths in the config
        $themeDirectoriesConfig = $this->config->get('theme-directories', 'api');
        if ($themeDirectoriesConfig) {
            if (\is_array($themeDirectoriesConfig)) {
                $themesDirectories = $themeDirectoriesConfig;
            } else {
                throw new InvalidArgumentException("Invalid value for theme config 'theme-directories'");
            }
        } else {
            $themesDirectories = [];
        }

        $themesDirectories[] = $this->templatesPath.'/Api/themes';
        $this->themesDirectories = $themesDirectories;

        // check if the path was set from the command
        if (!empty($path)) {
            if (file_exists($path) && is_dir($path)) {
                return $path;
            } else {
                throw new InvalidArgumentException(
                    sprintf(
                        "Invalid value for option 'theme-path': the theme '%s' was not found or is not a valid theme.",
                        $path
                    )
                );
            }
        }

        if (!isset($themeConfig['name']) || !$themeConfig['name']) {
            throw new ConfigException(
                'There is no theme neither in the the theme config nor as a command line argument'
            );
        }

        return $this->findThemePathByName($themeConfig['name']);
    }
}
