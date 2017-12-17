#ifndef __MH_CONFIG_H
#define __MH_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif 

#define     CRYPT_USE_SECURED_MODE            1

	
	
#if (CRYPT_USE_SECURED_MODE)    	
	
	
	
/****************************************************************
						 RSA CRT lib Config
*****************************************************************/
/*
*	RSA CRT Blinding
*/
#define MH_RSA_CRT_USE_BLINDING   
	
#if defined(MH_RSA_CRT_USE_BLINDING)
/*
 *	RSA CRT Blinding Option
 */
#define MH_RSA_CRT_USE_BASE_BLINDING
#define MH_RSA_CRT_USE_EXPONENT_BLINDING
#define MH_RSA_CRT_USE_MODULE_BLINDING
#define MH_RSA_CRT_USE_COMBIANTION_BLINDING
	
#endif //MH_RSA_CRT_USE_BLINDING


/*
*	Result Check and Pach Check
*/
#define MH_RSA_CRT_USE_RESULT_CHECK         
#define MH_RSA_CRT_USE_PATH_CHECK
/*
*	RSA CRT Use the CRYPT Security
*/
#define MH_RSA_CRT_USE_CRYPT_SECURITY      
/*
*	RSA CRT Use the System Security Control
*/
#define MH_RSA_CRT_USE_SSC                    

/****************************************************************
						RSA PRIVATE lib Config
*****************************************************************/
/*
*	RSA PRIVATE Blinding
*/
#define MH_RSA_PRIVATE_USE_BLINDING      
 /*
 *	RSA PRIVATE Blinding Options
 */
#if defined(MH_RSA_PRIVATE_USE_BLINDING)

#define MH_RSA_PRIVATE_USE_BASE_BLINDING
#define MH_RSA_PRIVATE_USE_EXPONENT_BLINDING

#endif //MH_RSA_PRIVATE_USE_BLINDING

/*
*	Result Check and Pach Check
*/
#define MH_RSA_PRIVATE_USE_RESULT_CHECK     
#define MH_RSA_PRIVATE_USE_PATH_CHECK
/*
*	RSA Use the CRYPT Security
*/
#define MH_RSA_PRIVATE_USE_CRYPT_SECURITY    
/*
*	PRIVATE-RSA Use the System Security Control
*/
#define MH_RSA_PRIVATE_USE_SSC          
 
/****************************************************************
						RSA PUBLIC lib Config
*****************************************************************/

/*
*	PUBLIC-RSA Path Check
*/
#define MH_RSA_PUBLIC_USE_PATH_CHECK    
/*
*	PUBLIC-RSA Use the CRYPT Security
*/
#define MH_RSA_PUBLIC_USE_CRYPT_SECURITY  
/*
*	PBULIC-RSA Use the System Security Control
*/
#define MH_RSA_PUBLIC_USE_SSC    
  
/****************************************************************
					RSA COMPLETE KEY lib Config
*****************************************************************/

/*
 *	RSA Complete Key Use the CRYPT Security
 */
#define MH_RSA_COMPLETE_KEY_USE_CRYPT_SECURITY   
/*
 *	RSA Complete Key Use the System Security Control
 */
#define MH_RSA_COMPLETE_KEY_USE_SSC   


/****************************************************************
						DES lib Config 
*****************************************************************/

/*
 *	ReEncrypt and compare the input data
 */
 

#define MH_DES_USE_RAND_PARA
#define MH_DES_USE_FAKE_KEY
#define MH_DES_USE_KEY_ADDR_RAND
#define MH_DES_USE_UNUSED_KEY_FILL_RAND
#define MH_DES_USE_RESULT_CHECK
#define MH_DES_USE_KEY_READBACK_CHECK
#define MH_DES_USE_PATH_CHECK
#define MH_DES_USE_DATA_RAND_COPY
#define MH_DES_USE_DATA_REG_RAND

/*
 *	DES Use the CRYPT Security
 */
#define MH_DES_USE_CRYPT_SECURITY 
/*
 *	DES Use the System Security Control  
 */
#define MH_DES_USE_SSC    


/****************************************************************
						TDES lib Config 
*****************************************************************/

/*
 *	TDES use path check
 */

#define MH_TDES_USE_RAND_PARA
#define MH_TDES_USE_FAKE_KEY
#define MH_TDES_USE_RESULT_CHECK
#define MH_TDES_USE_KEY_READBACK_CHECK
#define MH_TDES_USE_PATH_CHECK
#define MH_TDES_USE_DATA_RAND_COPY
#define MH_TDES_USE_DATA_REG_RAND

/*
 *	TDES Use the CRYPT Security
 */
#define MH_TDES_USE_CRYPT_SECURITY       
/*
 *	TDES Use the System Security Control    
 */
#define MH_TDES_USE_SSC   


/****************************************************************
						SM4 lib Config 
*****************************************************************/

/*
 *	SM4 Security Config
 */
#define MH_SM4_USE_FAKE_KEY     
#define MH_SM4_USE_RESULT_CHECK   
#define MH_SM4_USE_PATH_CHECK    
#define MH_SM4_USE_DATA_RAND_COPY     
#define MH_SM4_USE_DATA_REG_RAND     

/*
 *	SM4 Use the CRYPT Security
 */
#define MH_SM4_USE_CRYPT_SECURITY      
/*
 *	SM4 Use the System Security Control
 */
#define MH_SM4_USE_SSC    


/****************************************************************
						ECC lib Config 
*****************************************************************/

/*
 *	ECC Security Config
 */
#define MH_ECC_USE_SECURITY     

#if (defined(MH_ECC_USE_SECURITY))
#define MH_ECC_USE_SECURITY_PADD_A
#define MH_ECC_USE_SECURITY_PADD_JA
#define MH_ECC_USE_SECURITY_PDBL_A
#define MH_ECC_USE_SECURITY_PDBL_J
#define MH_ECC_USE_SECURITY_PDBL_MJ
#define MH_ECC_USE_SECURITY_PMUL_CALC
#define MH_ECC_USE_SECURITY_PMUL_CALC_BLINDING
#define MH_ECC_USE_SECURITY_PMUL
#define MH_ECC_USE_SECURITY_PMUL_F2M_CALC
#define MH_ECC_USE_SECURITY_PMUL_F2M_CALC_BLINDING
#define MH_ECC_USE_SECURITY_PMUL_F2M
#define MH_ECC_USE_SECURITY_VERIFY_KEY
#endif

#ifdef MH_ECC_USE_SECURITY_PMUL   
#define MH_ECC_USE_SECURITY_PMUL_KEY_BLINDING
#define MH_ECC_USE_SECURITY_PMUL_BASE_POINT_VERIFY_CHECK
#define MH_ECC_USE_SECURITY_PMUL_RESULT_POINT_VERIFY_CHECK
#define MH_ECC_USE_SECURITY_PMUL_PATH_CHECK
#endif

#ifdef MH_ECC_USE_SECURITY_PMUL_CALC_BLINDING
#define MH_ECC_USE_SECURITY_PMUL_CALC_BLINDING_PATH_CHECK
#endif

#ifdef MH_ECC_USE_SECURITY_PMUL_F2M
#define MH_ECC_USE_SECURITY_PMUL_F2M_KEY_BLINDING
#define MH_ECC_USE_SECURITY_PMUL_F2M_BASE_POINT_VERIFY_CHECK
#define MH_ECC_USE_SECURITY_PMUL_F2M_RESULT_POINT_VERIFY_CHECK
#define MH_ECC_USE_SECURITY_PMUL_F2M_PATH_CHECK
#endif

#ifdef MH_ECC_USE_SECURITY_PMUL_F2M_CALC_BLINDING
#define MH_ECC_USE_SECURITY_PMUL_F2M_CALC_BLINDING_PATH_CHECK
#endif

#ifdef MH_ECC_USE_SECURITY_VERIFY_KEY
#define MH_ECC_USE_SECURITY_VERIFY_KEY_RANG_CHECK
#define MH_ECC_USE_SECURITY_VERIFY_KEY_POINT_VERIFY
#define MH_ECC_USE_SECURITY_VERIFY_KEY_POINT_INFINITE_FAR
#endif


/****************************************************************
						SM2 lib Config 
*****************************************************************/

/*
 *	ECC Security Config
 */
#define MH_SM2_USE_SECURITY  

#if (defined(MH_SM2_USE_SECURITY))
#define MH_SM2_ENC_USE_PATH_CHECK
#define MH_SM2_DEC_USE_PATH_CHECK
#define MH_SM2_SIGN_USE_PATH_CHECK
#define MH_SM2_VERIFY_USE_PATH_CHECK
#endif
/*
 *	SM2 Use the CRYPT Security
 */
#define MH_SM2_ENC_USE_CRYPT_SECURITY
#define MH_SM2_DEC_USE_CRYPT_SECURITY
#define MH_SM2_SIGN_USE_CRYPT_SECURITY
#define MH_SM2_VERIFY_USE_CRYPT_SECURITY
/*
 *	SM2 Use the System Security Control
 */
#define MH_SM2_ENC_USE_SSC      
#define MH_SM2_DEC_USE_SSC
#define MH_SM2_SIGN_USE_SSC
#define MH_SM2_VERIFY_USE_SSC


/****************************************************************
						RANDOM BRANCH Config 
*****************************************************************/

/*
 *	config RANDOM BRANCH
 */
// #define MH_USE_USER_RANDOM_BRANCH
#define MH_USE_SSC_RANDOM_BRANCH    



/****************************************************************
				System Security Control Config
*****************************************************************/
/*
 *	System Security Control Random Polarity
 *	System Security Control Register Parity
 *	System Security Control Ignore Clock Gate
 *	System Security Control Clean Register Heap
 */
#define MH_USE_SSC_RANDOM_POLARITY
#define MH_USE_SSC_REGISTER_PARITY
#define MH_USE_SSC_IGNORE_CLOCK_GATE
// #define MH_USE_SSC_CLEAN_REGISTERS_HEAP

/*
 *	There Types Of SSC ACK, Only Enable One Of Them
#define MH_USE_SSC_ACK_IT_DISABLE
#define MH_USE_SSC_ACK_IT_ENABLE
 */
#define MH_USE_SSC_ACK_IT_ENABLE_WITH_CLEAN_KEY


/****************************************************************
					CRYPT ASYMMETRIC ALGORITHM Config
*****************************************************************/
	
/*
 *	The Crypt Bus is Encrypted
 *	At the Begin and End of Runing Insert IDLE Run
 *	Fix the ExpMod Runint Time
 *	INSERT Fake Calculate
 */
#define MH_CRYPT_ASYMMETRIC_BUS_CRYPT
#define MH_CRYPT_ASYMMETRIC_IDLE_RUN
#define MH_CRYPT_ASYMMETRIC_FIX_EXPMOD_TIME
#define MH_CRYPT_ASYMMETRIC_INSERT_FAKE_CALCULATE

/*
#define MH_CRYPT_ASYMMETRIC_RAND_SEL_1_2
#define MH_CRYPT_ASYMMETRIC_RAND_SEL_1_4
#define MH_CRYPT_ASYMMETRIC_INSERT_FAKE_CAL_MODMUL
#define MH_CRYPT_ASYMMETRIC_INSERT_FAKE_CAL_MODSQR
#define MH_CRYPT_ASYMMETRIC_INSERT_FAKE_CAL_MODMIX
#define MH_CRYPT_ASYMMETRIC_INSERT_FAKE_CAL_RAND
*/
#if (defined(MH_CRYPT_ASYMMETRIC_INSERT_FAKE_CALCULATE))
#define MH_CRYPT_ASYMMETRIC_RAND_SEL_1_4
#define MH_CRYPT_ASYMMETRIC_INSERT_FAKE_CAL_RAND
#endif


/****************************************************************
					CRYPT SYMMETRIC ALGORITHM Config
*****************************************************************/
/*
 *	Symmaetric algoritym use delay_run
 */
#define MH_CRYPT_SYMMETRIC_DELAY_RUN  



/****************************************************************
					MEM OPERATE Config
*****************************************************************/
/*
 *	MEM COPY Use the System Security Control
 *	MEM COPY Use Rand Branch
 */
#define MH_MEM_COPY_OPERATE_USE_RAND    

#ifdef MH_MEM_COPY_OPERATE_USE_RAND
#define MH_MEM_COPY_OPERATE_USE_SSC
#define MH_MEM_COPY_OPERATE_USE_RAND_BRANCH
#endif
/*
 *	MEM XOR Use the System Security Control
 *	MEM XOR Use Rand Branch
 */
#define MH_XOR_OPERATE_USE_RAND    

#ifdef MH_XOR_OPERATE_USE_RAND
#define MH_XOR_OPERATE_USE_SSC
#define MH_XOR_OPERATE_USE_RAND_BRANCH
#endif

/*
 *	MEM AND Use the System Security Control
 *	MEM AND Use Rand Branch
 */
#define MH_SUM_OPERATE_USE_RAND     

#ifdef MH_SUM_OPERATE_USE_RAND
#define MH_SUM_OPERATE_USE_SSC
#define MH_SUM_OPERATE_USE_RAND_BRANCH
#endif

/*
 *	MEM IS EQUAL Use the System Security Control
 *	MEM IS EQUAL Use Rand Branch
 */
#define MH_MEM_IS_EQUAL_OPERATE_USE_RAND    

#ifdef MH_MEM_IS_EQUAL_OPERATE_USE_RAND
#define MH_MEM_IS_EQUAL_OPERATE_USE_SSC
#define MH_MEM_IS_EQUAL_OPERATE_USE_RAND_BRANCH
#endif

/*
 *	MEM B2L Use the System Security Control
 *	MEM B2L Use Rand Branch
 */
#define MH_MEM_SWAP_OPERATE_USE_RAND      

#ifdef MH_MEM_SWAP_OPERATE_USE_RAND
#define MH_MEM_SWAP_OPERATE_USE_SSC
#define MH_MEM_SWAP_OPERATE_USE_RAND_BRANCH
#endif

/****************************************************************
						random lib rand check Config
*****************************************************************/
/*
 *	RAND CHECK Options
#define MH_RAND_USE_CHECK_FREQUENCY
#define MH_RAND_USE_CHECK_BLOCK_FREQUENCY
#define MH_RAND_USE_CHECK_RUNS
#define MH_RAND_USE_CHECK_LONGEST_RUN
 */
#define MH_RAND_USE_CHECK_FREQUENCY
#define MH_RAND_USE_CHECK_BLOCK_FREQUENCY
#define MH_RAND_USE_CHECK_RUNS
#define MH_RAND_USE_CHECK_LONGEST_RUN



#else



 
#define MH_RSA_CRT_USE_SSC         
#define MH_RSA_PRIVATE_USE_SSC           
#define MH_RSA_PUBLIC_USE_SSC    

#define MH_DES_USE_SSC      
#define MH_TDES_USE_SSC    
#define MH_SM4_USE_SSC    




#endif      //end of  CRYPT_USE_SECURED_MODE  




#ifdef __cplusplus
}
#endif

#endif
