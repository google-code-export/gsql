/***************************************************************************
 *            gsqlfiles.h
 *
 *  Sun Sep  2 23:48:00 2007
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
 
#ifndef _GSQLFILES_H
#define _GSQLFILES_H

#include <glib.h>
#include <libgsql/session.h>
#include <libgsql/content.h>

G_BEGIN_DECLS

void
gsql_files_menu_init();

void
gsql_files_open_file (GSQLSession *session, gchar *file, gchar *encoding);

gboolean
gsql_files_save_file (GSQLContent *content);

void
gsql_files_reload_file (GSQLContent *content, gchar *file, gchar *encoding);

G_END_DECLS

#endif /* _GSQLFILES_H */

 