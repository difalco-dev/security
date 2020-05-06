#include <stdio.h>
#include <gmp.h>
#include <time.h>
#include <string.h>

void decrypt(mpz_t c, mpz_t phi, mpz_t n, mpz_t nu, mpz_t m){
	mpz_t tmp;
	mpz_init(tmp);
	mpz_pow_ui(tmp,n,2);
	mpz_powm(m,c,phi,tmp);
	mpz_sub_ui(m,m,1);
	mpz_cdiv_q(m,m,n);
	mpz_mul(m,m,nu);
	mpz_mod(m,m,n);
}

int main(int argc, char** argv){
	mpz_t p, q, c, mm, phi, pp, qq, nu, n, m, g, n2, r, newC;
	mpz_init(p);
	mpz_init(q);
	mpz_init(c);
	mpz_init(mm);
	mpz_init(phi);
	mpz_init(pp);
	mpz_init(qq);
	mpz_init(nu);
	mpz_init(n);
	mpz_init(m);
	mpz_init(g);
	mpz_init(n2);
	mpz_init(r);
	mpz_init(newC);

	gmp_scanf("%Zd,%Zd,%Zd",c,p,q);
	gmp_scanf("%Zd",mm);

	// Find phi
	mpz_sub_ui(pp,p,1);
	mpz_sub_ui(qq,q,1);
	mpz_lcm(phi,pp,qq);

	// Find n
	mpz_mul(n,p,q);

	// Find nu
	mpz_invert(nu,phi,n);

	// Decrypt
	decrypt(c,phi,n,nu,m);

	// Encryption
	gmp_randstate_t seed;
	gmp_randinit_default(seed);
	gmp_randseed_ui(seed,time(NULL));
	mpz_urandomm(r,seed,n);	

	mpz_add_ui(g,n,1);
	mpz_pow_ui(n2,n,2);
	mpz_powm(g,g,mm,n2);
	mpz_powm(r,r,n,n2);
	mpz_mul(newC,g,r);

	// Output
	gmp_printf("%Zd\n",m);
	gmp_printf("%Zd,%Zd,%Zd\n",newC,p,q);

	// Clear
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(c);
	mpz_clear(mm);
	mpz_clear(phi);
	mpz_clear(pp);
	mpz_clear(qq);
	mpz_clear(nu);
	mpz_clear(n);
	mpz_clear(m);
	mpz_clear(g);
	mpz_clear(n2);
	mpz_clear(r);
	mpz_clear(newC);

}