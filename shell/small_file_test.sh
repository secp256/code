#!/bin/bash

local_dir="./small_file"
remote_dir="small_file"
bucket="google-test"
# MB
total_file_size=$(du -lh ${local_dir} | awk -F' ' '{print $1}' | grep -oP '\d+')
echo $total_file_size

gsutil -q -m rm -r gs://${bucket}/${remote_dir}

start_time=$(date +%s)
#gsutil -q cp "$local_dir"/* gs://${bucket}/"$remote_dir"
gsutil -q -m cp "$local_dir"/* gs://${bucket}/"$remote_dir"
end_time=$(date +%s)

total_time=$((end_time-start_time))
throughput=$(awk -v a=$total_file_size -v b=$total_time 'BEGIN{printf "%.1f", a/b}')

echo $(grep 'parallel_process_count = ' /home/zhu_liting/.boto)
echo $(grep 'parallel_thread_count = ' /home/zhu_liting/.boto)

echo "=====================upload total last :  + ${total_time} + ms====================="
echo "=====================upload totalFileSize :  + ${total_file_size} + MB====================="
echo "throughput: " $throughput " MB/S"
