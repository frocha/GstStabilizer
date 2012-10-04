/* GStreamer
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
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */
/**
 * SECTION:element-gstopticalflowfinder
 *
 * The opticalflowfinder element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v fakesrc ! opticalflowfinder ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include "gstopticalflowfinder.h"
#include "g-surffinder.h"

GST_DEBUG_CATEGORY_STATIC (gst_optical_flow_finder_debug_category);
#define GST_CAT_DEFAULT gst_optical_flow_finder_debug_category

G_DEFINE_TYPE (GstOpticalFlowFinder, gst_optical_flow_finder, GST_TYPE_ELEMENT);

/* prototypes */


static void gst_optical_flow_finder_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_optical_flow_finder_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_optical_flow_finder_finalize (GObject * object);
static GstStateChangeReturn gst_optical_flow_finder_change_state (GstElement *
    element, GstStateChange transition);

static gboolean gst_optical_flow_finder_handle_sink_event (GstPad * pad,
    GstObject * parent, GstEvent * event);
static GstFlowReturn
gst_optical_flow_finder_sink_chain (GstPad * pad, GstObject * parent,
    GstBuffer * buffer);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_optical_flow_finder_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_MAKE ("RGB"))
    );

static GstStaticPadTemplate gst_optical_flow_finder_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_MAKE ("RGB"))
    );


/* class initialization */

#define DEBUG_INIT(bla) \
  GST_DEBUG_CATEGORY_INIT (gst_optical_flow_finder_debug_category, "opticalflowfinder", 0, \
      "debug category for opticalflowfinder element");

static void
gst_optical_flow_finder_class_init (GstOpticalFlowFinderClass * klass)
{
  GObjectClass *gobject_class = (GObjectClass *) klass;
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  gobject_class->set_property = gst_optical_flow_finder_set_property;
  gobject_class->get_property = gst_optical_flow_finder_get_property;
  gobject_class->finalize =
      GST_DEBUG_FUNCPTR (gst_optical_flow_finder_finalize);
  element_class->change_state =
      GST_DEBUG_FUNCPTR (gst_optical_flow_finder_change_state);

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_optical_flow_finder_sink_template));
  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_optical_flow_finder_src_template));

  gst_element_class_set_metadata (element_class,
      "opticalflowfinder",
      "Filter/Effect/Video",
      "Attaches optical flow between frames as metadata",
      "Francisco Rocha <rocha.francisco.a@gmail.com>");
}

static void
gst_optical_flow_finder_init (GstOpticalFlowFinder * opticalflowfinder)
{

  opticalflowfinder->sinkpad =
      gst_pad_new_from_static_template (&gst_optical_flow_finder_sink_template,
      "sink");
  GST_PAD_SET_PROXY_CAPS (opticalflowfinder->sinkpad);
  gst_element_add_pad (GST_ELEMENT (opticalflowfinder),
      opticalflowfinder->sinkpad);


  gst_pad_set_event_function (opticalflowfinder->sinkpad,
      GST_DEBUG_FUNCPTR (gst_optical_flow_finder_handle_sink_event));
  gst_pad_set_chain_function (opticalflowfinder->sinkpad,
      GST_DEBUG_FUNCPTR (gst_optical_flow_finder_sink_chain));


  opticalflowfinder->srcpad =
      gst_pad_new_from_static_template (&gst_optical_flow_finder_src_template,
      "src");
  GST_PAD_SET_PROXY_CAPS (opticalflowfinder->srcpad);
  gst_element_add_pad (GST_ELEMENT (opticalflowfinder),
      opticalflowfinder->srcpad);


}

void
gst_optical_flow_finder_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  /* GstOpticalFlowFinder *opticalflowfinder = GST_OPTICAL_FLOW_FINDER (object); */

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_optical_flow_finder_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  /* GstOpticalFlowFinder *opticalflowfinder = GST_OPTICAL_FLOW_FINDER (object); */

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

static void
gst_optical_flow_finder_finalize (GObject * obj)
{
  GstOpticalFlowFinder *finder = GST_OPTICAL_FLOW_FINDER (obj);

  if (finder->cvImage) {
    cvReleaseImage (&finder->cvImage);
  }

  G_OBJECT_CLASS (gst_optical_flow_finder_parent_class)->finalize (obj);
}

static GstStateChangeReturn
gst_optical_flow_finder_change_state (GstElement * element,
    GstStateChange transition)
{
  /* FIXME GstOpticalFlowFinder *opticalflowfinder; */
  GstStateChangeReturn ret;

  g_return_val_if_fail (GST_IS_OPTICAL_FLOW_FINDER (element),
      GST_STATE_CHANGE_FAILURE);
  /* FIXME opticalflowfinder = GST_OPTICAL_FLOW_FINDER (element); */

  switch (transition) {
    case GST_STATE_CHANGE_NULL_TO_READY:
      break;
    case GST_STATE_CHANGE_READY_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_PLAYING:
      break;
    default:
      break;
  }

  ret =
      GST_ELEMENT_CLASS (gst_optical_flow_finder_parent_class)->change_state
      (element, transition);

  switch (transition) {
    case GST_STATE_CHANGE_PLAYING_TO_PAUSED:
      break;
    case GST_STATE_CHANGE_PAUSED_TO_READY:
      break;
    case GST_STATE_CHANGE_READY_TO_NULL:
      break;
    default:
      break;
  }

  return ret;
}

static gboolean
gst_optical_flow_finder_handle_sink_event (GstPad * pad, GstObject * parent,
    GstEvent * event)
{
  GstOpticalFlowFinder *filter;
  gint width, height;
  GstStructure *structure;
  gboolean res = TRUE;

  filter = GST_OPTICAL_FLOW_FINDER (parent);

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_CAPS:
    {
      GstCaps *caps;
      gst_event_parse_caps (event, &caps);

      structure = gst_caps_get_structure (caps, 0);
      gst_structure_get_int (structure, "width", &width);
      gst_structure_get_int (structure, "height", &height);

      if (!filter->cvImage) {
        filter->cvImage =
            cvCreateImage (cvSize (width, height), IPL_DEPTH_8U, 3);
        filter->cvStorage = cvCreateMemStorage (0);
      }
      break;
    }
    default:
      break;
  }

  res = gst_pad_event_default (pad, parent, event);
  return res;
}

/* Chain function */
static GstFlowReturn
gst_optical_flow_finder_sink_chain (GstPad * pad, GstObject * parent,
    GstBuffer * buffer)
{

  int ret;
  GstMapInfo map_info;
  guint8 *data;
  GstOpticalFlowFinder *finder =
      GST_OPTICAL_FLOW_FINDER (GST_OBJECT_PARENT (pad));
  g_print ("Have data\n");

  finder->surf_finder = G_FINDER (g_surffinder_new ());
  g_finder_optical_flow_image (finder->surf_finder, "I am doing an action");
  gst_buffer_map (buffer, &map_info, GST_MAP_READ);
  data = map_info.data;

  finder->cvImage->imageData = (char *) data;
  ret = cvSaveImage ("~/cvImage.png", finder->cvImage, 0);
  g_print ("cvSaveImage returns = %d", ret);

  gst_buffer_unmap (buffer, &map_info);
  return gst_pad_push (finder->srcpad, buffer);
}

gboolean
gst_optical_flow_finder_plugin_init (GstPlugin * plugin)
{

  /* debug category for fltering log messages */
  GST_DEBUG_CATEGORY_INIT (gst_optical_flow_finder_debug_category,
      "opticalflowfinder", 0,
      "Finds optical flow for consecutive video frames");

  return gst_element_register (plugin, "opticalflowfinder", GST_RANK_NONE,
      GST_TYPE_OPTICAL_FLOW_FINDER);
}
