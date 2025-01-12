
#ifndef PRESSIODEMOAPPS_EULER3D_INC_HPP_
#define PRESSIODEMOAPPS_EULER3D_INC_HPP_

#include "./predicates/all.hpp"
#include "./container_fncs/all.hpp"
#include "./mesh.hpp"
#include "./schemes_info.hpp"
#include "./euler_compute_energy.hpp"
#include "./adapter_mixins.hpp"

namespace pressiodemoapps{
enum class Euler3d{
  PeriodicSmooth,
  SedovSymmetry
};
}//end namespace pressiodemoapps

#include "./impl/euler_3d_prob_class.hpp"

namespace pressiodemoapps{
namespace implee3d{
template<class mesh_t, class T>
T createEe3dImpl(const mesh_t & meshObj,
		 ::pressiodemoapps::InviscidFluxReconstruction recEnum,
		 ::pressiodemoapps::Euler3d probEnum,
		 ::pressiodemoapps::InviscidFluxScheme fluxEnum)
{
  const auto stencilSize = meshObj.stencilSize();
  const auto check1 = stencilSizeCompatibleWithInviscidFluxReconstruction(recEnum, stencilSize);
  if (!check1){
    throw std::runtime_error
      ("Stencil size in the mesh object not compatible with desired inviscid flux reconstruction.");
  }

  if (probEnum == ::pressiodemoapps::Euler3d::PeriodicSmooth)
  {
    if (!meshObj.isPeriodic()){
      throw std::runtime_error
      ("For euler3d::PeriodicSmooth, mesh must be periodic.");
    }
  }

  return T(meshObj, probEnum, recEnum, fluxEnum);
}

#if defined PRESSIODEMOAPPS_ENABLE_BINDINGS
template<class mesh_t, class T>
T create_problem_for_pyA(const mesh_t & meshObj,
		      ::pressiodemoapps::Euler3d probEnum,
		      ::pressiodemoapps::InviscidFluxReconstruction recEnum)
{
  return implee3d::createEe3dImpl<mesh_t, T>(meshObj, recEnum, probEnum,
					     InviscidFluxScheme::Rusanov);
}

template<class mesh_t, class T>
T create_problem_for_pyB(const mesh_t & meshObj,
		      ::pressiodemoapps::Euler3d probEnum,
		      ::pressiodemoapps::InviscidFluxReconstruction recEnum,
		      const int ic)
{
  return implee3d::createEe3dImpl<mesh_t, T>(meshObj, recEnum, probEnum,
					     InviscidFluxScheme::Rusanov);
}
#endif

} //end pressiodemoapps::implee3d

#if not defined PRESSIODEMOAPPS_ENABLE_BINDINGS
template<class mesh_t>
auto create_problem_eigen(const mesh_t & meshObj,
			::pressiodemoapps::Euler3d probEnum,
			::pressiodemoapps::InviscidFluxReconstruction recEnum,
			::pressiodemoapps::InviscidFluxScheme fluxEnum)
{
  using p_t = ::pressiodemoapps::ee::impl::EigenEuler3dApp<mesh_t>;
  using RetType = PublicProblemMixinCpp<p_t>;
  return implee3d::createEe3dImpl<mesh_t, RetType>(meshObj, recEnum, probEnum, fluxEnum);
}

template<class mesh_t>
auto create_problem_eigen(const mesh_t & meshObj,
			::pressiodemoapps::Euler3d probEnum,
			::pressiodemoapps::InviscidFluxReconstruction recEnum,
			const int initCondIdentifier)
{
  return create_problem_eigen(meshObj, probEnum, recEnum,  InviscidFluxScheme::Rusanov);
}

template<class mesh_t>
auto create_problem_eigen(const mesh_t & meshObj,
			::pressiodemoapps::Euler3d probEnum,
			::pressiodemoapps::InviscidFluxReconstruction recEnum)
{
  return create_problem_eigen(meshObj, probEnum, recEnum,
			    InviscidFluxScheme::Rusanov);
}
#endif

}//end namespace pressiodemoapps
#endif
