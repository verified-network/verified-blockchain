/* Definition of the Verified provider
 * Copyright Kallol Borah 2020
 */

#ifdef __cplusplus

#define VERIFIEDBLOCKCHAIN_API __declspec(dllexport)
#include "pch.h"


class provider {


};


#endif

#ifdef __cplusplus
extern "C" {
#endif
#if defined(__STDC__) || defined(__cplusplus)


#else
	
	VERIFIEDBLOCKCHAIN_API extern bool registerContract(char*);
	VERIFIEDBLOCKCHAIN_API extern bool unregisterContract(char*);

#endif
#ifdef __cplusplus
}
#endif

