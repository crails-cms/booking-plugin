#include "controller.hpp"
#include "app/models/bookable_event.hpp"
#include "app/models/event_booking.hpp"
#include <chrono>
#include <iostream>
#include <crails/captcha/default.hpp>
#include "lib/odb/application-odb.hxx"
#include "lib/plugin-odb.hxx"

using namespace std;

BookingController::BookingController(Crails::Context& context) : Super(context)
{
  vars["local_route"] = std::string(scope);
}

pair<time_t, time_t> BookingController::default_range() const
{
  time_t from_date;
  time_t to_date;
  auto   now = chrono::system_clock::now();
  time_t timestamp = chrono::system_clock::to_time_t(now);
  tm     utc = *gmtime(&timestamp);

  utc.tm_sec = utc.tm_min = utc.tm_hour = utc.tm_mday = utc.tm_mon = utc.tm_wday = utc.tm_yday = 0;
  from_date = std::mktime(&utc);
  utc.tm_year++;
  to_date = std::mktime(&utc);
  return {from_date, to_date};
}

void BookingController::index()
{
  odb::result<BookableEventIndexView> events;
  pair<time_t, time_t> range{
    params["from"].defaults_to<time_t>(0),
    params["to"].defaults_to<time_t>(0)
  };

  if (range.first == 0 || range.second == 0)
    range = default_range();
  database.find<BookableEventIndexView>(
    events,
      (odb::query<BookableEvent>::date >= range.first
    && odb::query<BookableEvent>::date <= range.second)
    + "ORDER BY" + odb::query<BookableEvent>::date + "DESC"
  );
  vars["range"] = range;
  vars["models"] = &events;
  render("booking/index");
}

void BookingController::show()
{
  std::shared_ptr<BookableEvent> event;
  EventBooking booking;

  database.find_one(event, odb::query<BookableEvent>::slug == params["slug"].as<std::string>());
  if (event)
  {
    Crails::Captcha::Default captcha;

    vars["model"] = const_cast<const BookableEvent*>(event.get());
    vars["booking"] = const_cast<const EventBooking*>(&booking);
    vars["captcha_form"] = captcha.render(params);
    render("booking/show");
  }
  else
    respond_with(Crails::HttpStatus::not_found);
}

void BookingController::by_id()
{
  std::shared_ptr<BookableEvent> event;
  EventBooking booking;

  database.find_one(event, odb::query<BookableEvent>::id == params["id"].as<Crails::Odb::id_type>());
  if (event)
  {
    Crails::Captcha::Default captcha;

    vars["model"] = const_cast<const BookableEvent*>(event.get());
    vars["booking"] = const_cast<const EventBooking*>(&booking);
    vars["captcha_form"] = captcha.render(params);
    render("booking/show");
  }
  else
    respond_with(Crails::HttpStatus::not_found);
}

void BookingController::book()
{
  std::shared_ptr<BookableEvent> event;

  database.find_one(event, odb::query<BookableEvent>::id == params["id"].as<Crails::Odb::id_type>());
  if (event)
  {
    Crails::Captcha::Default captcha;
    auto self = shared_from_this();

    captcha.check(params, [this, self, event](bool success)
    {
      Crails::Captcha::Default captcha;
      EventBooking booking;
      unsigned int people_count = event->get_people_count();
      unsigned int remaining_space = event->get_people_capacity() - people_count;

      booking.set_event_id(event->get_id());
      booking.edit(params[EventBooking::scope]);
      if (!success)
        vars["error"] = EventBooking::WrongCaptcha;
      else if (booking.get_people_count() > remaining_space)
      {
        vars["error"] = EventBooking::NotEnoughSpace;
        vars["remaining_space"] = remaining_space;
      }
      else
      {
        people_count += booking.get_people_count();
        event->set_people_count(people_count);
        database.save(booking);
        database.save(*event);
      }
      vars["model"] = const_cast<const BookableEvent*>(event.get());
      vars["booking"] = const_cast<const EventBooking*>(&booking);
      if (!booking.is_persistent())
        vars["captcha_form"] = captcha.render(params);
      render("booking/show");
    });
  }
  else
    respond_with(Crails::HttpStatus::not_found);
}
