#pragma once
#include <crails/odb/model.hpp>
#include <crails/datatree.hpp>
#pragma db object
class EventBooking : public Crails::Odb::Model
{
  odb_instantiable()
public:
  static const std::string scope;
  static const std::string plural_scope;
  static const std::string view;

  #pragma db view object(EventBooking)
  struct Count
  {
    #pragma db column("count(" + EventBooking::id + ")")
    size_t value;
  };

  enum ValidationError
  {
     NoValidationError, WrongCaptcha, NotEnoughSpace
  };

  void edit(Data);
  void merge_data(Data) const;
  std::string to_json() const;
  void set_email(const std::string& value) { this->email = value; }
  const std::string& get_email() const { return email; }
  void set_event_id(const Crails::Odb::id_type& value) { this->event_id = value; }
  const Crails::Odb::id_type& get_event_id() const { return event_id; }
  void set_firstname(const std::string& value) { this->firstname = value; }
  const std::string& get_firstname() const { return firstname; }
  void set_lastname(const std::string& value) { this->lastname = value; }
  const std::string& get_lastname() const { return lastname; }
  void set_people_count(unsigned int value) { this->people_count = value; }
  unsigned int get_people_count() const { return people_count; }
  void set_phone(const std::string& value) { this->phone = value; }
  const std::string& get_phone() const { return phone; }

private:
  std::string email;
  Crails::Odb::id_type event_id = 0;
  std::string firstname;
  std::string lastname;
  unsigned int people_count = 1;
  std::string phone;
};
