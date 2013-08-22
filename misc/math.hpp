#ifndef LUI_MISC_MATH_HPP
#define LUI_MISC_MATH_HPP

#include <iterator>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>

namespace misc { namespace math {

/*!
	éwêîà⁄ìÆïΩãœÇåvéZÇ∑ÇÈ
*/
template<class InputIterator, class T>
T exp_average(InputIterator first, InputIterator last, T init )
{
	// a = 2/(size+1)
	// S[t] = a * Y[t-1] + (1-a)*S[t-1]
	// S1 = Y0Ç≈åvéZÇ∑ÇÈ
	const T a = static_cast<T>(
		T(2)/( T(std::distance(first,last)) + T(1) )
	);
	InputIterator it = first;
	T s = T(*it) + init;
	
	for(;it!=last; ++it)
	{
		s = a*T(*it) + (T(1)-a)*s;
	}
	return s;
}
	
/*!
	ãtçsóÒÇéZèoÇ∑ÇÈ
*/
template <class M>
M invert(const M& m)
{
	namespace ublas = boost::numeric::ublas;

	BOOST_UBLAS_CHECK(m.size1() == m.size2(), ublas::external_logic());

	ublas::matrix<typename M::value_type> lhs(m);
	ublas::matrix<typename M::value_type> rhs(ublas::identity_matrix<typename M::value_type>(m.size1()));
	ublas::permutation_matrix<> pm(m.size1());

	ublas::lu_factorize(lhs, pm);
	ublas::lu_substitute(lhs, pm, rhs);

	BOOST_UBLAS_CHECK(rhs.size1() == m.size1(), ublas::internal_logic());
	BOOST_UBLAS_CHECK(rhs.size2() == m.size2(), ublas::internal_logic());

#if BOOST_UBLAS_TYPE_CHECK
	BOOST_UBLAS_CHECK(
		ublas::detail::expression_type_check(
			ublas::prod(m, rhs),
			ublas::identity_matrix<typename M::value_type>(m.size1())
		),
		ublas::internal_logic()
	);
#endif

	return rhs;
}

}}//namespace

#endif
