/* 
 * GSQL - database development tool for GNOME
 *
 * Copyright (C) 2006-2009  Taras Halturin  halturin@gmail.com
 *
 *
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

 
#include <glib.h>
#include <gtk/gtk.h>
#include <glib-object.h>


#define ENGINE_MAJOR_VER 0
#define ENGINE_MINOR_VER 1
#define ENGINE_VERSION "0.0"
#define ENGINE_ID    "engine_example"
#define ENGINE_NAME  "Example Engine"
#define ENGINE_DESC  "GSQL module for Example support"
#define ENGINE_AUTHOR "Taras Halturin"
#define ENGINE_HOMEPAGE "http://gsql.org"

#include <config.h>
#include <libgsql/engines.h>
#include <libgsql/common.h>
#include <libgsql/workspace.h>

GtkWidget *
engine_logon_widget_new ();
void
engine_logon_widget_free (GtkWidget * logon_widget);


gboolean
engine_load (GSQLEngine *engine)
{
	GSQL_TRACE_FUNC;

	engine->info.author = ENGINE_AUTHOR;
	engine->info.id = ENGINE_ID;
	engine->info.name = ENGINE_NAME;
	engine->info.desc = ENGINE_DESC;
	engine->info.homepage = ENGINE_HOMEPAGE;
	engine->info.version = ENGINE_VERSION;
	engine->info.major_version = ENGINE_MAJOR_VER;
	engine->info.minor_version = ENGINE_MINOR_VER;

	engine->file_logo = "example/example.png";
	engine->stock_logo = "example-logo";//GSQLE_EXAMPLE_STOCK_EXAMPLE;
//	engine->session_open = engine_session_open;

	/* logon widget */
//	engine->logon_widget_new = engine_logon_widget_new;
//	engine->logon_widget_free = engine_logon_widget_free;
	
	/* prefs widget */
//	engine->conf_widget_new = engine_conf_widget_new;
//	engine->conf_widget_free = engine_conf_widget_free;
	
	/* cursor */
//	engine->cursor_open_with_bind = mysql_cursor_open_bind;
//	engine->cursor_open = mysql_cursor_open;
//	engine->cursor_fetch = mysql_cursor_fetch;
	
	
	add_pixmap_directory (PACKAGE_PIXMAPS_DIR "/example");
	// engine_stock_init();
	//engine_menu_init (engine);
	
    return TRUE;
}

gboolean
engine_unload (GSQLEngine * engine)
{
	GSQL_TRACE_FUNC;

	if (engine->in_use)
	{
		fprintf (stdout, "Engine unload failed. Still in use.\n");
		return FALSE;
	}
	
	// FIXME. need real unloading (g_module_close)
	GSQL_DEBUG ("Not implemented");
	
	return FALSE;
}


GtkWidget *
engine_logon_widget_new ()
{
	GSQL_TRACE_FUNC;

	GtkWidget *table;
	GtkWidget *hostname;
	GtkWidget *database_name;
	GtkWidget *database_name_entry;
	GtkWidget *username;
	GtkWidget *password;
	GtkWidget *label;

	table = gtk_table_new (6, 2, FALSE);
	gtk_table_set_row_spacings (GTK_TABLE (table), 2);
	gtk_widget_show (table);
	
	
	hostname = gtk_entry_new ();
	gtk_widget_show (hostname);
	gtk_table_attach (GTK_TABLE (table), hostname, 1, 2, 1, 2,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_max_length (GTK_ENTRY (hostname), 128);
	gtk_entry_set_invisible_char (GTK_ENTRY (hostname), 9679);
	gtk_entry_set_activates_default(GTK_ENTRY (hostname), TRUE);
        
	label = gtk_label_new (_("Hostname"));
	gtk_widget_show (label);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 1, 2,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
	gtk_misc_set_padding (GTK_MISC (label), 14, 0);
	
        
	database_name = gtk_combo_box_entry_new_text ();
	gtk_widget_show (database_name);
	gtk_table_attach (GTK_TABLE (table), database_name, 1, 2, 2, 3,
						(GtkAttachOptions) (GTK_FILL),
						(GtkAttachOptions) (GTK_FILL), 0, 0);
	database_name_entry = gtk_bin_get_child(GTK_BIN(database_name));
	gtk_entry_set_activates_default(GTK_ENTRY (database_name_entry), TRUE);

	label = gtk_label_new (_("Database name"));
	gtk_widget_show (label);
		
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 2, 3,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
	gtk_misc_set_padding (GTK_MISC (label), 14, 0);
       
	username = gtk_entry_new ();
	gtk_widget_show (username);
	gtk_table_attach (GTK_TABLE (table), username, 1, 2, 3, 4,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_max_length (GTK_ENTRY (username), 32);
	gtk_entry_set_invisible_char (GTK_ENTRY (username), 9679);
	gtk_entry_set_activates_default(GTK_ENTRY (username), TRUE);
        
	label = gtk_label_new (_("Username"));
	gtk_widget_show (label);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 3, 4,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
	gtk_misc_set_padding (GTK_MISC (label), 14, 0);
        
	password = gtk_entry_new ();
	gtk_widget_show (password);
	gtk_table_attach (GTK_TABLE (table), password, 1, 2, 4, 5,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (0), 0, 0);
	gtk_entry_set_max_length (GTK_ENTRY (password), 32);
	gtk_entry_set_visibility (GTK_ENTRY (password), FALSE);
	gtk_entry_set_invisible_char (GTK_ENTRY (password), 9679);
	gtk_entry_set_activates_default(GTK_ENTRY (password), TRUE);
    
	label = gtk_label_new (_("Password"));
	gtk_widget_show (label);
	gtk_table_attach (GTK_TABLE (table), label, 0, 1, 4, 5,
						(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
						(GtkAttachOptions) (0), 0, 0);
	gtk_misc_set_alignment (GTK_MISC (label), 0, 0.5);
	gtk_misc_set_padding (GTK_MISC (label), 14, 0);
        
	HOOKUP_OBJECT (table, hostname, "hostname");
	HOOKUP_OBJECT (table, database_name, "database");
	HOOKUP_OBJECT (table, username, "username");  
	HOOKUP_OBJECT (table, password, "password");  

	return table;	
}

void
engine_logon_widget_free (GtkWidget * logon_widget)
{
	GSQL_TRACE_FUNC;

}

