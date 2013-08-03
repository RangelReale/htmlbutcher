#!/bin/sh

# create paths
rm -Rf build

mkdir -p build/htmlbutcher/bin
mkdir -p build/htmlbutcher/lib
mkdir -p build/htmlbutcher/share/htmlbutcher
mkdir -p build/htmlbutcher/share/pixmaps
mkdir -p build/htmlbutcher/share/applications
mkdir -p build/htmlbutcher/share/application-registry
mkdir -p build/htmlbutcher/share/mime-info
mkdir -p build/htmlbutcher/share/mime/packages/
mkdir -p build/htmlbutcher/share/locale

mkdir -p build/htmlbutcher-demo/bin
mkdir -p build/htmlbutcher-demo/lib
mkdir -p build/htmlbutcher-demo/share/htmlbutcher-demo
mkdir -p build/htmlbutcher-demo/share/pixmaps
mkdir -p build/htmlbutcher-demo/share/applications
mkdir -p build/htmlbutcher-demo/share/locale

# copy executables
cp ../../bin/HTMLButcher build/htmlbutcher/bin/htmlbutcher
cp ../../resources/htmlbutcher.xpm build/htmlbutcher/share/pixmaps
cp ../../resources/avatar.png build/htmlbutcher/share/pixmaps/htmlbutcher-icon.png
cp ../../data/htmlbutcher.desktop build/htmlbutcher/share/applications
cp ../../data/htmlbutcher.applications build/htmlbutcher/share/application-registry
cp ../../data/htmlbutcher.keys build/htmlbutcher/share/mime-info
cp ../../data/htmlbutcher.mime build/htmlbutcher/share/mime-info
cp ../../data/htmlbutcher-xgd.xml build/htmlbutcher/share/mime/packages/htmlbutcher.xml
cp ../../doc/docbook/htmlbutcher.htb build/htmlbutcher/share/htmlbutcher
cp htmlbutcher/PXML.xml build/htmlbutcher
cp htmlbutcher/run.sh build/htmlbutcher/bin
chmod +x build/htmlbutcher/bin/run.sh

cp $SDK_PATH_TARGET/usr/lib/libwx*.so.0 build/htmlbutcher/lib
cp $SDK_PATH_TARGET/usr/lib/libfreeimage.so.3 build/htmlbutcher/lib

cp ../../bin/HTMLButcherDemo build/htmlbutcher-demo/bin/htmlbutcher-demo
cp ../../resources/htmlbutcher.xpm build/htmlbutcher-demo/share/pixmaps/htmlbutcher-demo.xpm
cp ../../resources/avatar.png build/htmlbutcher-demo/share/pixmaps/htmlbutcher-icon.png
cp ../../data/htmlbutcher-demo.desktop build/htmlbutcher-demo/share/applications
cp ../../doc/docbook/htmlbutcher.htb build/htmlbutcher-demo/share/htmlbutcher-demo
cp htmlbutcher-demo/PXML.xml build/htmlbutcher-demo
cp htmlbutcher-demo/run.sh build/htmlbutcher-demo/bin
chmod +x build/htmlbutcher-demo/bin/run.sh

cp $SDK_PATH_TARGET/usr/lib/libwx*.so.0 build/htmlbutcher-demo/lib
cp $SDK_PATH_TARGET/usr/lib/libfreeimage.so.3 build/htmlbutcher-demo/lib

# locale files
for file in ../../locale/*.po; do
  FBN=`basename $file`
  BN=${FBN%.po}
  D1=build/htmlbutcher/share/locale/$BN/LC_MESSAGES
  D2=build/htmlbutcher-demo/share/locale/$BN/LC_MESSAGES
  mkdir -p $D1
  mkdir -p $D2
  msgfmt -o $D1/htmlbutcher.mo $file
  msgfmt -o $D2/htmlbutcher-demo.mo $file
done


# remove old packages
rm -f htmlbutcher*.pnd

pnd_make.sh -p htmlbutcher_1.1.0.25.pnd -d build/htmlbutcher -x build/htmlbutcher/PXML.xml -i build/htmlbutcher/share/pixmaps/htmlbutcher-icon.png

pnd_make.sh -p htmlbutcher-demo_1.1.0.25.pnd -d build/htmlbutcher-demo -x build/htmlbutcher-demo/PXML.xml -i build/htmlbutcher-demo/share/pixmaps/htmlbutcher-icon.png

