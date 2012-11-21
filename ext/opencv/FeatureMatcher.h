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

#ifndef FEATUREMATCHER_H_
#define FEATUREMATCHER_H_

#include <cv.h>                 // includes OpenCV definitions
#include <glib.h>

//FeatureMatcher defines
#define FM_SURF_THRESHOLD 3000

using namespace std;

//FeatureMatcher structs

//FeatureMatcher class
class FeatureMatcher
{
public:

  FeatureMatcher ();
  virtual ~ FeatureMatcher ();

  // public methods
  int findMatchingSURFKeypoints(IplImage* image0,
                                IplImage* image1,
                                std::vector<cv::Point2f> &matched_keypoints0,
                                std::vector<cv::Point2f> &matched_keypoints1);

private:

  // private methods

  // private variables
  cv::SurfFeatureDetector *surf;
  cv::SurfDescriptorExtractor *surfDesc;
  cv::BruteForceMatcher<cv::L2<float> > *matcher;

};

#endif /* FEATUREMATCHER_H_ */
