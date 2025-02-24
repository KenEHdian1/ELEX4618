#pragma once

#include "stdafx.h"


class CGameObject
{
protected:
    cv::Point2f _position;
    cv::Point2f _velocity;
    cv::Rect _shape;
    int _lives;
    double _objectTime = 0;
    cv::Scalar _colour;

public:
    void move();
    bool collide(CGameObject& obj);
    bool collide_wall(cv::Size board);
    void hit();
    int get_lives() { return _lives; }
    void set_lives(int lives) { _lives = lives; }
    void set_pos(cv::Point2f pos) { _position = pos; }
    cv::Point2f get_pos() { return _position; }
    void set_shape(cv::Rect rect);
    cv::Rect get_shape() { return _shape; }  // Getter for _shape
    void set_velocity(cv::Point2f velocity) { _velocity = velocity; } // Setter for _velocity
    cv::Point2f get_velocity() { return _velocity; }  // Getter for _velocity
    void draw(cv::Mat& im);
};



