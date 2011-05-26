#ifndef RBL_EM_TYPES_META_H
#define RBL_EM_TYPES_META_H

#include <boost/cstdint.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/mpl/find.hpp>
#include <boost/variant.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/size.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

namespace rubble { namespace event_model { namespace types {

template<typename SEQ, typename T>
struct get_ordinal_in_sequence_f
{
  typedef typename boost::mpl::find<SEQ,T>::type it;
  typedef typename boost::mpl::end<SEQ>::type end;

  BOOST_MPL_ASSERT_NOT (( boost::is_same< it, end >));
  
  typedef typename it::pos pos;

};

template<typename SEQ ,typename VARIANT, typename TRAIT_EXTRACTOR>
struct variant_ordinal_map : 
  boost::array< VARIANT, boost::mpl::size<SEQ>::type::value> 
{
  typedef boost::array< VARIANT,boost::mpl::size<SEQ>::type::value> array_t;
  struct set_rule_f
  {
    template<typename T>
    void operator()(T, array_t & a)
    {
      typedef typename get_ordinal_in_sequence_f<SEQ,T>::pos pos;
      typedef typename  boost::mpl::apply_wrap1<TRAIT_EXTRACTOR,T >::type current_variant_type;      
 
      a.at(pos::value) = current_variant_type();
    }
  };
 
  variant_ordinal_map()
  {
    boost::mpl::for_each<SEQ> ( boost::bind<void>(set_rule_f(),_1, boost::ref(*this))); 
  }      
};
} } }

#endif
