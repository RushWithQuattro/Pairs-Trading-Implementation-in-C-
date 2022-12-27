#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include "DataPoint.h"

// Decorator pattern (Abstract Class)
class TS
{
public:
	TS() {};                                         // constructor
	virtual ~TS() {};                                // destructor

	// Modifiers
	virtual bool insert(std::string, double) = 0;    // insert new elements in the time series
	virtual void set_sample_date(/*std::string*/time_t) = 0;   // setter for the in-sample period end date
	virtual void reset_sample_date() = 0;
	// Observers
	virtual std::string get_name() = 0;              // getter for the name of the time series
	virtual std::string get_sample_date() = 0;       // getter for the in-sample period end date
	virtual time_t get_sample_timestamp() = 0;       // getter for the in-sample period end time stamp

	// Capacity
	virtual int series_size() = 0;                   // time series length
	virtual int sample_size() = 0;                   // in-sample series length

	// Delegation pattern (delegate the TS iterator logic to map's iterators)
	typedef std::map<time_t, DataPoint*>::iterator iterator;
	typedef std::map<time_t, DataPoint*>::reverse_iterator reverse_iterator;

	// Iterator pattern
	virtual iterator series_begin() = 0;              // iterator to the begin of the time series
	virtual iterator series_end() = 0;                // iterator to the end of the time series
	virtual reverse_iterator series_rbegin() = 0;     // reverse iterator to the end of the time series
	virtual reverse_iterator series_rend() = 0;       // reverse iterator to the begin of the time series
	virtual iterator in_sample_begin() = 0;           // iterator to the begin of the in-sample series
	virtual iterator in_sample_end() = 0;             // iterator to the end of the in-sample series
};

// Decorator pattern (Concrete Class)
class TimeSeries : public TS
{
private:
	std::string name;                    // time series name
	std::map<time_t, DataPoint*> series; // time series data points
	std::string sample_datetime;             // in-sample period end date
	time_t sample_timestamp;             // in-sample period end time stamp

public:
	TimeSeries(std::string, std::string);         // two different constructors
	TimeSeries(std::string);
	virtual ~TimeSeries();                        // destructor

	// Modifiers
	virtual bool insert(std::string, double);     // insert new elements in the time series
	virtual void set_sample_date(/*std::string*/time_t);    // remove the in/out-of-sample boundary
	virtual void reset_sample_date();
	// Observers
	virtual std::string get_name();               // getter for the name of the time series
	virtual std::string get_sample_date();        // getter for the in-sample period end date
	virtual time_t get_sample_timestamp();        // getter for the in-sample period end time stamp

	// Capacity
	virtual int series_size();                    // time series length
	virtual int sample_size();                    // in-sample series length

	// Iterator pattern
	virtual iterator series_begin();              // iterator to the begin of the time series
	virtual iterator series_end();                // iterator to the end of the time series
	virtual reverse_iterator series_rbegin();     // reverse iterator to the end of the time series
	virtual reverse_iterator series_rend();       // reverse iterator to the begin of the time series
	virtual iterator in_sample_begin();           // iterator to the begin of the in-sample series
	virtual iterator in_sample_end();             // iterator to the end of the in-sample series
	virtual reverse_iterator in_sample_rbegin();  // reverse iterator to the end of the in-sample series
	virtual reverse_iterator in_sample_rend(); 
};
// Constructor (read from CSV file)
TimeSeries::TimeSeries(std::string name, std::string filepath)
{
	std::ifstream file(filepath);
	std::string line = "";
	std::string datetime = "";
	double price = 0.0;
	DataPoint* dp;
	struct tm tm = {0};
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;

	this->name = name;
	this->sample_datetime = "";
	this->sample_timestamp = 0;

	if(!file.is_open())
	{
		return;
	}

	// Skip CSV header
	// getline(file, line);
	getline(file, line);

	// Read first line of relevant data
	getline(file, line);

	while(line.size() != 0)
	{
		datetime = line.substr(0, 16);                             // get date
		price = std::stod(line.substr(20, line.size() - 20));  // get value
		dp = new DataPoint(datetime, price);                       // create new DataPoint object

		sscanf(datetime.c_str(), "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute); // convert date to time stamp
		tm.tm_year = year - 1900;
		tm.tm_mon = month - 1;
		tm.tm_mday = day;
		tm.tm_hour = hour;
		tm.tm_min = minute;
		this->series.insert(std::pair<time_t, DataPoint*>(mktime(&tm), dp));

		// Read next line of relevant data
		getline(file, line);
	}

	return;
}

// Constructor
TimeSeries::TimeSeries(std::string name)
{
	this->name = name;
	this->sample_datetime = "";
	this->sample_timestamp = 0;

	return;
}

// Destructor
TimeSeries::~TimeSeries()
{
	// Delete all data points in the time series
	for(iterator it = this->series_begin(); it != this->series_end(); ++it)
	{
		delete (*it).second;
	}

	series.clear();

	return;
}

// Insert new elements in the time series
bool TimeSeries::insert(std::string datetime, double price)
{
	DataPoint* dp;
	struct tm tm = {0};
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int minute = 0;
	time_t timestamp;

	sscanf(datetime.c_str(), "%d-%d-%d %d:%d", &year, &month, &day, &hour, &minute); // convert date to time stamp
	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;
	timestamp = mktime(&tm);

	// Check if there is already a value for the desired
	// date in the time series, then add if negative
	dp = new DataPoint(datetime, price);
	this->series.insert(std::pair<time_t, DataPoint*>(timestamp, dp));

	return true;
	// if(this->series.count(timestamp) == 0)
	// {
	// 	dp = new DataPoint(datetime, price);
	// 	this->series.insert(std::pair<time_t, DataPoint*>(timestamp, dp));

	// 	return true;
	// }
	// else
	// {
	// 	return false;
	// }
}

// Setter for the in-sample period end date
void TimeSeries::set_sample_date(/*std::string*/time_t sample_datetime)
{
	// struct tm tm = {0};
	// int year = 0;
	// int month = 0;
	// int day = 0;
	// int hour = 0;
	// int minute = 0;

	// this->sample_datetime = sample_datetime;

	// sscanf(this->sample_datetime.c_str(), "%d-%d-%d-%d-%d", &year, &month, &day, &hour, &minute); // convert date to time stamp
	// tm.tm_year = year - 1900;
	// tm.tm_mon = month - 1;
	// tm.tm_mday = day;
	// tm.tm_hour = hour;
	// tm.tm_min = minute;
	// this->sample_timestamp = mktime(&tm);
    this->sample_timestamp = sample_datetime;
	return;
}

// Remove the in/out-of-sample boundary
void TimeSeries::reset_sample_date()
{
	this->sample_datetime = "";
	this->sample_timestamp = 0;

	return;
}

// Getter for the name of the time series
std::string TimeSeries::get_name()
{
	return this->name;
}

// Getter for the in-sample period end date
std::string TimeSeries::get_sample_date()
{
	return this->sample_datetime;
}

// Getter for the in-sample period end time stamp
time_t TimeSeries::get_sample_timestamp()
{
	return this->sample_timestamp;
}

// Time series length
int TimeSeries::series_size()
{
	return this->series.size();
}

// In-sample series length
int TimeSeries::sample_size()
{
	if(this->get_sample_timestamp() != 0)
	{
		return std::distance(this->series.begin(), this->series.lower_bound(this->sample_timestamp)) + 1;
	}
	else
	{
		return this->series.size();
	}
}

// Iterator to the begin of the time series
TimeSeries::iterator TimeSeries::series_begin()
{
	return this->series.begin();
}

// Iterator to the end of the time series
TimeSeries::iterator TimeSeries::series_end()
{
	return this->series.end();
}

// Reverse iterator to the end of the time series
TimeSeries::reverse_iterator TimeSeries::series_rbegin()
{
	return this->series.rbegin();
}

// Reverse iterator to the begin of the time series
TimeSeries::reverse_iterator TimeSeries::series_rend()
{
	return this->series.rend();
}

// Iterator to the begin of the in-sample series
TimeSeries::iterator TimeSeries::in_sample_begin()
{
	return this->series.begin();
}

// Iterator to the end of the in-sample series
TimeSeries::iterator TimeSeries::in_sample_end()
{
	if(this->get_sample_timestamp() != 0)
	{
		return ++this->series.lower_bound(this->sample_timestamp);
	}
	else
	{
		return this->series.end();
	}
}

// Reverse iterator to the end of the in-sample series
TimeSeries::reverse_iterator TimeSeries::in_sample_rbegin()
{
	if(this->get_sample_timestamp() != 0)
	{
		return TimeSeries::reverse_iterator(this->series.lower_bound(this->sample_timestamp));
	}
	else
	{
		return this->series.rbegin();
	}
}

// Reverse iterator to the begin of the in-sample series
TimeSeries::reverse_iterator TimeSeries::in_sample_rend()
{
	return this->series.rend();
}
#endif /* TIMESERIES_H_ */
