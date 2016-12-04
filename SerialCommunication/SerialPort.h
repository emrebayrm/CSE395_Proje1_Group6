#ifndef SerialPort_H
#define SerialPort_H

//#include <stdint.h>   /* Standard types */
//#include <unistd.h>   /* UNIX standard function definitions */
#include <string>

#ifdef _WIN32
#include <windows.h>
#define SP_TERMIOS DCB
#define SP_HANDLE HANDLE
#define SP_BAUDRATE DWORD
#define SP_FILE_ERROR INVALID_HANDLE_VALUE
#define SP_SLEEP(N) Sleep(N)
#else
#include <termios.h>
#define SP_TERMIOS struct termios
#define SP_HANDLE int
#define SP_BAUDRATE speed_t
#define SP_FILE_ERROR -1
#define SP_SLEEP(N) usleep(N * 1e3)
#endif

class SerialPort {

	public: enum BaudRate {BR_300 = 300, BR_1200 = 1200, BR_2400 = 2400, 
						   BR_4800 = 4800, BR_9600 = 9600, BR_19200 = 19200,
						   BR_38400 = 38400, BR_57600 = 57600, 
						   BR_115200 = 115200};

	private: SP_HANDLE port;

	private: std::string portName;

	private: BaudRate baudRate;

	/**
	 * Creates SerialPort instance.
	 */
    public: SerialPort();

	/**
	 * Creates SerialPort instance.
	 *
	 * @param deviceName - port device name, example "/dev/ttyUSB0" on unix or 
	 *					   "COM1" on windows OS
	 */
	public: SerialPort(const std::string& portName);

	/**
	 * Creates SerialPort instance.
	 *
	 * @param deviceName - port device name, example "/dev/ttyUSB0" on unix or 
	 *					   "COM1" on windows OS
	 */
	public: SerialPort(const std::string& portName, BaudRate baudRate);

	/**
	 * Closes the serial port if exist.
	 */
	public: virtual ~SerialPort();

	/**
	 *
	 */
	public: std::string getPortName() const; 

	/**
	 *
	 */
	public: BaudRate getBaudRate() const;

	/**
	 *
	 */
	public: void setPortName(const std::string& portName);

	/**
	 *
	 */
	public: void setBaudRate(BaudRate baudRate);

	/*
	 * Waits given time interval.
	 */
	public: static void wait(long millisec);

	/**
	 * Opens serial port with given data speed. 
	 * Openning a port does not indicate that fully connected.
	 */
	public: bool open();

	/**
	 * Prepares the port for communication. If this fails cannot be communicated
	 * through the port with its configures.
	 *
	 * @return If successfull returns true; otherwise, false
	 */
	public: bool prepare();

	/**
	 * Ends serial port communication.
	 */
	public: bool close();

	public: bool write(char b);

	public: bool write(const char * str);

	public: bool writeString(const std::string str);

	public: bool read(char * byte);
	public: bool read(int * byte);

	public: bool readUntil(std::string& str, char until);

	public: bool readString(std::string& str);

	private: void init();

	private: static SP_BAUDRATE getBaudRateInOS(BaudRate baudRate);

};

#endif	// End of SerialPort_H
