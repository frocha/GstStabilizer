/* g-finder.h */

#ifndef __G_FINDER_H__
#define __G_FINDER_H__

#include <glib-object.h>
#include <cv.h>
/* other include files */

G_BEGIN_DECLS

/* convenience macros */
#define G_TYPE_FINDER             (g_finder_get_type())
#define G_FINDER(obj)             (G_TYPE_CHECK_INSTANCE_CAST((obj),G_TYPE_FINDER,GFinder))
#define G_FINDER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST((klass),G_TYPE_FINDER,GFinderClass))
#define G_IS_FINDER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE((obj),G_TYPE_FINDER))
#define G_IS_FINDER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE((klass),G_TYPE_FINDER))
#define G_FINDER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS((obj),G_TYPE_FINDER,GFinderClass))

typedef struct _GFinder      GFinder;
typedef struct _GFinderClass GFinderClass;
typedef struct _GFinderPrivate         GFinderPrivate;

struct _GFinder {
	 GObject parent;
	/* insert public members, if any */

	/* private */
	GFinderPrivate *_priv;
};

struct _GFinderClass {
	GObjectClass parent_class;
	/* insert signal callback declarations, e.g. */
	/* void (* my_event) (GFinder* obj); */

	/* virtual public method */
    void (*optical_flow_image) (GFinder *self, IplImage* image0, IplImage* image1,
                                CvPoint2D32f **keypoints0, CvPoint2D32f **keypoints1,
                                int *n_matches);
};

/* member functions */
GType        g_finder_get_type    (void) G_GNUC_CONST;

/* parameter-less _new function (constructor) */
/* if this is a kind of GtkWidget, it should probably return at GtkWidget* */
GFinder*    g_finder_new         (void);

/* fill in other public functions */
void g_finder_optical_flow_image (GFinder *self, IplImage* image0, IplImage* image1,
                                  CvPoint2D32f **keypoints0, CvPoint2D32f **keypoints1,
                                  int *n_matches);

G_END_DECLS

#endif /* __G_FINDER_H__ */

