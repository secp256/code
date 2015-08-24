#!/bin/bash

begin=$(date +%s)

root_dir="/tmp/test"

if [ ! -d $root_dir ]; then
    mkdir -p $root_dir
fi

mkdir -p root_dir
cd $root_dir

function create_dir()
{

    mkdir $1
}

count=10000
rsnum=20
cishu=$(expr $count / $rsnum)

for ((i=0; i<$cishu;))
do
    start_num=$(expr $i \* $rsnum + $i)
    end_num=$(expr $start_num + $rsnum)
    for j in `seq $start_num $end_num`
    do
        create_dir $j &
    done
    wait
    i=$(expr $i + 1)
done

end=$(date +%s)
spend=$(expr $end - $begin)
echo "花费时间为$spend秒"
