#include "stdafx.h"
#include "CMissile.h"

CMissile::CMissile(cv::Point2f start_pos, cv::Point velocity, cv::Scalar colour)
{
    _position = start_pos; 
    _shape = cv::Rect(_position.x, _position.y, 5, 10);  // Use _position for the shape's top-left corner
    _colour = colour;
    _velocity = velocity;
}



