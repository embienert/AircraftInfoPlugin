# Aircraft Info Plugin

Aircraft Info Plugin for Euroscope gives you all important aircraft type information at a glance. 

## How to install
1. Download the latest release. The plugin is packaged with the plugin file `AircraftInfoPlugin.dll` and database `aircraft_data.csv` in a zip file.
2. Choose any location to extract the zip file. Make sure to keep both files in the same directory.
3. In Euroscope, add the plugin via `Other Set > Plug-ins...` and then the `Load` button to the right. 
Select the `AircraftInfoPlugin.dll` file at the location it was saved in step 2.
 
By default, the list with the aircraft information will appear in the top-left corner of the view. 
It may be hidden behind other lists. Once loaded, the plugin is saved in the profile and will be 
loaded as you left it when starting Euroscope.

## Euroscope Commands
All commands start with the prefix `.aip`, for **A**ircraft **I**nformation **P**lugin

| Command       | Description                               |
|---------------|-------------------------------------------|
| `.aip`        | Show current plugin status                |
| `.aip help`   | Show quick overview of available commands |
| `.aip show`   | Show the aircraft info list, if hidden    |
| `.aip hide`   | Hide the aircraft info list, if shown     |
| `.aip reload` | Reload the aircraft database file         |

