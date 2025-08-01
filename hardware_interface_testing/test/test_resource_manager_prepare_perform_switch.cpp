// Copyright 2023 Stogl Robotics Consulting UG (haftungsbeschränkt)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Authors: Dr. Denis

#include "ros2_control_test_assets/test_hardware_interface_constants.hpp"
#include "test_resource_manager.hpp"

#include <string>
#include <vector>

#include "hardware_interface/loaned_state_interface.hpp"
#include "ros2_control_test_assets/descriptions.hpp"

const auto hardware_resources_command_modes =
  R"(
  <ros2_control name="TestSystemCommandModes" type="system">
    <hardware>
      <plugin>test_hardware_components/TestSystemCommandModes</plugin>
    </hardware>
    <joint name="joint1">
      <command_interface name="position"/>
      <command_interface name="velocity"/>
      <state_interface name="position"/>
      <state_interface name="velocity"/>
    </joint>
    <joint name="joint2">
      <command_interface name="position"/>
      <command_interface name="velocity"/>
      <state_interface name="position"/>
      <state_interface name="velocity"/>
    </joint>
  </ros2_control>
  <ros2_control name="TestActuatorHardware" type="actuator">
    <hardware>
      <plugin>test_actuator</plugin>
    </hardware>
    <joint name="joint3">
      <command_interface name="position"/>
      <state_interface name="position"/>
      <state_interface name="velocity"/>
      <command_interface name="max_velocity" />
      </joint>
      </ros2_control>
      )";
const auto command_mode_urdf = std::string(ros2_control_test_assets::urdf_head) +
                               std::string(hardware_resources_command_modes) +
                               std::string(ros2_control_test_assets::urdf_tail);

class ResourceManagerPreparePerformTest : public ResourceManagerTest
{
public:
  void SetUp()
  {
    ResourceManagerTest::SetUp();

    rm_ = std::make_unique<TestableResourceManager>(node_, command_mode_urdf);
    ASSERT_EQ(1u, rm_->actuator_components_size());
    ASSERT_EQ(1u, rm_->system_components_size());

    // empty call can be done at any time and it doesn't propagates to HW components - always true
    ASSERT_TRUE(rm_->perform_command_mode_switch(empty_keys, empty_keys));
    // empty call can be done at any time and it doesn't propagates to HW components - always true
    ASSERT_TRUE(rm_->perform_command_mode_switch(empty_keys, empty_keys));

    // Set both HW to ACTIVE to claim interfaces. There should stay persistent because we are not
    // cleaning them for now, so this is a good way to keep the access and "f* the system"
    set_components_state(
      *rm_, {"TestSystemCommandModes"}, lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE, "active");
    set_components_state(
      *rm_, {"TestActuatorHardware"}, lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE, "active");
    // State to get feedback how many times "prepare_for_switch" is called
    claimed_system_acceleration_state_ = std::make_unique<hardware_interface::LoanedStateInterface>(
      rm_->claim_state_interface("joint1/acceleration"));
    claimed_actuator_position_state_ = std::make_unique<hardware_interface::LoanedStateInterface>(
      rm_->claim_state_interface("joint3/position"));
  }

  void preconfigure_components(
    const uint8_t system_state_id, const std::string syste_state_name,
    const uint8_t actuator_state_id, const std::string actuator_state_name)
  {
    set_components_state(*rm_, {"TestSystemCommandModes"}, system_state_id, syste_state_name);
    set_components_state(*rm_, {"TestActuatorHardware"}, actuator_state_id, actuator_state_name);

    auto status_map = rm_->get_components_status();
    EXPECT_EQ(status_map["TestSystemCommandModes"].state.id(), system_state_id);
    EXPECT_EQ(status_map["TestActuatorHardware"].state.id(), actuator_state_id);
  }

  std::unique_ptr<TestableResourceManager> rm_;

  std::unique_ptr<hardware_interface::LoanedStateInterface> claimed_system_acceleration_state_;
  std::unique_ptr<hardware_interface::LoanedStateInterface> claimed_actuator_position_state_;

  // Scenarios defined by example criteria
  rclcpp::Node node_{"ResourceManagerPreparePerformTest"};
  std::vector<std::string> empty_keys = {};
  std::vector<std::string> non_existing_keys = {"elbow_joint/position", "should_joint/position"};
  std::vector<std::string> legal_keys_system = {"joint1/position", "joint2/position"};
  std::vector<std::string> legal_keys_actuator = {"joint3/position"};
};

// System  : ACTIVE
// Actuator: UNCONFIGURED
TEST_F(
  ResourceManagerPreparePerformTest,
  when_system_active_and_actuator_unconfigured_expect_system_passing)
{
  preconfigure_components(
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE, "active",
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED, "unconfigured");

  // Default behavior for empty key lists, e.g., when a Broadcaster is activated
  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, empty_keys));
  // empty call can be done at any time and it doesn't propagates to HW components - always true
  EXPECT_TRUE(rm_->perform_command_mode_switch(empty_keys, empty_keys));

  // When TestSystemCommandModes is ACTIVE expect OK
  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 1.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 102.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 202.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 203.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  // When TestActuatorHardware is UNCONFIGURED expect ERROR
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, legal_keys_actuator))
    << "The actuator HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0)
    << "Shouldn't change as the perform command mode switch is done only for actuator interfaces";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0)
    << "Shouldn't change as the perform command mode switch is done only for actuator interfaces";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys))
    << "The actuator HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0)
    << "Shouldn't change as the perform command mode switch is done only for actuator interfaces";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0)
    << "Shouldn't change as the perform command mode switch is done only for actuator interfaces";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0)
    << "Shouldn't change as the perform command mode switch is done only for actuator interfaces";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
};

// System  : ACTIVE
// Actuator: INACTIVE
TEST_F(
  ResourceManagerPreparePerformTest, when_system_active_and_actuator_inactive_expect_both_passing)
{
  preconfigure_components(
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE, "active",
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE, "inactive");

  // When TestSystemCommandModes is ACTIVE expect OK
  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 1.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 102.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 202.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 203.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  // When TestActuatorHardware is INACTIVE expect not OK
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.0, 1e-7)
    << "Start interfaces with inactive should result in no change";
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.0, 1e-7)
    << "Start interfaces with inactive should result in no change";

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.0, 1e-7)
    << "Start interfaces with inactive should result in no change";
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.0, 1e-7)
    << "Start interfaces with inactive should result in no change";

  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.001, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator))
    << "Inactive with empty start interfaces is OK";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 303.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.101, 1e-7);
};

// System  : INACTIVE
// Actuator: ACTIVE
TEST_F(
  ResourceManagerPreparePerformTest, when_system_inactive_and_actuator_active_expect_both_passing)
{
  preconfigure_components(
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE, "inactive",
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE, "active");

  // When TestSystemCommandModes is INACTIVE expect not OK
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0)
    << "Start interfaces with inactive should result in no change";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0)
    << "System interfaces shouldn't affect the actuator";
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0)
    << "Start interfaces with inactive should result in no change";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0)
    << "System interfaces shouldn't affect the actuator";

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0)
    << "Start interfaces with inactive should result in no change";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0)
    << "System interfaces shouldn't affect the actuator";
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0)
    << "Start interfaces with inactive should result in no change";
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0)
    << "System interfaces shouldn't affect the actuator";

  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 1.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0)
    << "System interfaces shouldn't affect the actuator";
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0)
    << "System interfaces shouldn't affect the actuator";

  // When TestActuatorHardware is ACTIVE expect OK
  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.001, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.101, 1e-7);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.102, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.202, 1e-7);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.203, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 101.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.303, 1e-7);
};

// System  : UNCONFIGURED
// Actuator: ACTIVE
TEST_F(
  ResourceManagerPreparePerformTest,
  when_system_unconfigured_and_actuator_active_expect_actuator_passing)
{
  preconfigure_components(
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED, "unconfigured",
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE, "active");

  // When TestSystemCommandModes is UNCONFIGURED expect error
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, legal_keys_system))
    << "The system HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, empty_keys))
    << "The system HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_system))
    << "The system HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  // When TestActuatorHardware is ACTIVE expect OK
  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.001, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.101, 1e-7);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.102, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.202, 1e-7);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.203, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.303, 1e-7);
};

// System  : UNCONFIGURED
// Actuator: ERROR
TEST_F(
  ResourceManagerPreparePerformTest,
  when_system_unconfigured_and_actuator_active_and_then_error_expect_actuator_passing)
{
  preconfigure_components(
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED, "unconfigured",
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE, "active");

  // When TestSystemCommandModes is UNCONFIGURED expect error
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, legal_keys_system))
    << "The system HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, empty_keys))
    << "The system HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_system))
    << "The system HW component is unconfigured, so the perform should fail!";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  // When TestActuatorHardware is ACTIVE expect OK
  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.001, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.101, 1e-7);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.102, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.202, 1e-7);

  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.203, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.303, 1e-7);

  std::unique_ptr<hardware_interface::LoanedCommandInterface> claimed_actuator_velocity_command_ =
    std::make_unique<hardware_interface::LoanedCommandInterface>(
      rm_->claim_command_interface("joint3/position"));

  auto status_map = rm_->get_components_status();
  EXPECT_EQ(
    status_map["TestSystemCommandModes"].state.id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
  EXPECT_EQ(
    status_map["TestActuatorHardware"].state.id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_ACTIVE);

  // Now deactivate with write deactivate value
  claimed_actuator_velocity_command_->set_value(test_constants::WRITE_DEACTIVATE_VALUE);
  rm_->write(node_.now(), rclcpp::Duration(0, 1000000));

  status_map = rm_->get_components_status();
  EXPECT_EQ(
    status_map["TestSystemCommandModes"].state.id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
  EXPECT_EQ(
    status_map["TestActuatorHardware"].state.id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_INACTIVE);

  // Similar to deactivate callback from write cycle
  EXPECT_TRUE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.304, 1e-7);
  EXPECT_TRUE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.404, 1e-7);

  // Similar to the proximal activation
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.404, 1e-7);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys))
    << "Start interfaces with inactive should result in no change";
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.404, 1e-7)
    << "Start interfaces with inactive should result in no change";

  // Now return ERROR with write fail value
  claimed_actuator_velocity_command_->set_value(test_constants::WRITE_FAIL_VALUE);
  rm_->write(node_.now(), rclcpp::Duration(0, 1000000));

  status_map = rm_->get_components_status();
  EXPECT_EQ(
    status_map["TestSystemCommandModes"].state.id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);
  EXPECT_EQ(
    status_map["TestActuatorHardware"].state.id(),
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.404, 1e-7);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.404, 1e-7);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.404, 1e-7);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_NEAR(claimed_actuator_position_state_->get_optional().value(), 0.404, 1e-7);
};

// System  : UNCONFIGURED
// Actuator: FINALIZED
TEST_F(
  ResourceManagerPreparePerformTest,
  when_system_unconfigured_and_actuator_finalized_expect_none_passing)
{
  preconfigure_components(
    lifecycle_msgs::msg::State::PRIMARY_STATE_UNCONFIGURED, "unconfigured",
    lifecycle_msgs::msg::State::PRIMARY_STATE_FINALIZED, "finalized");

  // When TestSystemCommandModes is UNCONFIGURED expect error
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_system, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_system));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  // When TestActuatorHardware is FINALIZED expect ERROR
  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(legal_keys_actuator, empty_keys));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);

  EXPECT_FALSE(rm_->prepare_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
  EXPECT_FALSE(rm_->perform_command_mode_switch(empty_keys, legal_keys_actuator));
  EXPECT_EQ(claimed_system_acceleration_state_->get_optional().value(), 0.0);
  EXPECT_EQ(claimed_actuator_position_state_->get_optional().value(), 0.0);
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
