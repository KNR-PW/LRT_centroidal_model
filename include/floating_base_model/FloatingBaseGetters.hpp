#ifndef __FLOATING_BASE_GETTERS__
#define __FLOATING_BASE_GETTERS__

#include <Eigen/Core>
#include <floating_base_model/FloatingBaseModelInfo.hpp>


namespace floating_base_model
{
  /**
  * Provides read/write access to the contact forces.
  */
  template <typename Derived>
  Eigen::Block<Derived, 3, 1> getContactForces(const FloatingBaseModelInfo params, Eigen::MatrixBase<Derived>& input, size_t contactIndex);

  /**
   * Provides read access to the contact forces.
   */
  template <typename Derived>
  const Eigen::Block<const Derived, 3, 1> getContactForces(const FloatingBaseModelInfo params, const Eigen::MatrixBase<Derived>& input, size_t contactIndex);


  /**
   * Provides read/write access to the contact torques.
   */
  template <typename Derived>
  Eigen::Block<Derived, 3, 1> getContactTorques(const FloatingBaseModelInfo params, Eigen::MatrixBase<Derived>& input, size_t contactIndex);


  /**
   * Provides read access to the contact torques.
   */
  template <typename Derived>
  const Eigen::Block<const Derived, 3, 1> getContactTorques(const FloatingBaseModelInfo params, const Eigen::MatrixBase<Derived>& input, size_t contactIndex);


  /**
   * Provides read/write access to the joint velocities.
   */
  template <typename Derived>
  Eigen::Block<Derived, -1, 1> getJointVelocities(const FloatingBaseModelInfo params, Eigen::MatrixBase<Derived>& input);

  /**
   * Provides read access to the joint velocities.
   */
  template <typename Derived>
  const Eigen::Block<const Derived, -1, 1> getJointVelocities(const FloatingBaseModelInfo params, const Eigen::MatrixBase<Derived>& input);


  /**
   * Provides read/write access to the base pose.
   */
  template <typename Derived>
  Eigen::Block<Derived, 6, 1> getBasePose(const FloatingBaseModelInfo params, Eigen::MatrixBase<Derived>& state);

  /**
   * Provides read access to the base pose.
   */
  template <typename Derived>
  const Eigen::Block<const Derived, 6, 1> getBasePose(const FloatingBaseModelInfo params, const Eigen::MatrixBase<Derived>& state);

  /**
   * Provides read/write access to the base velocity.
   */
  template <typename Derived>
  Eigen::Block<Derived, 6, 1> getBaseVelocity(const FloatingBaseModelInfo params, Eigen::MatrixBase<Derived>& state);

  /**
   * Provides read access to the base velocity.
   */
  template <typename Derived>
  const Eigen::Block<const Derived, 6, 1> getBaseVelocity(const FloatingBaseModelInfo params, const Eigen::MatrixBase<Derived>& state);

  /**
   * Provides read/write access to the joint angles.
   */
  template <typename Derived>
  Eigen::Block<Derived, -1, 1> getJointAngles(const FloatingBaseModelInfo params, Eigen::MatrixBase<Derived>& state);

  /**
   * Provides read access to the joint angles.
   */
  template <typename Derived>
  const Eigen::Block<const Derived, -1, 1> getJointAngles(const FloatingBaseModelInfo params, const Eigen::MatrixBase<Derived>& state);

  /**
   * Provides read/write access to the generalized coordinates.
   */
  template <typename Derived>
  Eigen::Block<Derived, -1, 1> getGeneralizedCoordinates(const FloatingBaseModelInfo params, Eigen::MatrixBase<Derived>& state);

  /**
   * Provides read access to the generalized coordinates.
   */
  template <typename Derived>
  const Eigen::Block<const Derived, -1, 1> getGeneralizedCoordinates(const FloatingBaseModelInfo params, const Eigen::MatrixBase<Derived>& state);
}

#include "implementation/FloatingBaseGettersImpl.hpp"

#endif