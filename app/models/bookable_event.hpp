#pragma once
#include <crails/cms/models/editable.hpp>

#pragma db object
class BookableEvent : public Crails::Cms::Editable
{
  odb_instantiable()
public:
  static const std::string scope;
  static const std::string plural_scope;
  static const std::string view;

  #pragma db view object(BookableEvent)
  struct Count
  {
    #pragma db column("count(" + BookableEvent::id + ")")
    size_t value;
  };

  void edit(Data);
  void merge_data(Data) const;
  std::string to_json() const;
  void set_booking_ends_at(std::time_t value) { this->booking_ends_at = value; }
  std::time_t get_booking_ends_at() const { return booking_ends_at; }
  void set_booking_ends_at_from_form(const std::string& value);
  std::string get_form_booking_ends_at() const;
  void set_date(std::time_t value) { this->date = value; }
  std::time_t get_date() const { return date; }
  void set_date_from_form(const std::string& value);
  std::string get_form_date() const;
  void set_duration(std::time_t value) { this->duration = value; }
  std::time_t get_duration() const { return duration; }
  void set_people_count(unsigned int value) { this->people_count = value; }
  unsigned int get_people_count() const { return people_count; }
  void set_people_capacity(unsigned int value) { this->people_capacity = value; }
  unsigned int get_people_capacity() const { return people_capacity; }

  std::string slug_from_title(const std::string& title) const override;

  bool is_complete() const { return people_count >= people_capacity; }
  bool is_booking_closed() const;

private:
  std::time_t booking_ends_at = 0;
  std::time_t date = 0;
  std::time_t duration = 0;
  unsigned int people_count = 0;
  unsigned int people_capacity = 0;
};

#include "bookable_event_index_view.hpp"
