/*
 *  Copyright(C) 2014 Codnect Team
 *  Copyright(C) 2014 Burak Köken
 *
 *  This file is part of Nectlib.
 *  
 *  Nectlib is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, version 2 of the License.
 *
 *  Nectlib is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Uniq.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <uniq/types.h>
#include <uniq/kernel.h>

void kmain(uintptr_t mboot_magic){

	/* vga konsol */
	init_vga_console();

	if(mboot_magic != 0x2badb002)
		debug_print(KERN_WARNING,"Invalid the magic number. The magic number is \033[1;31m%P",mboot_magic);
	else
		debug_print(KERN_INFO,"Valid the magic number.The magic number is \033[1;37m%P",mboot_magic);
		
	/* gdt,idt tablosu */
	init_gdt();
	init_idt();

}
