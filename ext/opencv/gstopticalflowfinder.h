/* GStreamer
 * Copyright (C) 2012 FIXME <fixme@example.com>
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

#ifndef _GST_OPTICAL_FLOW_FINDER_H_
#define _GST_OPTICAL_FLOW_FINDER_H_

#include <gst/gst.h>
#include <cv.h>
#include <opencv2/highgui/highgui_c.h>
#include "gstopencvutils.h"
#include "g-finder.h"

G_BEGIN_DECLS

#define GST_TYPE_OPTICAL_FLOW_FINDER   (gst_optical_flow_finder_get_type())
#define GST_OPTICAL_FLOW_FINDER(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_OPTICAL_FLOW_FINDER,GstOpticalFlowFinder))
#define GST_OPTICAL_FLOW_FINDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_OPTICAL_FLOW_FINDER,GstOpticalFlowFinderClass))
#define GST_IS_OPTICAL_FLOW_FINDER(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_OPTICAL_FLOW_FINDER))
#define GST_IS_OPTICAL_FLOW_FINDER_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_OPTICAL_FLOW_FINDER))

typedef struct _GstOpticalFlowFinder GstOpticalFlowFinder;
typedef struct _GstOpticalFlowFinderClass GstOpticalFlowFinderClass;

struct _GstOpticalFlowFinder
{
  GstElement base_opticalflowfinder;

  GstPad *sinkpad;
  GstPad *srcpad;

  IplImage *cvImage;
  CvMemStorage *cvStorage;

  gint width, height;
  GFinder *surf_finder;
};

struct _GstOpticalFlowFinderClass
{
  GstElementClass base_opticalflowfinder_class;
};

GType gst_optical_flow_finder_get_type (void);

gboolean gst_optical_flow_finder_plugin_init (GstPlugin * plugin);

G_END_DECLS

#endif
