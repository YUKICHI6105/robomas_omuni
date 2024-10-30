#pragma once

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <robomas_plugins/msg/robomas_frame.hpp>
#include <robomas_plugins/msg/robomas_target.hpp>
#include <robomas_plugins/robomas_utils.hpp>

#define COS_60_DEG 0.5f
#define COS_30_DEG 0.86602540378f

class pubsub : public rclcpp::Node
{
  private:
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg);
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<robomas_plugins::msg::RobomasFrame>::SharedPtr robomas_frame_publisher_;
    rclcpp::Publisher<robomas_plugins::msg::RobomasTarget>::SharedPtr robomas_target1_publisher_;
    rclcpp::Publisher<robomas_plugins::msg::RobomasTarget>::SharedPtr robomas_target2_publisher_;
    rclcpp::Publisher<robomas_plugins::msg::RobomasTarget>::SharedPtr robomas_target3_publisher_;
    // rclcpp::Publisher<robomas_plugins::msg::RobomasTarget>::SharedPtr robomas_target4_publisher_;
    // rclcpp::Publisher<robomas_plugins::msg::RobomasTarget>::SharedPtr robomas_target5_publisher_;
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_subscriber_;
    size_t count_;
    int count = 0;
    float maxSpeed = 0.0f;//厳密にはちょっと違う。
    
  public:
    pubsub() : Node("robomas_omuni3_node"), count_(0)
    {
      robomas_frame_publisher_ = this->create_publisher<robomas_plugins::msg::RobomasFrame>("robomas_frame", 10);
      joy_subscriber_ = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, std::bind(&pubsub::joy_callback, this, std::placeholders::_1));
      robomas_target1_publisher_ = this->create_publisher<robomas_plugins::msg::RobomasTarget>("robomas_target1", 10);
      robomas_target2_publisher_ = this->create_publisher<robomas_plugins::msg::RobomasTarget>("robomas_target2", 10);
      robomas_target3_publisher_ = this->create_publisher<robomas_plugins::msg::RobomasTarget>("robomas_target3", 10);
      // robomas_target4_publisher_ = this->create_publisher<robomas_plugins::msg::RobomasTarget>("robomas_target4", 10);
      // robomas_target5_publisher_ = this->create_publisher<robomas_plugins::msg::RobomasTarget>("robomas_target5", 10);
      this->declare_parameter("maxSpeed", 6.28f);
      this->declare_parameter("velButton", 2);
      this->declare_parameter("disButton", 1);
      //ツイスト型の調査
      // timer_ = this->create_wall_timer(1000ms, std::bind(&pubsub::timer_callback, this));
      // timer_callback();
    }
    // void timer_callback();
    void move_omuni3(float x, float y, float theta);
};


