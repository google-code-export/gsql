/***************************************************************************
 *            conf.c
 *
 *  Sat Sep  1 17:55:08 2007
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

#include <gconf/gconf.h>
#include <gconf/gconf-client.h>
#include <libgsql/conf.h>
#include <libgsql/common.h>

static GConfClient *gconf_client;


void
gsql_conf_init()
{
	GSQL_TRACE_FUNC;
	
	gconf_client = gconf_client_get_default ();
	
	if (gconf_client == NULL)
	{
		g_warning (_("Cannot initialize configuration manager."));                        
		return;
	}
	
	gconf_client_add_dir (gconf_client, 
							GSQL_CONF_ROOT_KEY,
							GCONF_CLIENT_PRELOAD_NONE,
							NULL);

}

static void
gsql_conf_notify_handler (GConfClient *client, guint cnxn_id,
						  GConfEntry *entry, gpointer userdata)
{
	GSQL_TRACE_FUNC;
		
	GSQLConfNotifyFunc handler;
	
	handler = g_object_get_data (G_OBJECT(userdata), "fhandler");
	
	if (handler)
		handler (userdata);
	else
		GSQL_DEBUG ("gsql_conf_notify_handler: handler is not set");
	
}
	


void
gsql_conf_nitify_add (gchar *path, GSQLConfNotifyFunc func, gpointer userdata)
{
	GSQL_TRACE_FUNC;
	
	g_return_if_fail (G_IS_OBJECT (userdata));

	HOOKUP_OBJECT_NO_REF(userdata, func, "fhandler");
	
	gconf_client_notify_add (gconf_client, 
								path,
								gsql_conf_notify_handler,
								 userdata, NULL, NULL);
}


/**
 *	gsql_conf_value_get_int:
 *	@path: path to the key
 *	@return: int value
 */
gint
gsql_conf_value_get_int (gchar *path)
{
	GSQL_TRACE_FUNC;

	GError *error = NULL;
	gint value;
	
	g_return_val_if_fail (path != NULL, 0);

	value = gconf_client_get_int (gconf_client, 
								path,
								 &error);
	if (error)
		g_error_free (error);
	
	return value;

}



/**
 *	gsql_conf_value_get_string:
 *	@path: path to the key
 *	@return: string value. returned value should be freed by g_free()
 */
gchar *
gsql_conf_value_get_string (gchar *path)
{
	GSQL_TRACE_FUNC;

	g_return_val_if_fail (path != NULL, NULL);

	return gconf_client_get_string (gconf_client, 
								 path,
								 NULL);
}

gchar *
gsql_conf_value_get_string_at_root (gchar *path)
{
	GSQL_TRACE_FUNC;

	g_return_val_if_fail (path != NULL, NULL);
	
	return gconf_client_get_string (gconf_client, 
								 path,
								 NULL);
}

gboolean
gsql_conf_value_get_boolean (gchar *path)
{
	GSQL_TRACE_FUNC;

	GError *error = NULL;
	gboolean value;
	
	g_return_val_if_fail (path != NULL, FALSE);
	
	value = gconf_client_get_bool (gconf_client, 
								 path,
								 &error);
	if (error)
		g_error_free (error);
	
	return value;
};

void
gsql_conf_value_set_int (gchar *path, gint value)
{
	GSQL_TRACE_FUNC;

	gboolean ret;
	GError *error = NULL;
	
	g_return_if_fail (path);
	
	ret = gconf_client_set_int (gconf_client,
						  path,
						  value, &error);
	if (error)
		g_error_free (error);

}

void
gsql_conf_value_set_string (gchar *path, gchar *value)
{
	GSQL_TRACE_FUNC;

	gboolean ret;

	g_return_if_fail (path);
	
	ret = gconf_client_set_string (gconf_client,
						  path,
						  value, NULL);
}

void
gsql_conf_value_set_boolean (gchar *path, gboolean value)
{
	GSQL_TRACE_FUNC;

	gboolean ret;

	g_return_if_fail (path);

	GError *error = NULL;

	ret = gconf_client_set_bool (gconf_client,
						  path,
						  value, &error);
	if (error)
		g_error_free (error);
	
}