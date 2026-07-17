<#
    dev-bootstrap-win.ps1 — one-time local build toolchain for Ansible Desktop (Windows).

    Installs (via winget): Visual Studio 2022 Community with the C++ desktop workload,
    Windows 11 SDK 10.0.26100.0, the VC x64 toolset, plus Strawberry Perl, Go and 7-Zip.
    Python and Git are assumed already installed. NASM is installed by prepare.py itself.

    Run in an ELEVATED (Administrator) PowerShell. Idempotent — re-running skips what's present.
    After it finishes, follow docs/dev-build-local.md from step 2.
#>

$ErrorActionPreference = 'Stop'

function Require-Admin {
    $id = [Security.Principal.WindowsIdentity]::GetCurrent()
    $p  = New-Object Security.Principal.WindowsPrincipal($id)
    if (-not $p.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
        Write-Error "Run this script from an ELEVATED (Administrator) PowerShell."
        exit 1
    }
}

function Have-Winget {
    if (-not (Get-Command winget -ErrorAction SilentlyContinue)) {
        Write-Error "winget not found. Install 'App Installer' from the Microsoft Store, then re-run."
        exit 1
    }
}

function Winget-Install($id, $extraArgs) {
    Write-Host "==> installing $id" -ForegroundColor Cyan
    $wargs = @('install','--id',$id,'-e','--accept-package-agreements','--accept-source-agreements','--disable-interactivity')
    if ($extraArgs) { $wargs += $extraArgs }
    winget @wargs
    if ($LASTEXITCODE -ne 0 -and $LASTEXITCODE -ne -1978335189) {
        # -1978335189 = "already installed / no applicable upgrade" — treat as OK.
        Write-Warning "winget exit $LASTEXITCODE for $id (continuing)."
    }
}

Require-Admin
Have-Winget

Write-Host "Python: $((Get-Command python -ErrorAction SilentlyContinue).Source)"
Write-Host "Git:    $((Get-Command git -ErrorAction SilentlyContinue).Source)"

# Visual Studio 2022 Community + C++ desktop workload + Win11 SDK 26100 + VC x64 tools.
# The --override string is passed verbatim to the VS installer.
$vsOverride = '--quiet --wait --norestart ' +
    '--add Microsoft.VisualStudio.Workload.NativeDesktop ' +
    '--add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ' +
    '--add Microsoft.VisualStudio.Component.Windows11SDK.26100 ' +
    '--includeRecommended'
Winget-Install 'Microsoft.VisualStudio.2022.Community' @('--override', $vsOverride)

# Extra tools the dependency build (prepare.py) expects.
Winget-Install 'StrawberryPerl.StrawberryPerl'
Winget-Install 'GoLang.Go'
Winget-Install '7zip.7zip'

Write-Host ""
Write-Host "Done. Next: open the x64 Native Tools prompt and follow docs/dev-build-local.md (step 2+)." -ForegroundColor Green
Write-Host '  "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" -vcvars_ver=14.44' -ForegroundColor Green
