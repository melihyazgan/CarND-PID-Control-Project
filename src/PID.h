#ifndef PID_H
#define PID_H
#include <math.h>
#include <vector>

class PID {
public:
	/*
	* Errors
	*/
	double p_error;
	double i_error;
	double d_error;
	double prev_cte;
	/*
	* Coefficients
	*/
	double Kp;
	double Ki;
	double Kd;
	//

	double best_err = std::numeric_limits<double>::max();
	double total_error;
	int iter;
	//int n_settle_steps;
	int n_eval_steps;
	bool twiddle;
	std::vector<double> dp;
	int tune_index;
	/*
	* Constructor
	*/
	PID();

	/*
	* Destructor.
	*/
	virtual ~PID();

	/*
	* Initialize PID.
	*/
	void Init(double Kp, double Ki, double Kd);

	/*
	* Update the PID error variables given cross track error.
	*/
	void UpdateError(double cte);

	/*
	* Calculate the total PID error.
	*/
	double TotalError();
	void Tune(int index, double delta);
};

#endif /* PID_H */