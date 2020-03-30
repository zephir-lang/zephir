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

Function InstallPhpSdk {
    Write-Output "Install PHP SDK binary tools: ${env:PHP_SDK_VERSION}"

    $RemoteUrl = "https://github.com/microsoft/php-sdk-binary-tools/archive/php-sdk-${env:PHP_SDK_VERSION}.zip"
    $DestinationPath = "C:\Downloads\php-sdk-${env:PHP_SDK_VERSION}.zip"

    If (-not (Test-Path $env:PHP_SDK_PATH)) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading PHP SDK binary tools: $RemoteUrl ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        $DestinationUnzipPath = "${env:Temp}\php-sdk-binary-tools-php-sdk-${env:PHP_SDK_VERSION}"

        If (-not (Test-Path "$DestinationUnzipPath")) {
            Expand-Item7zip $DestinationPath $env:Temp
        }

        Move-Item -Path $DestinationUnzipPath -Destination $env:PHP_SDK_PATH
    }
}

Function InstallPhpDevPack {
    Write-Output "Install PHP Dev pack: ${env:PHP_VERSION}"

    If ($env:BUILD_TYPE -Match "nts") {
        $TS = "nts-"
    } Else {
        $TS = ""
    }

    $BaseUrl = "http://windows.php.net/downloads/releases"
    $LocalPart = "/php-devel-pack-${env:PHP_VERSION}-${TS}Win32-vc${env:VC_VERSION}-${env:PHP_ARCH}.zip"

    $RemoteUrl = "${BaseUrl}${LocalPart}"
    $DestinationPath = "C:\Downloads\php-devel-pack-${env:PHP_VERSION}-${TS}-VC${env:VC_VERSION}-${env:PHP_ARCH}.zip"

    If (-not (Test-Path $env:PHP_DEVPACK)) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading PHP Dev pack: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        $DestinationUnzipPath = "${env:Temp}\php-${env:PHP_VERSION}-devel-VC${env:VC_VERSION}-${env:PHP_ARCH}"

        If (-not (Test-Path "$DestinationUnzipPath")) {
            Expand-Item7zip $DestinationPath $env:Temp
        }

        Move-Item -Path $DestinationUnzipPath -Destination $env:PHP_DEVPACK
    }
}

Function InstallZephirParser {
    $BaseUri = "https://github.com/phalcon/php-zephir-parser/releases/download"
    $LocalPart = "zephir_parser_${env:PHP_ARCH}_vc${env:VC_VERSION}_php${env:PHP_MINOR}"

    If ($env:BUILD_TYPE -Match "nts") {
        $TS = "-nts"
    } Else {
        $TS = ""
    }

    $RemoteUrl = "${BaseUri}/v${env:PARSER_VERSION}/${LocalPart}${TS}_${env:PARSER_VERSION}-${env:PARSER_RELEASE}.zip"
    $DestinationPath = "C:\Downloads\${LocalPart}${TS}_${env:PARSER_VERSION}-${env:PARSER_RELEASE}.zip"

    If (-not (Test-Path "${env:PHPROOT}\ext\php_zephir_parser.dll")) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading Zephir Parser: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        Expand-Item7zip $DestinationPath "${env:PHPROOT}\ext"
    }
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

Function DownloadFile {
    Param(
        [Parameter(Mandatory=$true)][System.String] $RemoteUrl,
        [Parameter(Mandatory=$true)][System.String] $DestinationPath
    )

    $RetryMax   = 5
    $RetryCount = 0
    $Completed  = $false

    $WebClient = New-Object System.Net.WebClient
    $WebClient.Headers.Add('User-Agent', 'GitHub Actions PowerShell Script')

    While (-not $Completed) {
        Try {
            $WebClient.DownloadFile($RemoteUrl, $DestinationPath)
            $Completed = $true
        } Catch {
            If ($RetryCount -ge $RetryMax) {
                $ErrorMessage = $_.Exception.Message
                Write-Output "Error downloadingig ${RemoteUrl}: $ErrorMessage"
                $Completed = $true
            } Else {
                $RetryCount++
            }
        }
    }
}

Function PrintLogs {
    If (Test-Path -Path "${env:GITHUB_WORKSPACE}\compile-errors.log") {
        Get-Content -Path "${env:GITHUB_WORKSPACE}\compile-errors.log"
    }

    If (Test-Path -Path "${env:GITHUB_WORKSPACE}\compile.log") {
        Get-Content -Path "${env:GITHUB_WORKSPACE}\compile.log"
    }

    If (Test-Path -Path "${env:GITHUB_WORKSPACE}\ext\configure.js") {
        Get-Content -Path "${env:GITHUB_WORKSPACE}\ext\configure.js"
    }
}

Function PrintEnvVars {
    Write-Output ($env:Path).Replace(';', "`n")
    Get-ChildItem env:
}

Function PrintDirectoriesContent {
    Get-ChildItem -Path "${env:GITHUB_WORKSPACE}"

    If (Test-Path -Path "C:\Downloads") {
        Get-ChildItem -Path "C:\Downloads"
    }

    If (Test-Path -Path "C:\Projects") {
        Get-ChildItem -Path "C:\Projects"
    }

    If (Test-Path -Path "${env:PHPROOT}\ext") {
        Get-ChildItem -Path "${env:PHPROOT}\ext"
    }

    # Might be empty
    if ("${env:RELEASE_DLL_PATH}") {
        $ReleasePath = Split-Path -Path "${env:RELEASE_DLL_PATH}"
        If (Test-Path -Path "${ReleasePath}") {
            Get-ChildItem -Path "${ReleasePath}"
        }

        $BuildPath = Split-Path -Path "${ReleasePath}"
        If (Test-Path -Path "${BuildPath}") {
            Get-ChildItem -Path "${BuildPath}"
        }
    }
}

# TODO(klay): Add phpize and phpconfig here
Function PrintPhpInfo {
    $IniFile = "${env:PHPROOT}\php.ini"
    $PhpExe = "${env:PHPROOT}\php.exe"

    If (Test-Path -Path "${PhpExe}") {
        Write-Output ""
        & "${PhpExe}" -v

        Write-Output ""
        & "${PhpExe}" -m

        Write-Output ""
        & "${PhpExe}" -i
    } ElseIf (Test-Path -Path "${IniFile}") {
        Get-Content -Path "${IniFile}"
    }
}

Function PrintBuildDetails {
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


Function InitializeReleaseVars {
    If ($env:BUILD_TYPE -Match "nts") {
        $env:RELEASE_ZIPBALL = "${env:PACKAGE_PREFIX}_${env:PHP_ARCH}_vc${env:VC_VERSION}_php${env:PHP_MINOR}_nts"

        If ($env:PHP_ARCH -eq 'x86') {
            $env:RELEASE_FOLDER = "Release"
        } Else {
            $env:RELEASE_FOLDER = "x64\Release"
        }
    } Else {
        $env:RELEASE_ZIPBALL = "${env:PACKAGE_PREFIX}_${env:PHP_ARCH}_vc${env:VC_VERSION}_php${env:PHP_MINOR}"

        If ($env:PHP_ARCH -eq 'x86') {
            $env:RELEASE_FOLDER = "Release_TS"
        } Else {
            $env:RELEASE_FOLDER = "x64\Release_TS"
        }
    }

    $env:RELEASE_DLL_PATH = "${env:GITHUB_WORKSPACE}\ext\${env:RELEASE_FOLDER}\${env:EXTENSION_FILE}"

    Write-Output "::set-env name=RELEASE_ZIPBALL::${env:RELEASE_ZIPBALL}"
    Write-Output "::set-env name=RELEASE_DLL_PATH::${env:RELEASE_DLL_PATH}"
}

Function EnableTestExtension {
    if (-not (Test-Path env:RELEASE_DLL_PATH)) {
        InitializeReleaseVars
    }

    If (-not (Test-Path "${env:RELEASE_DLL_PATH}")) {
        Throw "Unable to locate extension path: ${env:RELEASE_DLL_PATH}"
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
