#include <stdio.h>
#include <gmp.h>

int main(int argc, char* argv[])
{
	unsigned long digits;
        // argument checking
	if (argc == 3)
		sscanf(argv[2], "%lu", &digits);
        if (argc > 3)
        {
                printf("\033[1m!!!ARGUMENT OVERLOAD!!!\033[0m\n");
                return -1;
        }
        else if (argc < 3)
        {
                printf("too few arguments supplied\n");
                return -1;
        }

	FILE* piFile = fopen(argv[1], "r");
	mpq_t pi;
	mpz_t numerator;
        mpz_t denominator;
        mpz_t quotent;

	mpq_init(pi);
	mpz_inits(numerator, denominator, quotent, NULL);

	mpq_inp_str(pi, piFile, 10);

	// seperates numerator and denominator for integer division
        mpq_get_num(numerator, pi);
        mpq_get_den(denominator, pi);

        // gets and prints first digit with decimal place after to stdout
        mpz_fdiv_qr(quotent, numerator, numerator, denominator);
        mpz_mul_ui(numerator, numerator, 10);
	gmp_printf("%Zd.", quotent);

        // prints rest of digits to stdout
        for (unsigned long i = 2; i <= digits; i++)
        {
                mpz_fdiv_qr(quotent, numerator, numerator, denominator);
                mpz_mul_ui(numerator, numerator, 10);
                gmp_printf("%Zd", quotent);
        }

	fclose(piFile);
}
