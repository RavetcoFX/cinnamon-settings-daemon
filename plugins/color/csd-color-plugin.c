/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2007 William Jon McCann <mccann@jhu.edu>
 * Copyright (C) 2011 Richard Hughes <richard@hughsie.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Suite 500, Boston, MA 02110-1335, USA.
 *
 */

#include "config.h"

#include <glib/gi18n-lib.h>
#include <gmodule.h>

#include "cinnamon-settings-plugin.h"
#include "csd-color-plugin.h"
#include "csd-color-manager.h"

struct CsdColorPluginPrivate {
        CsdColorManager *manager;
};

#define CSD_COLOR_PLUGIN_GET_PRIVATE(object) (G_TYPE_INSTANCE_GET_PRIVATE ((object), CSD_TYPE_COLOR_PLUGIN, CsdColorPluginPrivate))

CINNAMON_SETTINGS_PLUGIN_REGISTER (CsdColorPlugin, csd_color_plugin)

static void
csd_color_plugin_init (CsdColorPlugin *plugin)
{
        plugin->priv = CSD_COLOR_PLUGIN_GET_PRIVATE (plugin);

        g_debug ("CsdColorPlugin initializing");

        plugin->priv->manager = csd_color_manager_new ();
}

static void
csd_color_plugin_finalize (GObject *object)
{
        CsdColorPlugin *plugin;

        g_return_if_fail (object != NULL);
        g_return_if_fail (CSD_IS_COLOR_PLUGIN (object));

        g_debug ("CsdColorPlugin finalizing");

        plugin = CSD_COLOR_PLUGIN (object);

        g_return_if_fail (plugin->priv != NULL);

        if (plugin->priv->manager != NULL) {
                g_object_unref (plugin->priv->manager);
        }

        G_OBJECT_CLASS (csd_color_plugin_parent_class)->finalize (object);
}

static void
impl_activate (CinnamonSettingsPlugin *plugin)
{
        gboolean res;
        GError  *error;

        g_debug ("Activating color plugin");

        error = NULL;
        res = csd_color_manager_start (CSD_COLOR_PLUGIN (plugin)->priv->manager, &error);
        if (! res) {
                g_warning ("Unable to start color manager: %s", error->message);
                g_error_free (error);
        }
}

static void
impl_deactivate (CinnamonSettingsPlugin *plugin)
{
        g_debug ("Deactivating color plugin");
        csd_color_manager_stop (CSD_COLOR_PLUGIN (plugin)->priv->manager);
}

static void
csd_color_plugin_class_init (CsdColorPluginClass *klass)
{
        GObjectClass           *object_class = G_OBJECT_CLASS (klass);
        CinnamonSettingsPluginClass *plugin_class = CINNAMON_SETTINGS_PLUGIN_CLASS (klass);

        object_class->finalize = csd_color_plugin_finalize;

        plugin_class->activate = impl_activate;
        plugin_class->deactivate = impl_deactivate;

        g_type_class_add_private (klass, sizeof (CsdColorPluginPrivate));
}
