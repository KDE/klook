#!/bin/sh

NAME=klook
VER=1.1
DIR_NAME=$NAME-$VER

mkdir $DIR_NAME
cp -rf src locale icons README CMakeLists.txt $DIR_NAME
tar -czf $DIR_NAME.tar.gz $DIR_NAME
rm -rf $DIR_NAME
#cp -f $DIR_NAME.tar.gz ~/rpmbuild/SOURCES
rpmbuild -ba klook.spec
