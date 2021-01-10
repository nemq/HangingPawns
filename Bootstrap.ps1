New-Item -ItemType Directory -Path .\packages
#IMPORTANT: change dl=0 to dl=1 
$URL = "https://www.dropbox.com/s/g847uh50hgzafve/v0.0.0.zip?dl=1";
Invoke-WebRequest -Uri $URL  -OutFile .\packages\gtest.zip      
Expand-Archive -Path .\packages\gtest.zip  -DestinationPath .\packages
Remove-Item -Path .\packages\gtest.zip

