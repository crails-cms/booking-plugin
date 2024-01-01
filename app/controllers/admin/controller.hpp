#pragma once
#include "app/controllers/admin/application.hpp"

class BookableEvent;

class AdminBookingController : public AdminApplicationController
{
  typedef AdminApplicationController Super;
public:
  static constexpr const char* scope = "bookable-events";

  AdminBookingController(Crails::Context& context);

  void index();
  void create();
  void show();
  void new_();
  void update();
  void destroy();
  void bookings();

private:
  bool require_resource();
  std::shared_ptr<BookableEvent> event;
};
