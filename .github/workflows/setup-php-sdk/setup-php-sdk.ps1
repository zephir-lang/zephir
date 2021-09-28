param (
    # PHP version to build for (e.g: 7.4, 8.0)
    [Parameter(Mandatory = $true)] [ValidatePattern("[5-8]\.[0-9]")] [System.String] $PhpVersion,
    [Parameter(Mandatory = $false)] [ValidatePattern("^(ts|nts)$")] [System.String] $ThreadSafety,
    [Parameter(Mandatory = $false)] [ValidatePattern("^v(c|s)\d\d$")] [System.String] $VC,
    [Parameter(Mandatory = $false)] [ValidatePattern("^x(64|86)$")] [System.String] $Arch,
    [Parameter(Mandatory = $false)] [System.String] $InstallDir,
    [Parameter(Mandatory = $false)] [System.String] $CacheDir
)

function Install-SDK {
    <#
        .SYNOPSIS
            Install PHP SDK binary tools from sources.
    #>

    [CmdletBinding()]
    param (
        # PHP version for detect compatible SDK version
        [Parameter(Mandatory = $true)] [System.String] $version,
        [Parameter(Mandatory = $false)] [System.String] $installDir,
        [Parameter(Mandatory = $false)] [System.String] $cacheDir
    )

    process {
        # The PHP SDK 2.2+ is compatible with PHP 7.2 and above.
        # The PHP SDK 2.1 is required to build PHP 7.1 or 7.0.
        $SdkVersion = if ($PhpVersion -lt "7.2") {"2.1.10"} else {"2.2.0"}

        $PhpSdkZip = "php-sdk-${SdkVersion}.zip"
        $RemoteUrl = "https://github.com/microsoft/php-sdk-binary-tools/archive/${PhpSdkZip}"

        $temp = if (!$cacheDir) {
            New-TemporaryFile | Rename-Item -NewName {$_.Name + ".zip"} -PassThru
        } else {
            "${cacheDir}\${PhpSdkZip}"
        }

        if (-not (Test-Path $temp) -or (Get-Item $temp).length -eq 0kb) {
            Write-Output "Downloading PHP SDK binary tools v${SdkVersion}"
            Invoke-WebRequest $RemoteUrl -OutFile $temp
        }

        if (-not (Test-Path "$installDir\php-sdk")) {
            Expand-Archive $temp -DestinationPath $installDir
            Rename-Item (Resolve-Path "${installDir}\php-sdk-binary-tools-php-sdk-${SdkVersion}") "php-sdk"
        }

        Write-Output "PHP SDK v${SdkVersion} installed to ${installDir}\php-sdk"
    }
}

function Install-DevPack {
    <#
        .SYNOPSIS
            Intstall PHP Developer pack from sources.
    #>

    [CmdletBinding()]
    param (
        [Parameter(Mandatory = $true)] [System.String] $version,
        [Parameter(Mandatory = $true)] [System.String] $ts,
        [Parameter(Mandatory = $true)] [System.String] $msvc,
        [Parameter(Mandatory = $true)] [System.String] $arch,
        [Parameter(Mandatory = $true)] [System.String] $installDir,
        [Parameter(Mandatory = $false)] [System.String] $cacheDir
    )

    process {
        $baseUrl = "https://windows.php.net/downloads/releases"
        $releases = Invoke-WebRequest "${baseUrl}/releases.json" | ConvertFrom-Json
        $tsPrefix = if ($ts -eq 'ts') {'Win32'} else {'nts-Win32'}

        if (-not $releases.$PhpVersion) {
            # Download from archive using detected PHP version
            $phpVer = & php -r 'echo PHP_VERSION;'
            $baseUrl = "${baseUrl}/archives"
        } else {
            $phpVer = $releases.$PhpVersion.version
        }

        $devPackName = "php-devel-pack-${phpVer}-${tsPrefix}-${msvc}-${arch}.zip"
        $RemoteUrl = "${baseUrl}/${devPackName}"

        $temp = if (!$cacheDir) {
            New-TemporaryFile | Rename-Item -NewName {$_.Name + ".zip"} -PassThru
        } else {
            "${cacheDir}\${devPackName}"
        }

        if (-not (Test-Path $temp) -or (Get-Item $temp).length -eq 0kb) {
            Write-Output "Downloading PHP Developer Pack for PHP v${phpVer} from ${RemoteUrl}"
            Invoke-WebRequest $RemoteUrl -OutFile $temp
        }

        if (-not (Test-Path "$installDir\php-devpack")) {
            Expand-Archive $temp -DestinationPath $installDir
            Rename-Item (Resolve-Path "${installDir}\php-${phpVer}-devel-${msvc}-${arch}") "php-devpack"
        }

        Write-Output "PHP Developer Pack php-${phpVer}-devel-${msvc}-${arch} installed to ${installDir}\php-devpack"
    }
}

Write-Output "::group::Installing PHP SDK binary tools"
Install-SDK -version $PhpVersion -cacheDir $CacheDir -installDir $InstallDir
Write-Output "::endgroup::"

Write-Output "::group::Installing PHP Develeoper Pack"
Install-DevPack -version $PhpVersion -ts $ThreadSafety -msvc $VC -arch $Arch `
    -cacheDir $CacheDir -installDir $InstallDir
Write-Output "::endgroup::"

Write-Output "::group::Add PHP SDK and PHP Developer Pack to system PATH"
Add-Content $Env:GITHUB_PATH "${InstallDir}\php-sdk\bin"
Add-Content $Env:GITHUB_PATH "${InstallDir}\php-sdk\msys2\usr\bin"
Add-Content $Env:GITHUB_PATH "${InstallDir}\php-devpack"
Write-Output "::endgroup::"
