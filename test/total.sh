#!/usr/bin/bash

set -e

pattern='\d+ 0+(\d+).*'
lengths=$(rg --pre ./pre.sh "$pattern" -r '$1' test.elf)
total=0
for i in $lengths;do
  echo $i
  total=$(( $total + $i ))
done
echo "total: $total"
