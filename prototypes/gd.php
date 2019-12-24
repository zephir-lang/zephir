<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

if (!function_exists('gd_info')) {
    /**
     * Retrieve information about the currently installed GD library.
     *
     * @see http://www.php.net/manual/en/function.gd-info.php
     *
     * @return array
     */
    function gd_info()
    {
    }
}

if (!function_exists('getimagesize')) {
    /**
     * Get the size of an image.
     *
     * @see https://www.php.net/manual/en/function.getimagesize.php
     *
     * @param string $filename
     * @param array  $imageinfo
     *
     * @return array
     */
    function getimagesize($filename, array &$imageinfo)
    {
    }
}

if (!function_exists('getimagesizefromstring')) {
    /**
     * Get the size of an image from a string.
     *
     * @see https://www.php.net/manual/en/function.getimagesizefromstring.php
     *
     * @param string $imagedata
     * @param array  $imageinfo
     *
     * @return array
     */
    function getimagesizefromstring($imagedata, array &$imageinfo)
    {
    }
}

if (!function_exists('image_type_to_extension')) {
    /**
     * Get file extension for image type.
     *
     * @see https://www.php.net/manual/en/function.image-type-to-extension.php
     *
     * @param $imagetype
     * @param bool $include_dot
     *
     * @return string
     */
    function image_type_to_extension($imagetype, $include_dot = true)
    {
    }
}

if (!function_exists('image_type_to_mime_type')) {
    /**
     * Get Mime-Type for image-type returned by getimagesize, exif_read_data, exif_thumbnail, exif_imagetype.
     *
     * @see https://www.php.net/manual/en/function.image-type-to-mime-type.php
     *
     * @param int $imagetype
     *
     * @return string
     */
    function image_type_to_mime_type($imagetype)
    {
    }
}

if (!function_exists('image2wbmp') && PHP_VERSION_ID < 70400) {
    /**
     * Output image to browser or file.
     *
     * @see https://www.php.net/manual/en/function.image2wbmp.php
     *
     * @param resource $image
     * @param string   $filename
     * @param int      $threshold
     *
     * @return bool
     */
    function image2wbmp($image, $filename, $threshold)
    {
    }
}

if (!function_exists('imageaffine')) {
    /**
     * Return an image containing the affine transformed src image, using an optional clipping area.
     *
     * @see https://www.php.net/manual/en/function.imageaffine.php
     *
     * @param resource $image
     * @param array    $affline
     * @param array    $clip
     *
     * @return resource|bool
     */
    function imageaffine($image, array $affline, array $clip = [])
    {
    }
}

if (!function_exists('imageaffinematrixconcat')) {
    /**
     * Concatenate two affine transformation matrices.
     *
     * @see https://www.php.net/manual/en/function.imageaffinematrixconcat.php
     *
     * @param array $m1
     * @param array $m2
     *
     * @return array|bool
     */
    function imageaffinematrixconcat(array $m1, array $m2)
    {
    }
}

if (!function_exists('imageaffinematrixget')) {
    /**
     * Get an affine transformation matrix.
     *
     * @see https://www.php.net/manual/en/function.imageaffinematrixget.php
     *
     * @param int   $type
     * @param mixed $options
     *
     * @return array|bool
     */
    function imageaffinematrixget($type, $options)
    {
    }
}

if (!function_exists('imagealphablending')) {
    /**
     * Set the blending mode for an image.
     *
     * @see https://www.php.net/manual/en/function.imagealphablending.php
     *
     * @param resource $image
     * @param bool     $blendmode
     *
     * @return bool
     */
    function imagealphablending($image, $blendmode)
    {
    }
}

if (!function_exists('imageantialias')) {
    /**
     * Should antialias functions be used or not.
     *
     * @see https://www.php.net/manual/en/function.imageantialias.php
     *
     * @param resource $image
     * @param bool     $enabled
     *
     * @return bool
     */
    function imageantialias($image, $enabled)
    {
    }
}

if (!function_exists('imagearc')) {
    /**
     * Draws an arc.
     *
     * @see http://www.php.net/manual/ru/function.imagearc.php
     *
     * @param resource $image
     * @param int      $cx
     * @param int      $cy
     * @param int      $width
     * @param int      $height
     * @param int      $start
     * @param int      $end
     * @param int      $color
     *
     * @return bool
     */
    function imagearc($image, $cx, $cy, $width, $height, $start, $end, $color)
    {
    }
}

if (!function_exists('imagechar')) {
    /**
     * Draw a character horizontally.
     *
     * @see https://www.php.net/manual/en/function.imagechar.php
     *
     * @param resource $image
     * @param int      $font
     * @param int      $x
     * @param int      $y
     * @param string   $c
     * @param int      $color
     *
     * @return bool
     */
    function imagechar($image, $font, $x, $y, $c, $color)
    {
    }
}

if (!function_exists('imagecharup')) {
    /**
     * Draw a character vertically.
     *
     * @see https://www.php.net/manual/en/function.imagecharup.php
     *
     * @param resource $image
     * @param int      $font
     * @param int      $x
     * @param int      $y
     * @param string   $c
     * @param int      $color
     *
     * @return bool
     */
    function imagecharup($image, $font, $x, $y, $c, $color)
    {
    }
}

if (!function_exists('imagecolorallocate')) {
    /**
     * Allocate a color for an image.
     *
     * @see https://www.php.net/manual/en/function.imagecolorallocate.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     *
     * @return int|bool
     */
    function imagecolorallocate($image, $red, $green, $blue)
    {
    }
}

if (!function_exists('imagecolorallocatealpha')) {
    /**
     * Allocate a color for an image.
     *
     * @see https://www.php.net/manual/en/function.imagecolorallocatealpha.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     * @param int      $alpha
     *
     * @return int|bool
     */
    function imagecolorallocatealpha($image, $red, $green, $blue, $alpha)
    {
    }
}

if (!function_exists('imagecolorat')) {
    /**
     * Get the index of the color of a pixel.
     *
     * @see https://www.php.net/manual/en/function.imagecolorat.php
     *
     * @param resource $image
     * @param int      $x
     * @param int      $y
     *
     * @return int|false
     */
    function imagecolorat($image, $x, $y)
    {
    }
}

if (!function_exists('imagecolorclosest')) {
    /**
     * Get the index of the closest color to the specified color.
     *
     * @see https://www.php.net/manual/en/function.imagecolorclosest.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     *
     * @return int
     */
    function imagecolorclosest($image, $red, $green, $blue)
    {
    }
}

if (!function_exists('imagecolorclosestalpha')) {
    /**
     * Get the index of the closest color to the specified color + alpha.
     *
     * @see https://www.php.net/manual/en/function.imagecolorclosestalpha.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     * @param int      $alpha
     *
     * @return int
     */
    function imagecolorclosestalpha($image, $red, $green, $blue, $alpha)
    {
    }
}

if (!function_exists('imagecolorclosesthwb')) {
    /**
     * Get the index of the color which has the hue, white and blackness.
     *
     * @see https://www.php.net/manual/en/function.imagecolorclosesthwb.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     *
     * @return int
     */
    function imagecolorclosesthwb($image, $red, $green, $blue)
    {
    }
}

if (!function_exists('imagecolordeallocate')) {
    /**
     * De-allocate a color for an image.
     *
     * @see https://www.php.net/manual/en/function.imagecolordeallocate.php
     *
     * @param resource $image
     * @param int      $color
     *
     * @return bool
     */
    function imagecolordeallocate($image, $color)
    {
    }
}

if (!function_exists('imagecolorexact')) {
    /**
     * Get the index of the specified color.
     *
     * @see http://www.php.net/manual/ru/function.imagecolorexact.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     *
     * @return int
     */
    function imagecolorexact($image, $red, $green, $blue)
    {
    }
}

if (!function_exists('imagecolorexactalpha')) {
    /**
     * Get the index of the specified color + alpha.
     *
     * @see https://www.php.net/manual/en/function.imagecolorexactalpha.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     * @param int      $alpha
     *
     * @return int
     */
    function imagecolorexactalpha($image, $red, $green, $blue, $alpha)
    {
    }
}

if (!function_exists('imagecolormatch')) {
    /**
     * Makes the colors of the palette version of an image more closely match the true color version.
     *
     * @see http://www.php.net/manual/ru/function.imagecolormatch.php
     *
     * @param resource $image1
     * @param resource $image2
     *
     * @return bool
     */
    function imagecolormatch($image1, $image2)
    {
    }
}

if (!function_exists('imagecolorresolve')) {
    /**
     * Get the index of the specified color or its closest possible alternative.
     *
     * @see http://www.php.net/manual/ru/function.imagecolorresolve.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     *
     * @return int
     */
    function imagecolorresolve($image, $red, $green, $blue)
    {
    }
}

if (!function_exists('imagecolorresolvealpha')) {
    /**
     * Get the index of the specified color + alpha or its closest possible alternative.
     *
     * @see https://www.php.net/manual/en/function.imagecolorresolvealpha.php
     *
     * @param resource $image
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     * @param int      $alpha
     *
     * @return int
     */
    function imagecolorresolvealpha($image, $red, $green, $blue, $alpha)
    {
    }
}

if (!function_exists('imagecolorset')) {
    /**
     * Set the color for the specified palette index.
     *
     * @see https://www.php.net/manual/en/function.imagecolorset.php
     *
     * @param resource $image
     * @param int      $index
     * @param int      $red
     * @param int      $green
     * @param int      $blue
     * @param int      $alpha
     */
    function imagecolorset($image, $index, $red, $green, $blue, $alpha = 0)
    {
    }
}

if (!function_exists('imagecolorsforindex')) {
    /**
     * Get the colors for an index.
     *
     * @see https://www.php.net/manual/en/function.imagecolorsforindex.php
     *
     * @param resource $image
     * @param int      $index
     *
     * @return array
     */
    function imagecolorsforindex($image, $index)
    {
    }
}

if (!function_exists('imagecreate')) {
    /**
     * Create a new palette based image.
     *
     * @see https://www.php.net/manual/en/function.imagecreate.php
     *
     * @param $width
     * @param $height
     *
     * @return resource|bool
     */
    function imagecreate($width, $height)
    {
    }
}

if (!function_exists('imagettfbbox')) {
    /**
     * Give the bounding box of a text using TrueType fonts.
     *
     * @see https://www.php.net/manual/en/function.imagettfbbox.php
     *
     * @param float  $size
     * @param float  $angle
     * @param string $fontfile
     * @param string $text
     *
     * @return array|bool
     */
    function imagettfbbox($size, $angle, $fontfile, $text)
    {
    }
}

if (!function_exists('imagettftext')) {
    /**
     * Write text to the image using TrueType fonts.
     *
     * @see https://www.php.net/manual/en/function.imagettftext.php
     *
     * @param resource $image
     * @param float    $size
     * @param float    $angle
     * @param int      $x
     * @param int      $y
     * @param int      $color
     * @param string   $fontfile
     * @param string   $text
     *
     * @return array|bool
     */
    function imagettftext($image, $size, $angle, $x, $y, $color, $fontfile, $text)
    {
    }
}

/*
 * @link https://www.php.net/manual/en/image.constants.php
 */
defined('IMG_GIF') || define('IMG_GIF', 1);
defined('IMG_JPG') || define('IMG_JPG', 2);
defined('IMG_JPEG') || define('IMG_JPEG', 2);
defined('IMG_PNG') || define('IMG_PNG', 4);
defined('IMG_WBMP') || define('IMG_WBMP', 8);
defined('IMG_XPM') || define('IMG_XPM', 16);
defined('IMG_COLOR_TILED') || define('IMG_COLOR_TILED', -5);
defined('IMG_COLOR_STYLED') || define('IMG_COLOR_STYLED', -2);
defined('IMG_COLOR_BRUSHED') || define('IMG_COLOR_BRUSHED', -3);
defined('IMG_COLOR_STYLEDBRUSHED') || define('IMG_COLOR_STYLEDBRUSHED', -4);
defined('IMG_COLOR_TRANSPARENT') || define('IMG_COLOR_TRANSPARENT', -6);
defined('IMG_ARC_ROUNDED') || define('IMG_ARC_ROUNDED', 0);
defined('IMG_ARC_PIE') || define('IMG_ARC_PIE', 0);
defined('IMG_ARC_CHORD') || define('IMG_ARC_CHORD', 1);
defined('IMG_ARC_NOFILL') || define('IMG_ARC_NOFILL', 2);
defined('IMG_ARC_EDGED') || define('IMG_ARC_EDGED', 4);
defined('IMG_GD2_RAW') || define('IMG_GD2_RAW', 1);
defined('IMG_GD2_COMPRESSED') || define('IMG_GD2_COMPRESSED', 2);
defined('IMG_EFFECT_REPLACE') || define('IMG_EFFECT_REPLACE', 0);
defined('IMG_EFFECT_ALPHABLEND') || define('IMG_EFFECT_ALPHABLEND', 1);
defined('IMG_EFFECT_NORMAL') || define('IMG_EFFECT_NORMAL', 2);
defined('IMG_EFFECT_OVERLAY') || define('IMG_EFFECT_OVERLAY', 3);
defined('GD_BUNDLED') || define('GD_BUNDLED', 1);
defined('IMG_FILTER_NEGATE') || define('IMG_FILTER_NEGATE', 0);
defined('IMG_FILTER_GRAYSCALE') || define('IMG_FILTER_GRAYSCALE', 1);
defined('IMG_FILTER_BRIGHTNESS') || define('IMG_FILTER_BRIGHTNESS', 2);
defined('IMG_FILTER_CONTRAST') || define('IMG_FILTER_CONTRAST', 3);
defined('IMG_FILTER_COLORIZE') || define('IMG_FILTER_COLORIZE', 4);
defined('IMG_FILTER_EDGEDETECT') || define('IMG_FILTER_EDGEDETECT', 5);
defined('IMG_FILTER_GAUSSIAN_BLUR') || define('IMG_FILTER_GAUSSIAN_BLUR', 7);
defined('IMG_FILTER_SELECTIVE_BLUR') || define('IMG_FILTER_SELECTIVE_BLUR', 8);
defined('IMG_FILTER_EMBOSS') || define('IMG_FILTER_EMBOSS', 6);
defined('IMG_FILTER_MEAN_REMOVAL') || define('IMG_FILTER_MEAN_REMOVAL', 9);
defined('IMG_FILTER_SMOOTH') || define('IMG_FILTER_SMOOTH', 10);
defined('IMG_FILTER_PIXELATE') || define('IMG_FILTER_PIXELATE', 11);
defined('GD_VERSION') || define('GD_VERSION', '2.0.35');
defined('GD_MAJOR_VERSION') || define('GD_MAJOR_VERSION', 2);
defined('GD_MINOR_VERSION') || define('GD_MINOR_VERSION', 0);
defined('GD_RELEASE_VERSION') || define('GD_RELEASE_VERSION', 35);
defined('GD_EXTRA_VERSION') || define('GD_EXTRA_VERSION', '');
defined('PNG_NO_FILTER') || define('PNG_NO_FILTER', 0);
defined('PNG_FILTER_NONE') || define('PNG_FILTER_NONE', 8);
defined('PNG_FILTER_SUB') || define('PNG_FILTER_SUB', 16);
defined('PNG_FILTER_UP') || define('PNG_FILTER_UP', 32);
defined('PNG_FILTER_AVG') || define('PNG_FILTER_AVG', 64);
defined('PNG_FILTER_PAETH') || define('PNG_FILTER_PAETH', 128);
defined('PNG_ALL_FILTERS') || define('PNG_ALL_FILTERS', 248);
defined('IMG_AFFINE_TRANSLATE') || define('IMG_AFFINE_TRANSLATE', 0);
defined('IMG_AFFINE_SCALE') || define('IMG_AFFINE_SCALE', 1);
defined('IMG_AFFINE_ROTATE') || define('IMG_AFFINE_ROTATE', 2);
defined('IMG_AFFINE_SHEAR_HORIZONTAL') || define('IMG_AFFINE_SHEAR_HORIZONTAL', 3);
defined('IMG_AFFINE_SHEAR_VERTICAL') || define('IMG_AFFINE_SHEAR_VERTICAL', 4);
defined('IMG_CROP_DEFAULT') || define('IMG_CROP_DEFAULT', 0);
defined('IMG_CROP_TRANSPARENT') || define('IMG_CROP_TRANSPARENT', 1);
defined('IMG_CROP_BLACK') || define('IMG_CROP_BLACK', 2);
defined('IMG_CROP_WHITE') || define('IMG_CROP_WHITE', 3);
defined('IMG_CROP_SIDES') || define('IMG_CROP_SIDES', 4);
defined('IMG_FLIP_BOTH') || define('IMG_FLIP_BOTH', 3);
defined('IMG_FLIP_HORIZONTAL') || define('IMG_FLIP_HORIZONTAL', 1);
defined('IMG_FLIP_VERTICAL') || define('IMG_FLIP_VERTICAL', 2);
defined('IMG_BELL') || define('IMG_BELL', 1);
defined('IMG_BESSEL') || define('IMG_BESSEL', 2);
defined('IMG_BICUBIC') || define('IMG_BICUBIC', 4);
defined('IMG_BICUBIC_FIXED') || define('IMG_BICUBIC_FIXED', 5);
defined('IMG_BILINEAR_FIXED') || define('IMG_BILINEAR_FIXED', 3);
defined('IMG_BLACKMAN') || define('IMG_BLACKMAN', 6);
defined('IMG_BOX') || define('IMG_BOX', 7);
defined('IMG_BSPLINE') || define('IMG_BSPLINE', 8);
defined('IMG_CATMULLROM') || define('IMG_CATMULLROM', 9);
defined('IMG_GAUSSIAN') || define('IMG_GAUSSIAN', 10);
defined('IMG_GENERALIZED_CUBIC') || define('IMG_GENERALIZED_CUBIC', 11);
defined('IMG_HERMITE') || define('IMG_HERMITE', 12);
defined('IMG_HAMMING') || define('IMG_HAMMING', 13);
defined('IMG_HANNING') || define('IMG_HANNING', 14);
defined('IMG_MITCHELL') || define('IMG_MITCHELL', 15);
defined('IMG_POWER') || define('IMG_POWER', 17);
defined('IMG_QUADRATIC') || define('IMG_QUADRATIC', 18);
defined('IMG_SINC') || define('IMG_SINC', 19);
defined('IMG_NEAREST_NEIGHBOUR') || define('IMG_NEAREST_NEIGHBOUR', 16);
defined('IMG_WEIGHTED4') || define('IMG_WEIGHTED4', 21);
defined('IMG_TRIANGLE') || define('IMG_TRIANGLE', 20);
