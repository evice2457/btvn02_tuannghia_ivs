# btvn_tuannghia
# distance_warning

A ROS2 (Humble) package that demonstrates the core communication patterns of ROS2: **Publisher/Subscriber**, **Service**, and **Action**, built around a distance-based warning system.

---

## Overview

This package simulates a sensor that continuously publishes distance readings. A listener node monitors those readings and triggers warnings when objects get too close. The warning threshold can be adjusted at runtime via a service, and individual distance checks can be requested through an action server with step-by-step feedback.

```
[distance_publisher] ──/distance_topic──► [distance_listener]
                                                   ▲
[set_threshold_service] ◄──── /set_threshold ──────┘
[distance_action_server] ◄─── /check_distance (action)
```

---

## Package Structure

```
distance_warning/
├── src/
│   ├── distance_publisher.cpp          # Standard publisher (1 Hz)
│   ├── distance_publisher_qos.cpp      # Publisher with QoS settings
│   ├── distance_listener.cpp           # Standard subscriber
│   ├── distance_listener_qos.cpp       # Subscriber with QoS settings
│   ├── distance_tf_broadcaster.cpp     # TF broadcaster
│   ├── distance_tf_listener.cpp        # TF listener
│   ├── set_threshold_service.cpp       # Service server
│   ├── distance_action_server.cpp      # Action server (5-step feedback)
│   └── distance_action_client.cpp      # Action client
├── action/
│   └── CheckDistance.action
├── srv/
│   └── SetThreshold.srv
├── launch/
│   └── distance_warning.launch.py
├── CMakeLists.txt
└── package.xml
```

---

## Interfaces

**Service**: `SetThreshold.srv` — Adjust threshold (bool increase → bool success, float32 new_threshold, string message)

**Action**: `CheckDistance.action` — Check distance safety (float32 distance_to_check → bool is_safe, string result_message | feedback: int32 step, int32 total_steps, string feedback_msg)

---

## Nodes

| Node | Type | Interface |
|------|------|-----------| 
| `distance_publisher` | Publisher | `/distance_topic` |
| `distance_publisher_qos` | Publisher (QoS) | `/distance_topic` |
| `distance_listener` | Subscriber | `/distance_topic` |
| `distance_listener_qos` | Subscriber (QoS) | `/distance_topic` |
| `distance_tf_broadcaster` | TF Broadcaster | `/tf` |
| `distance_tf_listener` | TF Listener | `/tf` |
| `set_threshold_service` | Service Server | `/set_threshold` |
| `distance_action_server` | Action Server | `/check_distance` |
| `distance_action_client` | Action Client | `/check_distance` |

---

## Prerequisites

- ROS2 Humble
- Ubuntu 22.04
- Packages: `rclcpp`, `rclcpp_action`, `std_msgs`, `rosidl_default_generators`

---

## Build

```bash
cd ~/ros2_ws
colcon build --packages-select distance_warning
source install/setup.bash
```

---

## Run

### Launch all nodes

```bash
ros2 launch distance_warning distance_warning.launch.py
ros2 launch distance_warning distance_warning.launch.py threshold:=0.8
```

### Run action client

```bash
ros2 run distance_warning distance_action_client 0.3
ros2 run distance_warning distance_action_client 0.8
```

### Call threshold service

```bash
ros2 service call /set_threshold distance_warning/srv/SetThreshold '{increase: true}'
ros2 service call /set_threshold distance_warning/srv/SetThreshold '{increase: false}'
```

---

## Example Output

**Publisher/Listener**:
```
[distance_publisher]: Publishing: 0.82 m
[distance_listener]: Distance received: 0.82 m
[distance_listener]: Warning: Object too close! (0.34 m < threshold: 0.50 m)
```

**Action Client**:
```
[INFO] [distance_action_client]: Sending goal: check 0.30 m
[INFO] [distance_action_client]: Feedback [1/5]: Receiving distance value...
[INFO] [distance_action_client]: Feedback [5/5]: Done.
[WARN] [distance_action_client]: RESULT: NOT SAFE — distance is below threshold.
```

---

## Verification

```bash
ros2 topic hz /distance_topic           # Check publish rate (1.0 Hz)
ros2 node list                          # List active nodes
ros2 param get /distance_listener threshold  # Check threshold
rqt_graph                               # Visualize node graph
```

---

## Concepts Demonstrated

| Concept | Node(s) |
|---------|--------|
| Publisher / Subscriber | `distance_publisher`, `distance_listener` |
| QoS Settings | `distance_publisher_qos`, `distance_listener_qos` |
| TF Broadcasting / Listening | `distance_tf_broadcaster`, `distance_tf_listener` |
| Service | `set_threshold_service` |
| Action (with feedback) | `distance_action_server`, `distance_action_client` |
| Parameters | Threshold managed per-node |
| Launch file | Aggregates multiple nodes with shared parameters |

---

## Notes

- Threshold parameters are managed **independently** by each node; the service syncs updates to listeners via async parameter clients.
- `distance_action_client` is excluded from the launch file — run manually as a one-shot client.
- QoS and TF variants demonstrate quality-of-service settings and transform broadcasting/listening.
- CMake deprecation warnings about `rosidl_target_interfaces` are harmless.

---

## Author

evice2457 — ROS2 Humble, Ubuntu 22.04
