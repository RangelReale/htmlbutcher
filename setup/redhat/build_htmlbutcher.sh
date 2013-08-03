#!/bin/sh

if [ "$1" == "" ]; then
	echo "Release name parameter is required"
	exit 1
fi

# create paths
BUILDROOT_HTMLBUTCHER=/var/tmp/htmlbutcher-buildroot
BUILDROOT_HTMLBUTCHERDEMO=/var/tmp/htmlbutcher-demo-buildroot

rm -Rf $BUILDROOT_HTMLBUTCHER
rm -Rf $BUILDROOT_HTMLBUTCHERDEMO

mkdir -p $BUILDROOT_HTMLBUTCHER/usr/bin
mkdir -p $BUILDROOT_HTMLBUTCHER/usr/share/htmlbutcher
mkdir -p $BUILDROOT_HTMLBUTCHER/usr/share/pixmaps
mkdir -p $BUILDROOT_HTMLBUTCHER/usr/share/applications
mkdir -p $BUILDROOT_HTMLBUTCHER/usr/share/application-registry
mkdir -p $BUILDROOT_HTMLBUTCHER/usr/share/mime-info
mkdir -p $BUILDROOT_HTMLBUTCHER/usr/share/mime/packages/

mkdir -p $BUILDROOT_HTMLBUTCHERDEMO/usr/bin
mkdir -p $BUILDROOT_HTMLBUTCHERDEMO/usr/share/htmlbutcher-demo
mkdir -p $BUILDROOT_HTMLBUTCHERDEMO/usr/share/pixmaps
mkdir -p $BUILDROOT_HTMLBUTCHERDEMO/usr/share/applications

# copy executables
cp ../../bin/Release/HTMLButcher $BUILDROOT_HTMLBUTCHER/usr/bin/htmlbutcher
cp ../../resources/htmlbutcher.xpm $BUILDROOT_HTMLBUTCHER/usr/share/pixmaps
cp ../../data/htmlbutcher.desktop $BUILDROOT_HTMLBUTCHER/usr/share/applications
cp ../../data/htmlbutcher.applications $BUILDROOT_HTMLBUTCHER/usr/share/application-registry
cp ../../data/htmlbutcher.keys $BUILDROOT_HTMLBUTCHER/usr/share/mime-info
cp ../../data/htmlbutcher.mime $BUILDROOT_HTMLBUTCHER/usr/share/mime-info
cp ../../data/htmlbutcher-xgd.xml $BUILDROOT_HTMLBUTCHER/usr/share/mime/packages/htmlbutcher.xml
cp ../../doc/docbook/htmlbutcher.htb $BUILDROOT_HTMLBUTCHER/usr/share/htmlbutcher

cp ../../bin/DEMORelease/HTMLButcher $BUILDROOT_HTMLBUTCHERDEMO/usr/bin/htmlbutcher-demo
cp ../../resources/htmlbutcher.xpm $BUILDROOT_HTMLBUTCHERDEMO/usr/share/pixmaps/htmlbutcher-demo.xpm
cp ../../data/htmlbutcher-demo.desktop $BUILDROOT_HTMLBUTCHERDEMO/usr/share/applications
cp ../../doc/docbook/htmlbutcher.htb $BUILDROOT_HTMLBUTCHERDEMO/usr/share/htmlbutcher-demo

rpmbuild -ba $1/htmlbutcher.spec
rpmbuild -ba $1/htmlbutcher-demo.spec
