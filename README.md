# Pairs-Trading-Implementation-in-Cpp

This project includes:
  1. MAIN.cpp
    The file that executes our trading strategy and outputs the backtesting
    result.

  2. DataPoint.h and DataPoint.cpp
    DataPoint is one data structure that consists of TimeSeries data structure,
    which contains key(date) and value(stock price).

  3. TimeSeries.h and TimeSeries.cpp
    TimeSeries is one data structure that stores stock prices. Each of them
    contains only one stock or the one spread between two stocks.

  4. A Linear Regression Process
    A process that produces coefficients for each pair of stocks. Either written
    in python or cpp.
