#include "bookable_event.hpp"
#include "lib/plugin-odb.hxx"
#include <crails/cms/time.hpp>
#include <crails/utils/semantics.hpp>
#include <crails/url.hpp>
#include <iomanip>
#include <ctime>
#include <chrono>

const std::string BookableEvent::scope = "bookable-event";
const std::string BookableEvent::plural_scope = "bookable-events";
const std::string BookableEvent::view = "";

odb_instantiable_impl(BookableEvent)

std::string BookableEvent::slug_from_title(const std::string& title) const
{
  std::stringstream stream;

  stream << time_to_string(date, "%d-%M-%Y") << '-'
	 << Crails::Url::encode(Crails::dasherize(title));
  return stream.str();
}

void BookableEvent::edit(Data params)
{
  Crails::Cms::Editable::edit(params);
  if (params["booking_ends_at"].exists())
    set_booking_ends_at_from_form(params["booking_ends_at"]);
  if (params["date"].exists())
    set_date_from_form(params["date"]);
  if (params["duration"].exists())
    set_duration(params["duration"]);
  if (params["people_capacity"].exists())
    set_people_capacity(params["people_capacity"]);
}

void BookableEvent::merge_data(Data out) const
{
  Crails::Cms::Editable::merge_data(out);
  out["booking_ends_at"] = this->booking_ends_at;
  out["date"] = this->date;
  out["duration"] = this->duration;
  out["people_count"] = this->people_count;
  out["people_capacity"] = this->people_capacity;
}

std::string BookableEvent::to_json() const
{
  DataTree out;

  merge_data(out);
  return out.to_json();
}

void BookableEvent::set_date_from_form(const std::string& value)
{
  date = time_from_form_value(value);
}

std::string BookableEvent::get_form_date() const
{
  return time_to_form_value(date);
}

void BookableEvent::set_booking_ends_at_from_form(const std::string& value)
{
  booking_ends_at = time_from_form_value(value);
}

std::string BookableEvent::get_form_booking_ends_at() const
{
  return time_to_form_value(date);
}

bool BookableEvent::is_booking_closed() const
{
  auto now = std::chrono::system_clock::now();
  
  return booking_ends_at < std::chrono::system_clock::to_time_t(now);
}
