#include "PID.h"
#include <iostream>
using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
	PID::Kp = Kp;
	PID::Ki = Ki;
	PID::Kd = Kd;
	prev_cte = p_error = d_error = i_error = 0.0;

	// Twiddling parameters
	dp = { 0.1*Kp,0.1*Ki,0.1*Kd };
	//p = { Kp,Ki,Kd };
	n_eval_steps = 1000;
	twiddle = true;
	iter = 0;
	tune_index = 2;
	total_error = 0;
}

void PID::UpdateError(double cte) {

	p_error = cte;
	i_error += cte;
	d_error = cte - prev_cte;
	prev_cte = cte;

	if (twiddle && iter % n_eval_steps == 0) {
		total_error += pow(cte, 2);
		Tune(tune_index, dp[tune_index]);
		// next parameter
		tune_index = (tune_index + 1) % 3;
		cout << "Index: " << iter << endl;
		cout << "Total Error: " << total_error << endl;
		cout << "Best Error: " << best_err << endl;

		if (total_error < best_err) {
			// cout << "improvement!" << endl;
			best_err = total_error;
			dp[tune_index] *= 1.1;
		}
		else {
			Tune(tune_index, -2 * dp[tune_index]);

			if (total_error < best_err) {
				best_err = total_error;
				dp[tune_index] *= 1.1;
			}
			else {
				Tune(tune_index, dp[tune_index]);
				dp[tune_index] *= 0.9;
			}
		}
		total_error = 0;
		cout << "new parameters" << endl;
		cout << "P: " << Kp << ", I: " << Ki << ", D: " << Kd << endl;
	}
	iter++;
}

double PID::TotalError() {
	return p_error * Kp + i_error * Ki + d_error * Kd;;
}
void PID::Tune(int index, double delta) {
	if (index == 0) {
		Kp += delta;
	}
	else if (index == 1) {
		Ki += delta;
	}
	else if (index == 2) {
		Kd += delta;
	}
	else {
		std::cout << "index out of bounds";
	}
}