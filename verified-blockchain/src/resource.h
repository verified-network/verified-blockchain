/* Definition of Verified container resources
 * Copyright Kallol Borah 2020
 */

#ifdef __cplusplus

#define VERIFIEDBLOCKCHAIN_API __declspec(dllexport)
#include "pch.h"

class resource {

private :

public :

	struct usage {
		double cpu_cycles;
		double memory_used;
		double network_bandwidth;
		double disk_io;
	};

	long calculateCost(std::string);

};

#else

	typedef
	struct usage
		report;


#endif

#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__) || defined(__cplusplus)


#else

	VERIFIEDBLOCKCHAIN_API extern bool reportUse(report*);

#endif
#ifdef __cplusplus
}
#endif




