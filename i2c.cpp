#include <stdint.h>
#include <iostream>
#include <errno.h>
#include <wiringPiI2C.h>
#include <unistd.h>

using namespace std;

int reverseBytes(int num)
{
   unsigned int msbyte = num & 0x000000ff;
   unsigned int lsbyte = num & 0x0000ff00;
   lsbyte = lsbyte >>8;
   msbyte = msbyte<<8;
   num = msbyte | lsbyte;
   return num;
}

int main()
{
   int fd, result;
   // Initialize the interface by giving it an external device ID.
   //
   // It returns a standard file descriptor.
   //
   fd = wiringPiI2CSetup(0x40);
   wiringPiI2CWrite(fd, 0xFE );
   cout << "Init result: "<< fd << endl;

   while(true){
      //result = wiringPiI2CWriteReg16(fd, 0xF5,0x0 );
      result = wiringPiI2CWrite(fd, 0xF5 );
      if(result == -1)
      {
         cout << "Error.  Errno is: " << errno << endl;
      }

      //sleep for 25 milliseconds
      usleep(25*1000);
      unsigned int res = 0;

      int humidity = wiringPiI2CReadReg16(fd, 0xE5);
      humidity = reverseBytes(humidity);
      //cout << "humid: 0x" << hex << humidity << ", " << dec <<
humidity << endl;
      res = wiringPiI2CReadReg16(fd,0xA6);
      //res = wiringPiI2CReadReg16(fd,0xE5);
      //cout << "read result: " << dec << res << " hex: " << hex << res << endl;
      float rh = humidity*125.f/65536.f-6.f;
      cout << dec << "relative humidity: " << rh<< endl;

      res = wiringPiI2CReadReg16(fd,0xE0);
      //cout << "read result: " << dec << res << endl;
      res = reverseBytes(res);
      //res = wiringPiI2CRead(fd);
     //cout << "read result: " << dec << res << endl;
      //cout << "hex: " << hex << res << endl;

      float temp = res*175.72/65536 - 46.85;
      float fahr = temp * 9.f/5.f + 32;
      cout << "degrees celcius: " << temp << " f:" << fahr << endl;
      cout << "===============" << endl;
      usleep(30*1000);
   }
}
