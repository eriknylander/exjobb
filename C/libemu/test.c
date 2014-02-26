
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdint.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <getopt.h>


#include "/home/erik/libemu/libemu/include/emu/emu.h"
#include "/home/erik/libemu/libemu/include/emu/emu_memory.h"
#include "/home/erik/libemu/libemu/include/emu/emu_cpu.h"
#include "/home/erik/libemu/libemu/include/emu/emu_log.h"


int main()
{

	struct emu *e = emu_new();
	//emu_log_level_set(emu_logging_get(e), EMU_LOG_DEBUG);

	char scode[] = {0xb8, 0x01, 0x00, 0x00, 0x00, 0xb9, 0x0a, 0x00, 0x00, 0x00, 0x01, 0xc8, 0x31, 0xd2, 0xf7, 0xc2, 0x00, 0x00, 0x00, 0x00};

	struct emu_memory *mem = emu_memory_get(e);
	int static_offset = 4711;

	int i;
	for(i = 0; i < 20; i++)
	{
		emu_memory_write_byte(mem, static_offset+i, scode[i]);
	}
	emu_memory_write_byte(mem, static_offset+i, '\xcc');

	emu_cpu_eip_set(emu_cpu_get(e), static_offset);
	emu_cpu_run(emu_cpu_get(e));

	emu_cpu_debug_print(emu_cpu_get(e));

	int eflags = emu_cpu_eflags_get(emu_cpu_get(e));

	printf("EFLAGS = %d\n", eflags);

	emu_free(e);



	return 0;
}