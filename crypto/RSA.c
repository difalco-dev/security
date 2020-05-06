#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>

int main(){

	// Declarations
	mpz_t p,psub1,qsub1,q,e,n,m,me,d,inverse,phi,c;

	// Inits
	mpz_init(p);
	mpz_init(psub1);
	mpz_init(qsub1);
	mpz_init(q);
	mpz_init(e);
	mpz_init(n);
	mpz_init(m);
	mpz_init(me);
	mpz_init(d);
	mpz_init(inverse);
	mpz_init(phi);
	mpz_init(c);
       // mp_bitcnt_t tmp;
	// RSA Encryption
	gmp_scanf("%Zd,%Zd,%Zd,%Zd",p,q,e,m);
	mpz_mul(n,p,q);

	mpz_sub_ui(psub1,p,1);
	mpz_sub_ui(qsub1,q,1);
	mpz_mul(phi,psub1,qsub1);

	mpz_invert(d,e,phi);
	mpz_powm(c,m,e,n);

	gmp_printf("n = %Zd d = %Zd c = %Zd\n",n,d,c);


	mpz_clear(psub1);
	mpz_clear(qsub1);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(e);
	mpz_clear(n);
	mpz_clear(m);
	mpz_clear(me);
	mpz_clear(d);
	mpz_clear(inverse);
	mpz_clear(phi);
	mpz_clear(c);
}