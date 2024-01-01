#include <crails/odb/connection.hpp>

const char* drop_bookable_event_query =
  "DROP TABLE IF EXISTS \"bookingBookableEvent\" CASCADE";

const char* create_bookable_event_query =
  "CREATE TABLE \"bookingBookableEvent\" ("
    "\"id\" BIGSERIAL NOT NULL PRIMARY KEY,"
    "\"deleted\" BOOLEAN NOT NULL DEFAULT FALSE,"
    "\"created_at\" BIGINT NULL,"
    "\"updated_at\" BIGINT NULL,"
    "\"title\" TEXT NOT NULL,"
    "\"slug\" TEXT NOT NULL,"
    "\"body\" TEXT NOT NULL,"
    "\"description\" TEXT NOT NULL,"
    "\"thumbnail\" TEXT NOT NULL,"
    "\"preview\" BOOLEAN NOT NULL,"
    "\"booking_ends_at\" BIGINT NOT NULL,"
    "\"date\" BIGINT NOT NULL,"
    "\"duration\" BIGINT NOT NULL,"
    "\"people_count\" INTEGER NOT NULL,"
    "\"people_capacity\" INTEGER NOT NULL);";

const char* drop_event_booking_query =
  "DROP TABLE IF EXISTS \"bookingEventBooking\" CASCADE;";

const char* create_event_booking_query =
  "CREATE TABLE \"bookingEventBooking\" ("
    "\"id\" BIGSERIAL NOT NULL PRIMARY KEY,"
    "\"deleted\" BOOLEAN NOT NULL DEFAULT FALSE,"
    "\"created_at\" BIGINT NULL,"
    "\"updated_at\" BIGINT NULL,"
    "\"email\" TEXT NOT NULL,"
    "\"event_id\" BIGINT NOT NULL,"
    "\"firstname\" TEXT NOT NULL,"
    "\"lastname\" TEXT NOT NULL,"
    "\"people_count\" INTEGER NOT NULL,"
    "\"phone\" TEXT NOT NULL);";

void install_plugin_database()
{
  Crails::Odb::Connection database;

  database.transaction.require("odb");
  database.transaction.get_database().execute(create_bookable_event_query);
  database.transaction.get_database().execute(create_event_booking_query);
  database.commit();
}

void uninstall_plugin_database()
{
  Crails::Odb::Connection database;

  database.transaction.require("odb");
  database.transaction.get_database().execute(drop_bookable_event_query);
  database.transaction.get_database().execute(drop_event_booking_query);
  database.commit();
}
