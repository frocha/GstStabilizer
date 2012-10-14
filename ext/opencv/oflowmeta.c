/*
 * Copyright (C) 2012 Francisco Rocha <rocha.francisco.a@gmail.com>
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

#include "oflowmeta.h"

GType
o_flow_meta_api_get_type (void)
{
  static volatile GType type;
  static const gchar *tags[] = { "optical flow", "opencv", NULL };

  if (g_once_init_enter (&type)) {
    GType _type = gst_meta_api_type_register ("OFlowMetaAPI", tags);
    g_once_init_leave (&type, _type);
  }
  return type;
}

static gboolean
o_flow_meta_init (GstMeta * meta, gpointer params, GstBuffer * buffer)
{
  OFlowMeta *emeta = (OFlowMeta *) meta;

  emeta->num = 0;
  emeta->name = NULL;
  emeta->points0 = NULL;
  emeta->points1 = NULL;

  return TRUE;
}

static gboolean
o_flow_meta_transform (GstBuffer * transbuf, GstMeta * meta,
    GstBuffer * buffer, GQuark type, gpointer data)
{
  OFlowMeta *emeta = (OFlowMeta *) meta;

  /* we always copy no matter what transform */
  gst_buffer_add_o_flow_meta (transbuf, emeta->num, emeta->name, emeta->points0,
      emeta->points1);

  return TRUE;
}

static void
o_flow_meta_free (GstMeta * meta, GstBuffer * buffer)
{
  OFlowMeta *emeta = (OFlowMeta *) meta;

  g_free (emeta->name);
  emeta->name = NULL;

  g_free (emeta->points0);
  emeta->points0 = NULL;

  g_free (emeta->points1);
  emeta->points1 = NULL;
}

const GstMetaInfo *
o_flow_meta_get_info (void)
{
  static const GstMetaInfo *meta_info = NULL;

  if (g_once_init_enter (&meta_info)) {
    const GstMetaInfo *mi = gst_meta_register (O_FLOW_META_API_TYPE,
        "OFlowMeta",
        sizeof (OFlowMeta),
        o_flow_meta_init,
        o_flow_meta_free,
        o_flow_meta_transform);
    g_once_init_leave (&meta_info, mi);
  }
  return meta_info;
}

OFlowMeta *
gst_buffer_add_o_flow_meta (GstBuffer * buffer, gint num, const gchar * name,
    CvPoint2D32f * points0, CvPoint2D32f * points1)
{
  OFlowMeta *meta;

  g_return_val_if_fail (GST_IS_BUFFER (buffer), NULL);

  meta = (OFlowMeta *) gst_buffer_add_meta (buffer, O_FLOW_META_INFO, NULL);

  meta->num = num;
  meta->name = g_strdup (name);
  /* FIXME copy contents instead of assigning pointers? */
  meta->points0 = points0;
  meta->points1 = points1;

  return meta;
}
