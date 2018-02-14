#pragma once

#include "stdio.h"
#include <string>

enum SizerPose
{
	ScanNotStarted=0,
	StepIntoTheOutline,
	StepOutOfView,
	LearningBackground,
	StepBackIntoView,
	LegsApart,
	LegsTogether,
	SpradArms,
	RaiseHands,
	Rotating,
	RotationStarted,
	Rotation90,
	Rotation180,
	Rotation270,
	RotationEnded,
	LowerArms,
	ScanEnded
};

enum SizerPoseError
{
	PoseError_NoError,
	PoseError_MoveToCenter,
	PoseError_StepBack,
	PoseError_RaiseArmsHigher,
	PoseError_SpreadArmsMore,
	PoseError_UserNotInScene,
	PoseError_LegsApart,
	PoseError_LegsTogether,
	PoseError_UserInScene,
	PoseError_Validation_SceneError,
	PoseError_UserStandsTooLow,
	PoseError_BackgroundLearningTooLong,
	PoseError_UserStandTooClose
};


class SizerFwkLite
{
public:
	SizerFwkLite();
	~SizerFwkLite();

	static SizerFwkLite* getInstance();
	std::string getVersion();

	SizerPose GetCurrentDetectedPose() { return m_curDetectedPosition; }
	SizerPoseError GetCurrentDetectedPoseError() { return m_curDetectedPositionError; }

	void DetectPose(const int frameID, const float RGBImageTimestamp,
		const float deviceTiltDegrees,
		const int head_top_x, const int head_top_y, const int head_width, const int head_height);
		
	std::string GetPoseDescription(SizerPose pose);
	std::string GetCurDetectedPoseDescription();
	bool shouldUseCurrentFrameForPostMeasurement() { return m_bUseCurFrameForPostMeasurement; }
  	std::string getInfo();
private:
	static SizerFwkLite* m_instance;

	SizerPose			 m_curDetectedPosition;
	SizerPoseError  	 m_curDetectedPositionError;
	bool				 m_bUseCurFrameForPostMeasurement;
	float 				 m_CurPoseTimestampStart;

	void SetCurrentDetectedPose(SizerPose newPose, int frameid);
	bool isPoseTimeout(const float RGBImageTimestamp);
	bool resetPoseIfNeeded(float RGBImageTimestamp); // return true if reset was done
	void AddPostMeasurementImage(int frameID);
	void useFrameForPostMeasurement(const int frameID);

};


