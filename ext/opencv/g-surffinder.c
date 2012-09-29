/* g-surffinder.c */

#include "g-surffinder.h"
/* include other impl specific header files */

/* 'private'/'protected' functions */
static void g_surffinder_class_init (GSURFFinderClass * klass);
static void g_surffinder_init (GSURFFinder * obj);
static void g_surffinder_finalize (GObject * obj);
static void g_suffinder_get_surf (GSURFFinder * self, IplImage * image);

/* list signals  */
enum
{
  /* MY_SIGNAL_1, */
  /* MY_SIGNAL_2, */
  LAST_SIGNAL
};

struct _GSURFFinderPrivate
{
  /* private members go here */
  gboolean _frobnicate_mode;
};
#define G_SURFFINDER_GET_PRIVATE(o)      (G_TYPE_INSTANCE_GET_PRIVATE((o), \
                                          G_TYPE_SURFFINDER, \
                                          GSURFFinderPrivate))
/* globals */
static GFinderClass *parent_class = NULL;

/* uncomment the following if any signals are defined */
/* static guint signals[LAST_SIGNAL] = {0}; */

G_DEFINE_TYPE (GSURFFinder, g_surffinder, G_TYPE_FINDER);

static void
g_surffinder_class_init (GSURFFinderClass * klass)
{
  GObjectClass *gobject_class;
  gobject_class = (GObjectClass *) klass;

  gobject_class->finalize = g_surffinder_finalize;

  g_type_class_add_private (gobject_class, sizeof (GSURFFinderPrivate));

  /* signal definitions go here, e.g.: */
/*	signals[MY_SIGNAL_1] = */
/*		g_signal_new ("my_signal_1",....); */
/*	signals[MY_SIGNAL_2] = */
/*		g_signal_new ("my_signal_2",....); */
/*	etc. */

  GFinderClass *gfinder_class = (GFinderClass *) klass;
  gfinder_class->optical_flow_image = (void *) g_surffinder_do_something2;
}

static void
g_surffinder_init (GSURFFinder * obj)
{
  obj->_priv = G_SURFFINDER_GET_PRIVATE (obj);

/* init any of the private data */
  obj->_priv->_frobnicate_mode = FALSE;
}

static void
g_surffinder_finalize (GObject * obj)
{
/*	free/unref instance resources here */
  G_OBJECT_CLASS (parent_class)->finalize (obj);
}

GSURFFinder *
g_surffinder_new (void)
{
  return G_SURFFINDER (g_object_new (G_TYPE_SURFFINDER, NULL));
}

/* following: other function implementations */
/* such as g_surffinder_do_something, or g_surffinder_has_foo */

void
g_surffinder_do_something2 (GSURFFinder * self, const gchar * param)
{
  g_message ("GSURFFinder: %s", param);
}

static void
g_suffinder_get_surf (GSURFFinder * self, IplImage * image)
{
  /* Convert the RGB image obtained from camera into Grayscale */
  /* FIXME the default color format in OpenCV is BGR. */
  IplImage *image_gray = cvCreateImage (cvGetSize (image), IPL_DEPTH_8U, 1);
  cvCvtColor (image, image_gray, CV_RGB2GRAY);

  CvMemStorage *storage = cvCreateMemStorage (0);
  CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
  CvSURFParams params = cvSURFParams (1000, 1);
  cvExtractSURF (image_gray, 0, &imageKeypoints, &imageDescriptors, storage,
      params, 0);
  /* FIXME printf("Image Descriptors: %d\n", imageDescriptors->total); */

  free (image_gray);
  free (storage);
  /* FIXME shape */
}
