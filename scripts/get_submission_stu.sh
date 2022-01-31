#!/bin/bash 

####################################################################
# Summary: Getting tagged submission from git repository
# Author : yqhuang@uwaterloo.ca
# Date   : Jan 2022
####################################################################

if [ $# -ne 2 ]; then
    echo "Usage: $0 <group id> <project id>"
    echo "Example: $0 99 1"
    exit 1
fi


GROUP_ID="$1"
PROJ_ID="$2"
CWD=`pwd`

BASE_URL="https://git.uwaterloo.ca/se350-winter2022/"
REPO_PRFIX="se350-winter2022-lab-g"
REPO_DIR="${REPO_PRFIX}${GROUP_ID}"
REPO_URL="${BASE_URL}${REPO_DIR}.git"

SUBMIT_TAG="p${PROJ_ID}-submit"
LOG="${CWD}/g${GROUP_ID}-${SUBMIT_TAG}.txt"
rm -rf "$LOG"
touch "$LOG"
date | tee -a "$LOG"

git clone --depth 1 --branch "${SUBMIT_TAG}" "${REPO_URL}"  2>&1 |tee -a "$LOG"
cd "${REPO_DIR}"

git log -1 --format=%ai "${SUBMIT_TAG}" | tee -a  "$LOG"
mv "$LOG" .
