# This file is part of the Zephir.
#
# (c) Phalcon Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

Function AppendSessionPath {
    $PathsCollection  = @("C:\Program Files (x86)\MSBuild\${Env:VC_VERSION}.0\Bin")
    $PathsCollection += "C:\Program Files (x86)\Microsoft Visual Studio ${Env:VC_VERSION}.0\VC"
    $PathsCollection += "C:\Program Files (x86)\Microsoft Visual Studio ${Env:VC_VERSION}.0\VC\bin"
    $PathsCollection += "${Env:PHP_SDK_PATH}\bin"
    $PathsCollection += "${Env:PHP_PATH}\bin"
    $PathsCollection += "${Env:PHP_PATH}"

    $CurrentPath = (Get-Item -Path ".\" -Verbose).FullName

    ForEach ($PathItem In $PathsCollection) {
        Set-Location Env:
        $AllPaths = (Get-ChildItem Path).value.split(";")  | Sort-Object -Unique
        $AddToPath = $true

        ForEach ($AddedPath In $AllPaths) {
            If (-not "${AddedPath}") {
                continue
            }

            $AddedPath = $AddedPath -replace '\\$', ''

            If ($PathItem -eq $AddedPath) {
                $AddToPath = $false
            }
        }

        If ($AddToPath) {
            $Env:Path += ";$PathItem"
        }
    }

    Set-Location "${CurrentPath}"
}

Function SetupPhpVersionString {
    $RemoteUrl = 'http://windows.php.net/downloads/releases/sha1sum.txt';
    $DestinationPath = "${Env:Temp}\php-sha1sum.txt"

    If (-not [System.IO.File]::Exists($DestinationPath)) {
        Write-Output "Downloading PHP SHA sums file: ${RemoteUrl} ..."
        DownloadFile $RemoteUrl $DestinationPath
    }

    $VersionString = Get-Content $DestinationPath | Where-Object {
        $_ -match "php-($Env:PHP_MINOR\.\d+)-src"
    } | ForEach-Object { $matches[1] }

    If ($VersionString -NotMatch '\d+\.\d+\.\d+') {
        Throw "Unable to obtain PHP version string using pattern 'php-($Env:PHP_MINOR\.\d+)-src'"
     }

    $Env:PHP_VERSION = $VersionString
}

Function EnsureRequiredDirectoriesPresent {
    If (-not (Test-Path 'C:\Downloads')) {
        New-Item -ItemType Directory -Force -Path 'C:\Downloads' | Out-Null
    }

    If (-not (Test-Path 'C:\Downloads\Choco')) {
        New-Item -ItemType Directory -Force -Path 'C:\Downloads\Choco' | Out-Null
    }

    If (-not (Test-Path 'C:\Projects')) {
        New-Item -ItemType Directory -Force -Path 'C:\Projects' | Out-Null
    }
}

Function Ensure7ZipIsInstalled {
    If (-not (Get-Command "7z" -ErrorAction SilentlyContinue)) {
        $7zipInstallationDirectory = "${Env:ProgramFiles}\7-Zip"

        If (-not (Test-Path "$7zipInstallationDirectory")) {
            Throw "The 7-zip file archiver is needed to use this module"
        }

        $Env:Path += ";$7zipInstallationDirectory"
    }
}

Function EnsureChocolateyIsInstalled {
    If (-not (Get-Command "choco" -ErrorAction SilentlyContinue)) {
        $ChocolateyInstallationDirectory = "${Env:ProgramData}\chocolatey\bin"

        If (-not (Test-Path "$ChocolateyInstallationDirectory")) {
            Throw "The choco is needed to use this module"
        }

        $Env:Path += ";$ChocolateyInstallationDirectory"
    }
}

Function InstallSdk {
    Write-Output "Install PHP SDK binary tools: ${Env:PHP_SDK_VERSION}" -foregroundcolor Cyan

    $RemoteUrl = "https://github.com/OSTC/php-sdk-binary-tools/archive/php-sdk-${Env:PHP_SDK_VERSION}.zip"
    $DestinationPath = "C:\Downloads\php-sdk-${Env:PHP_SDK_VERSION}.zip"

    If (-not (Test-Path $Env:PHP_SDK_PATH)) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading PHP SDK binary tools: $RemoteUrl ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        $DestinationUnzipPath = "${Env:Temp}\php-sdk-binary-tools-php-sdk-${Env:PHP_SDK_VERSION}"

        If (-not (Test-Path "$DestinationUnzipPath")) {
            Expand-Item7zip $DestinationPath $Env:Temp
        }

        Move-Item -Path $DestinationUnzipPath -Destination $Env:PHP_SDK_PATH
    }
}

Function InstallPhp {
    Write-Output "Install PHP: ${Env:PHP_VERSION}" -foregroundcolor Cyan

    $RemoteUrl = "http://windows.php.net/downloads/releases/php-${Env:PHP_VERSION}-${Env:BUILD_TYPE}-vc${Env:VC_VERSION}-${Env:PLATFORM}.zip"
    $DestinationPath = "C:\Downloads\php-${Env:PHP_VERSION}-${Env:BUILD_TYPE}-VC${Env:VC_VERSION}-${Env:PLATFORM}.zip"

    If (-not (Test-Path $Env:PHP_PATH)) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading PHP source code: $RemoteUrl ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        Expand-Item7zip "$DestinationPath" "${Env:PHP_PATH}"
    }

    If (-not (Test-Path "${Env:PHP_PATH}\php.ini")) {
        Copy-Item "${Env:PHP_PATH}\php.ini-development" "${Env:PHP_PATH}\php.ini"
    }
}

Function InstallPhpDevPack {
    Write-Output "Install PHP Dev pack: ${Env:PHP_VERSION}" -foregroundcolor Cyan

    $RemoteUrl = "http://windows.php.net/downloads/releases/php-devel-pack-${Env:PHP_VERSION}-${Env:BUILD_TYPE}-vc${Env:VC_VERSION}-${Env:PLATFORM}.zip"
    $DestinationPath = "C:\Downloads\php-devel-pack-${Env:PHP_VERSION}-${Env:BUILD_TYPE}-VC${Env:VC_VERSION}-${Env:PLATFORM}.zip"

    If (-not (Test-Path $Env:PHP_DEVPACK)) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading PHP Dev pack: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        $DestinationUnzipPath = "${Env:Temp}\php-${Env:PHP_VERSION}-devel-VC${Env:VC_VERSION}-${Env:PLATFORM}"

        If (-not (Test-Path "$DestinationUnzipPath")) {
            Expand-Item7zip $DestinationPath $Env:Temp
        }

        Move-Item -Path $DestinationUnzipPath -Destination $Env:PHP_DEVPACK
    }
}

Function InstallParser {
    $BaseUri = "https://github.com/phalcon/php-zephir-parser/releases/download"
    $LocalPart = "zephir_parser_${Env:PLATFORM}_vc${Env:VC_VERSION}_php${Env:PHP_MINOR}"

    If ($Env:BUILD_TYPE -Match "nts-Win32") {
        $VersionPrefix = "-nts"
    } Else {
        $VersionPrefix = ""
    }

    $RemoteUrl = "${BaseUri}/v${Env:PARSER_VERSION}/${LocalPart}${VersionPrefix}_${Env:PARSER_VERSION}-${Env:PARSER_RELEASE}.zip"
    $DestinationPath = "C:\Downloads\${LocalPart}${VersionPrefix}_${Env:PARSER_VERSION}-${Env:PARSER_RELEASE}.zip"

    If (-not (Test-Path "${Env:PHP_PATH}\ext\php_zephir_parser.dll")) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading Zephir Parser: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        Expand-Item7zip $DestinationPath "${Env:PHP_PATH}\ext"
    }
}

Function InstallPsrExtension {
    $BaseUri = "https://windows.php.net/downloads/pecl/releases/psr/${Env:PSR_VERSION}"
    $LocalPart = "php_psr-${Env:PSR_VERSION}-${Env:PHP_MINOR}"

    If ($Env:BUILD_TYPE -Match "nts-Win32") {
        $TS = "nts"
    } Else {
        $TS = "ts"
    }

    $RemoteUrl = "${BaseUri}/${LocalPart}-${TS}-vc${Env:VC_VERSION}-${Env:PLATFORM}.zip"
    $DestinationPath = "C:\Downloads\${LocalPart}-${TS}-vc${Env:VC_VERSION}-${Env:PLATFORM}.zip"

    If (-not (Test-Path "${Env:PHP_PATH}\ext\php_psr.dll")) {
        If (-not [System.IO.File]::Exists($DestinationPath)) {
            Write-Output "Downloading PHP PSR: ${RemoteUrl} ..."
            DownloadFile $RemoteUrl $DestinationPath
        }

        Expand-Item7zip $DestinationPath "${Env:PHP_PATH}\ext"
    }
}

Function TuneUpPhp {
    $IniFile = "${Env:PHP_PATH}\php.ini"
    $ExtPath = "${Env:PHP_PATH}\ext"

    If (-not [System.IO.File]::Exists($IniFile)) {
        Throw "Unable to locate $IniFile file"
    }

    Write-Output ""                                  | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension_dir = ${ExtPath}"        | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "memory_limit = 256M"               | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "variables_order = EGPCS"           | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output ""                                  | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_curl.dll"          | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_openssl.dll"       | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_mbstring.dll"      | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_pdo_sqlite.dll"    | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_fileinfo.dll"      | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_gettext.dll"       | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_gd2.dll"           | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_gmp.dll"           | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_zephir_parser.dll" | Out-File -Encoding "ASCII" -Append $IniFile
    Write-Output "extension = php_psr.dll"           | Out-File -Encoding "ASCII" -Append $IniFile
}

Function InitializeBuildVars {
    switch ($Env:VC_VERSION) {
        '14' {
            If (-not (Test-Path $Env:VS120COMNTOOLS)) {
                Throw'The VS120COMNTOOLS environment variable is not set. Check your MS VS installation'
            }
            $Env:VSCOMNTOOLS = $Env:VS120COMNTOOLS -replace '\\$', ''

            break
        }
        '15' {
            If (-not (Test-Path $Env:VS140COMNTOOLS)) {
                Throw'The VS140COMNTOOLS environment variable is not set. Check your MS VS installation'
            }
            $Env:VSCOMNTOOLS = $Env:VS140COMNTOOLS -replace '\\$', ''
            break
        }
        default {
            Throw'This script is designed to run with MS VS 14/15. Check your MS VS installation'
            break
        }
    }

    If ($Env:PLATFORM -eq 'x64') {
        $Env:ARCH = 'x86_amd64'
    } Else {
        $Env:ARCH = 'x86'
    }
}

Function InitializeReleaseVars {
    If ($Env:BUILD_TYPE -Match "nts-Win32") {
        $Env:RELEASE_ZIPBALL = "${Env:PACKAGE_PREFIX}_${Env:PLATFORM}_vc${Env:VC_VERSION}_php${Env:PHP_MINOR}_${Env:APPVEYOR_BUILD_VERSION}_nts"

        If ($Env:PLATFORM -eq 'x86') {
            $Env:RELEASE_FOLDER = "Release"
        } Else {
            $Env:RELEASE_FOLDER = "x64\Release"
        }
    } Else {
        $Env:RELEASE_ZIPBALL = "${Env:PACKAGE_PREFIX}_${Env:PLATFORM}_vc${Env:VC_VERSION}_php${Env:PHP_MINOR}_${Env:APPVEYOR_BUILD_VERSION}"

        If ($Env:PLATFORM -eq 'x86') {
            $Env:RELEASE_FOLDER = "Release_TS"
        } Else {
            $Env:RELEASE_FOLDER = "x64\Release_TS"
        }
    }

    $Env:RELEASE_DLL_PATH = "${Env:APPVEYOR_BUILD_FOLDER}\ext\${Env:RELEASE_FOLDER}\${Env:EXTENSION_FILE}"
}

Function InstallBuildDependencies {
    EnsureChocolateyIsInstalled

    $InstallProcess = Start-Process "choco" -WindowStyle Hidden -ArgumentList 'install', '-y --cache-location=C:\Downloads\Choco pandoc' -WorkingDirectory "${Env:APPVEYOR_BUILD_FOLDER}"

    If (-not (Test-Path "${Env:APPVEYOR_BUILD_FOLDER}\package")) {
        New-Item -ItemType Directory -Force -Path "${Env:APPVEYOR_BUILD_FOLDER}\package" | Out-Null
    }

    $BuildFile = "${Env:APPVEYOR_BUILD_FOLDER}\install-php-deps.bat"

    If (-not (Test-Path "${Env:APPVEYOR_BUILD_FOLDER}\vendor")) {
        $Php = "${Env:PHP_PATH}\php.exe"
        $ComposerOptions = "--quiet --no-interaction --no-progress --optimize-autoloader --prefer-dist --no-suggest --ignore-platform-reqs"

        Write-Output "@echo off" | Out-File -Encoding "ASCII" -Append $BuildFile
        Write-Output "${Php} -r `"readfile('https://getcomposer.org/installer');`" | ${Env:PHP_PATH}\php.exe" | Out-File -Encoding "ASCII" -Append $BuildFile
        Write-Output "${Php} ${Env:APPVEYOR_BUILD_FOLDER}\composer.phar install ${ComposerOptions}" | Out-File -Encoding "ASCII" -Append $BuildFile

        Set-Location "${Env:APPVEYOR_BUILD_FOLDER}"
        & cmd /c ".\install-php-deps.bat"
    }
}

Function EnableExtension {
    If (-not (Test-Path "${Env:RELEASE_DLL_PATH}")) {
        Throw "Unable to locate extension path: ${Env:RELEASE_DLL_PATH}"
    }

    Copy-Item "${Env:RELEASE_DLL_PATH}" "${Env:PHP_PATH}\ext\${Env:EXTENSION_FILE}"
    Write-Output "extension = ${Env:EXTENSION_FILE}" | Out-File -Encoding "ASCII" -Append "${Env:PHP_PATH}\php.ini"

    $IniFile = "${Env:PHP_PATH}\php.ini"
    $PhpExe  = "${Env:PHP_PATH}\php.exe"

    If (-not [System.IO.File]::Exists($IniFile)) {
        Throw "Unable to locate ${IniFile}"
    }

    If (Test-Path -Path "${PhpExe}") {
        & "${PhpExe}" --ri "${Env:EXTENSION_NAME}"

        $PhpExitCode = $LASTEXITCODE
        If ($PhpExitCode -ne 0) {
            PrintPhpInfo
            Throw "An error occurred while enabling [${Env:EXTENSION_NAME}] in [$IniFile]. PHP Exit Code was [$PhpExitCode]."
        }
    }
}

Function PrintBuildDetails {
    $BuildDate = Get-Date -Format g

    Write-Output "Build date: ${BuildDate}"
    Write-Output "Build version: ${Env:APPVEYOR_BUILD_VERSION}"
    Write-Output "Build type: ${Env:BUILD_TYPE}"
    Write-Output "Build worker image: ${Env:APPVEYOR_BUILD_WORKER_IMAGE}"
    Write-Output "Git commit: ${Env:APPVEYOR_REPO_COMMIT}"
    Write-Output "Platform: ${Env:PLATFORM}"
    Write-Output "Target PHP version: ${Env:PHP_MINOR}"
}

Function PrintLogs {
    If (Test-Path -Path "${Env:APPVEYOR_BUILD_FOLDER}\compile-errors.log") {
        Get-Content -Path "${Env:APPVEYOR_BUILD_FOLDER}\compile-errors.log"
    }

    If (Test-Path -Path "${Env:APPVEYOR_BUILD_FOLDER}\compile.log") {
        Get-Content -Path "${Env:APPVEYOR_BUILD_FOLDER}\compile.log"
    }

    If (Test-Path -Path "${Env:APPVEYOR_BUILD_FOLDER}\ext\configure.js") {
        Get-Content -Path "${Env:APPVEYOR_BUILD_FOLDER}\ext\configure.js"
    }
}

Function PrintVars {
    Write-Output ($Env:Path).Replace(';', "`n")

    Get-ChildItem Env:
}

Function PrintDirectoriesContent {
    Get-ChildItem -Path "${Env:APPVEYOR_BUILD_FOLDER}"

    If (Test-Path -Path "C:\Downloads") {
        Get-ChildItem -Path "C:\Downloads"
    }

    If (Test-Path -Path "C:\Projects") {
        Get-ChildItem -Path "C:\Projects"
    }

    If (Test-Path -Path "${Env:PHP_PATH}\ext") {
        Get-ChildItem -Path "${Env:PHP_PATH}\ext"
    }

    $ReleasePath = Split-Path -Path "${Env:RELEASE_DLL_PATH}"
    If (Test-Path -Path "${ReleasePath}") {
        Get-ChildItem -Path "${ReleasePath}"
    }

    $BuildPath = Split-Path -Path "${ReleasePath}"
    If (Test-Path -Path "${BuildPath}") {
        Get-ChildItem -Path "${BuildPath}"
    }
}

Function PrintPhpInfo {
    $IniFile = "${Env:PHP_PATH}\php.ini"
    $PhpExe = "${Env:PHP_PATH}\php.exe"

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
    $WebClient.Headers.Add('User-Agent', 'AppVeyor PowerShell Script')

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
