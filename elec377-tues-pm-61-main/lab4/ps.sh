#! /bin/bash

# ELEC377 - Operating Systems
# Lab 4 - Shell Scripting, ps.sh

# This script gathers information about processes from the /proc directory, such as PID, UID, GID, RSS, and name.
# It fetches the corresponding username and groupname from /etc/passwd and /etc/group.
# The gathered data is then formatted and sorted by process ID to ensure a clear and organized presentation.

# Usage: ./ps.sh [-rss] [-comm] [-command] [-group]
# Ex: ./ps.sh -rss -comm

showRSS="no"
showComm="no"
showCommand="no"
showGroup="no"

while(($# > 0))
do
    if [[ $1 == "-rss" ]];then

        showRSS="yes"
    
    elif [[ $1 == "-comm" ]];then
    
        showComm="yes"
    
    elif [[ $1 == "-command" ]];then
    
        showCommand="yes"
    
    elif  [[ $1 == "-group" ]];then
    
        showGroup="yes"
    
    else 
    
        echo 'flag not recognized'
        exit
    
    fi
    
    shift 

done

if [[ "$showComm" == "yes" && "$showCommand" == "yes" ]]; then
    echo 'error: command and comm cannot run simultaneously'
    exit
fi

for p in /proc/[0-9]*;do
    
    if [ -d $p ];then
        pid=`grep '^Pid' "$p/status" | sed -e 's/^Pid:\s*//'`
        uid=`grep '^Uid' "$p/status" | sed -e 's/^Uid:\s*\([0-9]*\).*/\1/'`
        gid=`grep '^Gid' "$p/status" | sed -e 's/^Gid:\s*\([0-9]*\).*/\1/'`
        rss=`grep '^VmRSS' "$p/status" | sed -e 's/^VmRSS:\s*//' -e 's/[^0-9]//g'`
        name=`grep '^Name' "$p/status" | sed -e 's/^Name:\s*//'`
        username=`cut -d : -f 1,3 /etc/passwd | grep -w "$uid" | cut -d : -f 1`
        groupname=`cut -d : -f 1,3 /etc/group | grep -w "$gid" | cut -d : -f 1`
        cmdline=$(cat "$p/cmdline" | tr -s '\0' ' ')
        
        if [ -z "$cmdline" ];then
           
            cmdline=$(cat "$p/comm")

        fi

        if [ -z "$rss" ];then
            
            rss=0

        fi

        printf "%-10s %-17s" "$pid" "$username" >> /tmp/tmpPs$$.txt

        if [ "$showGroup" == "yes" ];then
            
            printf "%-17s" "$groupname" >> /tmp/tmpPs$$.txt
        
        fi

        if [ "$showRSS" == "yes" ];then
        
            printf "%-10s " "$rss" >> /tmp/tmpPs$$.txt
        
        fi
        
        if [ "$showComm" == "yes" ];then
        
            printf "%-s" "$name" >> /tmp/tmpPs$$.txt
        
        fi
        
        if [ "$showCommand" == "yes" ];then
        
            printf "%-10s" "$cmdline" >> /tmp/tmpPs$$.txt
        
        fi
        
        printf "\n" >> /tmp/tmpPs$$.txt
    
    fi
done

printf "PID %-6s USER" >> /tmp/tmpPs$$.txt

if [ "$showGroup" == "yes" ];then

    printf "%-12s Group" >> /tmp/tmpPs$$.txt

fi

if [ "$showRSS" == "yes" ];then

    printf "%-11s RSS" >> /tmp/tmpPs$$.txt

fi

if [ "$showComm" == "yes" ];then

    printf "%-7s Command" >> /tmp/tmpPs$$.txt

fi

if [ "$showCommand" == "yes" ];then

    printf "%-12s CommandLine" >> /tmp/tmpPs$$.txt

fi

cat /tmp/tmpPs$$.txt | sort -n 
rm /tmp/tmpPs$$.txt

exit