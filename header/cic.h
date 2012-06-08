#include "integrator.h"
#include "comb.h"

#define N 3

SC_MODULE(cic)
{
	sc_in<bool> 	CLR;
	sc_in<bool> 	CLK;
	sc_in<double> 	cicIN;
	sc_out<double> 	cicOUT;
	
	integrator *I[N];
	comb *C[N];
	
	sc_signal<double> internalI[N];
	sc_signal<double> internalC[N];
	
	SC_CTOR(cic)
	{
		SC_METHOD(output);
			sensitive << internalC[N-1];
		
		for (unsigned j = 0; j < N; j++)
		{
			I[j] = new integrator(sc_gen_unique_name("I"));
			if (j == 0)
			{
				I[j] -> CLR 			(CLR);
				I[j] -> CLK 			(CLK);
				I[j] -> integratorIN 	(cicIN);
				I[j] -> integratorOUT 	(internalI[j]);
			}
			else
			{
				I[j] -> CLR 			(CLR);
				I[j] -> CLK 			(CLK);
				I[j] -> integratorIN 	(internalI[j-1]);
				I[j] -> integratorOUT 	(internalI[j]);			
			}
		}
		
		for (unsigned i = 0; i < N; i++)
		{	
			C[i] = new comb(sc_gen_unique_name("C"));
			if (i == 0)
			{
				C[i] -> CLR 		(CLR);
				C[i] -> CLK 		(CLK);
				C[i] -> combIN 		(internalI[N-1]);
				C[i] -> combOUT 	(internalC[i]);
			}
			else
			{
				C[i] -> CLR 		(CLR);
				C[i] -> CLK 		(CLK);
				C[i] -> combIN 		(internalC[i-1]);
				C[i] -> combOUT 	(internalC[i]);			
			}			
		}
			
		 cicOUT.initialize(0);	
	}
	
	void output()
	{
		cicOUT.write(internalC[N-1]);		
	}

};
