if (Test-Path .\packages) {
    Remove-Item -Recurse -Force .\packages\vcpkg
}
else {
    New-Item -ItemType Directory -Path .\packages
}

#IMPORTANT: change dl=0 to dl=1 
$URL = "https://www.dropbox.com/s/lomvz29ndcqaahw/vcpkg-export-20210110-193925.zip?dl=1";
Invoke-WebRequest -Uri $URL  -OutFile .\packages\vcpkg.zip      
Expand-Archive -Path .\packages\vcpkg.zip  -DestinationPath .\packages
Remove-Item -Path .\packages\vcpkg.zip
$dirName = Get-ChildItem -Path .\packages\ -Directory | Select-Object -ExpandProperty Name 
Rename-Item -Path ".\packages\$dirName" -NewName "vcpkg"


