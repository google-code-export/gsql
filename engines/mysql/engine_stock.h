/***************************************************************************
 *            engine_stock.h
 *
 *  Fri Sep  7 22:36:21 2007
 *  Copyright  2007  Taras Halturin
 *  <halturin@gmail.com>
 ****************************************************************************/

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */
 
#ifndef _ENGINE_STOCK_H
#define _ENGINE_STOCK_H

#include <glib.h>

#define GSQLE_MYSQL_STOCK_MYSQL				"gsqle-mysql-mysql"

G_BEGIN_DECLS

void
engine_stock_init ();

G_END_DECLS

#endif /* _ENGINE_STOCK_H */

 