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

#ifndef FEATUREMATCHER_WRAPPER_H
#define FEATUREMATCHER_WRAPPER_H

#include <stdbool.h>

#ifdef __cplusplus
#include "FeatureMatcher.h"
struct instanceOfFM
{
  int id;
  FeatureMatcher *fm;
};
vector < instanceOfFM > featurematchervector;
vector < int >featurematcherfreeids;

int search_idx (int p_id);
extern "C"
{
#endif

  int feature_matcher_init ();
  int find_matching_surf_keypoints(IplImage* image0,
                                IplImage* image1,
                                CvPoint2D32f** keypoints0,
                                CvPoint2D32f** keypoints1,
                                int *n_matches,
                                int p_id);
  void feature_matcher_free (int p_id);

#ifdef __cplusplus
}
#endif

#endif  /* FEATUREMATCHER_WRAPPER_H */
