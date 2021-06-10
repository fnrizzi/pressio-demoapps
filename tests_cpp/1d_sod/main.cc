
#include "pressio_ode_explicit.hpp"
#include "euler1d.hpp"
#include "../observer.hpp"

int main(int argc, char *argv[])
{
  namespace pda = pressiodemoapps;
  const auto meshObj = pda::loadCellCenterUniformMeshEigen(".");
#ifdef USE_WENO5
  const auto order   = pda::reconstructionEnum::fifthOrderWeno;
#else
  const auto order   = pda::reconstructionEnum::firstOrder;
#endif

  const auto probId  = pda::euler1dproblemsEnum::sod;
  auto appObj      = pda::createEuler1dEigen(meshObj, order, probId);
  using app_t = decltype(appObj);
  using app_state_t = typename app_t::state_type;
  using ode_state_t = pressio::containers::Vector<app_state_t>;
  using scalar_t = typename app_t::scalar_type;

  const auto gamma = appObj.gamma();
  const auto stateSize = appObj.totalDofStencilMesh();
  using ode_state_t = pressio::containers::Vector<app_state_t>;
  ode_state_t state = appObj.initialCondition();

  auto stepperObj = pressio::ode::createRungeKutta4Stepper(state, appObj);
  FomObserver<ode_state_t> Obs("sod1d_solution.bin", 1);

  const auto dt = 0.001;
  const auto Nsteps = 100;
  pressio::ode::advanceNSteps(stepperObj, state, 0., dt, Nsteps, Obs);

  return 0;
}