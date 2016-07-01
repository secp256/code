#!/bin/sh

# fetch background picture of cn.bing.com
file_path="/home/zhuliting/KuaiPan/src/code/shell/bing"
mkdir -p $file_path

url="http://cn.bing.com/"
user_agent="Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.109 Safari/537.36"

cur_date=$(date +%F)
html_file=${file_path}/${cur_date}.html
log_file=${file_path}/${cur_date}.log
img_file=${file_path}/${cur_date}.jpg

wget "$url" -U "$user_agent" -O ${html_file} 1>${log_file} 2>&1
img_url=$(grep -oP 'g_img={url:\s"([^"]+)"' ${html_file} | sed 's/g_img.*\/\///' | sed 's/\"//')
img_url="http://"${img_url}
echo $img_url >>${log_file}
wget $img_url -U "$user_agent" -O ${img_file} 1>>${log_file} 2>&1
