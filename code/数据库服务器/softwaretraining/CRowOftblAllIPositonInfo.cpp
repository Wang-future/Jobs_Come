#include"CRowOfDBTable.h"
void CRowOftblAllIPositonInfo::Initialization(int _date, string _company, string _workplaceprovince, string _workplacecity
  , string _diploma, short _number, string _supplement, string _positiontype, string _keyword, int _experience,int _salary)
{
  this->date = _date;
  this->company = _company;
  this->workplaceprovince = _workplaceprovince;
  this->workplacecity = _workplacecity;
  this->diploma = _diploma;
  this->number = _number;
  this->supplement = _supplement;
  this->positiontype = _positiontype;
  this->keyword = _keyword;
  this->experience = _experience;
  this->salary = _salary;
}