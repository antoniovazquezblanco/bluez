/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2004-2007  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "gsta2dpsink.h"

GST_DEBUG_CATEGORY(bluetooth_debug);

static gboolean plugin_init(GstPlugin *plugin)
{
	GST_INFO("Bluetooth plugin %s", VERSION);

	if (gst_element_register(plugin, "a2dpsink",
			GST_RANK_PRIMARY, GST_TYPE_A2DP_SINK) == FALSE)
		return FALSE;

	GST_DEBUG_CATEGORY_INIT(bluetooth_debug, "bluetooth", 0,
							"Bluetooth plugin");

	return TRUE;
}

GST_PLUGIN_DEFINE(GST_VERSION_MAJOR, GST_VERSION_MINOR,
	"bluetooth", "Bluetooth plugin library",
	plugin_init, VERSION, "LGPL", "BlueZ", "http://www.bluez.org/")
