# Init project
# Install OLED libraries under the lib directory
# Run this script under the project root

platformio init
rm .travis.yml
wget https://github.com/patricklam/se101-f16/raw/master/lab/resources/Orbit_Oled_Library.zip
unzip Orbit_Oled_Library.zip
mkdir -p lib/
mv OrbitOled/ lib/
rm Orbit_Oled_Library.zip
