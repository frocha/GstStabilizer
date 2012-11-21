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
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */

#ifndef __G_SURFFINDER_H__
#define __G_SURFFINDER_H__

#include "g-finder.h"
#include <opencv/cv.h>

/* other include files */

G_BEGIN_DECLS

/* convenience macros */
#define G_TYPE_SURFFINDER             (g_surffinder_get_type())
#define G_SURFFINDER(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj),G_TYPE_SURFFINDER,GSURFFinder))
#define G_SURFFINDER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),G_TYPE_SURFFINDER,GSURFFinderClass))
#define G_IS_SURFFINDER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj),G_TYPE_SURFFINDER))
#define G_IS_SURFFINDER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),G_TYPE_SURFFINDER))
#define G_SURFFINDER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj),G_TYPE_SURFFINDER,GSURFFinderClass))

typedef struct _GSURFFinder      GSURFFinder;
typedef struct _GSURFFinderClass GSURFFinderClass;
typedef struct _GSURFFinderPrivate         GSURFFinderPrivate;

struct _GSURFFinder {
	 GFinder parent;
	 int id;
	/* public members, if any */

	/* private */
	GSURFFinderPrivate *_priv;
};

struct _GSURFFinderClass {
	GFinderClass parent_class;
	/* signal callback declarations, e.g. */
	/* void (* my_event) (GSURFFinder* obj); */
};

/* member functions */
GType        g_surffinder_get_type    (void) G_GNUC_CONST;

/* parameter-less _new function (constructor) */
GSURFFinder*    g_surffinder_new         (void);

/* other public functions */
void g_surffinder_optical_flow_image (GSURFFinder * self,
                                      IplImage * image0,
                                      IplImage * image1,
                                      CvPoint2D32f **keypoints0,
                                      CvPoint2D32f **keypoints1,
                                      int* n_matches);
G_END_DECLS

#endif /* __G_SURFFINDER_H__ */
