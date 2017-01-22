#!/bin/sh

echo . = `../saruman/objinfo --to-kuseg --highest-vaddr --round-to-page 4096 ../kernel/kernel.elf`\; > base_vaddr.lds

