#include <crails/cms/routes.hpp>
#include "controller.hpp"
#include "app/models/bookable_event.hpp"
#include "app/models/event_booking.hpp"
#include "lib/odb/application-odb.hxx"
#include "lib/plugin-odb.hxx"

AdminBookingController::AdminBookingController(Crails::Context& context) : Super(context)
{
  vars["page_title"] = i18n::t("admin.bookable-events");
  vars["local_route"] = std::string(Super::scope) + '/' + scope;
}

void AdminBookingController::index()
{
  odb::result<BookableEventIndexView> events;
  std::time_t from_date = params["from"].defaults_to<std::time_t>(0);
  std::time_t to_date   = params["to"].defaults_to<std::time_t>(0);

  vars["page_subtitle"] = i18n::t("admin.bookable-events-index");
  if (from_date != 0 && to_date != 0)
  {
    database.find<BookableEventIndexView>(
      events,
      odb::query<BookableEvent>::date >= from_date && odb::query<BookableEvent>::date <= to_date
    );
  }
  else
    database.find<BookableEventIndexView>(events);
  vars["models"] = &events;
  render("admin/booking/index");
}

void AdminBookingController::show()
{
  if (require_resource())
    render("admin/booking/show");
  else
    respond_with(Crails::HttpStatus::not_found);
}

void AdminBookingController::new_()
{
  BookableEvent event;

  vars["page_subtitle"] = i18n::t("admin.bookable-events-new");
  vars["model"] = const_cast<const BookableEvent*>(&event);
  render("admin/booking/show");
}

void AdminBookingController::create()
{
  BookableEvent event;
  Crails::Cms::LocalRoute route(vars);

  event.edit(params[BookableEvent::scope]);
  database.save(event);
  redirect_to(route.make(event.get_id()));
}

void AdminBookingController::update()
{
  if (require_resource())
  {
    Crails::Cms::LocalRoute route(vars);

    event->edit(params[BookableEvent::scope]);
    database.save(*event);
    redirect_to(route.make(event->get_id()));
  }
}

void AdminBookingController::destroy()
{
  if (require_resource())
  {
    Crails::Cms::LocalRoute route(vars);

    database.destroy(*event);
    redirect_to(route());
  }
}

void AdminBookingController::bookings()
{
  if (require_resource())
  {
    odb::result<EventBooking> bookings;

    database.find<EventBooking>(bookings, odb::query<EventBooking>::event_id == event->get_id());
    vars["bookings"] = &bookings;
    render("admin/booking/bookings");
  }
}

bool AdminBookingController::require_resource()
{
  database.find_one(event, odb::query<BookableEvent>::id == params["id"].as<Crails::Odb::id_type>());
  if (!event)
  {
    respond_with(Crails::HttpStatus::not_found);
    return false;
  }
  vars["model"] = const_cast<const BookableEvent*>(event.get());
  vars["page_subtitle"] = event->get_title();
  return true;
}
