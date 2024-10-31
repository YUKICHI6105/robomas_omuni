#include <cstdio>
#include <chrono>
#include <functional>
#include <memory>
#include <cmath>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <robomas_plugins/msg/robomas_frame.hpp>
#include <robomas_plugins/msg/robomas_target.hpp>
#include <robomas_plugins/robomas_utils.hpp>

#include <robomas_omuni/robomas_omuni_node.hpp>

using namespace std::chrono_literals;

void pubsub::move_omuni3(float x_vel, float y_vel, float omega)
{
  float r = omega * 0.16181f / 0.03f;
  // maxSpeed = std::sqrt(x_raw * x_raw + y_raw * y_raw);
  if (((x_vel != 0) || (y_vel != 0)) || (r != 0))
  {
    robomas_target1_publisher_->publish(robomas::get_target((-x_vel / 0.03 + r)*36));
    robomas_target2_publisher_->publish(robomas::get_target(((x_vel * COS_60_DEG - y_vel * COS_30_DEG) / 0.03 + r)*36));
    robomas_target3_publisher_->publish(robomas::get_target(((x_vel * COS_60_DEG + y_vel * COS_30_DEG) / 0.03 + r)*36));

    count = 0;

    RCLCPP_INFO(this->get_logger(), "Publishing:bokuha warukunai!");
    std::string str_x = std::to_string(x_vel);
    std::string str_y = std::to_string(y_vel);
    RCLCPP_INFO(this->get_logger(), str_x.c_str(), str_y.c_str());
  }
  else
  {
    if (count == 0)
    {
      robomas_target1_publisher_->publish(robomas::get_target(0.0f));
      robomas_target2_publisher_->publish(robomas::get_target(0.0f));
      robomas_target3_publisher_->publish(robomas::get_target(0.0f));
      count = 1;
    }
  }
}

void pubsub::joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
{

  //    RCLCPP_INFO(this->get_logger(), "I heard:");
  // if (msg->buttons[this->get_parameter("velButton").as_int()] == 1)
  if (msg->buttons[7] == 1)
  {
    robomas_frame_publisher_->publish(robomas::get_vel_frame(1, 0));
    robomas_frame_publisher_->publish(robomas::get_vel_frame(2, 0));
    robomas_frame_publisher_->publish(robomas::get_vel_frame(3, 0));
  }

  // if (msg->buttons[this->get_parameter("disButton").as_int()] == 1)
  if (msg->buttons[0] == 1)
  {
    robomas_frame_publisher_->publish(robomas::get_dis_frame(1, 0));
    robomas_frame_publisher_->publish(robomas::get_dis_frame(2, 0));
    robomas_frame_publisher_->publish(robomas::get_dis_frame(3, 0));
  }

  float x = -(msg->axes[0]);
  float y = (msg->axes[1]);
  float r = 0;
  if (msg->buttons[4] == 1)
  {
    r = 3.0f; // ↑左回転
  }
  else if (msg->buttons[5] == 1)
  {
    r = -3.0f; // 右回転
  }
  else if (msg->buttons[4] == msg->buttons[5])
  {
    r = 0.0f;
  }
  move_omuni3(x, y, r);
}


int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<pubsub>());
  rclcpp::shutdown();
  return 0;
}
