#!/bin/sh
cd src
qmake
make
cd ..
rm -rf cnchess.app
mv bin/cnchess.app .
mkdir cnchess.app/Contents/Resources
cp -r chesses cnchess.app/Contents/Resources
cp -r book cnchess.app/Contents/Resources
cp cnchess.icns cnchess.app/Contents/Resources
cp -f Info.plist cnchess.app/Contents
cp src/*.qm cnchess.app/Contents/Resources
mkdir cnchess.app/Contents/Resources/zh_CN.lproj
cp InfoPlist.strings cnchess.app/Contents/Resources/zh_CN.lproj
mkdir cnchess.app/Contents/Resources/English.lproj
cp InfoPlist.English cnchess.app/Contents/Resources/English.lproj/InfoPlist.strings
mkdir cnchess.app/Contents/Frameworks
/bin/cp $QTDIR/lib/libqt-mt.3.dylib cnchess.app/Contents/Frameworks
install_name_tool -change libqt-mt.3.dylib @executable_path/../Frameworks/libqt-mt.3.dylib cnchess.app/Contents/MacOS/cnchess
