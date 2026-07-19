$ErrorActionPreference = "Stop"

Set-Location (Join-Path $PSScriptRoot "..")

doxygen Doxyfile

$index = Join-Path $PWD "docs\generated\html\index.html"

if (-not (Test-Path $index)) {
    throw "Doxygen completed, but index.html was not generated."
}

Start-Process $index
