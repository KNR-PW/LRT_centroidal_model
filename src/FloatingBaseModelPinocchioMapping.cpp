#include "floating_base_model/FloatingBaseModelPinocchioMapping.hpp"


namespace floating_base_model
{ 

  /******************************************************************************************************/
  /******************************************************************************************************/
  /******************************************************************************************************/
  template <typename SCALAR_T>
  FloatingBaseModelPinocchioMappingTpl<SCALAR_T>::FloatingBaseModelPinocchioMappingTpl(FloatingBaseModelInfoTpl<SCALAR_T> floatingBaseModelInfo)
    : pinocchioInterfacePtr_(nullptr), floatingBaseModelInfo_(std::move(floatingBaseModelInfo)) {}

  /******************************************************************************************************/
  /******************************************************************************************************/
  /******************************************************************************************************/
  template <typename SCALAR_T>
  FloatingBaseModelPinocchioMappingTpl<SCALAR_T>* FloatingBaseModelPinocchioMappingTpl<SCALAR_T>::clone() const
  {
    return new FloatingBaseModelPinocchioMappingTpl<SCALAR_T>(*this);
  }

  /******************************************************************************************************/
  /******************************************************************************************************/
  /******************************************************************************************************/
  template <typename SCALAR_T>
  void FloatingBaseModelPinocchioMappingTpl<SCALAR_T>::setPinocchioInterface(const ocs2::PinocchioInterfaceTpl<SCALAR_T>& pinocchioInterface)
  {
    pinocchioInterfacePtr_ = &pinocchioInterface;
  }

  /******************************************************************************************************/
  /******************************************************************************************************/
  /******************************************************************************************************/
  template <typename SCALAR_T>
  auto FloatingBaseModelPinocchioMappingTpl<SCALAR_T>::getPinocchioJointPosition(const vector_t& state) const -> vector_t
  {
    const auto& info = floatingBaseModelInfo_;
    assert(info.stateDim == state.rows());

    const auto model = pinocchioInterfacePtr_->getModel();

    const auto basePosition = access_helper_functions::getBasePosition(state, info);
    const Eigen::Matrix<SCALAR_T, 3, 1> baseEulerAngles = access_helper_functions::getBaseOrientationZyx(state, info);
    const Eigen::Quaternion<SCALAR_T>  baseQuaterion = quaterion_euler_transforms::getQuaternionFromEulerAnglesZyx(baseEulerAngles);
    const auto  actuatedJointPostition = access_helper_functions::getJointAngles(state, info);
    
    vector_t pinocchioJointPosition(model.nq);
    pinocchioJointPosition << basePosition, baseQuaterion.coeffs(), actuatedJointPostition;

    return pinocchioJointPosition;
  }
  
  /******************************************************************************************************/
  /******************************************************************************************************/
  /******************************************************************************************************/
  template <typename SCALAR_T>
  auto FloatingBaseModelPinocchioMappingTpl<SCALAR_T>::getPinocchioJointVelocity(const vector_t& state,
     const vector_t& input) const -> vector_t
  {
    const auto& info = floatingBaseModelInfo_;
    assert(info.stateDim == state.rows());
    assert(info.inputDim == input.rows());

    const auto& model = pinocchioInterfacePtr_->getModel();
    const auto baseVelocity = access_helper_functions::getBaseVelocity(state, info);
    const auto actuatedJointVelocities = access_helper_functions::getJointVelocities(input, info);
    
    vector_t pinocchioJointVelocities(model.nv);
    pinocchioJointVelocities << baseVelocity, actuatedJointVelocities;

    return pinocchioJointVelocities;
  }
  
  /******************************************************************************************************/
  /******************************************************************************************************/
  /******************************************************************************************************/
  template <typename SCALAR_T>
  auto FloatingBaseModelPinocchioMappingTpl<SCALAR_T>::getOcs2Jacobian(const vector_t& state,
     const matrix_t& Jq,
     const matrix_t& Jv) const -> std::pair<matrix_t, matrix_t>
  {
    const auto& info = floatingBaseModelInfo_;
    assert(info.stateDim == state.rows());
    const auto& model = pinocchioInterfacePtr_->getModel();

    const Eigen::Vector<SCALAR_T, 3> eulerAngles = access_helper_functions::getBaseOrientationZyx(state, info);
    const auto baseRotationMatrix = ocs2::getRotationMatrixFromZyxEulerAngles(eulerAngles);
    const auto E_inv = ocs2::getMappingFromEulerAnglesZyxDerivativeToLocalAngularVelocity(eulerAngles);
    
    matrix_t dqdx = matrix_t::Zero(model.nv, info.stateDim);
    dqdx.template block<3, 3>(0, 6) = baseRotationMatrix;
    dqdx.template block<3, 3>(3, 9) = E_inv;
    dqdx.block(6, info.actuatedDofNum, info.actuatedDofNum, info.actuatedDofNum).setIdentity();

    matrix_t dvdx = matrix_t::Zero(model.nv, info.stateDim);
    dvdx.template block<6, 6>(0, 0).setIdentity();

    matrix_t dvdu = matrix_t::Zero(model.nv, info.inputDim);
    size_t actuatedDofStartCol = info.inputDim - info.actuatedDofNum;
    dqdx.block(6, actuatedDofStartCol, info.actuatedDofNum, info.actuatedDofNum).setIdentity();

    matrix_t dfdx = matrix_t::Zero(Jv.cols(), info.stateDim);
    dfdx.block(0, 0, Jv.cols(), 6) = Jv.leftCols(6);
    dfdx.noalias() = Jq * dqdx;

    matrix_t dfdu = matrix_t::Zero(Jv.cols(), info.stateDim);
    dfdx.block(0, 0, Jv.cols(), info.actuatedDofNum) = Jv.rightCols(info.actuatedDofNum);

    return {dfdx, dfdu};
  }
  
  /******************************************************************************************************/
  /******************************************************************************************************/
  /******************************************************************************************************/
  template <typename SCALAR_T>
  FloatingBaseModelPinocchioMappingTpl<SCALAR_T>::FloatingBaseModelPinocchioMappingTpl(const FloatingBaseModelPinocchioMappingTpl& rhs)
  : pinocchioInterfacePtr_(nullptr), floatingBaseModelInfo_(rhs.floatingBaseModelInfo_) {}


// explicit template instantiation
template class FloatingBaseModelPinocchioMappingTpl<ocs2::scalar_t>;
template class FloatingBaseModelPinocchioMappingTpl<ocs2::ad_scalar_t>;

} // namespace floating_base_model