#include <rclcpp/rclcpp.hpp>
#include "distance_warning/srv/set_threshold.hpp"
#include <sstream>
#include <iomanip>
// file dùng để xét giá trị ngướnga

using SetThreshold = distance_warning::srv::SetThreshold;

class SetThresholdService : public rclcpp::Node
{
public:
  SetThresholdService() : Node("set_threshold_service")
  {
    this->declare_parameter<double>("threshold", 0.5);
    service_ = this->create_service<SetThreshold>(
      "set_threshold",
      std::bind(&SetThresholdService::handleSetThreshold, this,
                std::placeholders::_1, std::placeholders::_2)); 
    // TODO: Tạo service server
    // service_ = this->create_service<SetThreshold>(
    //   "set_threshold",
    //   std::bind(&SetThresholdService::handleSetThreshold, this,
    //             std::placeholders::_1, std::placeholders::_2));
    RCLCPP_INFO(this->get_logger(), "SetThresholdService is ready to receive requests.");
  }

private:
  void handleSetThreshold(
    const SetThreshold::Request::SharedPtr request,
    SetThreshold::Response::SharedPtr response)
  {
    double current = this->get_parameter("threshold").as_double();
    double new_threshold = current;
    // TODO: Tăng hoặc giảm new_threshold theo request->increase
    if (request -> increase) {
      new_threshold += 0.1;
    } else {
      new_threshold -= 0.1;
    }
    // TODO: Clamp giá trị trong khoảng [0.1, 1.5]
    if (new_threshold < 0.1) {
      new_threshold = 0.1;
    } else if (new_threshold > 1.5) {
      new_threshold = 1.5;
    }
    // TODO: Cập nhật parameter bằng this->set_parameter(...)
    this ->set_parameter(rclcpp::Parameter("threshold", new_threshold));
    // TODO: Set response->success, new_threshold, message
    response -> success = true;
    response -> new_threshold = static_cast<float>(new_threshold);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    
    if (request->increase) {
      oss << "Threshold increased to " << new_threshold << " m";
    } else {
      oss << "Threshold decreased to " << new_threshold << " m";
    }
    response->message = oss.str();
    RCLCPP_INFO(this->get_logger(), "Threshold updated: %.2f -> %.2f m",
                current, new_threshold);
  }

  rclcpp::Service<SetThreshold>::SharedPtr service_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SetThresholdService>());
  rclcpp::shutdown();
  return 0;
}