/* GStreamer
 * Copyright (C) <2010> Thiago Santos <thiago.sousa.santos@collabora.co.uk>
 *
 * gstplugin.c: camerabin2 plugin registering
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstviewfinderbin.h"
#include "gstwrappercamerabinsrc.h"
#include "gstcamerabin2.h"

static gboolean
plugin_init (GstPlugin * plugin)
{
  if (!gst_viewfinder_bin_plugin_init (plugin))
    return FALSE;
  if (!gst_wrapper_camera_bin_src_plugin_init (plugin))
    return FALSE;
  if (!gst_camera_bin2_plugin_init (plugin))
    return FALSE;

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    camerabin, "Take image snapshots and record movies from camera",
    plugin_init, VERSION, "LGPL", GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN)
