#include "stdafx.h"
#include "CInvader.h"


CInvader::CInvader(cv::Point2f position, cv::Point2f velocity, cv::Scalar colour)
{
    _position = position;
    _velocity = velocity;
    _shape = cv::Rect(position.x, position.y, 30, 20); // Assuming fixed dimensions for invader shape
    _colour = colour;
    _lives = 3;  // Default number of lives
}
