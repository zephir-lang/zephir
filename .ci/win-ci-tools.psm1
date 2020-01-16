# This file is part of the Zephir.
#
# (c) Phalcon Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

Function SetupCommonEnvironmet {
    If (-not (Test-Path 'C:\Downloads')) {
        New-Item -ItemType Directory -Force -Path 'C:\Downloads' | Out-Null
    }

    If (-not (Test-Path 'C:\Downloads\Choco')) {
        New-Item -ItemType Directory -Force -Path 'C:\Downloads\Choco' | Out-Null
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

    $RemoteUrl = "http://windows.php.net/downloads/releases/php-devel-pack-${env:PHP_VERSION}-${env:BUILD_TYPE}-vc${env:VC_VERSION}-${env:PHP_ARCH}.zip"
    $DestinationPath = "C:\Downloads\php-devel-pack-${env:PHP_VERSION}-${env:BUILD_TYPE}-VC${env:VC_VERSION}-${env:PHP_ARCH}.zip"

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
    $LocalPart = "zephir_parser_${Env:PHP_ARCH}_vc${Env:VC_VERSION}_php${Env:PHP_MINOR}"

    If ($Env:BUILD_TYPE -Match "nts-Win32") {
        $VersionPrefix = "-nts"
    } Else {
        $VersionPrefix = ""
    }

    $RemoteUrl = "${BaseUri}/v${Env:PARSER_VERSION}/${LocalPart}${VersionPrefix}_${Env:PARSER_VERSION}-${Env:PARSER_RELEASE}.zip"
    $DestinationPath = "C:\Downloads\${LocalPart}${VersionPrefix}_${Env:PARSER_VERSION}-${Env:PARSER_RELEASE}.zip"

    If (-not (Test-Path "${Env:PHPROOT}\ext\php_zephir_parser.dll")) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading Zephir Parser: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        Expand-Item7zip $DestinationPath "${Env:PHPROOT}\ext"
    }
}


Function Expand-Item7zip {
    Param(
        [Parameter(Mandatory=$true)][System.String] $Archive,
        [Parameter(Mandatory=$true)][System.String] $Destination
    )

    If (-not (Test-Path -Path $Archive -PathType Leaf)) {
        Throw "Specified archive File is invalid: [$Archive]"
    }

    If (-not (Test-Path -Path $Destination -PathType Container)) {
        New-Item $Destination -ItemType Directory | Out-Null
    }

    $Result = (& 7z x "$Archive" "-o$Destination" -aoa -bd -y -r)

    If ($LastExitCode -ne 0) {
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
    Get-ChildItem -Path "${Env:GITHUB_WORKSPACE}"

    If (Test-Path -Path "C:\Downloads") {
        Get-ChildItem -Path "C:\Downloads"
    }

    If (Test-Path -Path "C:\Projects") {
        Get-ChildItem -Path "C:\Projects"
    }

    If (Test-Path -Path "${Env:PHPROOT}\ext") {
        Get-ChildItem -Path "${Env:PHPROOT}\ext"
    }

    # TODO(klay): Not used.
    #
    # $ReleasePath = Split-Path -Path "${Env:RELEASE_DLL_PATH}"
    # If (Test-Path -Path "${ReleasePath}") {
    #     Get-ChildItem -Path "${ReleasePath}"
    # }
    #
    # $BuildPath = Split-Path -Path "${ReleasePath}"
    # If (Test-Path -Path "${BuildPath}") {
    #     Get-ChildItem -Path "${BuildPath}"
    # }
}

# TODO(klay): Add phpize and phpconfig here
Function PrintPhpInfo {
    $IniFile = "${Env:PHPROOT}\php.ini"
    $PhpExe = "${Env:PHPROOT}\php.exe"

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
    Write-Output "Git commit: ${Env:GITHUB_SHA}"
    Write-Output "Target PHP version: ${Env:PHP_MINOR}"
    Write-Output "PHP SDK Toolset Version: ${Env:PHP_SDK_VC_TOOLSET_VER}"
    Write-Output "Build Worker Image Version: ${Env:ImageVersion}"
    Write-Output "Processor ID: ${Env:PROCESSOR_IDENTIFIER}"
    Write-Output "Processor Architecture: ${Env:PROCESSOR_ARCHITECTURE}"
    Write-Output "Number of Processors: ${Env:NUMBER_OF_PROCESSORS}"
    Write-Output "Visual Studio Version: ${Env:VisualStudioVersion}"
    Write-Output "Host Architecture: ${Env:VSCMD_ARG_HOST_ARCH}"
    Write-Output "Target Architecture: ${Env:VSCMD_ARG_TGT_ARCH}"
    Write-Output "VC Tools Version: ${Env:VCToolsVersion}"
    Write-Output "Windows SDK Version: ${Env:WindowsSDKVersion}"
}
