# EvoTherm

Command-line utility to control your Evohome thermostat.

With EvoTherm you can:

* list your zones and current operating mode
* set the target temperature of any zone, permanently or temporarily
* set the operating mode, permanently or temporarily

<img src="https://raw.githubusercontent.com/giannivh/EvoTherm/master/doc/evotherm_01.png" width="600">
<img src="https://raw.githubusercontent.com/giannivh/EvoTherm/master/doc/evotherm_02.png" width="600">

## Install

### Prerequisites

You will need:

* make 3.81 or higher
* CMake 3.6 or higher
* cURL

### Installation

Installation of EvoTherm is easy:

```
$ git clone git@github.com:giannivh/EvoTherm.git
$ cd EvoTherm
$ cmake .
$ make
$ make install
```

If you get an error on `make install`, issue `sudo make install`.

Or, if you're on a mac:
    
```
$ brew install giannivh/tap/evotherm
```

## Usage

Use `evotherm --help` to get all options.

### Config

If you're running EvoTherm for the first time, you have to create a configuration file which contains your Honeywell My Total Connect Comfort credentials. 
You can do this manually, or let EvoTherm do it for you by issuing:

```
$ evotherm --config
```

And follow the on-screen instructions.

### List

To list your zones, issue:

```
$ evotherm #(or evotherm --list)
```

### Set target temperature

To set the target temperature of zones permanently, issue:

```
$ evotherm --zone "My Zone" --temp 21.5
```

To set the target temperature of zones temporarily, issue:

```
$ evotherm --zone "My Zone" --temp 21.5 --until "2016-10-17 22:30:00"
```

*Please note that the until option only accepts format `"%Y-%m-%d %H:%M:%S`*

To cancel a target temperature override, and to return to the normal schedule, issue:

```
$ evotherm --zone "My Zone" --cancel
```

You can also set multiple zones at once. Use a comma separated list for this. Example:

```
$ evotherm --zone "Zone1,Zone2,Zone3" --temp 16 --until "2016-10-19 23:00:00"
```

### Set mode

To change the mode of your thermostat permanently, issue:

```
$ evotherm --mode auto
```

To change the mode of your thermostat temporarily, issue:

```
$ evotherm --mode auto --until "2016-10-17 22:30:00"
```

Where `--mode` is one of the following:

* **auto**: follow the normal schedule
* **eco**: follow the eco schedule
* **away**: follow the away schedule
* **dayoff**: follow the weekend schedule
* **off**: all heating off
* **custom**: follow the custom schedule
