# Builds TRpsGame.tcd from TRPSGAMECHART, using the field order in
# CTChart::InitTRPS: WORD count, then for each row BYTE type, BYTE win,
# WORD need item, WORD item1, BYTE count1, WORD item2, BYTE count2, DWORD money.
# Usage: build_tcd.ps1 -Output <path> [-Database TGAME_GSP]
param(
    [Parameter(Mandatory = $true)][string]$Output,
    [string]$Database = 'TGAME_GSP'
)
$ErrorActionPreference = 'Stop'

$lines = sqlcmd -S 'localhost\FOURSTORY' -E -C -l 10 -d $Database -b -W -h -1 -s '|' -Q "SET NOCOUNT ON; SELECT bType,bWinCount,wItemID,wRewardItem_1,bItemCount_1,wRewardItem_2,bItemCount_2,dwRewardMoney FROM TRPSGAMECHART ORDER BY bType,bWinCount"
if ($LASTEXITCODE -ne 0) { throw "sqlcmd failed" }

$rows = @($lines | Where-Object { $_ -match '\|' })
if ($rows.Count -ne 48) { throw "Expected 48 rows, got $($rows.Count)" }

$ms = New-Object IO.MemoryStream
$w = New-Object IO.BinaryWriter($ms)
$w.Write([uint16]$rows.Count)
foreach ($r in $rows) {
    $p = $r.Split('|')
    if ($p.Count -ne 8) { throw "Bad row: $r" }
    $w.Write([byte]$p[0])
    $w.Write([byte]$p[1])
    $w.Write([uint16]$p[2])
    $w.Write([uint16]$p[3])
    $w.Write([byte]$p[4])
    $w.Write([uint16]$p[5])
    $w.Write([byte]$p[6])
    $w.Write([uint32]$p[7])
}
$w.Flush()
[IO.File]::WriteAllBytes($Output, $ms.ToArray())
"Wrote $Output ($($ms.Length) bytes, $($rows.Count) rows)"
