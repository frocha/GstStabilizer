#include "g-finder.h"
#include "g-surffinder.h"
#include <opencv2/highgui/highgui_c.h>


void showImage (void);

void
showImage (void)
{
  IplImage *img;
  cvNamedWindow ("test", 1);
  img = cvLoadImage ("image1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  cvShowImage ("test", img);
  while (1) {
    if (cvWaitKey (100) == 27)
      break;
  }
  cvDestroyWindow ("test");
  cvReleaseImage (&img);
}

int
main (int argc, char *argv[])
{

  GFinder *finder;
  IplImage *image0;
  IplImage *image1;

  CvPoint2D32f *keypoints0, *keypoints1;
  int n_matches;
  int i;

  g_type_init ();

  finder = G_FINDER (g_surffinder_new ());

  image0 = cvLoadImage ("image1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  image1 = cvLoadImage ("image2.jpg", CV_LOAD_IMAGE_GRAYSCALE);

  g_finder_optical_flow_image (finder, image0, image1, &keypoints0, &keypoints1,
      &n_matches);

  g_print ("test-g-finder: n_matches = %d", n_matches);
  for (i = 0; i < n_matches; i++)
    g_print ("test-g-finder: %d: (%f, %f)\n", i, (keypoints0[i]).x,
        (keypoints0[i]).y);

  cvReleaseImage (&image0);
  cvReleaseImage (&image1);
  return 0;

}
