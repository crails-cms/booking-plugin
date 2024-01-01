#include "event_booking.hpp"
#include "lib/plugin-odb.hxx"

const std::string EventBooking::scope = "event-booking";
const std::string EventBooking::plural_scope = "event-bookings";
const std::string EventBooking::view = "";

odb_instantiable_impl(EventBooking)

void EventBooking::edit(Data params)
{
  if (params["email"].exists())
    set_email(params["email"]);
  if (params["event_id"].exists())
    set_event_id(params["event_id"]);
  if (params["firstname"].exists())
    set_firstname(params["firstname"]);
  if (params["lastname"].exists())
    set_lastname(params["lastname"]);
  if (params["people_count"].exists())
    set_people_count(params["people_count"]);
  if (params["phone"].exists())
    set_phone(params["phone"]);
}

void EventBooking::merge_data(Data out) const
{
  out["email"] = this->email;
  out["event_id"] = this->event_id;
  out["firstname"] = this->firstname;
  out["lastname"] = this->lastname;
  out["people_count"] = this->people_count;
  out["phone"] = this->phone;
}

std::string EventBooking::to_json() const
{
  DataTree out;

  merge_data(out);
  return out.to_json();
}
