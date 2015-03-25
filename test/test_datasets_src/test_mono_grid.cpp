//  (C) Copyright Gennadiy Rozental 2011-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief tests monomorphic grid
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/grid.hpp>


#ifndef BOOST_TEST_NO_GRID_COMPOSITION_AVAILABLE


#include <boost/test/data/monomorphic/singleton.hpp>
#include <boost/test/data/monomorphic/array.hpp>
#include <boost/test/data/monomorphic/collection.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>

namespace data = boost::unit_test::data;

#include "test_datasets.hpp"

BOOST_AUTO_TEST_CASE( test_mono_grid_size_and_composition )
{
  BOOST_TEST( (data::make( 1 ) * data::make( 5 )).size() == 1 );
  BOOST_TEST( (data::make( std::vector<int>(2) ) * data::make( std::list<float>(2) )).size() == 4 );
  BOOST_TEST( (data::make( std::vector<int>(2) ) * data::make( 5. )).size() == 2 );
  BOOST_TEST( (data::make( std::vector<int>(3) ) * data::make( std::list<int>(1) )).size() == 3 );

  BOOST_TEST( (data::make( std::vector<int>(3) ) * data::make( std::list<std::string>(3) ) * data::make( 5 )).size() == 9 );
  BOOST_TEST( (data::make( std::vector<int>(1) ) * data::make( std::list<int>(3) ) * data::make( 5 )).size() == 3 );
}

#if !defined(BOOST_NO_CXX11_AUTO_DECLARATIONS)
BOOST_AUTO_TEST_CASE( test_mono_grid_with_xrange )
{
  auto ds1 = data::make(1);
  auto ds2 = data::xrange(5);
  
  BOOST_TEST( (ds1 * ds2).size() == 5 );
  BOOST_TEST( (ds1 * ds2).size() == 5 );
}
#endif

#if !defined(BOOST_NO_CXX11_AUTO_DECLARATIONS) && !defined(BOOST_NO_CXX11_LAMBDAS)
BOOST_AUTO_TEST_CASE( test_mono_grid_cpp11_features )
{
  int arr1[]         = {1,2};
  char const* arr2[] = {"a","b"};
  int* exp1 = arr1;
  char const** exp2 = arr2;
  invocation_count ic;

  auto samples1 = data::make( arr1 ) * data::make( arr2 );

  BOOST_TEST( samples1.size() == 4 );

  ic.m_value = 0;
  data::for_each_sample( samples1, ic );
  BOOST_TEST( ic.m_value == 4 );

  data::for_each_sample( samples1, check_arg_type_like<std::tuple<int,char const*>>() );

  int c = 0;
  data::for_each_sample( samples1, [&c,exp1,exp2](int i,char const* s) {
      BOOST_TEST( i == exp1[c/2] );
      BOOST_TEST( s == exp2[c%2] );
      ++c;
  });

  std::vector<double> vec1;
  vec1.push_back(2.1);
  vec1.push_back(3.2);
  vec1.push_back(4.7);
  int arr3[] = {4,2,1};

  auto samples2 = data::make( vec1 ) * data::make( "qqq" ) * data::make( arr3 );

  BOOST_TEST( samples2.size() == 9 );

  ic.m_value = 0;
  data::for_each_sample( samples2, ic );
  BOOST_TEST( ic.m_value == 9 );

  data::for_each_sample( samples2, check_arg_type_like<std::tuple<double,char const*,int>>() );

  int* exp3 = arr3;
  c = 0;

  data::for_each_sample( samples2, [&c,&vec1,exp3](double a1,char const* a2,int a3) {
      BOOST_TEST( a1 == vec1[c/3] );
      BOOST_CHECK_EQUAL( a2, "qqq" );
      BOOST_TEST( a3 == exp3[c%3] );
      ++c;
  });

}
#endif


BOOST_AUTO_TEST_CASE( test_mono_grid_number_of_copies )
{
  const int value_to_check = 0;

  copy_count::value() = 0;
  data::for_each_sample( data::make( copy_count() ) * data::make( copy_count() ), check_arg_type<std::tuple<copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  data::for_each_sample( data::make( copy_count() ) * data::make( copy_count() ) * data::make( copy_count() ), 
                         check_arg_type<std::tuple<copy_count,copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  data::for_each_sample( data::make( copy_count() ) * (data::make( copy_count() ) * data::make( copy_count() )), 
                         check_arg_type<std::tuple<copy_count,copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );
}


#if !defined(BOOST_NO_CXX11_AUTO_DECLARATIONS)
BOOST_AUTO_TEST_CASE( test_mono_grid_number_of_copies_auto )
{
  auto ds1        = data::make( copy_count() );
  auto const ds2  = data::make( copy_count() );

  copy_count::value() = 0;
  data::for_each_sample( ds1 * ds1, check_arg_type<std::tuple<copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  data::for_each_sample( ds2 * ds2, check_arg_type<std::tuple<copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  data::for_each_sample( ds1 * ds2, check_arg_type<std::tuple<copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  auto zp1 = ds1 * data::make( copy_count() );
  BOOST_TEST( zp1.size() == 1 );
  data::for_each_sample( zp1, check_arg_type<std::tuple<copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  data::for_each_sample( data::make( copy_count() ) * ds1, check_arg_type<std::tuple<copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  data::for_each_sample( ds1 * ds2 * ds1, check_arg_type<std::tuple<copy_count,copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );

  copy_count::value() = 0;
  data::for_each_sample( ds1 * (ds1 * ds2), check_arg_type<std::tuple<copy_count,copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == 0 );


  copy_count::value() = 0;
  int std_vector_constructor_count = 0;
  {
    std::vector<copy_count> v(2); // we cannot do better than std::vector constructor
    std_vector_constructor_count = copy_count::value()/2;
  }



  copy_count::value() = 0;
  auto ds3 = data::make( make_copy_count_collection() ) * data::make( make_copy_count_collection() );
  BOOST_TEST( ds3.size() == 9 );
  data::for_each_sample( ds3, check_arg_type<std::tuple<copy_count,copy_count>>() );
  BOOST_TEST( copy_count::value() == std_vector_constructor_count *2 *3);
}
#endif // !defined(BOOST_NO_CXX11_AUTO_DECLARATIONS)

#endif // BOOST_TEST_NO_GRID_COMPOSITION_AVAILABLE

