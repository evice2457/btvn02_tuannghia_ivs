#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include "distance_warning/action/check_distance.hpp"

using CheckDistance = distance_warning::action::CheckDistance;
using GoalHandle = rclcpp_action::ClientGoalHandle<CheckDistance>;

class DistanceActionClient : public rclcpp::Node
{
public:
  DistanceActionClient() : Node("distance_action_client")
  {
    client_ = rclcpp_action::create_client<CheckDistance>(this, "check_distance");
  }

  void sendGoal(float distance)
  {
    if (!client_->wait_for_action_server(std::chrono::seconds(5))) {
      RCLCPP_ERROR(this->get_logger(), "Action server not available");
      return;
    }

    auto goal_msg = CheckDistance::Goal();
    goal_msg.distance_to_check = distance;
    RCLCPP_INFO(this->get_logger(), "Sending goal: check %.2f m", distance);

    auto send_goal_options = rclcpp_action::Client<CheckDistance>::SendGoalOptions();

    // TODO: Set send_goal_options.feedback_callback
    //   - In từng bước: [step/total_steps]: feedback_msg
    goal_msg = CheckDistance::Goal();
    goal_msg.distance_to_check = distance;
    RCLCPP_INFO(this->get_logger(), "Sending goal: check %.2f m", distance);


    // TODO: Set send_goal_options.result_callback
    send_goal_options.feedback_callback = [this](GoalHandle::SharedPtr, const std::shared_ptr<const CheckDistance::Feedback> feedback) {
      RCLCPP_INFO(this->get_logger(), "Feedback: Step %d/%d: %s",
                  feedback->step, feedback->total_steps, feedback->feedback_msg.c_str());
    };
    //   - In SAFE hoặc NOT SAFE theo result->is_safe
    send_goal_options.result_callback = [this](const GoalHandle::WrappedResult & wrapped_result) {
        auto result = wrapped_result.result;
        if (result -> is_safe) {
          RCLCPP_INFO(this->get_logger(), "Result: SAFE - %s", result->result_message.c_str());
        } else {
          RCLCPP_WARN(this->get_logger(), "Result: NOT SAFE - %s", result->result_message.c_str());
        }   
    };

    client_->async_send_goal(goal_msg, send_goal_options);
  }

private:
  rclcpp_action::Client<CheckDistance>::SharedPtr client_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<DistanceActionClient>();
  node->sendGoal(0.3f);  // TODO: Thay bằng argv nếu muốn
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}