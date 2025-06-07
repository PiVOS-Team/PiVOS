#!/bin/bash

parse_args() {
  declare -n flags=$1
  declare -n opts=$2
  shift 2

  while (( "$#" )); do
    case "$1" in
      --*=*)
        key="${1%%=*}"
        val="${1#*=}"
        opts["$key"]="$val"
        shift
        ;;
      --*)
        flags["$1"]=true
        shift
        ;;
      -*)
        arg="${1#-}"
        for ((i=0; i<${#arg}; i++)); do
          flags["-${arg:i:1}"]=true
        done
        shift
        ;;
    esac
  done
}