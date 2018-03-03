# FIDGET

## Opcode for all UDP/TCP Packets

- `0` - Position and rotation from Myo
	- Sends 4 floats seperated by `,`
	- `0:xPost,yPos,zPos,armRotation`
		- `armRotation` is in radians 
- `1` - Speed of Leap Motion
	- Sends int
	- `0` is stop
	- `1` - `12` are speeds
- `2` - Rotation value from Leap Motion
	- `-1` - Rotate CCW
	- `1` - Rotate CW
- `3` - Toggle value from Leap Motion
	- Value doesn't matter, toggles On/Off
- `4` - 
- `5` - 
- `6` - 
- `7` - 
- `8` - 
- `9` - 
- `10` - 
- `11` - 
- `12` - 