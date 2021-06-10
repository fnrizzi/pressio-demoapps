
#include "pressio_ode_explicit.hpp"
#include "euler.hpp"
#include "../observer.hpp"

template<class scalar_type>
void analytical(const scalar_type x,
     scalar_type t,
     std::array<scalar_type,3> & prim)
{
  prim[0] = 1. + 0.2*std::sin(M_PI*(x-t) );
  prim[1] = 1.;
  prim[2] = 1.;
}

int main(int argc, char *argv[])
{

  namespace pda = pressiodemoapps;
  const auto meshObj = pda::loadCellCenterUniformMeshEigen(".");
  const auto order   = pda::reconstructionEnum::fifthOrderWeno;
  const auto probId  = pda::euler1dproblemsEnum::periodic;
  auto appObj	     = pda::createEuler1dEigen(meshObj, order, probId);

  using app_t = decltype(appObj);
  using app_state_t = typename app_t::state_type;
  using ode_state_t = pressio::containers::Vector<app_state_t>;
  using scalar_t = typename app_t::scalar_type;

  const auto gamma = appObj.gamma();
  const auto stateSize = appObj.totalDofStencilMesh();
  using ode_state_t = pressio::containers::Vector<app_state_t>;
  ode_state_t state(stateSize);
  std::array<scalar_t, 3> prim;
  const auto x = meshObj.viewX();
  for (int i=0; i<x.size(); ++i){
    analytical(x(i), 0.0, prim);
    const auto ind = i*3;
    state(ind)   = prim[0];
    state(ind+1) = prim[0]*prim[1];
    state(ind+2) = pressiodemoapps::ee::computeEnergyFromPrimitive(gamma, prim);
  }

  auto stepperObj = pressio::ode::createRungeKutta4Stepper(state, appObj);
  FomObserver<ode_state_t> Obs("1d_euler_convergence_solution.bin", 1);

  const auto dt = 0.001;
  const auto Nsteps = 2./dt;
  pressio::ode::advanceNSteps(stepperObj, state, 0., dt, Nsteps, Obs);

  return 0;
}