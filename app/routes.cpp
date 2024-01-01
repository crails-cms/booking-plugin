#include <crails/router.hpp>
#include <crails/cms/views/admin_menu.hpp>
#include "controllers/controller.hpp"
#include "controllers/admin/controller.hpp"

void initialize_plugin_routes()
{
  auto& router = Crails::Router::singleton::require();
  auto& admin_menu = Crails::Cms::AdminMenu::singleton::require();

  router.scope(BookingController::scope, [&]()
  {
    router.match_action("GET", "/", BookingController, index);
    router.match_action("GET", "/:slug", BookingController, show);
    router.match_action("GET", "/by-id/:id", BookingController, by_id);
    router.match_action("POST", "/by-id/:id/booking", BookingController, book);
  });

  router.scope(AdminApplicationController::scope, [&]()
  {
    router.scope(AdminBookingController::scope, [&]()
    {
      admin_menu.add({3, "bookings", router.get_current_scope()});
      router.match_action("GET", "/", AdminBookingController, index);
      router.match_action("POST", "/", AdminBookingController, create);
      router.match_action("GET", "/new", AdminBookingController, new_);
      router.match_action("GET", "/:id", AdminBookingController, show);
      router.match_action("PUT", "/:id", AdminBookingController, update);
      router.match_action("DELETE", "/:id", AdminBookingController, destroy);
      router.match_action("GET", "/:id/bookings", AdminBookingController, bookings);
    });
  });
}
