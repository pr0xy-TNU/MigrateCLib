#include <jni.h>
#include <string>
#include <iostream>
#include "SizerFwkLite.h"

SizerFwkLite *SizerFwkLite::m_instance = NULL;

using namespace std;

float m_StepintoTimestampStart = 0.0;

float poseMaximalTimouts[32];
int poseFrameCount[32] = {0};

SizerFwkLite::SizerFwkLite() {
  m_curDetectedPosition = SizerPose::ScanNotStarted;
  m_bUseCurFrameForPostMeasurement = false;

  poseMaximalTimouts[ScanNotStarted] = 0.0;
  poseMaximalTimouts[StepIntoTheOutline] = 10.0;
  poseMaximalTimouts[LegsApart] = 12.0;
  poseMaximalTimouts[LegsTogether] = 12.0;
  poseMaximalTimouts[SpradArms] = 12.0;
  poseMaximalTimouts[RaiseHands] = 15.0;
  poseMaximalTimouts[RotationEnded] = 32.0;
  poseMaximalTimouts[LowerArms] = 8.0;
}


SizerFwkLite::~SizerFwkLite() {
}


SizerFwkLite *SizerFwkLite::getInstance() {
  if (!m_instance) {
    m_instance = new SizerFwkLite();
  }

  return m_instance;
}

void SizerFwkLite::SetCurrentDetectedPose(SizerPose newPose, int frameid) {
  m_curDetectedPosition = newPose;
}

bool SizerFwkLite::isPoseTimeout(const float RGBImageTimestamp) {
  float poseTimeDiff = RGBImageTimestamp - m_CurPoseTimestampStart;
  if (poseTimeDiff >= poseMaximalTimouts[m_curDetectedPosition]) {
    return true;
  }

  return false;
}

void SizerFwkLite::useFrameForPostMeasurement(const int frameID) {
  m_bUseCurFrameForPostMeasurement = true;
}


bool SizerFwkLite::resetPoseIfNeeded(float RGBImageTimestamp) {
  if (poseFrameCount[m_curDetectedPosition] <= 1) {
    m_curDetectedPositionError = SizerPoseError::PoseError_NoError;
    m_CurPoseTimestampStart = RGBImageTimestamp;
    return true;
  } else {
    m_bUseCurFrameForPostMeasurement = false;
    return false;
  }
}

void SizerFwkLite::DetectPose(const int frameID,
                              const float RGBImageTimestamp,
                              const float deviceTiltDegrees,
                              const int head_top_x,
                              const int head_top_y,
                              const int head_width,
                              const int head_height) {

  switch (m_curDetectedPosition) {
    case SizerPose::ScanNotStarted: {
      SetCurrentDetectedPose(SizerPose::StepIntoTheOutline, frameID);
      m_curDetectedPositionError = SizerPoseError::PoseError_NoError;
      m_CurPoseTimestampStart = RGBImageTimestamp;
      break;
    }
    case SizerPose::StepIntoTheOutline: {
      bool bResetDone = resetPoseIfNeeded(RGBImageTimestamp);
      if (isPoseTimeout(RGBImageTimestamp)) {
        SetCurrentDetectedPose(SizerPose::LegsApart, frameID);
      }
      break;
    }
    case SizerPose::StepOutOfView: {
      SetCurrentDetectedPose(SizerPose::LegsApart, frameID);
      break;
    }
    case SizerPose::LearningBackground: {
      SetCurrentDetectedPose(SizerPose::LegsApart, frameID);
      break;
    }
    case SizerPose::StepBackIntoView: {
      SetCurrentDetectedPose(SizerPose::LegsApart, frameID);
      break;
    }
    case SizerPose::LegsApart: {
      bool bResetDone = resetPoseIfNeeded(RGBImageTimestamp);
      if (bResetDone) useFrameForPostMeasurement(frameID);

      if (isPoseTimeout(RGBImageTimestamp)) {
        SetCurrentDetectedPose(SizerPose::LegsTogether, frameID);
      }
      break;
    }
    case SizerPose::LegsTogether: {
      bool bResetDone = resetPoseIfNeeded(RGBImageTimestamp);
      if (bResetDone) useFrameForPostMeasurement(frameID);

      if (isPoseTimeout(RGBImageTimestamp)) {
        SetCurrentDetectedPose(SizerPose::SpradArms, frameID);
      }
      break;
    }
    case SizerPose::SpradArms: {
      bool bResetDone = resetPoseIfNeeded(RGBImageTimestamp);
      if (bResetDone) useFrameForPostMeasurement(frameID);

      if (isPoseTimeout(RGBImageTimestamp)) {
        SetCurrentDetectedPose(SizerPose::RaiseHands, frameID);
      }
      break;
    }
    case SizerPose::RaiseHands: {
      bool bResetDone = resetPoseIfNeeded(RGBImageTimestamp);
      if (bResetDone) useFrameForPostMeasurement(frameID);

      if (isPoseTimeout(RGBImageTimestamp)) {
        SetCurrentDetectedPose(SizerPose::RotationEnded, frameID);
      }
      break;
    }
    case SizerPose::RotationEnded: {
      bool bResetDone = resetPoseIfNeeded(RGBImageTimestamp);
      if (frameID % 2)
        useFrameForPostMeasurement(frameID);

      if (isPoseTimeout(RGBImageTimestamp)) {
        SetCurrentDetectedPose(SizerPose::LowerArms, frameID);
      }
      break;
    }
    case SizerPose::LowerArms: {
      bool bResetDone = resetPoseIfNeeded(RGBImageTimestamp);

      if (isPoseTimeout(RGBImageTimestamp)) {
        SetCurrentDetectedPose(SizerPose::ScanEnded, frameID);
      }
      break;
    }
    default: {
      break;
    }
  }
  poseFrameCount[m_curDetectedPosition] = 1 + poseFrameCount[m_curDetectedPosition];
  //	AddSizerFrameInfo(frameID, m_CurrentDeviceTiltDegrees, RGBImageTimestamp, GetCurrentDetectedPose(), SFI, binFilename, m_BinImagesManager.m_scoreFilename, m_BinImagesManager.m_postbinFilename);
}

void SizerFwkLite::AddPostMeasurementImage(int frameID) {
  m_bUseCurFrameForPostMeasurement = true;
}

std::string SizerFwkLite::GetCurDetectedPoseDescription() {
  return GetPoseDescription(m_curDetectedPosition);
}

std::string SizerFwkLite::GetPoseDescription(SizerPose pose) {
  switch (pose) {
    case ScanNotStarted:
      return "Not started";
    case StepIntoTheOutline:
      return "Step into the outline";
    case StepOutOfView:
      return "Step out of view";
    case LearningBackground:
      return "Learning Background";
    case StepBackIntoView:
      return "Step back into view";
    case LegsApart:
      return "Legs apart";
    case LegsTogether:
      return "Legs together";
    case SpradArms:
      return "Spread Arms";
    case RaiseHands:
      return "Raise Hands";
    case Rotating:
      return "Rotating";
    case RotationStarted:
      return "Rotation Started";
    case Rotation90:
      return "Rotation 90";
    case Rotation180:
      return "Rotation 180";
    case Rotation270:
      return "Rotation 270";
    case RotationEnded:
      return "Rotation Ended";
    case LowerArms:
      return "Lower your arms";
    case ScanEnded:
      return "Scan completed";
    default:
      return "Unknown";
  }
}

std::string SizerFwkLite::getVersion() {
  return "1.0";
}

std::string SizerFwkLite::getInfo() {

  std::string buffer;
  for (int i = 0; i < 32; ++i) {
    if (poseMaximalTimouts[i] == 0) {
      buffer += "2";
    } else {
      buffer += poseMaximalTimouts[i];
    }
    buffer += " ";
  }
  buffer += '\0';

  return buffer;

}


SizerFwkLite *c_instance = NULL;
SizerPose getNativePose(int pose);

extern "C"
JNIEXPORT void JNICALL
Java_com_hardtopcrm_metaldud_test3_SizerFwkLite_SizerFwkLiteNative(JNIEnv *env, jclass instance) {

  // TODO
  c_instance = SizerFwkLite::getInstance();

}extern "C"
JNIEXPORT void JNICALL
Java_com_hardtopcrm_metaldud_test3_SizerFwkLite_detectPoseNative(JNIEnv *env,
                                                                 jclass instance,
                                                                 jint frameID,
                                                                 jfloat RGBImageTimestamp,
                                                                 jfloat deviceTiltDegrees,
                                                                 jint head_top_x,
                                                                 jint head_top_y,
                                                                 jint head_width,
                                                                 jint head_height) {

  // TODO
  c_instance->DetectPose(frameID,
                         RGBImageTimestamp,
                         deviceTiltDegrees,
                         head_top_x,
                         head_top_y,
                         head_width,
                         head_height);
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_hardtopcrm_metaldud_test3_SizerFwkLite_getCurDetectedPoseDescriptionNative(JNIEnv *env,
                                                                                    jclass instance) {

  // TODO
  std::string temp = c_instance->GetCurDetectedPoseDescription();
  return env->NewStringUTF(temp.c_str());
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_hardtopcrm_metaldud_test3_SizerFwkLite_getPoseDescriptionNative(JNIEnv *env,
                                                                         jclass instance,
                                                                         jint sizerPose) {

  // TODO
  SizerPose pose = getNativePose(sizerPose);
  std::string temp = c_instance->GetPoseDescription(pose);
  return env->NewStringUTF(temp.c_str());
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_hardtopcrm_metaldud_test3_SizerFwkLite_getVersionNative(JNIEnv *env, jclass instance) {

  // TODO
  std::string temp = c_instance->getVersion();
  return env->NewStringUTF(temp.c_str());
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_hardtopcrm_metaldud_test3_SizerFwkLite_getInfoNative(JNIEnv *env, jclass instance) {

  // TODO
  std::string temp = c_instance->getInfo();
  return env->NewStringUTF(temp.c_str());
}

SizerPose getNativePose(int pose) {
  int poseFromJava = pose;
  SizerPose nativePose;
  // TODO
  switch (poseFromJava) {
    case 0:
      nativePose = ScanNotStarted;
      break;
    case 1:
      nativePose = StepIntoTheOutline;
      break;
    case 2:
      nativePose = StepOutOfView;
      break;
    case 3:
      nativePose = LearningBackground;
      break;
    case 4:
      nativePose = StepBackIntoView;
      break;
    case 5:
      nativePose = LegsApart;
      break;
    case 6:
      nativePose = LegsTogether;
      break;
    case 8:
      nativePose = SpradArms;
      break;
    case 9:
      nativePose = RaiseHands;
      break;
    case 10:
      nativePose = Rotating;
      break;
    case 12:
      nativePose = Rotation90;
      break;
    case 13:
      nativePose = Rotation180;
      break;
    case 14:
      nativePose = Rotation270;
      break;
    case 15:
      nativePose = RotationEnded;
      break;
    case 16:
      nativePose = LowerArms;
      break;
    case 17:
      nativePose = ScanEnded;
      break;
  }

  return nativePose;

}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_hardtopcrm_metaldud_test3_SizerFwkLite_shouldUseCurrentFrameForPostMeasurementNative(JNIEnv *env,
                                                                                              jclass type) {

  // TODO
  bool temp = c_instance->shouldUseCurrentFrameForPostMeasurement();
  return (jboolean) temp;
}