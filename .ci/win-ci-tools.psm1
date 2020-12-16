# This file is part of the Zephir.
#
# (c) Phalcon Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

function SetupCommonEnvironment {
    <#
        .SYNOPSIS
            Creates common directories if not exists
    #>

    $CommonPath = "C:\Downloads", "C:\Downloads\Choco"

    foreach ($path in $CommonPath) {
        if (-not (Test-Path $path)) {
            New-Item -ItemType Directory -Force -Path $path | Out-Null
        }
    }

    # Hide "You are in 'detached HEAD' state" message
    git config --global advice.detachedHead false
}

function InstallPhpSdk {
    <#
        .SYNOPSIS
            Install PHP SDK binary tools from sources.
    #>

    Write-Output "Install PHP SDK binary tools: ${env:PHP_SDK_VERSION}"

    $PhpSdk = "php-sdk-${env:PHP_SDK_VERSION}.zip"
    $RemoteUrl = "https://github.com/microsoft/php-sdk-binary-tools/archive/${PhpSdk}"
    $DestinationPath = "C:\Downloads\${PhpSdk}"

    if (-not (Test-Path $env:PHP_SDK_PATH)) {
        if (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading PHP SDK binary tools: $RemoteUrl ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        $DestinationUnzipPath = "${env:Temp}\php-sdk-binary-tools-php-sdk-${env:PHP_SDK_VERSION}"

        if (-not (Test-Path "$DestinationUnzipPath")) {
            Expand-Item7zip $DestinationPath $env:Temp
        }

        Move-Item -Path $DestinationUnzipPath -Destination $env:PHP_SDK_PATH
    }
}

function InstallPhpDevPack {
    <#
        .SYNOPSIS
            Intstall PHP Developer pack from sources.
    #>

    Write-Output "Install PHP Dev pack: ${env:PHP_VERSION}"

    $TS = Get-ThreadSafety

    $BaseUrl = "http://windows.php.net/downloads/releases"
    $DevPack = "php-devel-pack-${env:PHP_VERSION}${TS}-Win32-vc${env:VC_VERSION}-${env:PHP_ARCH}.zip"

    $RemoteUrl = "${BaseUrl}/${DevPack}"
    $RemoteArchiveUrl = "${BaseUrl}/archives/${DevPack}"
    $DestinationPath = "C:\Downloads\php-devel-pack-${env:PHP_VERSION}${TS}-VC${env:VC_VERSION}-${env:PHP_ARCH}.zip"

    if (-not (Test-Path $env:PHP_DEVPACK)) {
        if (-not [System.IO.File]::Exists($DestinationPath)) {
            DownloadFileUsingAlternative -RemoteUrl $RemoteUrl `
                -RemoteArchiveUrl $RemoteArchiveUrl `
                -DestinationPath $DestinationPath `
                -Message "Downloading PHP Dev pack"
        }

        $DestinationUnzipPath = "${env:Temp}\php-${env:PHP_VERSION}-devel-VC${env:VC_VERSION}-${env:PHP_ARCH}"

        if (-not (Test-Path "$DestinationUnzipPath")) {
            Expand-Item7zip $DestinationPath $env:Temp
        }

        Move-Item -Path $DestinationUnzipPath -Destination $env:PHP_DEVPACK
    }
}

function InstallZephirParser {
    <#
        .SYNOPSIS
            Download and install Zephir parser PHP extension
    #>

    $BaseUri = "https://github.com/phalcon/php-zephir-parser/releases/download"
    $LocalPart = "zephir_parser_${env:PHP_ARCH}_vc${env:VC_VERSION}_php${env:PHP_MINOR}"

    $TS = Get-ThreadSafety

    $RemoteUrl = "${BaseUri}/v${env:PARSER_VERSION}/${LocalPart}${TS}_${env:PARSER_VERSION}-${env:PARSER_RELEASE}.zip"
    $DestinationPath = "C:\Downloads\${LocalPart}${TS}_${env:PARSER_VERSION}-${env:PARSER_RELEASE}.zip"

    if (-not (Test-Path "${env:PHPROOT}\ext\php_zephir_parser.dll")) {
        if (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading Zephir Parser: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        Expand-Item7zip $DestinationPath "${env:PHPROOT}\ext"
    }
}

function Get-ThreadSafety {
    <#
        .SYNOPSIS
            Detects if Build is Thread Safety or not and returns `ts` suffix.
    #>

    if ($env:BUILD_TYPE -Match "nts") {
        return "-nts"
    }

    return [string]::Empty
}


function Expand-Item7zip {
    <#
        .SYNOPSIS
            Extracts ZIP archives to specified directory
    #>

    param(
        [Parameter(Mandatory = $true)] [System.String] $Archive,
        [Parameter(Mandatory = $true)] [System.String] $Destination
    )

    if (-not (Test-Path -Path $Archive -PathType Leaf)) {
        throw "Specified archive File is invalid: [$Archive]"
    }

    if (-not (Test-Path -Path $Destination -PathType Container)) {
        New-Item $Destination -ItemType Directory | Out-Null
    }

    $Result = (& 7z x "$Archive" "-o$Destination" -aoa -bd -y -r)

    if ($LastExitCode -ne 0) {
        Write-Output "An error occurred while unzipping [$Archive] to [$Destination]. Error code was: ${LastExitCode}"
        Exit $LastExitCode
    }
}

function DownloadFileUsingAlternative {
    <#
        .SYNOPSIS
            Downloads files from URL using alternative ULR if primary URL not found
    #>

    [CmdletBinding()]
    param(
        [parameter(Mandatory = $true, ValueFromPipeline = $true)] [ValidateNotNullOrEmpty()] [System.String] $RemoteUrl,
        [parameter(Mandatory = $true, ValueFromPipeline = $true)] [ValidateNotNullOrEmpty()] [System.String] $RemoteArchiveUrl,
        [parameter(Mandatory = $true, ValueFromPipeline = $true)] [ValidateNotNullOrEmpty()] [System.String] $DestinationPath,
        [parameter(Mandatory = $true, ValueFromPipeline = $true)] [ValidateNotNullOrEmpty()] [System.String] $Message
    )

    process {
        try {
            Write-Output "${Message}: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        } catch [System.Net.WebException] {
            Write-Output "${Message} from archive: ${RemoteArchiveUrl} ..."
            DownloadFile $RemoteArchiveUrl $DestinationPath
        }
    }
}

function DownloadFile {
    <#
        .SYNOPSIS
            Downloads file from providing URL to specified destionation.

        .NOTES
            Throws System.Net.WebException if $RequestUrl not found.
    #>

    [CmdletBinding()]
    param(
        [parameter(Mandatory = $true)] [ValidateNotNullOrEmpty()] [System.String] $RemoteUrl,
        [parameter(Mandatory = $true)] [ValidateNotNullOrEmpty()] [System.String] $DestinationPath
    )

    process {
        $RetryMax   = 5
        $RetryCount = 0
        $Completed  = $false

        $WebClient = New-Object System.Net.WebClient
        $WebClient.Headers.Add('User-Agent', 'GitHub Actions PowerShell Script')

        while (-not $Completed -or $RetryCount -eq $RetryMax) {
            try {
                $WebClient.DownloadFile($RemoteUrl, $DestinationPath)
                $Completed = $true
            } catch [System.Net.WebException] {
                $ErrorMessage = $_.Exception.Message

                if ($_.Exception.Response.StatusCode -eq 404) {
                    Write-Warning -Message "Error downloading ${RemoteUrl}: $ErrorMessage"
                    throw [System.Net.WebException] "Error downloading ${RemoteUrl}"
                }

                if ($RetryCount -ge $RetryMax) {
                    Write-Output "Error downloading ${RemoteUrl}: $ErrorMessage"
                    $Completed = $true
                } else {
                    $RetryCount++
                }
            }
        }
    }
}

function PrintLogs {
    <#
        .SYNOPSIS
            Prints logs files details.
    #>

    $Logs = "${env:GITHUB_WORKSPACE}\compile-errors.log",
            "${env:GITHUB_WORKSPACE}\compile.log",
            "${env:GITHUB_WORKSPACE}\ext\configure.js"

    foreach ($logFile in $Logs) {
        if (Test-Path -Path $logFile) {
            Get-Content -Path $logFile
        }
    }
}

function PrintEnvVars {
    <#
        .SYNOPSIS
            Prints environment variables.
    #>

    Write-Output ($env:Path).Replace(';', "`n")
    Get-ChildItem env:
}

function PrintDirectoriesContent {
    <#
        .SYNOPSIS
            Prints Builds, Release, Projects, Downloads and Extensions directories contents.
    #>

    Get-ChildItem -Path "${env:GITHUB_WORKSPACE}"

    $Directories =  "C:\Downloads",
                    "C:\Projects",
                    "${env:PHPROOT}\ext"

    if ("${env:RELEASE_DLL_PATH}") {
        # Might be empty
        $ReleasePath = Split-Path -Path "${env:RELEASE_DLL_PATH}"
        $BuildPath = Split-Path -Path "${ReleasePath}"

        $Directories.Add($ReleasePath)
        $Directories.Add($BuildPath)
    }

    foreach ($dir in $Directories) {
        if (Test-Path -Path $dir) {
            Get-ChildItem -Path $dir
        }
    }
}

# TODO(klay): Add phpize and phpconfig here
function PrintPhpInfo {
    <#
        .SYNOPSIS
            Prints PHP info.
    #>

    $IniFile = "${env:PHPROOT}\php.ini"
    $PhpExe = "${env:PHPROOT}\php.exe"

    if (Test-Path -Path "${PhpExe}") {
        Write-Output ""
        & "${PhpExe}" -v

        Write-Output ""
        & "${PhpExe}" -m

        Write-Output ""
        & "${PhpExe}" -i
    } elseif (Test-Path -Path "${IniFile}") {
        Get-Content -Path "${IniFile}"
    }
}

function PrintBuildDetails {
    <#
        .SYNOPSIS
            Prints various Build details information.
    #>

    $BuildDate = Get-Date -Format g

    Write-Output "Build date: ${BuildDate}"
    Write-Output "Build Worker Image Version: ${env:ImageVersion}"
    Write-Output "Build Type: ${env:BUILD_TYPE}"
    Write-Output "Git commit: ${env:GITHUB_SHA}"
    Write-Output "Target PHP version: ${env:PHP_MINOR}"
    Write-Output "PHP SDK Toolset Version: ${env:PHP_SDK_VC_TOOLSET_VER}"
    Write-Output "Processor ID: ${env:PROCESSOR_IDENTIFIER}"
    Write-Output "Processor Architecture: ${env:PROCESSOR_ARCHITECTURE}"
    Write-Output "Number of Processors: ${env:NUMBER_OF_PROCESSORS}"
    Write-Output "Visual Studio Version: ${env:VisualStudioVersion}"
    Write-Output "Host Architecture: ${env:VSCMD_ARG_HOST_ARCH}"
    Write-Output "Target Architecture: ${env:VSCMD_ARG_TGT_ARCH}"
    Write-Output "VC Tools Version: ${env:VCToolsVersion}"
    Write-Output "Windows SDK Version: ${env:WindowsSDKVersion}"
}


function InitializeReleaseVars {
    <#
        .SYNOPSIS
            Configures Environment variables for Release build.
    #>

    if ($env:BUILD_TYPE -Match "nts") {
        $env:RELEASE_ZIPBALL = "${env:PACKAGE_PREFIX}_${env:PHP_ARCH}_vc${env:VC_VERSION}_php${env:PHP_MINOR}_nts"

        if ($env:PHP_ARCH -eq 'x86') {
            $env:RELEASE_FOLDER = "Release"
        } else {
            $env:RELEASE_FOLDER = "x64\Release"
        }
    } else {
        $env:RELEASE_ZIPBALL = "${env:PACKAGE_PREFIX}_${env:PHP_ARCH}_vc${env:VC_VERSION}_php${env:PHP_MINOR}"

        if ($env:PHP_ARCH -eq 'x86') {
            $env:RELEASE_FOLDER = "Release_TS"
        } else {
            $env:RELEASE_FOLDER = "x64\Release_TS"
        }
    }

    $env:RELEASE_DLL_PATH = "${env:GITHUB_WORKSPACE}\ext\${env:RELEASE_FOLDER}\${env:EXTENSION_FILE}"

    Write-Output "RELEASE_ZIPBALL=${env:RELEASE_ZIPBALL}" | Out-File -FilePath $env:GITHUB_ENV -Encoding utf8 -Append
    Write-Output "RELEASE_DLL_PATH=${env:RELEASE_DLL_PATH}" | Out-File -FilePath $env:GITHUB_ENV -Encoding utf8 -Append
}

function EnableTestExtension {
    <#
        .SYNOPSIS
            Enables PHP Extension.
    #>

    if (-not (Test-Path env:RELEASE_DLL_PATH)) {
        InitializeReleaseVars
    }

    if (-not (Test-Path "${env:RELEASE_DLL_PATH}")) {
        throw "Unable to locate extension path: ${env:RELEASE_DLL_PATH}"
    }

    Copy-Item "${env:RELEASE_DLL_PATH}" "${env:PHPROOT}\ext\${env:EXTENSION_FILE}"

    # TODO(klay): Sort out with this:
    #
    # Multiple extensions match the name (or handle) "test":
    # - handle: test version 1.0.0
    # - handle: test version 0.1.0
    # You can filter the extension to enable by adding :version to the -Extension parameter
    # (example: "-Extension 'test:0.1.0'")
    #
    Enable-PhpExtension -Extension "${env:EXTENSION_NAME}:1.0.0" -Path "${env:PHPROOT}"
}
