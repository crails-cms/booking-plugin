#pragma once
#include "app/controllers/application.hpp"
#include <ctime>

class BookingController : public ApplicationController
{
  typedef ApplicationController Super;
public:
   static constexpr const char* scope = "booking";

   BookingController(Crails::Context&);

   void index();
   void show();
   void by_id();
   void book();
protected:
   virtual std::pair<std::time_t, std::time_t> default_range() const;
};
