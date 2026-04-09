#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>

class DistanceListener : public rclcpp::Node
{
public:
  DistanceListener() : Node("distance_listener")
  {
    this->declare_parameter<double>("threshold", 0.5);
    subscription_ = this->create_subscription<std_msgs::msg::Float32>(
      "distance_topic",
      10,
      std::bind(&DistanceListener::listenerCallback, this, std::placeholders::_1)
    );
    // TODO: Tạo subscriber lắng nghe 'distance_topic'
    // subscription_ = this->create_subscription<std_msgs::msg::Float32>(...);
  }

private:
  void listenerCallback(const std_msgs::msg::Float32::SharedPtr msg)
  {
    double distance = msg->data;
    double threshold = this->get_parameter("threshold").as_double();
    
    RCLCPP_INFO(this->get_logger(), "Received distance: %.2f m", distance);
    if (distance < threshold) {
      RCLCPP_WARN(this->get_logger(), "Warning: Object too close! Distance: %.2f m", distance);
    }   
    // TODO: In khoảng cách nhận được
    // TODO: Nếu distance < threshold, in cảnh báo bằng RCLCPP_WARN
  }

  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DistanceListener>());
  rclcpp::shutdown();
  return 0;
}