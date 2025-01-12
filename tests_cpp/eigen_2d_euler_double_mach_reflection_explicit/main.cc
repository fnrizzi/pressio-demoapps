
#include "pressio/ode_steppers_explicit.hpp"
#include "pressio/ode_advancers.hpp"
#include "pressiodemoapps/euler2d.hpp"
#include "../observer.hpp"

int main(int argc, char *argv[])
{

  namespace pda = pressiodemoapps;
  const auto meshObj = pda::load_cellcentered_uniform_mesh_eigen(".");
#if defined USE_WENO3
  const auto order   = pda::InviscidFluxReconstruction::Weno3;
#else
  const auto order   = pda::InviscidFluxReconstruction::FirstOrder;
#endif

  const auto probId  = pda::Euler2d::DoubleMachReflection;
  auto appObj      = pda::create_problem_eigen(meshObj, probId, order);
  using app_t = decltype(appObj);
  using state_t = typename app_t::state_type;

  const auto stateSize = appObj.totalDofStencilMesh();
  state_t state = appObj.initialCondition();

  const auto dt = 0.0005;
  const auto Nsteps = 500;
  auto time = 0.0;

  auto stepperObj = pressio::ode::create_ssprk3_stepper(state, appObj);
  FomObserver<state_t> Obs("doubleMach2d_solution.bin", 50);
  pressio::ode::advance_n_steps_and_observe(stepperObj, state, 0., dt, Nsteps, Obs);

  return 0;
}
