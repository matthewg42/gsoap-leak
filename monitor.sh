#!/bin/bash

get_pids () {
    declare -a pids=()
    pids+=($(pgrep client_calc))
    pids+=($(pgrep server_calc))
    echo ${pids[@]} | tr ' ' ','
}

while true; do 
    ps u -p $(get_pids) 2> /dev/null |sed -n '2,$ p'
    sleep 1
done

