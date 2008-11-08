/***************************************************************************
 *            nav_tree__objects.c
 *
 *  Mon Oct 29 00:25:19 2007
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
 
#include <glib.h>
#include <gtk/gtk.h>
#include <string.h>
#include <libgsql/common.h>
#include <libgsql/stock.h>
#include <libgsql/session.h>
#include <libgsql/navigation.h>
#include <libgsql/cvariable.h>
#include "nav_objects.h"
#include "engine_stock.h"
#include "nav_sql.h"

#include "nav_tree__depend.h"
#include "nav_tree__privileges.h"

static GSQLNavigationItem object_types[] = {
	
	{	DEPENDSON_ID,
		GSQLE_ORACLE_STOCK_DEPENDS_ON,
		N_("Depends On"), 
		sql_oracle_depends_on,						// sql
		NULL, 						// object_popup
		NULL,						// object_handler
		(GSQLNavigationHandler) nav_tree_refresh_depend,						// expand_handler
		NULL,						// event_handler
		NULL, 0},					// child, childs
	
	{	DEPENDENT_ID,
		GSQLE_ORACLE_STOCK_DEPENDENT,
		N_("Dependent Objects"), 
		sql_oracle_dependent_objects,						// sql
		NULL, 						// object_popup
		NULL,						// object_handler
		(GSQLNavigationHandler) nav_tree_refresh_depend,						// expand_handler
		NULL,						// event_handler
		NULL, 0}					// child, childs
};

void
nav_tree_refresh_objects (GSQLNavigation *navigation,
						 GtkTreeView *tv,
						 GtkTreeIter *iter)
{
	GSQL_TRACE_FUNC;
	GtkTreeModel *model;
	GtkTreeIter child;
	GtkTreeIter child_fake;
	GtkTreeIter	child_last;
	gint n, id, o_id;
	gchar	   key[256];
	gchar		*sql = NULL;
	gchar		*realname = NULL;
	gchar		*name = NULL;
	gchar		*owner = NULL;
	GSQLCursor * cursor;
	GSQLCursorState state;
	GSQLSession *session;
	GSQLWorkspace *workspace;
	GSQLVariable *var;
	GtkListStore *details;
	void  *s_elements;
	guint n_elements;
	gchar *stock;

	model = gtk_tree_view_get_model(tv);
	n = gtk_tree_model_iter_n_children(model, iter);
	
	for (; n>1; n--)
	{
		gtk_tree_model_iter_children(model, &child, iter);
		gtk_tree_store_remove(GTK_TREE_STORE(model), &child);
	}
	
	gtk_tree_model_iter_children(model, &child_last, iter);
	
	gtk_tree_model_get (model, iter,  
						GSQL_NAV_TREE_REALNAME, 
						&realname, -1);
	
	gtk_tree_model_get (model, iter,  
						GSQL_NAV_TREE_SQL, 
						&sql, -1);
	
	gtk_tree_model_get (model, iter,  
						GSQL_NAV_TREE_OWNER, 
						&owner, -1);
	
	gtk_tree_model_get (model, iter,  
						GSQL_NAV_TREE_ID, 
						&id, -1);
	
	session = gsql_session_get_active ();
	
	GSQL_DEBUG ("realname:[%s]    sql:[%s]   owner:[%s]", realname, sql, owner); 
	
	switch (id)
	{
		case OBJECT_TYPES_ID:
			if (strncmp (owner, gsql_session_get_username (session), 64))
				sql = (gchar *) sql_oracle_object_types;
			
			GSQL_DEBUG ("types SQL: %s", sql);
	
			cursor = gsql_cursor_new (session, sql);
			state = gsql_cursor_open_with_bind (cursor,
												FALSE,
												GSQL_CURSOR_BIND_BY_NAME,
												G_TYPE_STRING, ":owner",
												G_TYPE_STRING, owner,
												G_TYPE_STRING, ":object_name",
												G_TYPE_STRING, "%",
												-1);
			s_elements = object_types;
			n_elements = G_N_ELEMENTS (object_types);
			stock = GSQLE_ORACLE_STOCK_OBJ_TYPES;
			
			break;
			
		case OBJECT_TYPE_BODIES_ID:
			if (strncmp (owner, gsql_session_get_username (session), 64))
				sql = (gchar *) sql_oracle_users_objects;
			
			cursor = gsql_cursor_new (session, sql);
			state = gsql_cursor_open_with_bind (cursor,
										FALSE,
										GSQL_CURSOR_BIND_BY_NAME,
										G_TYPE_STRING, ":owner",
										G_TYPE_STRING, owner,
										G_TYPE_STRING, ":object_name",
										G_TYPE_STRING, "%",
										G_TYPE_STRING, ":object_type",
										G_TYPE_STRING, "TYPE BODY",
										-1);
			s_elements = object_types;
			n_elements = G_N_ELEMENTS (object_types);
			stock = GSQLE_ORACLE_STOCK_OBJ_TYPE_BODIES;
			
			break;
			
		case COLLECTION_TYPES_ID:
			if (strncmp (owner, gsql_session_get_username (session), 64))
				sql = (gchar *) sql_oracle_collection_types;
			
			GSQL_DEBUG ("collection SQL: %s", sql);
			cursor = gsql_cursor_new (session, sql);
			state = gsql_cursor_open_with_bind (cursor,
												FALSE,
												GSQL_CURSOR_BIND_BY_NAME,
												G_TYPE_STRING, ":owner",
												G_TYPE_STRING, owner,
												G_TYPE_STRING, ":object_name",
												G_TYPE_STRING, "%",
												-1);
			s_elements = object_types;
			n_elements = G_N_ELEMENTS (object_types);
			stock = GSQLE_ORACLE_STOCK_COLLECTIONS_TYPES;
			
			break;		
	}
			
	
	if (state != GSQL_CURSOR_STATE_OPEN)
	{
		gsql_cursor_close (cursor);
		return;
	}

	var = g_list_nth_data(cursor->var_list, 0);
	
	n = 0;
	
	while (gsql_cursor_fetch (cursor, 1) > 0)
	{
		n++;
		
		if (var->value_type != G_TYPE_STRING)
		{
			GSQL_DEBUG ("The name of object should be a string (char *). Is the bug");
			name = N_("Incorrect data");
		} else {
			name = (gchar *) var->value;
			// make a key for a hash of details
			memset (key, 0, 256);
			g_snprintf (key, 255, "%x%s%d%s",
						session, owner, id, name);
			
			details = gsql_navigation_get_details (navigation, key);
			oracle_navigation_fill_details (cursor, details);
		}
		
		gtk_tree_store_append (GTK_TREE_STORE(model), &child, iter);
		gtk_tree_store_set (GTK_TREE_STORE(model), &child,
					GSQL_NAV_TREE_ID,			id,
					GSQL_NAV_TREE_OWNER,		owner,
					GSQL_NAV_TREE_IMAGE,		stock,
					GSQL_NAV_TREE_NAME,			name,
					GSQL_NAV_TREE_REALNAME, 	name,
					GSQL_NAV_TREE_ITEM_INFO, 	NULL,
					GSQL_NAV_TREE_SQL,			NULL,
					GSQL_NAV_TREE_OBJECT_POPUP, NULL,
					GSQL_NAV_TREE_OBJECT_HANDLER, NULL,
					GSQL_NAV_TREE_EXPAND_HANDLER, NULL,
					GSQL_NAV_TREE_EVENT_HANDLER, NULL,
					GSQL_NAV_TREE_STRUCT, object_types,
					GSQL_NAV_TREE_DETAILS, details,
					GSQL_NAV_TREE_NUM_ITEMS, G_N_ELEMENTS(object_types),
					-1);

		gtk_tree_store_append (GTK_TREE_STORE (model), &child_fake, &child);
		gtk_tree_store_set (GTK_TREE_STORE (model), &child_fake,
				GSQL_NAV_TREE_ID,				-1,
				GSQL_NAV_TREE_IMAGE,			NULL,
				GSQL_NAV_TREE_NAME,				N_("Processing..."),
				GSQL_NAV_TREE_REALNAME,			NULL,
				GSQL_NAV_TREE_ITEM_INFO,		NULL,
				GSQL_NAV_TREE_SQL,				NULL,
				GSQL_NAV_TREE_OBJECT_POPUP,		NULL,
				GSQL_NAV_TREE_OBJECT_HANDLER,	NULL,
				GSQL_NAV_TREE_EXPAND_HANDLER,	NULL,
				GSQL_NAV_TREE_EVENT_HANDLER,	NULL,
				GSQL_NAV_TREE_STRUCT,			NULL,
				GSQL_NAV_TREE_NUM_ITEMS, 		NULL,
				-1);
	}
	
	GSQL_DEBUG ("Items fetched: [%d]", n);
	
	if (n > 0)
	{
		name = g_strdup_printf("%s<span weight='bold'> [%d]</span>", 
												realname, n);
		gtk_tree_store_set (GTK_TREE_STORE(model), iter,
							GSQL_NAV_TREE_NAME, 
							name,
							-1);
		g_free (name);
	}
	
	gtk_tree_store_remove(GTK_TREE_STORE(model), &child_last);
	
	gsql_cursor_close (cursor);

}