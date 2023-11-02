#$files = Get-Item *.png
#Get-Item *.png

# 現在のディレクトリを取得
$currentDirectory = Get-Location
$currentDirectoryPath = $currentDirectory.Path
function CurrentDirectoryRecursive($filePath) 
{
    # 現在のディレクトリ内のフォルダを取得
    #$folders = Get-ChildItem -Path $filePath -Directory
    
    # ファイルパスからディレクトリパスを取得
    $directoryPath = [System.IO.Path]::GetDirectoryName($filePath)
    Write-Host "DirectoryPath : " $directoryPath

    ## フォルダ分再帰する
    #foreach ($f in $folders)
    #{
    #    CurrentDirectoryRecursive($f.Name)
#
    #    #Write-Host "DirectoryPath : "
    #    Write-Host "Folder Name : " $f.Name "`n"
    #}
#
#    # 現在のディレクトリ内の .png ファイルを取得
#    $pngFiles = Get-ChildItem -Path $directoryPath -Filter *.png -File
#
#    foreach ($f in $pngFiles)
#    {
#        Write-Host "png Name : "
#        Write-Host $f.Name
#
#        ## ミップレベル
#        #$mipLevel = 2  
#    #
#        #$arguments = @(
#        #    $f.FullName,
#        #    "-ml", $mipLevel
#        #    # 他の引数を追加できます
#        #)
#       #
#        #Start-Process -FilePath $currentDirectoryPath.Name + "TextureConverter.exe" -ArgumentList $arguments -Wait
#    }
}

Write-Host "Current DirectoryPath : "
Write-Host $currentDirectoryPath

CurrentDirectoryRecursive($currentDirectoryPath)

Pause