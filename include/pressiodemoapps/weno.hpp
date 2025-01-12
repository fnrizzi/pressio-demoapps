
#ifndef PRESSIODEMOAPPS_WENO_PUBLIC_HPP_
#define PRESSIODEMOAPPS_WENO_PUBLIC_HPP_

#include "array"
#include "vector"
#include "./impl/weno3.hpp"
#include "./impl/weno5.hpp"
#include "Eigen/Dense"

namespace pressiodemoapps{

//*********
// WENO3 //
//*********
template<class sc_t>
typename std::enable_if<std::is_floating_point<sc_t>::value>::type
weno3(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      const sc_t & qm2,
      const sc_t & qm1,
      const sc_t & qq,
      const sc_t & qp1,
      const sc_t & qp2)
{
  impl::weno3(uLeftNeg, uLeftPos, qm2, qm1, qq, qp1);
  impl::weno3(uRightNeg, uRightPos, qm1, qq, qp1, qp2);
}

template<class sc_t, class GradContainer>
typename std::enable_if<std::is_floating_point<sc_t>::value>::type
weno3(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      GradContainer & gradLNeg,
      GradContainer & gradLPos,
      GradContainer & gradRNeg,
      GradContainer & gradRPos,
      const sc_t & qm2,
      const sc_t & qm1,
      const sc_t & qq,
      const sc_t & qp1,
      const sc_t & qp2)
{
  impl::weno3WithGrad(uLeftNeg, uLeftPos,
		      gradLNeg, gradLPos,
		      qm2, qm1, qq, qp1);
  impl::weno3WithGrad(uRightNeg, uRightPos,
		      gradRNeg, gradRPos,
		      qm1, qq, qp1, qp2);
}

template<class sc_t>
typename std::enable_if<std::is_floating_point<sc_t>::value>::type
weno3(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      const std::array<sc_t, 5> & q)
{
  weno3(uLeftNeg, uLeftPos,
	uRightNeg, uRightPos,
	q[0], q[1], q[2], q[3], q[4]);
}

template<class sc_t, class state_t>
typename std::enable_if<
  std::is_floating_point<sc_t>::value and
  !std::is_void<decltype(std::declval<state_t>()(0))>::value
  >::type
weno3(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      const state_t & q)
{
  weno3(uLeftNeg, uLeftPos,
	uRightNeg, uRightPos,
	q(0), q(1), q(2), q(3), q(4));
}

template<class edge_t, class state_t, class index_t>
typename std::enable_if<
  !std::is_floating_point<edge_t>::value and
  !std::is_void<decltype(std::declval<state_t>()(0))>::value
  >::type
weno3(edge_t & uLeftNeg,
      edge_t & uLeftPos,
      edge_t & uRightNeg,
      edge_t & uRightPos,
      const state_t & q,
      index_t l1i, index_t l0i,
      index_t r0i, index_t r1i,
      index_t uIndex,
      int numDof)
{

  for (int dof=0; dof<numDof; ++dof){
    impl::weno3(uLeftNeg(dof), uLeftPos(dof),
		q(l1i+dof), q(l0i+dof), q(uIndex+dof), q(r0i+dof));

    impl::weno3(uRightNeg(dof), uRightPos(dof),
		q(l0i+dof), q(uIndex+dof), q(r0i+dof), q(r1i+dof));
  }
}

template<class edge_t, class state_t, class index_t, class sc_t, int numRows>
typename std::enable_if<
  !std::is_floating_point<edge_t>::value and
  !std::is_void<decltype(std::declval<state_t>()(0))>::value
  >::type
weno3(edge_t & uLeftNeg,
      edge_t & uLeftPos,
      edge_t & uRightNeg,
      edge_t & uRightPos,
      Eigen::Matrix<sc_t, numRows, -1> & gradLNeg,
      Eigen::Matrix<sc_t, numRows, -1> & gradLPos,
      Eigen::Matrix<sc_t, numRows, -1> & gradRNeg,
      Eigen::Matrix<sc_t, numRows, -1> & gradRPos,
      const state_t & q,
      index_t l1i, index_t l0i,
      index_t r0i, index_t r1i,
      index_t uIndex,
      int numDof)
{

  for (int dof=0; dof<numDof; ++dof)
  {
    auto gLNeg = gradLNeg.row(dof);
    auto gLPos = gradLPos.row(dof);
    impl::weno3WithGrad(uLeftNeg(dof), uLeftPos(dof),
			gLNeg, gLPos,
			q(l1i+dof), q(l0i+dof), q(uIndex+dof), q(r0i+dof));

    auto gRNeg = gradRNeg.row(dof);
    auto gRPos = gradRPos.row(dof);
    impl::weno3WithGrad(uRightNeg(dof), uRightPos(dof),
			gRNeg, gRPos,
			q(l0i+dof), q(uIndex+dof), q(r0i+dof), q(r1i+dof));
  }
}

template<class sc_t, class state_t, class index_t>
typename std::enable_if<
  !std::is_void<decltype(std::declval<state_t>()[0])>::value
  >::type
weno3(std::vector<sc_t> & uLeftNeg,
      std::vector<sc_t> & uLeftPos,
      std::vector<sc_t> & uRightNeg,
      std::vector<sc_t> & uRightPos,
      const state_t & q,
      index_t l1i, index_t l0i,
      index_t r0i, index_t r1i,
      index_t uIndex,
      int numDof)
{

  for (int dof=0; dof<numDof; ++dof){
    impl::weno3(uLeftNeg[dof], uLeftPos[dof],
		q[l1i+dof], q[l0i+dof], q[uIndex+dof], q[r0i+dof]);

    impl::weno3(uRightNeg[dof], uRightPos[dof],
		q[l0i+dof], q[uIndex+dof], q[r0i+dof], q[r1i+dof]);
  }
}


//*********
// WENO5 //
//*********
template<class sc_t>
typename std::enable_if<std::is_floating_point<sc_t>::value>::type
weno5(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      const sc_t & qm3,
      const sc_t & qm2,
      const sc_t & qm1,
      const sc_t & qq,
      const sc_t & qp1,
      const sc_t & qp2,
      const sc_t & qp3)
{
  impl::weno5(uLeftNeg, uLeftPos,
	      qm3, qm2, qm1, qq, qp1, qp2);
  impl::weno5(uRightNeg, uRightPos,
	      qm2, qm1, qq, qp1, qp2, qp3);
}

template<class sc_t, class GradContainer>
typename std::enable_if<std::is_floating_point<sc_t>::value>::type
weno5(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      GradContainer & gradLNeg,
      GradContainer & gradLPos,
      GradContainer & gradRNeg,
      GradContainer & gradRPos,
      const sc_t & qm3,
      const sc_t & qm2,
      const sc_t & qm1,
      const sc_t & qq,
      const sc_t & qp1,
      const sc_t & qp2,
      const sc_t & qp3)
{
  impl::weno5WithGrad(uLeftNeg, uLeftPos,
		      gradLNeg, gradLPos,
		      qm3, qm2, qm1, qq, qp1, qp2);
  impl::weno5WithGrad(uRightNeg, uRightPos,
		      gradRNeg, gradRPos,
		      qm2, qm1, qq, qp1, qp2, qp3);
}

template<class sc_t>
typename std::enable_if<std::is_floating_point<sc_t>::value>::type
weno5(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      const std::array<sc_t, 7> & q)
{
  weno5(uLeftNeg, uLeftPos,
	uRightNeg, uRightPos,
	q[0], q[1], q[2],
	q[3],
	q[4], q[5], q[6]);
}

template<class sc_t, class state_t>
typename std::enable_if<
  std::is_floating_point<sc_t>::value and
  !std::is_void<decltype(std::declval<state_t>()(0))>::value
  >::type
weno5(sc_t & uLeftNeg,
      sc_t & uLeftPos,
      sc_t & uRightNeg,
      sc_t & uRightPos,
      const state_t & q)
{
  weno5(uLeftNeg, uLeftPos,
	uRightNeg, uRightPos,
	q(0), q(1), q(2),
	q(3),
	q(4), q(5), q(6));
}

template<class edge_t, class state_t, class index_t>
typename std::enable_if<
  !std::is_floating_point<edge_t>::value and
  !std::is_void<decltype(std::declval<state_t>()(0))>::value
  >::type
weno5(edge_t & uLeftNeg,
      edge_t & uLeftPos,
      edge_t & uRightNeg,
      edge_t & uRightPos,
      const state_t & q,
      index_t l2i, index_t l1i, index_t l0i,
      index_t r0i, index_t r1i, index_t r2i,
      index_t uIndex,
      int numDof)
{

  for (int dof=0; dof<numDof; ++dof)
    {
      impl::weno5(uLeftNeg(dof), uLeftPos(dof),
		  q(l2i+dof), q(l1i+dof), q(l0i+dof),
		  q(uIndex+dof),
		  q(r0i+dof), q(r1i+dof));

      impl::weno5(uRightNeg(dof), uRightPos(dof),
		  q(l1i+dof), q(l0i+dof),
		  q(uIndex+dof),
		  q(r0i+dof), q(r1i+dof), q(r2i+dof));
    }
}

template<class edge_t, class state_t, class index_t, class sc_t, int numRows>
typename std::enable_if<
  !std::is_floating_point<edge_t>::value and
  !std::is_void<decltype(std::declval<state_t>()(0))>::value
  >::type
weno5(edge_t & uLeftNeg,
      edge_t & uLeftPos,
      edge_t & uRightNeg,
      edge_t & uRightPos,
      Eigen::Matrix<sc_t, numRows, -1> & gradLNeg,
      Eigen::Matrix<sc_t, numRows, -1> & gradLPos,
      Eigen::Matrix<sc_t, numRows, -1> & gradRNeg,
      Eigen::Matrix<sc_t, numRows, -1> & gradRPos,
      const state_t & q,
      index_t l2i, index_t l1i, index_t l0i,
      index_t r0i, index_t r1i, index_t r2i,
      index_t uIndex,
      int numDof)
{

  for (int dof=0; dof<numDof; ++dof)
  {
    auto gLNeg = gradLNeg.row(dof);
    auto gLPos = gradLPos.row(dof);
    impl::weno5WithGrad(uLeftNeg(dof), uLeftPos(dof),
			gLNeg, gLPos,
			q(l2i+dof), q(l1i+dof), q(l0i+dof),
			q(uIndex+dof),
			q(r0i+dof), q(r1i+dof));

    auto gRNeg = gradRNeg.row(dof);
    auto gRPos = gradRPos.row(dof);
    impl::weno5WithGrad(uRightNeg(dof), uRightPos(dof),
			gRNeg, gRPos,
			q(l1i+dof), q(l0i+dof),
			q(uIndex+dof),
			q(r0i+dof), q(r1i+dof), q(r2i+dof));
  }
}

template<class sc_t, class state_t, class index_t>
typename std::enable_if<
  !std::is_void<decltype(std::declval<state_t>()[0])>::value
  >::type
weno5(std::vector<sc_t> & uLeftNeg,
      std::vector<sc_t> & uLeftPos,
      std::vector<sc_t> & uRightNeg,
      std::vector<sc_t> & uRightPos,
      const state_t & q,
      index_t l2i, index_t l1i, index_t l0i,
      index_t r0i, index_t r1i, index_t r2i,
      index_t uIndex,
      int numDof)
{

  for (int dof=0; dof<numDof; ++dof)
    {
      impl::weno5(uLeftNeg[dof], uLeftPos[dof],
		  q[l2i+dof], q[l1i+dof], q[l0i+dof],
		  q[uIndex+dof],
		  q[r0i+dof], q[r1i+dof]);

      impl::weno5(uRightNeg[dof], uRightPos[dof],
		  q[l1i+dof], q[l0i+dof],
		  q[uIndex+dof],
		  q[r0i+dof], q[r1i+dof], q[r2i+dof]);
    }
}

} //end namespace pressiodemoapps
#endif
