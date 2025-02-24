#pragma once
#include "CGameObject.h"

class CMissile : public CGameObject 
{
public:
    CMissile(cv::Point2f start_pos, cv::Point veloctiy, cv::Scalar colour);
};
