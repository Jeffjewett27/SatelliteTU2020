# SatelliteTU2020

Taylor University satellite program

## Sensor Values

These are the different values returned by the sensors. Compression turns `float` values to a 2 byte short precision.

- Acceleration xyz: `float`

- Gyroscope xyz: `float`

- Magnetometer xyz: `float`

- UV1: `uint16_t`

- Temperature 1: `float`

- Temperature 2: `uint16_t`

- Current Reading: `uint16_t`

- Light Frequency: `uint16_t`

- Gamma: `uint16_t` (in compressed form, it is `uint8_t`, capped at 255)

## Packets

`0x01`: General Packet
   - 2 bytes compressed acceleration x
   - 2 bytes compressed acceleration y
   - 2 bytes compressed acceleration z
   - 2 bytes compressed gyroscope x
   - 2 bytes compressed gyroscope y
   - 2 bytes compressed gyroscope z
   - 2 bytes compressed magnetometer x
   - 2 bytes compressed magnetometer y
   - 2 bytes compressed magnetometer z
   - 2 bytes compressed UV1 **potentially erroneous values**
   - 4 bytes temperature 1
   - 2 bytes temperature 2
   - 2 bytes current reading
   - 2 bytes compressed light frequency **potentially erroneous values**
   - 1 byte gamma reading
   - 1 byte empty
   
`0x20`: compressed magnetometer x `[2x16]`

`0x21`: compressed magnetometer y `[2x16]`

`0x22`: compressed magnetometer z `[2x16]`

`0x23`: compressed temperature 1 `[2x16]`

`0x24`: compressed light frequency `[2x16]` **perhaps badly compressed**

`0x25`: compressed gyroscope x `[2x16]`

`0x26`: compressed gyroscope y `[2x16]`

`0x27`: compressed gyroscope z `[2x16]`

`0x28`: compressed acceleration x `[2x16]`

`0x29`: compressed acceleration y `[2x16]`

`0x2A`: compressed acceleration z `[2x16]`

`0x2B`: compressed gamma `[1x32]`

`0x41`: light frequency `[2x16]`

`0x42`: magnetometer x `[4x8]`

`0x43`: magnetometer y `[4x8]`

`0x44`: magnetometer z `[4x8]`

`0x45`: temperature 1 `[4x8]`

`0x46`: temperature 2 `[2x16]`

`0x47`: temperature 3 `[4x8]` **unused**

`0x48`: light frequency `[2x16]`

`0x49`: gyroscope x `[4x8]`
		
`0x4A`: gyroscope y `[4x8]`
		
`0x4B`: gyroscope z `[4x8]`

`0x4C`: acceleration x `[4x8]`
		
`0x4D`: acceleration y `[4x8]`
		
`0x4E`: acceleration z `[4x8]`

`0x4F`: UV1 `[2x16]`

`0x50`: Gamma `[2x16]`