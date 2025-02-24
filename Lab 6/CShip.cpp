#include "stdafx.h"
#include "CShip.h"

CShip::CShip() 
{
    _position = cv::Point2f(500, 900);
    _velocity = cv::Point2f(100, 100);
    _lives = 9;
    _colour = cv::Scalar(127, 0, 200);
    _shape = cv::Rect(_position.x, _position.y, 40, 40);
}
