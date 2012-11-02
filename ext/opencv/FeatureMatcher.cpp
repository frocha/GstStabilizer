/*
 * Copyright (C) 2012 Francisco Rocha <rocha.francisco.a@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
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

/* This breaks the build for reasons that aren't entirely clear to me yet */
//#if 0
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
//#endif

#include "FeatureMatcher.h"
#include <opencv2/highgui/highgui.hpp>

# define FM_DEBUG_MATCHES 1

FeatureMatcher::FeatureMatcher ()
{
	this->surf = new cv::SurfFeatureDetector(FM_SURF_THRESHOLD);
	this->surfDesc = new cv::SurfDescriptorExtractor();
	this->matcher = new cv::BruteForceMatcher<cv::L2<float> > ();
}

FeatureMatcher::~FeatureMatcher ()
{
    delete (this->surf);
    delete (this->surfDesc);
    delete (this->matcher);
}

int
FeatureMatcher::findMatchingSURFKeypoints(IplImage* image0,
                                          IplImage* image1,
                                          std::vector<cv::Point2f> &matched_keypoints0,
                                          std::vector<cv::Point2f> &matched_keypoints1)
{
    // Feature detection
	std::vector<cv::KeyPoint> keypoints0;
	std::vector<cv::KeyPoint> keypoints1;

	surf->detect(image0,keypoints0);
	surf->detect(image1,keypoints1);

	std::cout << "Number of SURF points 0: " << keypoints0.size() << std::endl;
	std::cout << "Number of SURF points 1: " << keypoints1.size() << std::endl;

    // Feature description

	cv::Mat descriptors0, descriptors1;
	surfDesc->compute(image0,keypoints0,descriptors0);
	surfDesc->compute(image1,keypoints1,descriptors1);

	std::cout << "Descriptor 0 size: " << descriptors0.rows << " by " << descriptors0.cols << std::endl;
	std::cout << "Descriptor 1 size: " << descriptors1.rows << " by " << descriptors1.cols << std::endl;

    // Feature matching

	std::vector<cv::DMatch> descMatches;
	matcher->match(descriptors0,descriptors1, descMatches);

	std::cout << "Found " << descMatches.size() << " matched descriptors." << std::endl;

    if (FM_DEBUG_MATCHES) {
        // Draw the best 25 matches
        std::nth_element(descMatches.begin(),
                         descMatches.begin()+24,
                         descMatches.end());
        descMatches.erase(descMatches.begin()+25, descMatches.end());

        cv::Mat imageMatches;
        cv::drawMatches(image0,keypoints0,
                        image1,keypoints1,
                        descMatches,
                        imageMatches,
                        cv::Scalar(0,0,255)); // color of the lines

        cv::imwrite("/var/tmp/matches.jpg", imageMatches);
        for( int i = 0; i < (int) descMatches.size(); i++ )
        {
            std::cout << i << ": (" << keypoints0[ descMatches[i].queryIdx ].pt.x
                << ", " << keypoints0[ descMatches[i].queryIdx ].pt.y << ")" << std::endl;
            std::cout << i << ": (" << keypoints1[ descMatches[i].queryIdx ].pt.x
                << ", " << keypoints1[ descMatches[i].queryIdx ].pt.y << ")" << std::endl;
        }
    }

	std::cout << "Rearranging matched keypoints." << std::endl;
    // Rearrange keypoints so that matched ones share index
    std::vector<cv::KeyPoint> keypoints_img0;
    std::vector<cv::KeyPoint> keypoints_img1;
    for( int i = 0; i < (int) descMatches.size(); i++ )
    {
        keypoints_img0.push_back( keypoints0[ descMatches[i].queryIdx ] );
        keypoints_img1.push_back( keypoints1[ descMatches[i].trainIdx ] );
    }

    // TODO Filter keypoints with diverging angles

    // Return only the coordinates of each keypoint
    // FIXME This needs to be improved by avoiding the following for loop.
    // A vector of Keypoints must be returned and the coordinates filtered
    // in the feature matcher.
    for( int i = 0; i < (int) descMatches.size(); i++ )
    {
        matched_keypoints0.push_back( keypoints0[ descMatches[i].queryIdx ].pt );
        matched_keypoints1.push_back( keypoints1[ descMatches[i].trainIdx ].pt );
    }
	return 0;
}
