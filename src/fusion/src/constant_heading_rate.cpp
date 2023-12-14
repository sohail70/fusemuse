
#include "fusion/constant_heading_rate.hpp"

namespace Filter{
    ConstantHeadingRate::ConstantHeadingRate()
    {
        std::cout<<"Ctor of constant heading rate \n";
        init();
    }

    void ConstantHeadingRate::init()
    {
        previous_time_ = rclcpp::Clock().now();
    }

    void ConstantHeadingRate::update(const rclcpp::Time& current_time_)
    {
        rclcpp::Duration dt = current_time_ - previous_time_;
        // if(dt.seconds()>0.0001)
        // {
            position_.x = position_.x + velocity_.x_dot*cos(angle_.yaw)*dt.seconds();
            position_.y = position_.y + velocity_.x_dot*sin(angle_.yaw)*dt.seconds();
            angle_.yaw = angle_.yaw + angular_velocity_.yaw_dot*dt.seconds();
            previous_time_ = current_time_;
        // }
    }

    void ConstantHeadingRate::setVelocity(const Filter::Velocity& velocity_)
    {
        this->velocity_.x_dot = velocity_.x_dot;
        this->velocity_.y_dot = velocity_.y_dot;
        this->velocity_.z_dot = velocity_.z_dot;
    }

    void ConstantHeadingRate::setAngularVelocity(const Filter::AngularVelocity& angular_velocity_)
    {
        this->angular_velocity_.pitch_dot = angular_velocity_.pitch_dot;
        this->angular_velocity_.roll_dot = angular_velocity_.roll_dot;
        this->angular_velocity_.yaw_dot = angular_velocity_.yaw_dot;
    }

    Filter::Position& ConstantHeadingRate::getPosition()
    {
        return position_;
    }

    Filter::Angle& ConstantHeadingRate::getAngle()
    {
        return angle_;
    }

} //namespace Filter