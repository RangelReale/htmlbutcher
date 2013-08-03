#!/bin/sh

for file in *.po; do
  BN=${file%.po}
  msgfmt -o $BN.mo $file
  mkdir -p $BN
  cp $BN.mo $BN/htmlbutcher.mo
done

exit 0

