/***************************************************************************
 *            engine_confcb.h
 *
 *  Fri Sep  7 00:01:14 2007
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
 
#ifndef _ENGINE_CONFCB_H
#define _ENGINE_CONFCB_H

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

void
on_use_custom_charset_check_toggled (GtkToggleButton *togglebutton,
											gpointer user_data);

G_END_DECLS

#endif /* _ENGINE_CONFCB_H */

 
