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
	p_error = d_error = i_error = 0.0;

	// Twiddling parameters
	dp = { 0.1*Kp,0.1*Ki,0.1*Kd };
	p = { Kp,Ki,Kd };
	n_settle_steps = 50;
	n_eval_steps = 1000;
	twiddle = true;
	iter = 1;
	tune_index = 2;
}

void PID::UpdateError(double cte) {
	if (iter == 1) {
		// to get correct initial d_error
		p_error = cte;
	}
	d_error = cte - p_error;
	p_error = cte;
	i_error += cte;

	// update total error only if we're past number of settle steps
	if (iter % (n_settle_steps + n_eval_steps) > n_settle_steps) {
		total_error += pow(cte, 2);
	}
	if (twiddle && iter % (n_settle_steps + n_eval_steps) == 0) {
		cout << "Iteration and Best error: " << iter << best_err << endl;
		for (unsigned int i = 0; i< 3; i++) {
			p[i] += dp[i];
			if (total_error<best_err) {
				cout << "Improvement" << endl;
				best_err = total_error;
				dp[i] *= 1.1;
			}
			else {
				p[i] -= 2.0 * dp[i];
				if (total_error < best_err) {
					best_err = total_error;
					dp[i] *= 1.1;
				}
				else {
					p[i] += dp[i];
					dp[i] *= 0.9;
				}
			}
		}
		total_error = 0;
		Kp = p[0]; Ki = p[1]; Kd = p[2];
		std::cout << "new parameters" << endl;
		std::cout << "P: " << Kp << ", I: " << Ki << ", D: " << Kd << endl;
	}
	iter++;
}

double PID::TotalError() {
	return 0.0;
}