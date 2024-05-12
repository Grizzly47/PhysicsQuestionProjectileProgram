@echo off

echo Downloading SFML...
set sfml_version=SFML-2.6.1
set sfml_url=https://www.sfml-dev.org/files/SFML-2.6.1-windows-vc17-64-bit.zip
curl -L -o SFML.zip %sfml_url%
echo Extracting SFML...
tar -xf SFML.zip
move %sfml_version% SFML
del SFML.zip
echo COMPLETE: SFML downloaded and extracted
pause
