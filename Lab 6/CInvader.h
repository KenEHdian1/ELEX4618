#pragma once
#include "CGameObject.h"

class CInvader : public CGameObject 
{
public:
    CInvader(cv::Point2f position, cv::Point2f velocity, cv::Scalar colour);
};

