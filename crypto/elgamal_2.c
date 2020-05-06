#include <gmp.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#define BIT_CNT 1000
#define DEBUG 0

void find_p(mpz_t p, mpz_t q){
	mpz_t kq;
	mpz_init(kq);

	// Multiply by 2 & add 1
	mpz_mul_ui(kq,q,2);
	mpz_add_ui(p,kq,1);
	mpz_clear(kq);
}

void find_c2(mpz_t c2, mpz_t message, mpz_t p, mpz_t h, mpz_t r){
	mpz_t tmp1;
	mpz_init(tmp1);

	// Calculate  m * h^(r) mod p	
	mpz_powm(tmp1,h,r,p);
	mpz_mul(c2,message,tmp1);
	mpz_mod(c2,c2,p);
	mpz_clear(tmp1);
}

void find_q(mpz_t q){
	mpz_t tmp;
	mpz_init(tmp);

	// Find random large prime within bit global var	
	gmp_randstate_t state2;
	gmp_randinit_default(state2);
	gmp_randseed_ui(state2,time(NULL));
	mpz_urandomb(tmp,state2,BIT_CNT-1);	
	mpz_nextprime(q,tmp);
	mpz_clear(tmp);
}

bool check_prime(mpz_t p){
	int rtr;
	rtr = mpz_probab_prime_p(p,50);
	// Return whether true or not
	if (rtr == 2 || rtr == 1){
		return true;
	}else{
		return false;
	}

}

void find_g(mpz_t g, mpz_t p, mpz_t q){
	mpz_t exp1;
	mpz_t exp2;
	bool flag = false;
	unsigned long int r1, r2, i;
	mpz_init(exp1);
	mpz_init(exp2);

	// Find generator for k > 2
	for (i = 2; mpz_cmp_ui(p,i) != 0 && !flag; i++){
		mpz_set_ui(g,i);
		
		mpz_powm_ui(exp2,g,2,p);
		r1 = mpz_cmp_ui(exp2,1);

		mpz_powm(exp1,g,q,p);
		r2 = mpz_cmp_ui(exp1,1);
	
		if (r1!= 0 && r2 != 0){
			flag = true;
		}	
	}
}

int main(){

	/*---------------------  Set Up   -------------------------*/

	// Inits
	char buf[2048];
	mpz_t q, p, x, generator, h, r, c1, c1_1, c1_2, c2, m, tmp_A, tmp_B;
	bool flg2;
	int flg1;
	int it;
	mpz_init(r);
	gmp_randstate_t state2;
	gmp_randinit_default(state2);

	// Get Message
	mpz_init(m);
	scanf("%s",buf);
	mpz_set_str(m,buf,10);
	if (DEBUG==1){
		gmp_printf("[101] : m = %Zd",m);
	}

	/*---------------------  Main Loop   -------------------------*/

	// Run 5 times
	for (it = 1; it < 6; it++){
		
		if (DEBUG == 1){
			printf("\n\n -----===== Iteraton %d =====-----\n",it);
		}

		// Find p & q	
		mpz_init(q);
		find_q(q);
		mpz_init(p);
		find_p(p,q);
		while (!check_prime(p)){
			mpz_nextprime(q,q);
			find_p(p,q);
		}

		if (DEBUG==1){
			gmp_printf("[120] : p = %Zd && q = %Zd\n",p,q);
		}

		// Get Generator
		mpz_init(generator);
		find_g(generator,p,q);

		if (DEBUG==1){
			gmp_printf("[128] : g = %Zd\n",generator);
		}

		// Hardcode the private key value
		mpz_init(x);
		mpz_set_ui(x,40622201812345);

		/*---------------------  Ecryption   -------------------------*/
		
		// Find g^x --> h
		mpz_init(h);
		mpz_powm(h,generator,x,p);
		if (DEBUG==1){
			gmp_printf("[141] : h = %Zd\n",h);
		}

		// Find r -- used in h^r * m
		gmp_randseed_ui(state2,time(NULL));
		mpz_urandomm(r,state2,p);

		if (DEBUG==1){
			gmp_printf("[148] : r = %Zd\n",r);
		}

		// Find c1 = g^r
		mpz_init(c1);
		mpz_powm(c1,generator,r,p);

		// Compute c2 = m*h^R
		find_c2(c2,m,p,h,r);


		if(DEBUG == 1){
			gmp_printf("\nENCRYPTION---------------------------\nc1 : %Zd\nc2 : %Zd\nm : %Zd\nh : %Zd\nr : %Zd \np : %Zd",c1,c2,m,h,r,p);
			printf("\n--------------------------------------\n");
			printf("\nDECRYPTION----------------------------\n");	
		}
		gmp_printf("%Zd, %Zd, %Zd\n\n",c1,c2,p);

		/*---------------------  Deccryption   -------------------------*/

		
		// Find c1^x
		mpz_init(c1_1);
		mpz_powm(c1_1,c1,x,p);
		if (DEBUG==1){
			gmp_printf("[168] : c1^x = %Zd\n",c1_1);
		}	

		// Find c1_1's inverse (c1^x)^-1
		mpz_init(c1_2);
		mpz_invert(c1_2,c1_1,p);
		if (DEBUG==1){
			gmp_printf("[175] : c1^x^-1 = %Zd\n",c1_2);
		}	

		// Multiply m by number found
		mpz_init(tmp_B);
		mpz_init(tmp_B);
		mpz_mul(tmp_A,c1_2,c2);
		mpz_mod(tmp_B,tmp_A,p);

		if (DEBUG == 1){
			puts("Decrypted: ");
			printf("\n--------------------------------------\n\n");
			mpz_out_str(stdout,10,tmp_B);
			puts("\n");
		}

	}

	// Clears
	mpz_clear(tmp_A);
	mpz_clear(tmp_B);
	mpz_clear(q);
	mpz_clear(p); 
	mpz_clear(x);
	mpz_clear(generator); 
	mpz_clear(h); 
	mpz_clear(c1);
	mpz_clear(c1_1);
	mpz_clear(c1_2); 
	mpz_clear(c2);
	mpz_clear(r); 
	mpz_clear(m);
}