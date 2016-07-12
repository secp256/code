#!/bin/sh

# check and start eyerest-daemon
# */5 * * * * bash /home/xxxx/start_eyerest_daemon.sh

log="/home/zhuliting/KuaiPan/src/code/shell/eyerest.log"
pid=$(pgrep eyerest-daemon)
if [ ${pid:-0} -gt 0 ] ; then
    echo "yes" > ${log}
else
    bash -c /usr/local/bin/eyerest-daemon 1>${log} 2>&1
fi
