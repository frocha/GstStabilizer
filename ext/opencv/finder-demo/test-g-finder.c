#include "../g-finder.h"
#include "../g-surffinder.h"
#include <opencv2/highgui/highgui_c.h>

int
showImage ()
{
  cvNamedWindow ("test", 1);
  IplImage *img = cvLoadImage ("church01.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  cvShowImage ("test", img);
  while (1) {
    if (cvWaitKey (100) == 27)
      break;
  }
  cvDestroyWindow ("test");
  cvReleaseImage (&img);
  return (0);
}

int
main (int argc, char *argv[])
{

  g_type_init ();

  GFinder *finder;
  finder = G_FINDER (g_surffinder_new ());

  IplImage *image0 = cvLoadImage ("church01.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  IplImage *image1 = cvLoadImage ("church02.jpg", CV_LOAD_IMAGE_GRAYSCALE);

  g_finder_optical_flow_image (finder, image0, image1);

  cvReleaseImage (&image0);
  cvReleaseImage (&image1);
  return 0;

}
