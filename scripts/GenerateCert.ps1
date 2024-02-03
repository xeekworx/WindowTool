<#
.SYNOPSIS
Generates a self-signed Code Signing certificate.

.DESCRIPTION
This script generates a self-signed Code Signing certificate for the specified company name,
exports it to a PFX file, and then removes the certificate from the certificate store.

.PARAMETER CompanyName
The name of the company for which the certificate is being generated. This name is used for the Common Name (CN) and Organization (O) fields in the certificate subject.

.PARAMETER ExportPath
The file path where the PFX file containing the self-signed certificate will be exported. The default value is "cert.pfx".

.PARAMETER CertPassword
The password to secure the exported PFX file. This parameter is mandatory and must be provided as a SecureString.

.PARAMETER YearsValid
The number of years the certificate will be valid for. The default value is 2 years.

.EXAMPLE
$SecurePassword = ConvertTo-SecureString -String "P@ssw0rd" -AsPlainText -Force
.\GenerateCert.ps1 -CompanyName "ExampleCompany" -ExportPath ".\ExampleCert.pfx" -CertPassword $SecurePassword

This example generates a certificate for "ExampleCompany", exports it to ".\ExampleCert.pfx", using the password "P@ssw0rd".
#>

param(
    [Parameter(Mandatory=$true, HelpMessage="The name of the company for which the certificate is being generated. Used in the CN and O fields of the certificate subject.")]
    [ValidateNotNullOrEmpty()]
    [string]$CompanyName,
    
    [Parameter(HelpMessage="The file path where the PFX file will be exported. Default is 'cert.pfx'.")]
    [string]$ExportPath = "cert.pfx",
    
    [Parameter(Mandatory=$true, HelpMessage="The password for the exported PFX file. Must be a SecureString.")]
    [ValidateScript({
        if (([Runtime.InteropServices.Marshal]::PtrToStringAuto([Runtime.InteropServices.Marshal]::SecureStringToBSTR($_))).Length -eq 0) {
            throw "Certificate password must not be empty."
        }
        return $true
    })]
    [SecureString]$CertPassword,

    [ValidateRange(1, 10)]
    [int]$YearsValid = 2
)

# Check if ExportPath already exists and ask for overwrite confirmation
if (Test-Path $ExportPath) {
    $overwrite = Read-Host "File '$ExportPath' already exists. Do you want to overwrite it? (Y/N)"
    if ($overwrite -ne 'Y') {
        Write-Host "Operation cancelled by user."
        exit
    }
}

$certSubject = "CN=$CompanyName CA, O=$CompanyName, C=US"
$certStoreLocation = "Cert:\CurrentUser\My" 

# Generate a self-signed certificate
$cert = New-SelfSignedCertificate `
    -Type CodeSigningCert -Subject $certSubject `
    -TextExtension @("2.5.29.19={text}false") `
    -KeyUsage DigitalSignature `
    -KeyLength 2048 `
    -NotAfter (Get-Date).AddYears($YearsValid) `
    -CertStoreLocation $certStoreLocation `
    -KeyExportPolicy Exportable

# Export the certificate to a PFX file
$certPath = "$certStoreLocation\" + $cert.Thumbprint
Export-PfxCertificate -Cert $certPath -FilePath $ExportPath -Password $CertPassword

# Remove the certificate from the store after export
Get-ChildItem -Path $certPath | Remove-Item

Write-Host "Certificate generated and exported to $ExportPath"
