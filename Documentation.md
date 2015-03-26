# URI documentation #

uri are in the form "scheme:?arg1=value1&arg2=value2..."

Parameters can be set in arbitrary order. Parameters not set use default values.

### Default parameters ###
The uri "" sets all parameters to default

### Debug level ###
  * **debugLevel** (int) sets the debug level. Default is 0.
    * 1 and above: lists connected Gametraks and their related information (e.g. path, serial number)
    * 2 and above: display metric values for each degree of freedom
    * 3 and above: display raw sensor information

### Device parameters ###
  * **devicePath** (string) sets the device by its path. Default value: "". Use debugLevel 1 to know the path
  * **serial\_number** (string) sets the device using its serial number. Default value: "". Only works with Pictrak.

### Filtering ###
  * **filter** (bool) sets filtering on or off. Default value: true
  * **mincutoff** (double) sets € filter min cutoff frequency. Default value: 1.0
  * **beta** (double) sets 1€ filter beta value. Default value: 0.001
  * **dcutoff** (double) sets 1€ filter dcutoff. Default value: 1.0 (should not be changed)

### Calibration ###
  * **useCalibration** (bool) defines if calibration is used. Default value: false
  * **milt**, **milp**, **mill**, **mirt**, **mirp**, **mirl**, **malt**, **malp**, **mall**, **mart**, **marp**, **marl** (double). Min (**mi**) and max (**ma**) values for left (**l**) and right (**r**) theta (**t**), phi (**p**) and L (**l**).

### Mode (for data processing) ###
  * **usePullMode** (bool) defines if the datas are available using getters (true) or using a callback function (false). Default value: false

### Connection ###
  * **nbOfTryMax** (int) defines a maximum number of try for connecting a Gametrak. Default value: -1 (infinite)