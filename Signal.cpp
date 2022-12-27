// # include "Signal.h"


// //constructor
// Signal::Signal(TimeSeries* spread) {

// 	this->spread = spread;

// 	double sum;
// 	int counter;
// 	tm *mytime;
	
// 	//get the fcirst 30min time stamp of this timeseries
// 	time_t time = (*this->spread->series_begin()).first;
//     //add 30 mins to specify a sigma calculation period 
// 	mytime = localtime(&time);
// 	mytime->tm_min += 30;
// 	time_t firstThirty = mktime(mytime);
	
// 	//get the last 30 min timestamp of this timeseries
//     time= (*this->spread->series_rbegin()).first;
// 	mytime = localtime(&time);
// 	mytime->tm_min -= 30;
// 	time_t LastThirty = mktime(mytime);

// 	// calculate mean 
// 	spread->set_sample_date(LastThirty);
// 	counter = 0;
// 	sum = 0;
// 	for (TimeSeries::reverse_iterator itRS = this->spread->series_rbegin(); itRS != this->spread->in_sample_rbegin(); ++itRS) {
// 		sum += (*itRS).second->get_value();
// 		counter += 1;
// 		}
// 	this->mean = sum / counter;

// 	// calculate sigma
// 	// this->spread->reset_sample_date();
//     spread->set_sample_date(firstThirty);
// 	counter = 0;
// 	sum = 0;
// 	for (TimeSeries::iterator itS = this->spread->series_begin(); itS != this->spread->in_sample_end(); ++itS) {
// 		sum += pow((*itS).second->get_value() - mean,2);
// 		counter += 1;
// 	}
// 	this->sigma =sqrt( sum / counter);

// 	// calculate upper and lower bound
// 	this->upper_bound = this->mean + 2 * this->sigma;
// 	this->lower_bound= this->mean - 2 * this->sigma;
// }

// double Signal::get_mean(){
// 	return this->mean;
// }
// double Signal::get_sigma(){
// 	return this->sigma;
// }
// double Signal::long_signal(){
// 	return this->lower_bound;
// }
// double Signal::short_signal(){
// 	return this->upper_bound;
// }
// double Signal::close_long_signal(){
// 	return this->lower_bound + this->sigma;
// }
// double Signal::close_short_signal(){
// 	return this->upper_bound - this->sigma;
// }