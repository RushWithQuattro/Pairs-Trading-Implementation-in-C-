#ifndef LINEARREGRESSION_H_
#define LINEARREGRESSION_H_





#include <cmath>
#include <string>
#include "TimeSeries.h"

// This module performs a CADF test(cointergrated Augmented Dickey Fuller test) to determine hedging ratio between pairs(beta) by a simple linear regression





// Mediator pattern
class LinearRegression{
private:
	TimeSeries* Y; // dependent variable of the linear regression
	TimeSeries* X; // independent variable of the linear regression
	// double rho;    // correlation between the two time series
	// double alpha;  // intercept of the linear regression
	double beta;   // slope of the linear regression
	time_t datetime;

public:
	LinearRegression(TimeSeries* , TimeSeries* ); // constructor
	virtual ~LinearRegression();                // destructor

	// void reset();                               // re-estimate the linear regression
	TimeSeries* residuals(std::string);         // calculate the series of residuals

	// Observers
	// double get_rho();                           // getter of the correlation
	// double get_alpha();                         // getter of the intercept
	double get_beta();                          // getter of the slope
};

 /* LINEARREGRESSION_H_ */
// Constructor
LinearRegression::LinearRegression (TimeSeries* Y, TimeSeries* X) // a为传过来的日期（如果第n天，传过来的a应为n-1)
{
	double sumXY = 0.0;
	double sumX = 0.0;
	double sumY = 0.0;
	double sumX2 = 0.0;
	double sumY2 = 0.0;
	time_t datetime= datetime;
	this->Y = Y;
	this->X = X;
	// For every value in the sample of the two time series
	for(TimeSeries::iterator itY = this->Y->series_begin(), itX = this->X->series_begin(); itY != this->Y->series_end() && itX != this->X->series_end(); ++itY, ++itX)
	{
		sumXY += (*itX).second->get_value() * (*itY).second->get_value();
		sumX += (*itX).second->get_value();
		sumY += (*itY).second->get_value();
		sumX2 += (*itX).second->get_value() * (*itX).second->get_value();
		sumY2 += (*itY).second->get_value() * (*itY).second->get_value();
	}

	// Correlation between the two time series
	// this->rho = (this->Y->sample_size() * sumXY - sumX * sumY) / (sqrt((this->Y->sample_size() * sumX2 - sumX * sumX) * (this->Y->sample_size() * sumY2 - sumY * sumY)));

	// Slope of the linear regression
	this->beta = (this->Y->series_size() * sumXY - sumX * sumY) / (this->Y->series_size() * sumX2 - sumX * sumX);

	// Intercept of the linear regression
	// this->alpha = (sumY - this->beta * sumX) / this->Y->sample_size();

	return;
}

// Destructor
LinearRegression::~LinearRegression()
{
	return;
}

// Re-estimate the linear regression // 对我们来说是不是没用？我们只要一开始那个就好了
/*void LinearRegression::reset()
{
	double sumXY = 0.0;
	double sumX = 0.0;
	double sumY = 0.0;
	double sumX2 = 0.0;
	double sumY2 = 0.0;
	int a=0; // 不明确这个是否要用
	// For every value in the sample of the two time series
	for(TimeSeries::iterator itY = next(this->Y->in_sample_begin(), a*390), itX = next(this->X->in_sample_begin(), a*390); itY != next(this->Y->in_sample_begin(),(a+1)*390) && itX != next(this->X->in_sample_begin(),(a+1)*390); ++itY, ++itX)
	{
		sumXY += (*itX).second->get_value() * (*itY).second->get_value();
		sumX += (*itX).second->get_value();
		sumY += (*itY).second->get_value();
		sumX2 += (*itX).second->get_value() * (*itX).second->get_value();
		sumY2 += (*itY).second->get_value() * (*itY).second->get_value();
	}

	// Correlation between the two time series
	this->rho = (390 * sumXY - sumX * sumY) / (sqrt((390 * sumX2 - sumX * sumX) * (390 * sumY2 - sumY * sumY)));

	// Intercept of the linear regression
	this->beta = (390 * sumXY - sumX * sumY) / (390 * sumX2 - sumX * sumX);

	// Slope of the linear regression
	this->alpha = (sumY - this->beta * sumX) / 390;

	return;
}*/

// Calculate the series of
TimeSeries* LinearRegression::residuals(std::string name)
{
	TimeSeries* R;
	double value = 0.0;

	R = new TimeSeries(name);
	// For every value in the two time series
	for(TimeSeries::iterator itY = this->Y->series_begin(), itX = this->X->series_begin(); itY != this->Y->series_end() && itX != this->X->series_end(); ++itY, ++itX)
	{
		// R(t) = Y(t) - alpha - beta * X(t)
		value = (*itY).second->get_value() /*- this->get_beta()*/ - this->get_beta() * (*itX).second->get_value(); //  modify a little
		R->insert((*itY).second->get_datetime(), value);
	}

	//R->set_sample_date(this->Y->get_sample_date());   //     ?? need to change?  in fact,  there is nothing with the map‘s element, just a sample date;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

	return R; // R.series 没问题，但是set_sample date 有问题，如果一定要sample date，其实可以直接对R中的key去读取然后作为sample date，看看要不要用吧后面。
}

// Getter of the correlation
// double LinearRegression::get_rho()
// {
// 	return this->rho;
// }

// Getter of the intercept
// double LinearRegression::get_alpha()
// {
// 	return this->alpha;
// }

// Getter of the slope
double LinearRegression::get_beta(){
	return this->beta;
}

#endif