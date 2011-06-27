/*=====================================================================

PIXHAWK Micro Air Vehicle Flying Robotics Toolkit

(c) 2009-2011 PIXHAWK PROJECT  <http://pixhawk.ethz.ch>

This file is part of the PIXHAWK project

    PIXHAWK is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    PIXHAWK is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with PIXHAWK. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

/**
* @file
*   @brief Shared memory interface for reading images.
*
*   This interface is a wrapper around PxSHM.
*
*   @author Lionel Heng  <hengli@inf.ethz.ch>
*
*/

#ifndef PXSHMIMAGECLIENT_H
#define PXSHMIMAGECLIENT_H

#include <mavconn.h>
#include <opencv2/core/core.hpp>

#include "PxSHM.h"

class PxSHMImageClient
{
public:
	PxSHMImageClient();
	
	/**
	 * Initializes the image client.
	 *
	 * @param subscribeLatest If true, all read functions skip all stored
	 * 						  images until the most recent image which is
	 * 						  then returned. Otherwise, the next available
	 * 						  image is returned; the function has to be called
	 * 						  quickly enough to avoid overwriting of image data.
	 * @param cam1 Camera 1. If it is part of a stereo rig, it is the left camera.
	 * @param cam2 Camera 2. If it is part of a stereo rig, it is the right camera.
	 * 						 Otherwise, leave the parameter empty.
	 *
	 * @return Result of shared memory segment access.
	 */
	bool init(bool subscribeLatest,
			  PxSHM::Camera cam1, PxSHM::Camera cam2 = PxSHM::CAMERA_NONE);
	
	static uint64_t getTimestamp(const mavlink_message_t* msg);
	static uint64_t getValidUntil(const mavlink_message_t* msg);
	static uint64_t getCameraID(const mavlink_message_t* msg);
	static int getCameraNo(const mavlink_message_t* msg);
	static bool getRollPitch(const mavlink_message_t* msg, float& roll, float& pitch);
	static bool getRollPitchYaw(const mavlink_message_t* msg, float& roll, float& pitch, float& yaw);
	static bool getLocalHeight(const mavlink_message_t* msg, float& height);
	static bool getGPS(const mavlink_message_t* msg, float& lat, float& lon, float& alt);
	
	bool readMonoImage(const mavlink_message_t* msg, cv::Mat& img);
	bool readStereoImage(const mavlink_message_t* msg, cv::Mat& imgLeft, cv::Mat& imgRight);
	bool readKinectImage(const mavlink_message_t* msg, cv::Mat& imgBayer, cv::Mat& imgDepth);

private:	
	bool readCameraProperties(int& cameraType, int& imageWidth,
							  int& imageHeight, int& imageType);
	
	bool subscribeLatest;
	int cameraType;
	int imageWidth;
	int imageHeight;
	int imageType;
	
	PxSHM shm;
};

#endif
