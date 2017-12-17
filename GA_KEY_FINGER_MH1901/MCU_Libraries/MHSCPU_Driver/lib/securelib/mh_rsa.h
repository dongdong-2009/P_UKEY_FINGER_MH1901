#ifndef __MH_RSA_H
#define __MH_RSA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mh_bignum.h"

#define MH_MIN_RSA_MODULUS_BITS			1024
#define MH_MAX_RSA_MODULUS_BITS			2048
#define MH_MAX_RSA_MODULUS_BYTES		((MH_MAX_RSA_MODULUS_BITS + 7) / 8)
#define MH_MAX_RSA_PRIME_BITS			  ((MH_MAX_RSA_MODULUS_BITS + 1) / 2)
#define MH_MAX_RSA_PRIME_BYTES			((MH_MAX_RSA_PRIME_BITS + 7) / 8)

	typedef struct
	{
		uint32_t bytes;							          //�ֽڳ���
		uint8_t e[4];							            //��Կָ��
		uint8_t p[MH_MAX_RSA_PRIME_BYTES];		//����p
		uint8_t q[MH_MAX_RSA_PRIME_BYTES];		//����q
		uint8_t n[MH_MAX_RSA_MODULUS_BYTES];  //ģ��n
		uint8_t d[MH_MAX_RSA_MODULUS_BYTES];  //˽Կָ��
		uint8_t dp[MH_MAX_RSA_PRIME_BYTES];		//d mod (p-1)
		uint8_t dq[MH_MAX_RSA_PRIME_BYTES];		//d mod (q-1)
		uint8_t qp[MH_MAX_RSA_PRIME_BYTES];		//q^-1 mod p 
		
		uint8_t n_c[MH_MAX_RSA_MODULUS_BYTES];           //ģ��n����C
		uint8_t n_q[4];				                           //ģ��n����Q
		uint8_t p_c[MH_MAX_RSA_PRIME_BYTES];	           //����p����C
		uint8_t p_q[4];							                     //����p����Q
		uint8_t q_c[MH_MAX_RSA_PRIME_BYTES];	           //����q����C
		uint8_t q_q[4];							                     //����q����Q
	}mh_rsa_private_key;


typedef struct
{
	uint32_t bytes;								          //�ֽڳ���
	uint8_t e[4];								            //��Կָ��
	uint8_t n[MH_MAX_RSA_MODULUS_BYTES];		//ģ��n
	uint8_t n_c[MH_MAX_RSA_MODULUS_BYTES];	//ģ��n����C
	uint8_t n_q[4];								          //ģ��n����Q
}mh_rsa_public_key;

typedef enum
{
	MH_RSA_1024	= 1024,
	MH_RSA_2048	= 2048,
}mh_rsa_key_bits_def;

//macro define
#define MH_RET_RSA_SUCCESS 					(('R'<<24)|('E'<<16)|('R'<<8)|('S'))
#define MH_RET_RSA_FAILURE 					(('R'<<24)|('E'<<16)|('R'<<8)|('E'))

#define MH_RET_RSA_GKEY_SUCCESS		 		(('R'<<24)|('G'<<16)|('K'<<8)|('S'))
#define MH_RET_RSA_GKEY_FAILURE		 		(('R'<<24)|('G'<<16)|('K'<<8)|('F'))

#define MH_RET_RSA_CKEY_SUCCESS		 		(('R'<<24)|('C'<<16)|('K'<<8)|('S'))
#define MH_RET_RSA_CKEY_FAILURE		 		(('R'<<24)|('C'<<16)|('K'<<8)|('F'))

#define MH_RET_PRIME_NOT_ACCEPTABLE			(('R'<<24)|('P'<<16)|('N'<<8)|('A'))
#define MH_RET_PRIME_IS_PRIME				    (('R'<<24)|('P'<<16)|('I'<<8)|('P'))
#define MH_RET_PRIME_INPUT_PARA_ERR			(('R'<<24)|('P'<<16)|('P'<<8)|('E'))


/**
  * @method	RSA_Private
  * @brief	Raw RSA private-key operation. Useing CRT Algorithm
  * @param	output		:output data buffer
  * @param	input		:input data buffer
  * @param	key			:RSA private key
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval				:MH_RET_RSA_SUCCESS or MH_RET_RSA_ERROR
  */
uint32_t mh_rsa_private_crt(uint8_t *output, uint8_t *input, mh_rsa_private_key *key,
							mh_rng_callback f_rng, void *p_rng);

/**
  * @method	RSA_Public
  * @brief	Raw RSA public-key operation. 
  * @param	output		:output data buffer
  * @param	input		:input data buffer
  * @param	key			:RSA public key
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval				:MH_RET_RSA_SUCCESS or MH_RET_RSA_ERROR
  */
uint32_t mh_rsa_public(uint8_t *output, uint8_t *input, mh_rsa_public_key *key,
							mh_rng_callback f_rng, void *p_rng);

/**
  * @method	RSA_GenerateKey
  * @brief	Generate a RSA private key
  * @param	key			:rsa private key point
  * @param	exponent	:public exponent
  * @param	nbits		:rsa module bits
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval 
  */
uint32_t mh_rsa_efc_gen_key(mh_rsa_private_key *key, uint32_t exponent, uint32_t nbits,
							mh_rng_callback f_rng, void *p_rng);
/**
  * @method	mh_rsa_complete_key
  * @brief	complete the private key by the rsa key paras p q d. 
  * @param	key			:rsa private key point
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval 
  */
uint32_t mh_rsa_complete_key(mh_rsa_private_key *key, mh_rng_callback f_rng, void *p_rng);
/**
  * @method	mh_rsa_complete_key_e
  * @brief	complete the private key by the rsa key paras p q e. 
  * @param	key			:rsa private key point
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval 
  */
uint32_t mh_rsa_complete_key_e(mh_rsa_private_key *key, mh_rng_callback f_rng, void *p_rng);
/**
  * @method	mh_rsa_get_private_key_C_Q
  * @brief	
  * @param	key			:rsa private key point
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval 
  */
uint32_t mh_rsa_get_private_key_C_Q(mh_rsa_private_key *key, mh_rng_callback f_rng, void *p_rng);
/**
  * @method	mh_rsa_get_public_key_C_Q
  * @brief	
  * @param	key			:rsa private key point
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval 
  */
uint32_t mh_rsa_get_public_key_C_Q(mh_rsa_public_key *key, mh_rng_callback f_rng, void *p_rng);
/**
  * @method	mh_rsa_is_prime
  * @brief	
  * @param	X			:big number array
  * @param	size		:the big number len (32bit)
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval 
  */
uint32_t mh_rsa_is_prime( uint32_t *X, uint32_t size, mh_rng_callback f_rng, void *p_rng);

/**
  * @method	mh_rsa_efc_gen_prime
  * @brief	
  * @param	X			:big number array
  * @param	size		:the big number bits (192/256/512/1024)
  * @param	f_rng		:true random number generation function point
  * @param	p_rng		:true random number generation para
  * @retval 
  */
uint32_t mh_rsa_efc_gen_prime( uint32_t *X, uint32_t nbits, mh_rng_callback f_rng, void *p_rng);
uint32_t mh_rsa_private(uint8_t *output, uint8_t *input, mh_rsa_private_key *key, mh_rng_callback f_rng, void *p_rng);
uint32_t mh_rsa_private_tmp(uint8_t *output, uint8_t *input, mh_rsa_private_key *key, mh_rng_callback f_rng, void *p_rng);
#ifdef __cplusplus
}
#endif

#endif
