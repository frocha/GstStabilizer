/*
 * GStreamer
 * Copyright (C) 2008 Nokia Corporation <multimedia@maemo.org>
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

/**
 * SECTION:camerabingeneral
 * @short_description: helper functions for #GstCameraBin and it's modules
 *
 * Common helper functions for #GstCameraBin, #GstCameraBinImage and
 * #GstCameraBinVideo.
 *
 */
#include <string.h>

#include <glib.h>
#include <gst/app/gstappsrc.h>
#include <gst/app/gstappsink.h>
#include <gst/basecamerabinsrc/gstbasecamerasrc.h>
#include "camerabingeneral.h"

/**
 * gst_camerabin_add_element:
 * @bin: add an element to this bin
 * @new_elem: new element to be added
 *
 * Adds given element to given @bin. Looks for an unconnected src pad
 * from the @bin and links the element to it.  Raises an error if adding
 * or linking failed. Unrefs the element in the case of an error.
 *
 * Returns: %TRUE if adding and linking succeeded, %FALSE otherwise.
 */
gboolean
gst_camerabin_add_element (GstBin * bin, GstElement * new_elem)
{
  return gst_camerabin_add_element_full (bin, NULL, new_elem, NULL);
}

/**
 * gst_camerabin_add_element_full:
 * @bin: add an element to this bin
 * @srcpad:  src pad name, or NULL for any
 * @new_elem: new element to be added
 * @dstpad:  dst pad name, or NULL for any
 *
 * Adds given element to given @bin. Looks for an unconnected src pad
 * (with name @srcpad, if specified) from the @bin and links the element
 * to it.  Raises an error if adding or linking failed. Unrefs the element
 * in the case of an error.
 *
 * Returns: %TRUE if adding and linking succeeded, %FALSE otherwise.
 */
gboolean
gst_camerabin_add_element_full (GstBin * bin, const gchar * srcpad,
    GstElement * new_elem, const gchar * dstpad)
{
  gboolean ret;

  g_return_val_if_fail (bin, FALSE);
  g_return_val_if_fail (new_elem, FALSE);

  ret = gst_camerabin_try_add_element (bin, srcpad, new_elem, dstpad);

  if (!ret) {
    gchar *elem_name = gst_element_get_name (new_elem);
    GST_ELEMENT_ERROR (bin, CORE, NEGOTIATION, (NULL),
        ("linking %s failed", elem_name));
    g_free (elem_name);
    gst_object_unref (new_elem);
  }

  return ret;
}

/**
 * gst_camerabin_try_add_element:
 * @bin: tries adding an element to this bin
 * @srcpad:  src pad name, or NULL for any
 * @new_elem: new element to be added
 * @dstpad:  dst pad name, or NULL for any
 *
 * Adds given element to given @bin. Looks for an unconnected src pad
 * (with name @srcpad, if specified) from the @bin and links the element to
 * it.
 *
 * Returns: %TRUE if adding and linking succeeded, %FALSE otherwise.
 */
gboolean
gst_camerabin_try_add_element (GstBin * bin, const gchar * srcpad,
    GstElement * new_elem, const gchar * dstpad)
{
  GstPad *bin_pad;
  GstElement *bin_elem;
  gboolean ret = TRUE;

  g_return_val_if_fail (bin, FALSE);
  g_return_val_if_fail (new_elem, FALSE);

  /* Get pads for linking */
  bin_pad = gst_bin_find_unlinked_pad (bin, GST_PAD_SRC);
  /* Add to bin */
  gst_bin_add (GST_BIN (bin), new_elem);
  /* Link, if unconnected pad was found, otherwise just add it to bin */
  if (bin_pad) {
    GST_DEBUG_OBJECT (bin, "linking %s to %s:%s", GST_OBJECT_NAME (new_elem),
        GST_DEBUG_PAD_NAME (bin_pad));
    bin_elem = gst_pad_get_parent_element (bin_pad);
    gst_object_unref (bin_pad);
    if (!gst_element_link_pads (bin_elem, srcpad, new_elem, dstpad)) {
      gst_object_ref (new_elem);
      gst_bin_remove (bin, new_elem);
      ret = FALSE;
    }
    gst_object_unref (bin_elem);
  } else {
    GST_INFO_OBJECT (bin, "no unlinked source pad in bin");
  }

  return ret;
}

/**
 * gst_camerabin_create_and_add_element:
 * @bin: tries adding an element to this bin
 * @elem_name: name of the element to be created
 * @instance_name: name of the instance of the element to be created
 *
 * Creates an element according to given name and
 * adds it to given @bin. Looks for an unconnected src pad
 * from the @bin and links the element to it.
 *
 * Returns: pointer to the new element if successful, NULL otherwise.
 */
GstElement *
gst_camerabin_create_and_add_element (GstBin * bin, const gchar * elem_name,
    const gchar * instance_name)
{
  GstElement *new_elem;

  g_return_val_if_fail (bin, FALSE);
  g_return_val_if_fail (elem_name, FALSE);

  new_elem = gst_element_factory_make (elem_name, instance_name);
  if (!new_elem) {
    GST_ELEMENT_ERROR (bin, CORE, MISSING_PLUGIN, (NULL),
        ("could not create \"%s\" element.", elem_name));
  } else if (!gst_camerabin_add_element (bin, new_elem)) {
    new_elem = NULL;
  }

  return new_elem;
}

/* try to change the state of an element. This function returns the element when
 * the state change could be performed. When this function returns NULL an error
 * occured and the element is unreffed if @unref is TRUE. */
static GstElement *
try_element (GstElement * bin, GstElement * element, gboolean unref)
{
  GstStateChangeReturn ret;

  if (element) {
    ret = gst_element_set_state (element, GST_STATE_READY);
    if (ret == GST_STATE_CHANGE_FAILURE) {
      GST_DEBUG_OBJECT (bin, "failed state change..");
      gst_element_set_state (element, GST_STATE_NULL);
      if (unref)
        gst_object_unref (element);
      element = NULL;
    }
  }
  return element;
}

GstElement *
gst_camerabin_setup_default_element (GstBin * bin, GstElement * user_elem,
    const gchar * auto_elem_name, const gchar * default_elem_name,
    const gchar * instance_name)
{
  GstElement *elem;

  if (user_elem) {
    GST_DEBUG_OBJECT (bin, "trying configured element");
    elem = try_element (GST_ELEMENT_CAST (bin), user_elem, FALSE);
  } else {
    /* only try fallback if no specific sink was chosen */
    GST_DEBUG_OBJECT (bin, "trying %s", auto_elem_name);
    elem = gst_element_factory_make (auto_elem_name, instance_name);
    elem = try_element (GST_ELEMENT_CAST (bin), elem, TRUE);
    if (elem == NULL) {
      /* if default sink from config.h is different then try it too */
      if (strcmp (default_elem_name, auto_elem_name)) {
        GST_DEBUG_OBJECT (bin, "trying %s", default_elem_name);
        elem = gst_element_factory_make (default_elem_name, instance_name);
        elem = try_element (GST_ELEMENT_CAST (bin), elem, TRUE);
      }
    }
  }
  return elem;
}

/**
 * gst_camerabin_remove_elements_from_bin:
 * @bin: removes all elements from this bin
 *
 * Removes all elements from this @bin.
 */
void
gst_camerabin_remove_elements_from_bin (GstBin * bin)
{
  GstIterator *iter = NULL;
  gpointer data = NULL;
  GstElement *elem = NULL;
  gboolean done = FALSE;

  iter = gst_bin_iterate_elements (bin);
  while (!done) {
    switch (gst_iterator_next (iter, &data)) {
      case GST_ITERATOR_OK:
        elem = GST_ELEMENT (data);
        gst_bin_remove (bin, elem);
        gst_element_set_state (GST_ELEMENT (elem), GST_STATE_NULL);
        /* Iterator increased the element refcount, so unref */
        gst_object_unref (elem);
        break;
      case GST_ITERATOR_RESYNC:
        gst_iterator_resync (iter);
        break;
      case GST_ITERATOR_ERROR:
        GST_WARNING_OBJECT (bin, "error in iterating elements");
        done = TRUE;
        break;
      case GST_ITERATOR_DONE:
        done = TRUE;
        break;
    }
  }
  gst_iterator_free (iter);
}

/**
 * gst_camerabin_drop_eos_probe:
 * @pad: pad receiving the event
 * @event: received event
 * @u_data: not used
 *
 * Event probe that drop all eos events.
 *
 * Returns: FALSE to drop the event, TRUE otherwise
 */
gboolean
gst_camerabin_drop_eos_probe (GstPad * pad, GstEvent * event, gpointer u_data)
{
  gboolean ret = TRUE;

  switch (GST_EVENT_TYPE (event)) {
    case GST_EVENT_EOS:
      GST_DEBUG ("dropping eos in %s:%s", GST_DEBUG_PAD_NAME (pad));
      ret = FALSE;
      break;
    default:
      break;
  }
  return ret;
}

static GstFlowReturn
gst_camerabin_preview_pipeline_new_preroll (GstAppSink * appsink,
    gpointer user_data)
{
  GstBuffer *buffer;

  buffer = gst_app_sink_pull_preroll (appsink);
  gst_buffer_unref (buffer);

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_camerabin_preview_pipeline_new_buffer (GstAppSink * appsink,
    gpointer user_data)
{
  GstBuffer *buffer;
  GstStructure *s;
  GstMessage *msg;
  GstCameraBinPreviewPipelineData *data;

  data = user_data;

  buffer = gst_app_sink_pull_buffer (appsink);
  s = gst_structure_new (GST_BASE_CAMERA_SRC_PREVIEW_MESSAGE_NAME,
      "buffer", GST_TYPE_BUFFER, buffer, NULL);
  gst_buffer_unref (buffer);
  msg = gst_message_new_element (GST_OBJECT (data->element), s);

  GST_DEBUG_OBJECT (data->element, "sending message with preview image");
  if (gst_element_post_message (data->element, msg) == FALSE) {
    GST_WARNING_OBJECT (data->element,
        "This element has no bus, therefore no message sent!");
  }

  return GST_FLOW_OK;
}

/**
 * gst_camerabin_create_preview_pipeline:
 * @element: Owner of this pipeline
 * @filter: Custom filter to process preview data (an extra ref is taken)
 *
 * Creates a new previewing pipeline that can receive buffers
 * to be posted as camerabin preview messages for @element
 *
 * Returns: The newly created #GstCameraBinPreviewPipelineData
 */
GstCameraBinPreviewPipelineData *
gst_camerabin_create_preview_pipeline (GstElement * element,
    GstElement * filter)
{
  GstCameraBinPreviewPipelineData *data;
  GstElement *csp;
  GstElement *csp2;
  GstElement *vscale;
  gboolean added = FALSE;
  GstAppSinkCallbacks callbacks = { 0, };

  data = g_new (GstCameraBinPreviewPipelineData, 1);

  data->pipeline = gst_pipeline_new ("preview-pipeline");
  data->appsrc = gst_element_factory_make ("appsrc", "preview-appsrc");
  data->capsfilter = gst_element_factory_make ("capsfilter",
      "preview-capsfilter");
  data->appsink = gst_element_factory_make ("appsink", "preview-appsink");
  csp = gst_element_factory_make ("ffmpegcolorspace", "preview-csp0");
  csp2 = gst_element_factory_make ("ffmpegcolorspace", "preview-csp1");
  vscale = gst_element_factory_make ("videoscale", "preview-vscale");

  if (!data->appsrc || !data->capsfilter || !data->appsink || !csp ||
      !csp2 || !vscale) {
    goto error;
  }

  gst_bin_add_many (GST_BIN (data->pipeline), data->appsrc, data->capsfilter,
      data->appsink, csp, csp2, vscale, NULL);
  if (filter)
    gst_bin_add (GST_BIN (data->pipeline), gst_object_ref (filter));
  added = TRUE;

  if (filter) {
    if (!gst_element_link_many (data->appsrc, filter, csp, vscale, csp2,
            data->capsfilter, data->appsink, NULL))
      goto error;
  } else {
    if (!gst_element_link_many (data->appsrc, csp, vscale, csp2,
            data->capsfilter, data->appsink, NULL))
      goto error;
  }

  callbacks.new_preroll = gst_camerabin_preview_pipeline_new_preroll;
  callbacks.new_buffer = gst_camerabin_preview_pipeline_new_buffer;
  gst_app_sink_set_callbacks ((GstAppSink *) data->appsink, &callbacks, data,
      NULL);

  g_object_set (data->appsink, "sync", FALSE, NULL);

  data->element = element;
  data->filter = filter;

  return data;
error:
  GST_WARNING ("Failed to create camerabin's preview pipeline");
  if (!added) {
    if (csp)
      gst_object_unref (csp);
    if (csp2)
      gst_object_unref (csp2);
    if (vscale)
      gst_object_unref (vscale);
    if (data->appsrc)
      gst_object_unref (data->appsrc);
    if (data->capsfilter)
      gst_object_unref (data->capsfilter);
    if (data->appsink)
      gst_object_unref (data->appsink);
  }
  gst_camerabin_destroy_preview_pipeline (data);
  return NULL;
}

/**
 * gst_camerabin_destroy_preview_pipeline:
 * @preview: the #GstCameraBinPreviewPipelineData
 *
 * Frees a #GstCameraBinPreviewPipelineData
 */
void
gst_camerabin_destroy_preview_pipeline (GstCameraBinPreviewPipelineData *
    preview)
{
  if (preview->pipeline) {
    gst_element_set_state (preview->pipeline, GST_STATE_NULL);
    gst_object_unref (preview->pipeline);
  }
  g_free (preview);
}

/**
 * gst_camerabin_preview_pipeline_post:
 * @preview: the #GstCameraBinPreviewPipelineData
 * @buffer: the buffer to be posted as a preview
 *
 * Converts the @buffer to the desired format and posts the preview
 * message to the bus.
 *
 * Returns: %TRUE on success
 */
gboolean
gst_camerabin_preview_pipeline_post (GstCameraBinPreviewPipelineData * preview,
    GstBuffer * buffer)
{
  g_return_val_if_fail (preview != NULL, FALSE);
  g_return_val_if_fail (preview->pipeline != NULL, FALSE);
  g_return_val_if_fail (buffer, FALSE);

  gst_app_src_push_buffer ((GstAppSrc *) preview->appsrc,
      gst_buffer_ref (buffer));

  return TRUE;
}

/**
 * gst_camerabin_preview_set_caps:
 * @preview: the #GstCameraBinPreviewPipelineData
 * @caps: the #GstCaps to be set
 *
 * The caps that preview buffers should have when posted
 * on the bus 
 */
void
gst_camerabin_preview_set_caps (GstCameraBinPreviewPipelineData * preview,
    GstCaps * caps)
{
  GstState state, pending;
  GstStateChangeReturn ret;

  g_return_if_fail (preview != NULL);

  ret = gst_element_get_state (preview->pipeline, &state, &pending, 0);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    /* make it try again */
    state = GST_STATE_PLAYING;
    pending = GST_STATE_VOID_PENDING;
  }

  gst_element_set_state (preview->pipeline, GST_STATE_NULL);
  g_object_set (preview->capsfilter, "caps", caps, NULL);
  if (pending != GST_STATE_VOID_PENDING)
    state = pending;
  gst_element_set_state (preview->pipeline, state);
}