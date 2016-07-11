#!/bin/sh

# fetch background picture of cn.bing.com
file_path="/home/zhuliting/KuaiPan/src/code/shell/bing_history"
mkdir -p $file_path

user_agent="Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36"
nc=$(awk -v a=$(date --date='1 days ago' +%s%N) -v b=1000000 'BEGIN{printf "%.0f", a/b}')

start=1
step=20

count=1
for idx in `seq ${start} 20`
do
    url="http://cn.bing.com/HPImageArchive.aspx?format=js&idx=${idx}&n=1&nc=${nc}&pid=hp&video=1"
    html_file=${file_path}/${idx}.html
    log_file=${file_path}/${idx}.log
    img_file=${file_path}/${idx}.jpg

    wget "$url" -U "$user_agent" -O ${html_file} 1>${log_file} 2>&1
    img_url=$(grep -oP '"url":"([^"]+)' ${html_file} | sed -e 's/.*http:\/\///')
    img_url="http://"${img_url}
    echo $img_url >>${log_file}
    wget $img_url -U "$user_agent" -d -O ${img_file} 1>>${log_file} 2>&1

    count=$((count+1))
    if [ $count -gt $step] ; then
        break
    fi
done
