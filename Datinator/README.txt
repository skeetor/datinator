v0.5:
- A value from a different column can now be used (i.E. combine multiple columns to a single value)
- Changes to avoid excessive internal string conversion. Reduces memory footprint and inproves performance.
- Added string manipulation for multiline conversion (needed for CSV).

v0.4:
- When configuring manipulators a test value can now be entered.
- Added operations for text manipulation
  * Fixed Text
  * Limit size
  * To upper case
  * To lower case
  * Replace by text/regular expression
  * Select by regular expression
  * Select by character position

v0.3:
- Profiles now also have a most recent menu.
- Added support for commit options for SQLite3
- Fixed crash when cancelling "Run Profile" file selection dialog.
- Fixed wrongly reported rows when copying is canceled.
- Added rowcount to CSV and SQLite3 readers.
- Fixed stretching for Oracle writer panel.
- Fixed bug that tables were not refreshed/emptied when disconnecting from database
- Removed disconnect after copying is done.
- Frame was not restored to correct size when an error occured during startup.
- Connectors as plugin DLLs.

v0.2:
- Improved handling of CSV reading.
- Fixed bugs so that umlauts are now properly handled and preserved.
- Bug fixed where reading a CSV file.
- Proper error message when no table is specified for creating a new SQLite3 database.
- Creating an insert with an empty sqlite3 fixed.
- Added exporting inserts to file
- Fixed crash when loading profiles
