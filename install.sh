# Install OLED libraries under the lib directory
# Run this script under the project root

wget https://github.com/patricklam/se101-f16/raw/master/lab/resources/Orbit_Oled_Library.zip
unzip Orbit_Oled_Library.zip
mkdir lib
mv OrbitOled/* lib/ 
rm Orbit_Oled_Library.zip
rm -r OrbitOled
