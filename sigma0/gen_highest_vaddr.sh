#!/bin/sh

# Expect CMAKE_BINARY_DIR as arg1

DIR=`dirname $0`
SARUMAN=${DIR}/../saruman
KERNEL=$1/kernel/kernel.elf
BASE_VADDR_LDS=$1/sigma0/base_vaddr.lds

echo . = `${SARUMAN}/objinfo --to-kuseg --highest-vaddr --round-to-page 4096 ${KERNEL}`\; > ${BASE_VADDR_LDS}

