#!/bin/bash

#开始时间
begin=$(date +%s)

#测试根目录
root_dir="/tmp/test"

if [ ! -d $root_dir ]; then
    mkdir -p $root_dir
fi
mkdir -p root_dir
cd $root_dir


#循环创建10000个目录
for ((i=0; i<10000; ))
do
    mkdir -p $i
    i=$(expr $i + 1)
done

#结束时间
end=$(date +%s)
spend=$(expr $end - $begin)
echo "花费时间为$spend秒"
