#include<iostream>
#include<string>
#include"DataPoint.h"
#include"TimeSeries.h"
#include"LinearRegression.h"
#include"Signal.h"

using namespace std; 

// TimeSeries* get_lastX(){

// }

// TimeSeries* get_lastY(){

// }

// TimeSeries* get_currentX(){

// }

// TimeSeries* get_currentY(){

// }

double backtest(TimeSeries *lastY, TimeSeries *lastX, TimeSeries *currentY, TimeSeries *currentX){

  double result;
  int position;

  LinearRegression *lastLR;
  lastLR = new LinearRegression(lastX, lastY);

  Signal *sig; 
  sig = new Signal(lastLR->residuals("lastSpread"));

  // LinearRegression *currentLR;
  // currentLR = new LinearRegression(currentX, currentY)
  // TimeSeries *currentSpread;
  // currentSpread = new TimeSeries(currentLR->residuals("currentSpread"));

  double currentSpread;
  position = 0;
  double profitY;
  double profitX;
  for (TimeSeries::iterator itCurrentX = currentX->series_begin(), itCurrentY = currentY->series_begin();itCurrentY != currentY->series_end() && itCurrentX != currentX->series_end();++itCurrentY, ++itCurrentX){
    if (itCurrentY == currentY->series_end() && itCurrentX == currentX->series_end()){
      if (position != 0){
        if (position == 1){
          // close long
          profitY += (*itCurrentY).second->get_value();
          profitX -= lastLR->get_beta() * (*itCurrentX).second->get_value();
        }
        else{
          // close short
          profitY -= (*itCurrentY).second->get_value();
          profitX += lastLR->get_beta() * (*itCurrentX).second->get_value();
        }
      }
    }
    currentSpread = (*itCurrentY).second->get_value() - lastLR->get_beta() * (*itCurrentX).second->get_value();
    if(position == 0){
      if (currentSpread > sig->short_signal()){
        // short
        profitY += (*itCurrentY).second->get_value();
        profitX -= lastLR->get_beta() * (*itCurrentX).second->get_value();
        position = -1;
      }
      else if (currentSpread < sig->long_signal()){
        // long
        profitY -= (*itCurrentY).second->get_value();
        profitX += lastLR->get_beta() * (*itCurrentX).second->get_value();
        position = 1;
      }
    }
    else if (position == 1){
      if (currentSpread > sig->close_long_signal()){
        // close long
        profitY += (*itCurrentY).second->get_value();
        profitX -= lastLR->get_beta() * (*itCurrentX).second->get_value();
        position = 0;
      }
    }
    else if (position == -1){
      if (currentSpread < sig->close_short_signal()){
        // close short
        profitY -= (*itCurrentY).second->get_value();
        profitX += lastLR->get_beta() * (*itCurrentX).second->get_value();
        position = 0;
      }
    }
  }

  position = 0;

  // delete lastSpread;
  delete lastLR;

  result = profitX + profitY;

  return result;
}

int main(){

  double profit;

  TimeSeries *X;
  TimeSeries *Y;
  X = new TimeSeries("TSLA", "TSLA.csv");
  Y = new TimeSeries("XPEV", "XPEV.csv");

  for(int i=0;i<1;i++){
    TimeSeries *lastX;
    TimeSeries *lastY;
    TimeSeries *currentX;
    TimeSeries *currentY;
    lastX = new TimeSeries("lastX");
    lastY = new TimeSeries("lastY");
    currentX = new TimeSeries("currentX");
    currentY = new TimeSeries("currentY");

    // for(TimeSeries::iterator itY = next(Y->series_begin(),i*390), itX = next(X->series_begin(),i*390); itY != next(Y->series_begin(),(i+1)*390) && itX != next(X->series_begin(),(i+1)*390); ++itY, ++itX){
    //   lastX->insert((*itX).second->get_datetime(), (*itX).second->get_value());
    //   lastY->insert((*itY).second->get_datetime(), (*itY).second->get_value());
    // }
    for(TimeSeries::iterator itX = next(X->series_begin(),i*390); itX != next(X->series_begin(),(i+1)*390); ++itX){
      lastX->insert((*itX).second->get_datetime(), (*itX).second->get_value());
    }
    for(TimeSeries::iterator itY = next(Y->series_begin(),i*390); itY != next(Y->series_begin(),(i+1)*390); ++itY){
      lastY->insert((*itY).second->get_datetime(), (*itY).second->get_value());
    }
    for(TimeSeries::iterator itX = next(X->series_begin(),(i+1)*390); itX != next(X->series_begin(),(i+2)*390); ++itX){
      currentX->insert((*itX).second->get_datetime(), (*itX).second->get_value());
    }
    for(TimeSeries::iterator itY = next(Y->series_begin(),(i+1)*390); itY != next(Y->series_begin(),(i+2)*390); ++itY){
      currentY->insert((*itY).second->get_datetime(), (*itY).second->get_value());
    }

    profit += backtest(lastX, lastY, currentX, currentY);

    delete lastX;
    delete lastY;
    delete currentX;
    delete currentY;
  }

  delete X;
  delete Y;

  cout << "The Profit is: " << profit << endl;

  return 0;
}
