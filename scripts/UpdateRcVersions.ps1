# Updates the versions in /source/resource.rc based on the version in version.txt

$scriptDir = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent
$versionFilePath = Join-Path -Path $scriptDir -ChildPath '..\version.txt'
$resourceFilePath = Join-Path -Path $scriptDir -ChildPath '..\source\resources.rc'

$version = Get-Content $versionFilePath | Select-Object -First 1
$versionParts = $version -split '\.'

if ($versionParts.Length -ne 3) {
    Write-Error "Version '$version' is not in expected format 'X.Y.Z'"
    Exit 1
}

$major = $versionParts[0]
$minor = $versionParts[1]
$patch = $versionParts[2]
$build = '0'  # Assuming build number is 0

$versionCommaText = "$major,$minor,$patch,$build"
$versionDotText = "$major.$minor.$patch.$build"

$content = Get-Content $resourceFilePath -Raw

# Regex to match the version pattern in "1,2,3,4" form
$content = $content -creplace '\d+,\d+,\d+,\d+', $versionCommaText
# Regex to match the version pattern in "1.2.3.4" form
$content = $content -creplace '\d+\.\d+\.\d+\.\d+', $versionDotText

$content | Out-File -FilePath $resourceFilePath -Encoding UTF8 -NoNewline
Write-Host "Updated $resourceFilePath with version $versionCommaText and $versionDotText"
