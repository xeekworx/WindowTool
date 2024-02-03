<#
.SYNOPSIS
Signs a binary file with a digital certificate.

.DESCRIPTION
This script signs a specified binary file using a digital certificate. The certificate can be provided either as a file path or as a Base64-encoded string. A password for the certificate is required.

.PARAMETER CertPath
The file path to the digital certificate (.pfx file). If this parameter is not provided and CertBase64 is used, a temporary certificate file will be created.

.PARAMETER CertBase64
The Base64-encoded string of the digital certificate. If provided, it will be decoded and saved as a temporary .pfx file for signing. This parameter is ignored if CertPath is provided.

.PARAMETER CertPassword
The password for the digital certificate's private key. This parameter is mandatory and must be provided as a SecureString.

.PARAMETER BinaryPath
The file path to the binary that will be signed. This parameter is mandatory.

.EXAMPLE
PS> .\SignCode.ps1 -CertPath "C:\path\to\your\certificate.pfx" -CertPassword (ConvertTo-SecureString -String "YourPassword" -AsPlainText -Force) -BinaryPath "C:\path\to\your\binary.exe"

Signs the binary "binary.exe" using the certificate specified at "certificate.pfx".

.EXAMPLE
PS> $certBase64 = Get-Content "C:\path\to\your\certificateBase64.txt" -Raw
PS> .\SignCode.ps1 -CertBase64 $certBase64 -CertPassword (ConvertTo-SecureString -String "YourPassword" -AsPlainText -Force) -BinaryPath "C:\path\to\your\binary.exe"

Signs the binary "binary.exe" using a Base64-encoded digital certificate read from a text file.

.NOTES
This script uses signtool.exe, which must be available in your system's PATH. The script assumes the use of SHA256 for signing and timestamps the signature using http://timestamp.digicert.com.

#>

param(
    [Parameter(Mandatory=$false)]
    [string]$CertPath,

    [Parameter(Mandatory=$false)]
    [string]$CertBase64,

    [Parameter(Mandatory=$true)]
    [System.Security.SecureString]$CertPassword,

    [Parameter(Mandatory=$true)]
    [string]$BinaryPath
)

function Find-SignTool {
    $possiblePaths = @(
        "${env:ProgramFiles(x86)}\Windows Kits\10\bin\*\x64",
        "${env:ProgramFiles(x86)}\Windows Kits\10\bin\10.0.*\x64",
        "${env:ProgramFiles(x86)}\Microsoft SDKs\Windows\v7.1A\Bin",
        "${env:ProgramFiles}\Microsoft SDKs\Windows\v8.1A\Bin\x64"
    )

    foreach ($path in $possiblePaths) {
        $signtool = Get-ChildItem -Path $path -Filter signtool.exe -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($null -ne $signtool) {
            return $signtool.FullName
        }
    }

    throw "signtool.exe not found. Please ensure the Windows SDK is installed."
}

$signtoolPath = Find-SignTool

if ($CertBase64 -and !$CertPath) {
    $certContent = [System.Convert]::FromBase64String($CertBase64)
    $CertPath = "tempCert.pfx"
    [IO.File]::WriteAllBytes($CertPath, $certContent)
}

if (-not (Test-Path $CertPath)) {
    throw "Certificate path is invalid or certificate is not provided."
}

if (-not (Test-Path $BinaryPath)) {
    throw "Binary path is invalid."
}

$certPasswordPlainText = [Runtime.InteropServices.Marshal]::PtrToStringAuto([Runtime.InteropServices.Marshal]::SecureStringToBSTR($CertPassword))

& $signtoolPath sign /fd SHA256 /a /f $CertPath /p $certPasswordPlainText /tr http://timestamp.digicert.com /td SHA256 $BinaryPath

if ($LASTEXITCODE -ne 0) {
    throw "Signing failed with exit code $LASTEXITCODE."
}
