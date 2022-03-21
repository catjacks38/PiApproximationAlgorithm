#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <math.h>
#include <time.h>
#include <signal.h>

#define BAR_SIZE 32

// thx SO
#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

void mpq_atan_1_5(mpq_t out, unsigned long n, mpz_t exp)
{
        mpz_t denominator;

        // calc denominator: (2n + 1)(5^[2n + 1]) -> denominator
        mpz_init_set_ui(denominator, 5);

	// applies and saves the new exponent page (exp) if its greater than zero
	if (mpz_cmp_ui(exp, 0) > 0)
	{
        	mpz_pow_ui(denominator, denominator, 2);
		mpz_mul(denominator, exp, denominator);
	}
	mpz_set(exp, denominator);

        mpz_mul_ui(denominator, denominator, 2*n + 1);

        // finalizes out: (-1)^n/denominator -> out
        mpq_set_z(out, denominator);
        mpq_inv(out, out);

        if (n % 2 == 1)
                mpq_neg(out, out);

	// mem clean-up
	mpz_clear(denominator);
}


void mpq_atan_1_239(mpq_t out, unsigned long n, mpz_t exp)
{
	mpz_t denominator;

	// calc denominator: (2n + 1)(239^[2n + 1]) -> denominator
	mpz_init_set_ui(denominator, 239);

        // applies (if exp > 0) and saves the new exponent page to exp
        if (mpz_cmp_ui(exp, 0) > 0)
        {
                mpz_pow_ui(denominator, denominator, 2);
                mpz_mul(denominator, exp, denominator);
        }
        mpz_set(exp, denominator);

	mpz_mul_ui(denominator, denominator, 2*n + 1);

	// finalizes out: (-1)^n/denominator -> out
	mpq_set_z(out, denominator);
	mpq_inv(out, out);

	if (n % 2 == 1)
		mpq_neg(out, out);

	// mem clean-up
	mpz_clear(denominator);
}

void intHandler(int bruhmomento)
{
	// lets user have cursor back
	printf("\e[?25h\n");
	exit(-1);
}

int main(int argc, char* argv[])
{
	unsigned long n;

	// argument checking
	if (argc == 3)
		sscanf(argv[2], "%lu", &n);
	else if (argc > 3)
	{
		printf("\033[1m!!!ARGUMENT OVERLOAD!!!\033[0m\n");
		return -1;
	}
	else
	{
		printf("too few arguments supplied\n");
		return -1;
	}

	FILE* piFile = fopen(argv[1], "w");

	mpq_t atan1_5;
	mpq_t atan1_239;
	mpq_t temp;
	mpq_t pi;

	mpz_t exp1_5;
	mpz_t exp1_239;

	mpq_inits(atan1_5, atan1_239, temp, pi, NULL);
	mpz_inits(exp1_5, exp1_239, NULL);

	clear();
	printf("\e[?25l");
	printf("│");
	gotoxy(BAR_SIZE + 2, 0);
	printf("│");
	gotoxy(2, 0);

	unsigned long iterationsPerBlock = ceil((double) n/BAR_SIZE);
	clock_t startTime = clock();
	clock_t iterationStartTime;

	for (unsigned long i = 0; i < n; i++)
	{
		iterationStartTime = clock();
		signal(SIGINT, intHandler);

		gotoxy(2, 0);
		for (int j = 0; j < (i + 1) / iterationsPerBlock; j++)
			printf("█");

		mpq_atan_1_5(temp, i, exp1_5);
		mpq_add(atan1_5, atan1_5, temp);

		mpq_atan_1_239(temp, i, exp1_239);
		mpq_add(atan1_239, atan1_239, temp);

                gotoxy(BAR_SIZE + 4, 0);
                printf("%lu/%lu - %.3f it/s [%.1fs/%.1fs]        ", i + 1, n, 1 / ((double) (clock() - iterationStartTime) / CLOCKS_PER_SEC), (double) (clock() - startTime) / CLOCKS_PER_SEC, ((n - 1 - i)*((double) (clock() - iterationStartTime) / CLOCKS_PER_SEC)));
	}

	// lets user have cursor back and adds a new line
	printf("\e[?25h\n");

	// calculates the rational approximation of pi and saves to file to be read later
	mpq_set_ui(temp, 16, 1);
	mpq_mul(pi, temp, atan1_5);

	mpq_set_ui(temp, 4, 1);
	mpq_mul(temp, temp, atan1_239);
	mpq_sub(pi, pi, temp);

	mpq_out_str(piFile, 10, pi);
	fclose(piFile);

	return 0;
}
