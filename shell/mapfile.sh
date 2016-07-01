#!/bin/bash

# 将一个文本文件直接变成一个数组，每行作为数组的一个元素。这对某些程序的处理是很方便的。尤其是当你要对某些文件进行全文的分析或者处理的时候，比一行一行读进来处理方便的多。
exec 3< /etc/passwd
mapfile -u 3 passwd 
exec 3<&-

echo ${#passwd}
for ((i=0;i<${#passwd};i++))
do
    echo ${passwd[$i]}
    echo
done
