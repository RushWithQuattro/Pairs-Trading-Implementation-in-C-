#pragma warning(disable : 4996).
#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <cmath>
#include "DataPoint.h"
#include "TimeSeries.h"


using namespace std;



class SN // Abstract Class
{
public:
	SN() {};

	// virtual ~SN() {};

	virtual double get_mean() = 0;
	virtual double get_sigma() = 0;
	virtual double long_signal() = 0;
	virtual double short_signal() = 0;
	virtual double close_long_signal() = 0;
	virtual double close_short_signal()= 0;
};




class Signal :public SN // Concrete Class
{
private:
	double mean;
	double sigma;
	double upper_bound;
	double lower_bound;
	TimeSeries *spread;
	// time_t sample_time_length;	
public:
	Signal(TimeSeries *spread);
	// virtual ~Signal();
	
	virtual double get_mean();
	virtual double get_sigma();
	virtual double long_signal();
	virtual double short_signal();
	virtual double close_long_signal();
	virtual double close_short_signal();
};

 /* SIGNAL_H_ */
// #pragma once
//constructor
Signal::Signal(TimeSeries* spread) {

	this->spread = spread;

	double sum;
	int counter;
	tm *mytime1;
	tm *mytime2;
	
	//get the fcirst 30min time stamp of this timeseries
	time_t time1 = this->spread->series_begin()->first;
    //add 30 mins to specify a sigma calculation period 
	mytime1 = localtime(&time1);
	mytime1->tm_min += 30;
	time_t firstThirty = mktime(mytime1);

	//get the last 30 min timestamp of this timeseries
    time_t time2= (*this->spread->series_end()).first;
	mytime2 = localtime(&time2);
	mytime2->tm_min -= 30;
	time_t LastThirty = mktime(mytime2);

	// calculate mean 
	spread->set_sample_date(LastThirty);
	counter = 0;
	sum = 0;
	for (TimeSeries::reverse_iterator itRS = this->spread->series_rbegin(); itRS != this->spread->in_sample_rbegin(); ++itRS) {
		sum += (*itRS).second->get_value();
		counter += 1;
		}
	this->mean = sum / counter;


	// calculate sigma
	spread->reset_sample_date();
    spread->set_sample_date(firstThirty);
	counter = 0;
	sum = 0;
	for (TimeSeries::iterator itS = this->spread->series_begin(); itS != this->spread->in_sample_end(); ++itS) {
		sum += pow((*itS).second->get_value() - mean,2);
		counter += 1;
	}
	this->sigma =sqrt( sum / counter);

	// calculate upper and lower bound
	this->upper_bound = this->mean + 2 * this->sigma;
	this->lower_bound= this->mean - 2 * this->sigma;
}

double Signal::get_mean(){
	return this->mean;
}
double Signal::get_sigma(){
	return this->sigma;
}
double Signal::long_signal(){
	return this->lower_bound;
}
double Signal::short_signal(){
	return this->upper_bound;
}
double Signal::close_long_signal(){
	return this->lower_bound + this->sigma;
}
double Signal::close_short_signal(){
	return this->upper_bound - this->sigma;
}
#endif