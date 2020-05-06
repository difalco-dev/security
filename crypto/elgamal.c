#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#define BIT_CNT 1000
#define DEBUG 1

int main(){
	// Declarations
	gmp_randstate_t state;
	gmp_randinit_default(state);
	mpz_t g, G, q, Zq, x, h, pk, r, c1, c2, rop, p, k, q;
	mpz_inits(g, G, q, Zq, x, h, pk, r, c1, c2, rop, p, k, q);
       
	// Initialize Bit Count 
	mp_bitcnt_t bits;
	mpz_init(bits);
	mpz_set_ui(bits,BIT_CNT);

	/* Calculate p&q values */
	bool flag = true;
	while(flag){

		// -- Find random prime q
		mpz_urandomb(rop,state,bits);	
		while(mpz_probab_prime_p(rop,20)!=2){
			mpz_urandomb(rop,state,bits);
		}
		mpz_set(q,rop);

		// -- Multiply by 2 & add 1
		mpz_set_ui(k,2);
		mpz_mul(rop,rop,k);
		mpz_add_ui(rop,rop,1);

		// -- Found Prime p
		if (mpz_probab_prime_p(rop,20)==2){
			mpz_set(p,rop);
			flag = false;
`		}
	}


	/* Find order of G --> q */
	mpz_t ordG;
	mpz_init(ordG);
	mpz_mult(ordG,k,q);

	// -- Debugging
	if (DEBUG==1){
		gmp_printf("q : %Zd\n",q);
		gmp_printf("p : %Zd\n",p);
		gmp_printf("ord(G) : %Zd\n",ordG);
	}

}

// Ciphertext
struct Ciphertext{
     mpz_t c1;
     mpz_t c2;
};

// Public Key
struct PuK{
     mpz_t G; // Group
     mpz_t g; // Generator
     mpz_t h; // g^(x)
};

// Private Key
struct PrK{
     mpz_t x;
};