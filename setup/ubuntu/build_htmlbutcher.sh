#!/bin/sh

# create paths
rm -Rf build

mkdir -p build/htmlbutcher/DEBIAN
mkdir -p build/htmlbutcher/usr/bin
mkdir -p build/htmlbutcher/usr/share/htmlbutcher
mkdir -p build/htmlbutcher/usr/share/pixmaps
mkdir -p build/htmlbutcher/usr/share/applications
mkdir -p build/htmlbutcher/usr/share/application-registry
mkdir -p build/htmlbutcher/usr/share/mime-info
mkdir -p build/htmlbutcher/usr/share/mime/packages/
mkdir -p build/htmlbutcher/usr/share/locale
cp htmlbutcher/DEBIAN/control build/htmlbutcher/DEBIAN
cp htmlbutcher/DEBIAN/postinst build/htmlbutcher/DEBIAN
cp htmlbutcher/DEBIAN/postrm build/htmlbutcher/DEBIAN
chmod 0555 build/htmlbutcher/DEBIAN/post*

mkdir -p build/htmlbutcher-demo/DEBIAN
mkdir -p build/htmlbutcher-demo/usr/bin
mkdir -p build/htmlbutcher-demo/usr/share/htmlbutcher-demo
mkdir -p build/htmlbutcher-demo/usr/share/pixmaps
mkdir -p build/htmlbutcher-demo/usr/share/applications
mkdir -p build/htmlbutcher-demo/usr/share/locale
cp htmlbutcher-demo/DEBIAN/control build/htmlbutcher-demo/DEBIAN

# copy executables
cp ../../bin/HTMLButcher build/htmlbutcher/usr/bin/htmlbutcher
cp ../../resources/htmlbutcher.xpm build/htmlbutcher/usr/share/pixmaps
cp ../../data/htmlbutcher.desktop build/htmlbutcher/usr/share/applications
cp ../../data/htmlbutcher.applications build/htmlbutcher/usr/share/application-registry
cp ../../data/htmlbutcher.keys build/htmlbutcher/usr/share/mime-info
cp ../../data/htmlbutcher.mime build/htmlbutcher/usr/share/mime-info
cp ../../data/htmlbutcher-xgd.xml build/htmlbutcher/usr/share/mime/packages/htmlbutcher.xml
cp ../../doc/docbook/htmlbutcher.htb build/htmlbutcher/usr/share/htmlbutcher

cp ../../bin/HTMLButcherDemo build/htmlbutcher-demo/usr/bin/htmlbutcher-demo
cp ../../resources/htmlbutcher.xpm build/htmlbutcher-demo/usr/share/pixmaps/htmlbutcher-demo.xpm
cp ../../data/htmlbutcher-demo.desktop build/htmlbutcher-demo/usr/share/applications
cp ../../doc/docbook/htmlbutcher.htb build/htmlbutcher-demo/usr/share/htmlbutcher-demo

# locale files
for file in ../../locale/*.po; do
  FBN=`basename $file`
  BN=${FBN%.po}
  D1=build/htmlbutcher/usr/share/locale/$BN/LC_MESSAGES
  D2=build/htmlbutcher-demo/usr/share/locale/$BN/LC_MESSAGES
  mkdir -p $D1
  mkdir -p $D2
  msgfmt -o $D1/htmlbutcher.mo $file
  msgfmt -o $D2/htmlbutcher-demo.mo $file
done


# set install size
INSTSIZE=`du -s build/htmlbutcher | awk '{ print ($1) }'`
IREP=s/%%SIZE%%/$INSTSIZE/g
cat htmlbutcher/DEBIAN/control | sed $IREP > build/htmlbutcher/DEBIAN/control

INSTSIZE=`du -s build/htmlbutcher-demo | awk '{ print ($1) }'`
IREP=s/%%SIZE%%/$INSTSIZE/g
cat htmlbutcher-demo/DEBIAN/control | sed $IREP > build/htmlbutcher-demo/DEBIAN/control


# remove old packages
rm -f htmlbutcher*.deb

dpkg-deb --build build/htmlbutcher .
dpkg-deb --build build/htmlbutcher-demo .

