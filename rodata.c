/*
 *  linux/arch/arm/mm/rodata.c
 *
 *  Copyright (C) 2011 Google, Inc.
 *
 *  Author: Colin Cross <ccross@android.com>
 *
 *  Based on x86 implementation in arch/x86/mm/init_32.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>

#include <asm/cache.h>
#include <asm/pgtable.h>
#include <asm/rodata.h>
#include <asm/sections.h>
#include <asm/tlbflush.h>

#include "mm.h"

static int kernel_set_to_readonly __read_mostly;

static inline void rodata_test(void) { }


void set_kernel_text_rw(void)
{
  //0xc0008000 - 0xc0ad2e0c
	//unsigned long start = PAGE_ALIGN((unsigned long)_text);
	//unsigned long size = PAGE_ALIGN((unsigned long)__end_rodata) - start;
	unsigned long start = PAGE_ALIGN((unsigned long)0xc0008000);
	//unsigned long size = PAGE_ALIGN((unsigned long)0xc0ad2e0c) - start;
	unsigned long size = PAGE_ALIGN((unsigned long)0xc0adc000) - start;

	if (!kernel_set_to_readonly)
		return;

	pr_debug("Set kernel text: %lx - %lx to read-write\n",
		 start, start + size);

	set_memory_rw(start, size >> PAGE_SHIFT);
}

void set_kernel_text_ro(void)
{
  //0xc0008000 - 0xc0ad2e0c
	//unsigned long start = PAGE_ALIGN((unsigned long)_text);
	//unsigned long size = PAGE_ALIGN((unsigned long)__end_rodata) - start;
	unsigned long start = PAGE_ALIGN((unsigned long)0xc0008000);
	//unsigned long size = PAGE_ALIGN((unsigned long)0xc0ad2e0c) - start;
	unsigned long size = PAGE_ALIGN((unsigned long)0xc0adc000) - start;

	if (!kernel_set_to_readonly)
		return;

	pr_info_once("Write protecting the kernel text section %lx - %lx\n",
		start, start + size);

	pr_debug("Set kernel text: %lx - %lx to read only\n",
		 start, start + size);

	set_memory_ro(start, size >> PAGE_SHIFT);
}

void mark_rodata_ro(void)
{
	kernel_set_to_readonly = 1;

	set_kernel_text_ro();

	rodata_test();
}
