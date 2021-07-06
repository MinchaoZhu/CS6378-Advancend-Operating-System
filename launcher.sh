#!/bin/bash

id="$1"
is_active="$2"
conf_path="$3"
snapshots="$4"

./build/Main $id $is_active $conf_path $snapshots