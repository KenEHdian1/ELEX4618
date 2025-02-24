#include "stdafx.h"
#include "CGameObject.h"
#include "cvui.h"

void CGameObject::draw(cv::Mat& im)
{
	cv::Rect object(_position.x, _position.y, _shape.width, _shape.height);

	cv::rectangle(im, object, _colour, -1, cv::LINE_AA);
}

void CGameObject::move()
{
    //Timers
    double deltaT = (cv::getTickCount() - _objectTime) / cv::getTickFrequency();
    _objectTime = cv::getTickCount();

    if (_velocity.y == -10)
    {
        // Missile logic: Move with a constant velocity
        _position += _velocity; 
    }
    else if (_velocity.y == 5)
    {
        // Missile logic: Move with a constant velocity
        _position += _velocity;
    }
    else
    {
        // General object logic: Apply time scaling
        _position += _velocity * deltaT;  // Apply time scaling for other objects
    }

    // Update the shape's position based on the new position
    _shape = cv::Rect(_position, _shape.size());

    // Update the position using the set_pos function (if needed)
    set_pos(_position);
}

bool CGameObject::collide_wall(cv::Size board)
{
	// Check collision with left and right walls
	return (_position.x <= 0 || _position.x + _shape.width >= board.width);
}

void CGameObject::hit()
{
	_lives--;
}

bool CGameObject::collide(CGameObject& obj)
{
    // Get center points of both objects
    cv::Point2f missileCenter = _position + cv::Point2f(_shape.width / 2, _shape.height / 2);
    cv::Point2f object = obj.get_pos() + cv::Point2f(obj.get_shape().width / 2, obj.get_shape().height / 2);

    // Calculate distance between centers
    float distance = cv::norm(missileCenter - object);

    // If the distance is less than the sum of the half-widths/heights of both objects, they collide
    float missileRadius = _shape.width / 2; // Assuming missile is roughly circular
    float invaderRadius = obj.get_shape().width / 2; // Assuming invader is roughly circular
    float combinedRadius = missileRadius + invaderRadius;

    return distance < combinedRadius;
}






