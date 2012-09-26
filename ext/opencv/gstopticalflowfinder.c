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
#include <gst/gst.h>
#include "gstopticalflowfinder.h"

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
    GST_STATIC_CAPS ("video/x-raw,format=RGB")
    );

static GstStaticPadTemplate gst_optical_flow_finder_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-raw,format=RGB")
    );


/* class initialization */

#define DEBUG_INIT(bla) \
  GST_DEBUG_CATEGORY_INIT (gst_optical_flow_finder_debug_category, "opticalflowfinder", 0, \
      "debug category for opticalflowfinder element");

static void
gst_optical_flow_finder_class_init (GstOpticalFlowFinderClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstElementClass *element_class = GST_ELEMENT_CLASS (klass);

  gobject_class->set_property = gst_optical_flow_finder_set_property;
  gobject_class->get_property = gst_optical_flow_finder_get_property;
  gobject_class->finalize = gst_optical_flow_finder_finalize;
  element_class->change_state =
      GST_DEBUG_FUNCPTR (gst_optical_flow_finder_change_state);

  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_optical_flow_finder_sink_template));
  gst_element_class_add_pad_template (element_class,
      gst_static_pad_template_get (&gst_optical_flow_finder_src_template));

  gst_element_class_set_details_simple (element_class,
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

  gst_pad_set_chain_function (opticalflowfinder->sinkpad,
      GST_DEBUG_FUNCPTR (gst_optical_flow_finder_sink_chain));

  gst_element_add_pad (GST_ELEMENT (opticalflowfinder),
      opticalflowfinder->sinkpad);

  opticalflowfinder->srcpad =
      gst_pad_new_from_static_template (&gst_optical_flow_finder_src_template,
      "src");

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

void
gst_optical_flow_finder_finalize (GObject * object)
{
  /* GstOpticalFlowFinder *opticalflowfinder = GST_OPTICAL_FLOW_FINDER (object); */

  /* clean up object here */

  G_OBJECT_CLASS (gst_optical_flow_finder_parent_class)->finalize (object);
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

static GstFlowReturn
gst_optical_flow_finder_sink_chain (GstPad * pad, GstObject * parent,
    GstBuffer * buffer)
{

  GstOpticalFlowFinder *finder =
      GST_OPTICAL_FLOW_FINDER (GST_OBJECT_PARENT (pad));
  g_print ("Have data\n");
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
