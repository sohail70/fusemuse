#include<gtest/gtest.h>
#include<fusion/motion_model_factory.hpp>
#include<fusion/motion_model.hpp>
#include<typeinfo>
#include<vector>
#include<matplotlib-cpp/matplotlibcpp.h>
#include<cmath>
TEST(FactoryTest , creatingObjectTest) //1st is group and 2nd is specific thing you wanna do
{
    ASSERT_EQ(1,1);
    Filter::MotionModelFactory factory_;
    std::unique_ptr<Filter::MotionModel> constant_heading_model_ = factory_.createModel(Filter::ModelType::CONSTANT_HEADING_RATE);
    std::unique_ptr<Filter::MotionModel> car_ = factory_.createModel(Filter::ModelType::CAR);

    ASSERT_NE(constant_heading_model_,nullptr);
    ASSERT_NE(car_,nullptr);


    ASSERT_EQ(typeid(*constant_heading_model_) , typeid(Filter::ConstantHeadingRate));
    ASSERT_EQ(typeid(*car_) , typeid(Filter::Car));

}
namespace plt = matplotlibcpp;

TEST(MotionModel , update)
{
    Filter::MotionModelFactory factory_;
    std::unique_ptr<Filter::MotionModel> constant_heading_model_ = factory_.createModel(Filter::ModelType::CONSTANT_HEADING_RATE);
    rclcpp::Time current_time_ = rclcpp::Clock().now();
    
    struct Position {double x,y,z;
        Position& operator=(const Position& other)
        {
            x = other.x; y = other.y; z= other.z;
            return *this;
        }
    };Position position_{0.0,0.0,0.0};
    struct Angle{double roll,pitch,yaw;
        Angle& operator=(const Angle& other) {
        // Check for self-assignment
        if (this != &other) {
            roll = other.roll;
            pitch = other.pitch;
            yaw = other.yaw;
        }
        return *this;
    }
    };Angle angle_{0.0,0.0,0.0};


    struct Point {
        double x;
        double y;
        double yaw;
    };
    std::vector<Point> points;
    rclcpp::Rate loop_rate_(1000);
    for (int i = 0; i < 50; i++) {
        current_time_ = rclcpp::Clock().now();
        constant_heading_model_->update(current_time_);
        position_.x =  dynamic_cast<Filter::ConstantHeadingRate*>(constant_heading_model_.get())->getPosition().x;
        position_.y =  dynamic_cast<Filter::ConstantHeadingRate*>(constant_heading_model_.get())->getPosition().y;
        position_.z =  dynamic_cast<Filter::ConstantHeadingRate*>(constant_heading_model_.get())->getPosition().z;
        angle_.roll =  dynamic_cast<Filter::ConstantHeadingRate*>(constant_heading_model_.get())->getAngle().roll;
        angle_.pitch =  dynamic_cast<Filter::ConstantHeadingRate*>(constant_heading_model_.get())->getAngle().pitch;
        angle_.yaw=  dynamic_cast<Filter::ConstantHeadingRate*>(constant_heading_model_.get())->getAngle().yaw;
        std::cout<<angle_.yaw<<"\n";
        Point p;
        p.x = position_.x;
        p.y = position_.y;
        p.yaw = angle_.yaw;
        points.push_back(p);
        loop_rate_.sleep();
    }


    // Extract x, y, and yaw data for plotting
    std::vector<double> x, y, dx, dy;
    for (const auto& point : points) {
        x.push_back(point.x);
        y.push_back(point.y);
        // Calculate the endpoint of the vector based on yaw
        dx.push_back(cos(point.yaw)); // X component of the vector
        dy.push_back(sin(point.yaw)); // Y component of the vector
    }

    // Modify arrow size and color
    std::map<std::string, std::string> keywords;
    keywords["scale"] = 1.0; // Adjust arrow size (scale factor)
    keywords["color"] = "red"; // Change arrow color to red
    // Plot points with arrows representing their headings
    plt::quiver(x, y, dx, dy);
    plt::plot(x, y, ".");
    plt::show();
    



}