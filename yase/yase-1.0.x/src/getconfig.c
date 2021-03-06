/***
*    YASE (Yet Another Search Engine) 
*    Copyright (C) 2000-2002  Dibyendu Majumdar.
*
*    This program is free software; you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation; either version 2 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program; if not, write to the Free Software
*    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*    Author : Dibyendu Majumdar
*    Email  : dibyendu@mazumdar.demon.co.uk
*    Website: www.mazumdar.demon.co.uk/yase_index.html
*/
#include "yase.h"
#include "getconfig.h"
#include "param.h"

static void *Parameters;

const char *
ys_get_config(const char *name)
{
	if (Parameters == 0) {
		const char *dbpath = getenv("YASE_DBPATH");
		char filename[1024];
		if (dbpath == 0) 
			return 0;
		snprintf(filename, sizeof filename, "%s/%s", 
			dbpath, "yase.config");
		Parameters = ys_load_properties(filename);
	}
	if (Parameters == 0)
		return 0;
	return ys_get_property(Parameters, name);
}

