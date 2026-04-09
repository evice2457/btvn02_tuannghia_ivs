#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <thread>
#include "distance_warning/action/check_distance.hpp"

using CheckDistance = distance_warning::action::CheckDistance;
using GoalHandle = rclcpp_action::ServerGoalHandle<CheckDistance>;

class DistanceActionServer : public rclcpp::Node
{
public:
  DistanceActionServer() : Node("distance_action_server")
  {
    this->declare_parameter<double>("threshold", 0.5);
    action_server_ = rclcpp_action::create_server<CheckDistance>(
      this, "check_distance",
      std::bind(&DistanceActionServer::handleGoal, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&DistanceActionServer::handleCancel, this, std::placeholders::_1),
      std::bind(&DistanceActionServer::handleAccepted, this, std::placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "DistanceActionServer is ready to receive goals");
    // TODO: Tạo action server
    // action_server_ = rclcpp_action::create_server<CheckDistance>(
    //   this, "check_distance",
    //   std::bind(&DistanceActionServer::handleGoal, this, ...),
    //   std::bind(&DistanceActionServer::handleCancel, this, ...),
    //   std::bind(&DistanceActionServer::handleAccepted, this, ...));
  }

private:
  rclcpp_action::GoalResponse handleGoal(
    const rclcpp_action::GoalUUID &,
    std::shared_ptr<const CheckDistance::Goal> goal)
  {
    RCLCPP_INFO(this->get_logger(), "Received goal: check %.2f m", goal->distance_to_check);
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  rclcpp_action::CancelResponse handleCancel(const std::shared_ptr<GoalHandle>)
  {
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  void handleAccepted(const std::shared_ptr<GoalHandle> goal_handle)
  {
    std::thread{std::bind(&DistanceActionServer::execute, this, goal_handle)}.detach();
  }

  void execute(const std::shared_ptr<GoalHandle> goal_handle)
  {
    float distance = goal_handle->get_goal()->distance_to_check;
    auto feedback = std::make_shared<CheckDistance::Feedback>();
    auto result = std::make_shared<CheckDistance::Result>();

    std::vector<std::string> steps = {
      "Receiving distance value...",
      "Fetching threshold parameter...",
      "Comparing values...",
      "Generating result...",
      "Done."
    };

    // TODO: Loop qua 5 bước:
    //   - Set feedback->step, total_steps, feedback_msg
    //   - goal_handle->publish_feedback(feedback)
    //   - std::this_thread::sleep_for(std::chrono::milliseconds(500))
    for (int i = 0; i < steps.size(); ++i) {
      feedback->step = i + 1;
      feedback->total_steps = steps.size();
      feedback->feedback_msg = steps[i];
      goal_handle->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "Step %d/5: %s", i + 1, steps[i].c_str());
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // TODO: Lấy threshold, so sánh với distance
    double threshold = this->get_parameter("threshold").as_double();
    result->is_safe = (distance >= static_cast<float>(threshold));
    // TODO: Set result->is_safe và result->result_message
    result->result_message = result->is_safe ? "Safe to proceed." : "Too close! Stop!";
    // TODO: goal_handle->succeed(result)
    goal_handle->succeed(result);
    RCLCPP_INFO(this->get_logger(), "Goal completed: distance=%.2f m, threshold=%.2f m, is_safe=%s",
                distance, threshold, result->is_safe ? "true" : "false");
  }

  rclcpp_action::Server<CheckDistance>::SharedPtr action_server_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DistanceActionServer>());
  rclcpp::shutdown();
  return 0;
}