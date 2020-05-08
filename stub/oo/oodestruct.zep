
/**
 * Class with dynamic new
 */

namespace Stub\Oo;

class OoDestruct
{
	protected static checked = false;

	protected file;

	/**
	 * Image height
	 *
	 * @var int
	 */
	protected height { get };

	protected image { get };
	/**
	 * Image mime type
	 *
	 * @var string
	 */
	protected mime { get };

	protected realpath { get };

	/**
	 * Image type
	 *
	 * Driver dependent
	 *
	 * @var int
	 */
	protected type { get };

	/**
	 * Image width
	 *
	 * @var int
	 */
	protected width { get };

	public function __construct(string! file, int width = null, int height = null)
	{
		var imageinfo;

		if !self::checked {
			self::check();
		}

		let this->file = file;

		if file_exists(this->file) {
			let this->realpath = realpath(this->file);
			let imageinfo = getimagesize(this->file);

			if imageinfo {
				let this->width = imageinfo[0];
				let this->height = imageinfo[1];
				let this->type = imageinfo[2];
				let this->mime = imageinfo["mime"];
			}

			switch this->type {
				case 1:
					let this->image = imagecreatefromgif(this->file);
					break;

				case 2:
					let this->image = imagecreatefromjpeg(this->file);
					break;

				case 3:
					let this->image = imagecreatefrompng(this->file);
					break;

				case 15:
					let this->image = imagecreatefromwbmp(this->file);
					break;

				case 16:
					let this->image = imagecreatefromxbm(this->file);
					break;

				default:
					if this->mime {
						throw new Exception(
							"Installed GD does not support " . this->mime . " images"
						);
					}

					throw new Exception(
						"Installed GD does not support such images"
					);
			}

			imagesavealpha(this->image, true);

		} else {
			if unlikely !width || !height {
				throw new Exception(
					"Failed to create image from file " . this->file
				);
			}

			let this->image = imagecreatetruecolor(width, height);

			imagealphablending(this->image, true);
			imagesavealpha(this->image, true);

			let this->realpath = this->file;
			let this->width	= width;
			let this->height   = height;
			let this->type	 = 3;
			let this->mime	 = "image/png";
		}
	}

	public function __destruct()
	{
		var image;

		let image = this->image;

		if typeof image  == "resource" {
			imagedestroy(image);
		}
	}


	public static function check() -> bool
	{
		var version;

		if self::checked {
			return true;
		}

		if unlikely !function_exists("gd_info") {
			throw new Exception(
				"GD is either not installed or not enabled, check your configuration"
			);
		}

		let version = self::getVersion();

		if unlikely !version_compare(version, "2.0.1", ">=") {
			throw new Exception(
				"Requires GD version '2.0.1' or greater, you have " . version
			);
		}

		let self::checked = true;

		return self::checked;
	}



	public static function getVersion() -> string
	{
		var version, info, matches;

		if unlikely !function_exists("gd_info") {
			throw new Exception(
				"GD is either not installed or not enabled, check your configuration"
			);
		}

		let version = null;

		if defined("GD_VERSION") {
			let version = GD_VERSION;
		} else {
			let info = gd_info(),
				matches = null;

			if preg_match("/\\d+\\.\\d+(?:\\.\\d+)?/", info["GD Version"], matches) {
				let version = matches[0];
			}
		}

		return version;
	}

}
