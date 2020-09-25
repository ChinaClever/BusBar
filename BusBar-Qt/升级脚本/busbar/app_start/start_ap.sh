#! /bin/bash
# author Lzy
# time 2018-3-20 
 
ps  | grep app | grep -v grep
if [ $? -ne 0 ]
then
#	reboot
	/opt/app &
fi
