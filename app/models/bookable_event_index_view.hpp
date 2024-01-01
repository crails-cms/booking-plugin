#pragma once
#include <string>
#include "bookable_event.hpp"

#pragma db view object(BookableEvent)
struct BookableEventIndexView
{
  std::string get_database_name() const { return BookableEvent().get_database_name(); }

  Crails::Odb::id_type id;
  std::string title;
  std::string slug;
  std::string description;
  std::string thumbnail;
  std::time_t date;
  std::time_t duration;
  std::time_t booking_ends_at;
  unsigned int people_count;
  unsigned int people_capacity;
};
