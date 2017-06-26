#pragma once

#include <memory>
#include <string>

#include <franka/exception.h>
#include <franka/gripper_state.h>

/**
 * @file gripper.h
 * Contains the Gripper class.
 */

namespace franka {

/**
 * Maintains a connection to FRANKA CONTROL, provides the current gripper state,
 * and allows to execute gripper commands.
 */
class Gripper {
 public:
  /**
   * Version of the gripper server running on FRANKA CONTROL.
   */
  using ServerVersion = uint16_t;

  /**
   * Establishes a connection with FRANKA CONTROL.
   *
   * @param[in] franka_address IP/hostname of FRANKA CONTROL
   *
   * @throw NetworkException if the connection is unsuccessful.
   * @throw IncompatibleVersionException if this library is not supported by FRANKA CONTROL.
   * @throw ProtocolException if data received from the host is invalid.
   */
  explicit Gripper(const std::string& franka_address);

  /**
   * Closes the connection.
   */
  ~Gripper() noexcept;

  /**
   * Performs homing.
   *
   * @throw CommandException if an error occurred.
   */
  void homing();

  /**
   * Grasp an object.
   *
   * @param[in] width Size of object to grasp.
   * @param[in] speed Closing speed.
   * @param[in] force Grasping force.
   *
   * @throw CommandException if an error occurred.
   */
  void grasp(double width, double speed, double force);

  /**
   * Moves the gripper fingers to a specified width.
   *
   * @param[in] width Intended opening width.
   * @param[in] speed Gripper closing speed.
   *
   * @throw CommandException if an error occurred.
   */
  void move(double width, double speed);

  /**
   * Stops moving the fingers of the gripper.
   *
   * @throw CommandException if an error occurred.
   */
  void stop();

  /**
   * Waits for a gripper state update and returns it.
   *
   * @return Current gripper state.
   *
   * @throw NetworkException if the connection is lost, e.g. after a timeout.
   * @throw ProtocolException if received data has invalid format.
   */
  GripperState readOnce();

  /**
   * Returns the software version reported by the connected server.
   *
   * @return Software version of the connected server.
   */
  ServerVersion serverVersion() const noexcept;

  Gripper(const Gripper&) = delete;
  Gripper& operator=(const Gripper&) = delete;

  class Impl;

 private:
  std::unique_ptr<Impl> impl_;
};

}  // namespace franka
