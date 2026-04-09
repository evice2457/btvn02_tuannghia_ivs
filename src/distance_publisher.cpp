#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <random>

class DistancePublisher : public rclcpp::Node
{
public:
  DistancePublisher() : Node("distance_publisher"),
	rng_(std::random_device{}()),
	dist_(0.1f, 1.5f)
  {
    publisher_ = this->create_publisher<std_msgs::msg::Float32>("distance_topic", 10);
    // TODO: Tạo timer gọi timerCallback mỗi 1 giây
    // timer_ = this->create_wall_timer(...);
	  timer_ = this->create_wall_timer(
    std::chrono::milliseconds(1000),
    std::bind(&DistancePublisher::timerCallback, this)
  );
  }

private:
  void timerCallback()
  {
    auto msg = std_msgs::msg::Float32();
    // TODO: Gán giá trị ngẫu nhiên từ 0.1 đến 1.5
    // Gợi ý: dùng std::uniform_real_distribution
    // msg.data = ...;
	  msg.data = dist_(rng_);
    publisher_->publish(msg);
    RCLCPP_INFO(this->get_logger(), "Publishing: %.2f m", msg.data);
  }

  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::mt19937 rng_;
  std::uniform_real_distribution<float> dist_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DistancePublisher>());
  rclcpp::shutdown();
  return 0;
}
