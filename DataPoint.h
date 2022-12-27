#ifndef DATAPOINT_H_
#define DATAPOINT_H_

#include<string>

class DataPoint
{
private:
  std::string datetime;
  double price;

public:
  DataPoint(std::string, double);
  virtual ~DataPoint();

  void set_datetime(std::string);
  void set_value(double);

  std::string get_datetime();
  double get_value();

};
DataPoint::DataPoint(std::string datetime, double price)
{
	this->datetime = datetime;
	this->price = price;

	return;
}

// Destructor
DataPoint::~DataPoint()
{
	return;
}

// Setter of the data point date
void DataPoint::set_datetime(std::string datetime)
{
	this->datetime = datetime;

	return;
}

// Setter of the data point value
void DataPoint::set_value(double price)
{
	this->price = price;

	return;
}

// Getter of the data point date
std::string DataPoint::get_datetime()
{
	return this->datetime;
}

// Getter of the data point value
double DataPoint::get_value()
{
	return this->price;
}
#endif
