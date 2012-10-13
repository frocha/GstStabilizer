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

#ifndef _O_FLOW_META_H_
#define _O_FLOW_META_H_

#ifndef GST_USE_UNSTABLE_API
#warning "OFlowMeta is unstable API and may change in future."
#warning "You can define GST_USE_UNSTABLE_API to avoid this warning."
#endif

#include <gst/gst.h>

G_BEGIN_DECLS

/* API definition */

typedef struct _OFlowMeta OFlowMeta;

struct _OFlowMeta {
    GstMeta meta;

    gint num;
    gchar *name;
};

GType o_flow_meta_api_get_type (void);
#define O_FLOW_META_API_TYPE (o_flow_meta_api_get_type())

#define gst_buffer_get_o_flow_meta(b) \
    ((OFlowMeta*)gst_buffer_get_meta((b),O_FLOW_META_API_TYPE))

/* API implementation */

const GstMetaInfo *o_flow_meta_get_info (void);
#define O_FLOW_META_INFO (o_flow_meta_get_info())

OFlowMeta * gst_buffer_add_o_flow_meta (GstBuffer *buffer,
                                        gint num,
                                        const gchar *name);

G_END_DECLS

#endif
