/*
 * Port_Assert.h
 *
 *  Created on: Jun 23, 2023
 *      Author: ali20
 */

#ifndef ASSERT_H_
#define ASSERT_H_


#define vLib_ASSERT(exp, errCode)                                         \
{                                                                         \
	if ((exp) == 0)                                                       \
	{                                                                     \
		while(1);                                            		      \
	}                                                                     \
}



#endif /* ASSERT_H_ */
