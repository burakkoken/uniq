/*
 *  Copyright(C) 2014 Codnect Team
 *  Copyright(C) 2014 Burak Köken
 *
 *  This file is part of Uniq.
 *  
 *  Uniq is free software: you can redistribute it and/or modify it under the
 *  terms of the GNU General Public License as published by the Free Software
 *  Foundation, version 2 of the License.
 *
 *  Uniq is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Uniq.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <uniq/types.h>
#include <ctype.h>
#include <va_list.h>
#include <drivers/vga.h>
#include <string.h>


/* flags */
#define SIGN	0x001
#define PLUS	0x002
#define SMALL	0x004
#define LEFT	0x008
#define CHAR	0x010
#define STRING	0x020
#define INTEGER	0x040
#define OCTAL	0x080
#define HEX	0x100
#define COLOR	0x200
#define SPECIAL	0x400
#define DOT	0x800

static int skip_atoi(const char **s){
	int i = 0;
	
	while (isdigit(**s))
		i = i * 10 + *((*s)++) - '0';
	return i;
}


int vasprintf(const char *fmt, va_list arg_list){
	
	uint8_t attr = DEFAULT_ATTR;
	int flags,base;
	int len = strlen(fmt),slen;
	int i,printed;
	uint32_t d;
	char *s,c;
	bool cntrl = false;
	i = printed = 0;

	for( ;*fmt;++fmt){
		if(*fmt != '%'){
			putchar(*fmt,attr);
			printed++;
			continue;
		}
		fmt++;
			
		flags = 0;
		if(*fmt == '-')
			flags |= LEFT;
		else
			flags |= PLUS;

		if(flags & LEFT)
			fmt++;
		uint32_t arg_width,field_width;
		arg_width = field_width = 0;
		if(isdigit(*fmt))
			field_width = skip_atoi(&fmt);			

		if(*fmt == '.'){
			flags |= DOT;
			fmt++;
			arg_width = skip_atoi(&fmt);
		}

		switch(*fmt){
			case 'c':
				c = (char)va_arg(arg_list,int);
				flags |= CHAR;
				break;
			case 's':
				s = (char*)va_arg(arg_list,char*);
				if(!s)
					s = "(null)";
				flags |= STRING;
				slen = strlen(s);
				break;
			case 'd':
			case 'i':
			case 'x':
			case 'X':
			case 'o':
				d = (uint32_t)va_arg(arg_list,unsigned int);
				if(*fmt == 'd' || *fmt == 'i')
					flags |= INTEGER;
				else if(*fmt == 'x'){
					flags |= HEX;
					flags |= SMALL;
				}
				else if(*fmt == 'X')
					flags |= HEX;
				else
					flags |= OCTAL;										
				break;
			case 'C':
				attr = (uint8_t)va_arg(arg_list,unsigned int);
				flags |= COLOR;
				break;
			case '%':
				putchar('%',attr);
				flags |= SPECIAL;
				break;
			default:
				putchar(*fmt,attr);
				break;

		}
		cntrl = false;
				
		if((flags & COLOR) || (flags & SPECIAL))		
			continue;
		begin:

			if(flags & CHAR){
				if(!cntrl){
					if(field_width)
						field_width--;
				}
				if((flags & PLUS) && !cntrl)
					goto blank;
				if( ((flags & PLUS) && cntrl) || ((flags & LEFT) && !cntrl) ){
					putchar(c,attr);
					printed++;
				}
				if((flags & LEFT) && !cntrl)
					goto blank;
				continue;
			}
			else if(flags & STRING){
				if(!cntrl){
					/*
					 * "%-10.3s"
 					 * %(+/-)field_width.arg_width(char)
					 */
					if(!arg_width){
						if(field_width > slen)
							field_width -= slen;
						else
							field_width = 0;
					}
					else if(field_width <= arg_width)
						field_width = 0;
					else{
						if(arg_width && field_width)
							field_width -=arg_width;
					} 
				}

				if((flags & PLUS) && !cntrl)
					goto blank;
				if( ((flags & PLUS) && cntrl) || ((flags & LEFT) && !cntrl) ){
					if(!arg_width)
						putstr(s,attr);
					else{
						while(arg_width-- && *s){
							putchar(*s++,attr);
							printed++;
						}
					}
				}
				if((flags & LEFT) && !cntrl)
					goto blank;
				continue;

			}

			blank:
				while(field_width && field_width >0){
					putchar(' ',attr);
					printed++;
					field_width--;
				}
				cntrl = true;
				goto begin;
	}

	return 0;
}

int printf(const char *fmt, ...){

	va_list arg_list;
	va_start(arg_list, fmt);
	int printed = vasprintf(fmt,arg_list);
	va_end(arg_list);
	return printed;

}
