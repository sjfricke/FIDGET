#include "accel.h"

void AccelSetup(int bus) {
  uint8_t modeReg;

  I2cOpen(bus);
  I2cSetSlave(bus, 0x4c);

  I2cReadByte(bus, MMA7660_REG_MODE, &modeReg);
  
  modeReg &= ~MMA7660_MODE_TON;
  modeReg &= ~MMA7660_MODE_MODE;

  I2cWriteByte(bus, MMA7660_REG_MODE, modeReg);

  I2cWriteByte(bus, MMA7660_REG_SR, MMA7660_AUTOSLEEP_64);

  I2cReadByte(bus, MMA7660_REG_MODE, &modeReg);
  
  modeReg &= ~MMA7660_MODE_TON;
  modeReg |= MMA7660_MODE_MODE;
  
  I2cWriteByte(bus, MMA7660_REG_MODE, modeReg);
  
}


void AccelGetValue(int bus, float* x, float* y, float* z) {

  uint8_t valueX = 0;
  uint8_t valueY = 0;
  uint8_t valueZ = 0;
  int valueRaw;
  
  do {
    I2cReadByte(bus, MMA7660_REG_XOUT, &valueX);
  } while (valueX & 0x40); // check alert bit
  
  do {
    I2cReadByte(bus, MMA7660_REG_YOUT, &valueY);
  } while (valueY & 0x40); // check alert bit
  
  do {
    I2cReadByte(bus, MMA7660_REG_ZOUT, &valueZ);
  } while (valueZ & 0x40); // check alert bit
  
  
  // shift the sign bit over, and compensate
  // 21.33, typical counts/g
  valueRaw = ((int8_t)(valueX << 2) / 4);
  *x = valueRaw / 21.33;

  valueRaw = ((int8_t)(valueY << 2) / 4);
  *y = valueRaw / 21.33;

  valueRaw = ((int8_t)(valueZ << 2) / 4);
  *z = valueRaw / 21.33;
}
