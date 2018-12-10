/**
 * \defgroup TestSuite
 * \brief Source for BDD package test suite
 * \file Tests.cpp
 * \author vdscp_8
*/
/// Tests
#include "Tests.h"

/// main
int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
