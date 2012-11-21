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

/* This breaks the build for reasons that aren't entirely clear to me yet */
//#if 0
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
//#endif

#include <stdio.h>
#include "featurematcher_wrapper.h"

static int instanceCounter = 0;
static gboolean element_id_was_max = false;

FeatureMatcher *fm;
char p_string[] = "idx failed";

int
feature_matcher_init ()
{
  fm = new FeatureMatcher ();
  instanceOfFM tmpfm;
  tmpfm.id = instanceCounter;
  tmpfm.fm = fm;
  featurematchervector.push_back (tmpfm);
  if ((instanceCounter < INT_MAX) && !element_id_was_max) {
    instanceCounter++;
    element_id_was_max = false;
  } else {
    element_id_was_max = true;
    instanceCounter = featurematcherfreeids.back ();
    featurematcherfreeids.pop_back ();
  }
  return tmpfm.id;
}

int
search_idx (int p_id)
{
  for (unsigned int i = 0; i < featurematchervector.size (); i++) {
    instanceOfFM tmpfm;
    tmpfm = featurematchervector.at (i);
    if (tmpfm.id == p_id) {
      return i;
    }
  }
  return -1;
}

int
find_matching_surf_keypoints(IplImage* image0,
                                IplImage* image1,
                                CvPoint2D32f** keypoints0,
                                CvPoint2D32f** keypoints1,
                                int *n_matches,
                                int p_id)
{
    int ret = 0;
    int idx = 0;
    idx = search_idx ( p_id);
    std::vector<cv::Point2f> matched_points0;
    std::vector<cv::Point2f> matched_points1;
    ret = featurematchervector.at (idx).fm -> findMatchingSURFKeypoints (image0,
                                                                         image1,
                                                                         matched_points0,
                                                                         matched_points1);
    if (ret != 0) {
        std::cout << "ERROR retrieving matching keypoints" << std::endl;
    } else {
        std::cout << "wrapper: Number of retrieved matching keypoints: "
            << matched_points0.size() << std::endl;
        *keypoints0 = (CvPoint2D32f*) malloc ( matched_points0.size() * sizeof(CvPoint2D32f) );
        *keypoints1 = (CvPoint2D32f*) malloc ( matched_points1.size() * sizeof(CvPoint2D32f) );
        for( int i = 0; i < (int) matched_points0.size(); i++ )
        {
            std::cout << "wrapper: assigning point 0: " << i << "; x = " << matched_points0[i].x
                << "; y = " << matched_points0[i].y << std::endl;
            std::cout << "wrapper: assigning point 1: " << i << "; x = " << matched_points1[i].x
                << "; y = " << matched_points1[i].y << std::endl;
            (*keypoints0)[i] = cvPoint2D32f(matched_points0[i].x, matched_points0[i].y);
            std::cout << "wrapper: assigned." << std::endl;
            (*keypoints1)[i] = cvPoint2D32f(matched_points1[i].x, matched_points1[i].y);
        }
        std::cout << "wrapper: generated array from vector of keypoints." << std::endl;
        *n_matches = (int) matched_points0.size();
    }
    return 0;
}

void
feature_matcher_free (int p_id)
{
  int idx = 0;
  idx = search_idx (p_id);
  if (idx > -1) {
    delete featurematchervector.at (idx).fm;
    featurematchervector.erase (featurematchervector.begin () + idx);
    featurematcherfreeids.push_back (p_id);
  }
}
