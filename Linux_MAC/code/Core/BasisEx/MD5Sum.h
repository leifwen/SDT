/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */
/*
 * Copyright (c) 2012-2014 
 * All rights reserved.
 *
 * File Name: MD5Sum.h
 * File ID	:
 * Remark	:
 * Writer	: Leif Wen
 * Date		: 2013.12.16
 * @2013.12.16 : use string to replace cstring
 				add read file method to save memory

*/
//------------------------------------------------------------------------------------------//
#ifdef MD5SumH
//#define MD5SumH
//------------------------------------------------------------------------------------------//
#include "BasicDef.h"
#include "BasicClass.h"
//------------------------------------------------------------------------------------------//
typedef uint8 * POINTER;
typedef unsigned short int UINT2;
typedef unsigned long int UINT4;

typedef struct{
    UINT4 state[4];
    UINT4 count[2];
    uint8 buffer[64];
} MD5_CTX;

void MD5Init	(MD5_CTX *);
void MD5Update	(MD5_CTX *, const uint8 *, uint32);
void MD5Final	(uint8 [16], MD5_CTX *);
//------------------------------------------------------------------------------------------//
class MD5Sum{
	public:
				 MD5Sum(void);
		virtual ~MD5Sum(void){;};
	private:
		std::string		m_sHash;
		uint8			m_rawHash[16];
	public:
		const std::string	&Calculate		(const std::string &sSource);
		const std::string	&Calculate		(const uint8 *pachSource, uint32 nLen);
		const std::string	&CalculateFile	(const std::string &tFileName);
		const std::string	&GetHash		(void){return m_sHash;};
		const uint8			*GetRawHash		(void){return(m_rawHash);};
};
//------------------------------------------------------------------------------------------//
#endif