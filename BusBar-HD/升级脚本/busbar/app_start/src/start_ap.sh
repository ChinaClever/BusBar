#! /bin/bash
# author Lzy
# time 2018-3-20 
# program : 判断进行是否存在，并重新启动
 
ps  | grep app | grep -v grep
if [ $? -ne 0 ]
then
	/opt/app &
fi
