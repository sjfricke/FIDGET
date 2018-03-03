# FIDGET

## Opcode for all UDP/TCP Packets

- `0` - Toggles mode on Myo to control
	- `0` - Fidget mode
	- `1` - Cloth mode
- `1` - Speed of Leap Motion
	- Sends int
	- `0` is stop
	- `1` - `12` are speeds
- `2` - Rotation value from Leap Motion
	- `-1` - Rotate CW
	- `0` - Toggle On/Off
	- `1` - Rotate CCW
- `3` 
- `4` - Rotation value from Myo
	- `-1` - Rotate CW
	- `0` - Toggle On/Off
	- `1` - Rotate CCW
- `5` - Cloth direction from Myo
	- `-1` - Left
	- `0` - Select
	- `1` - Right 
- `6` - 
- `7` - 
- `8` - 
- `9` - 
- `10` - 
- `11` - 
- `12` - 