#ifndef __MH_CALC_ECC_H
#define __MH_CALC_ECC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mh_bignum.h"

typedef struct
{
	uint32_t *x;
	uint32_t *y;
	uint32_t *_y;
}ecc_point_naf;
typedef struct
{
	uint32_t *x;  //x������
	uint32_t *y;  //y������
}ecc_point_a;//��������
typedef struct
{
	uint32_t *x;  //x������
	uint32_t *y;  //y������
	uint32_t *z;  //z������
}ecc_point_j;//�ſɱ�����
typedef struct
{
	uint32_t *x;  //x������
	uint32_t *y;  //y������
	uint32_t *z;  //z������
	uint32_t *t;  //t������
}ecc_point_mj;//�����ſɱ�����
typedef struct
{
	uint32_t *p;				//ģ��--���ⲿ����
	uint32_t *a;				//��Բ���߳���a--���ⲿ����
	uint32_t *b;				//��Բ���߳���b--���ⲿ����
	ecc_point_a g;				//����--���ⲿ����
	uint32_t *n;				//����Ľ�--���ⲿ����
	uint32_t *p_c;				//��nΪȡģ�ĳ���c
	uint32_t p_q;				//��nΪȡģ�ĵ�������q
	uint32_t *n_c;				//��pΪȡģ�ĳ���c
	uint32_t n_q;				//��pΪȡģ�ĵ�������q
	uint32_t len_bits;			//����λ��,����
	uint32_t len_words;			//���߳���,��
	uint32_t field;				//��
	uint32_t a_type;			//�ж�a�Ƿ����-3 MH_ECC_A_IS_NEGATIVE_3 �� MH_ECC_A_NOT_NEGATIVE_3--���ⲿ����
}ecc_para;						//ecc������������ݽṹ
typedef struct
{
	uint32_t *d;        //ecc˽Կ
	ecc_point_a e;    	//ecc��Կ
}ecc_key;//ecc��Կ�����ݽṹ

//macro define
#define MH_ECC_P192 0
#define MH_ECC_P224 1
#define MH_ECC_P256 2
#define MH_ECC_B163 3
#define MH_ECC_B193 4
#define MH_ECC_B233 5

#define MH_ECC_PRIME							(('E'<<8)|('P'))
#define MH_ECC_BINARY							(('E'<<8)|('B'))
#define MH_ECC_A_IS_NEGATIVE_3					(('A'<<24)|('I'<<16)|('N'<<8)|('3'))
#define MH_ECC_A_NOT_NEGATIVE_3					(('A'<<24)|('N'<<16)|('N'<<8)|('3'))
#define MH_ECC_SCALAR_IS_EVEN					(('S'<<16)|('I'<<8)|('E'))
#define MH_ECC_SCALAR_IS_ODD 					(('S'<<16)|('I'<<8)|('O'))
#define MH_ECC_EMBEDED_PUBLIC_KEY_VERIFY		(('E'<<24)|('P'<<16)|('K'<<8)|('V'))
#define MH_ECC_COMMON_PUBLIC_KEY_VERIFY			(('C'<<24)|('P'<<16)|('K'<<8)|('V'))

#define MH_RET_ECC_POINT_SUCCESS				(('E'<<24)|('P'<<16)|('S'<<8)|('U'))
#define MH_RET_ECC_POINT_FAILED					(('E'<<24)|('P'<<16)|('F'<<8)|('A'))
#define MH_RET_ECC_POINT_ADD_ERROR				(('E'<<24)|('P'<<16)|('A'<<8)|('E'))
#define MH_RET_ECC_POINT_MULT_ERROR				(('E'<<24)|('P'<<16)|('M'<<8)|('E'))
#define MH_RET_ECC_POINT_INFINITE_FAR			(('E'<<24)|('P'<<16)|('I'<<8)|('F'))
#define MH_RET_ECC_PUBLIC_KEY_FAILED			(('E'<<24)|('P'<<16)|('K'<<8)|('F'))
#define MH_RET_ECC_PUBLIC_KEY_PASS				(('E'<<24)|('P'<<16)|('K'<<8)|('P'))
#define MH_RET_ECC_KEY_GENERATION_SUCCESS		(('E'<<24)|('K'<<16)|('G'<<8)|('S'))
#define MH_RET_ECC_KEY_GENERATION_FAILED		(('E'<<24)|('K'<<16)|('G'<<8)|('F'))

//function
void ecc_config(ecc_para *para,uint32_t config);
uint32_t ecc_genkey(ecc_key *key, ecc_para *para,
					mh_rng_callback f_rng, void *p_rng);
uint32_t ecc_verifykey(ecc_key *key,ecc_para *para,uint32_t config,
					mh_rng_callback f_rng, void *p_rng);
uint32_t ecc_pmul(ecc_point_a *r,ecc_point_a *a,uint32_t *b,ecc_para *para,
					mh_rng_callback f_rng, void *p_rng);
uint32_t ecc_pmul_f2m(ecc_point_a *r,ecc_point_a *a,uint32_t *k,ecc_para *para,
					mh_rng_callback f_rng, void *p_rng);
uint32_t ecc_verifypoint(ecc_point_a *a,ecc_para *para);
uint32_t ecc_pmul_calc(ecc_point_a *r, ecc_point_a *a, uint32_t *k, ecc_para *para);
uint32_t ecc_pmul_calc_blinding(ecc_point_a *r, ecc_point_a *a, uint32_t *k, ecc_para *para,
								mh_rng_callback f_rng, void *p_rng);
uint32_t ecc_pmul_f2m_calc(ecc_point_a *r, ecc_point_a *a, uint32_t *k,uint32_t len,ecc_para *para);
uint32_t ecc_pmul_f2m_calc_blinding(ecc_point_a *r, ecc_point_a *a, uint32_t *k, ecc_para *para,
								mh_rng_callback f_rng, void *p_rng);

void ecc_j2a(ecc_point_a *r,ecc_point_j *a,const ecc_para *para);
void ecc_j2mj(ecc_point_mj *r,ecc_point_j *a,const ecc_para *para);
void ecc_j2mj_t(uint32_t *r,uint32_t *z,const ecc_para *para);

uint32_t ecc_padd_a(ecc_point_a *r,ecc_point_a *a,ecc_point_a *b,const ecc_para *para);
uint32_t ecc_padd_ja(ecc_point_j *r,ecc_point_j *a,ecc_point_a *b,const ecc_para *para);
uint32_t ecc_pdbl_a(ecc_point_a *r,ecc_point_a *a,const ecc_para *para);
uint32_t ecc_pdbl_j(ecc_point_j *r,ecc_point_j *a,const ecc_para *para);
uint32_t ecc_pdbl_mj(ecc_point_mj *r,ecc_point_mj *a,const ecc_para *para);

#ifdef __cplusplus
}
#endif

#endif

