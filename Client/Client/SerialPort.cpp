#include <iostream>    /* Standard input/output definitions */
#include <cstdlib> 
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <unistd.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>

#endif

#include "SerialPort.h"

#ifndef WIN32
namespace {
	int openPort(const char * fileName, int flags) {
		return (open(fileName, flags));
	}
	
	size_t readData(int fileDes, char * data, size_t size) {
		return (read(fileDes, data, size));
	}
	
	size_t writeData(int fileDes, const char * data, size_t size) {
		return (write(fileDes, data, size));
	}
	
	int closePort(int fileName) {
		return (close(fileName));
	}
}
#endif

SerialPort::SerialPort() {
	setPortName("");
	setBaudRate(SerialPort::BR_9600);
	init();
}

SerialPort::SerialPort(const std::string& portName) {
	setPortName(portName);
	setBaudRate(SerialPort::BR_9600);
	init();
}

SerialPort::SerialPort(const std::string& portName, BaudRate baudRate) {
	setPortName(portName);
	setBaudRate(baudRate);
	init();
}

SerialPort::~SerialPort() {
    close();
}

void SerialPort::init() {
#ifdef WIN32
    port = INVALID_HANDLE_VALUE;
#else
    port = -1;
#endif
}

std::string SerialPort::getPortName() const {
	return (portName);
}

SerialPort::BaudRate SerialPort::getBaudRate() const {
	return (baudRate);
}

void SerialPort::setPortName(const std::string& portName) {
	this->portName = portName;
}

void SerialPort::setBaudRate(BaudRate baudRate) {
	this->baudRate = baudRate;
}

void SerialPort::wait(long millisec) {
	SP_SLEEP(millisec);
}

bool SerialPort::open() {
#ifdef _WIN32
	port = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, 
					  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(port == INVALID_HANDLE_VALUE) {
		DWORD error = GetLastError();
		switch(error){  
	        case ERROR_ACCESS_DENIED:
	            std::cerr << "Unable to open port: Access denied (port in use?)\n" << std::endl;
	            return (false);
	        case ERROR_FILE_NOT_FOUND:
	            std::cerr << "Unable to open port '" << portName << "'. File not found.\n" << std::endl;
	            return (false);
	        default:
	        	std::cerr << "Unknown error in CreateFile call.\n" << std::endl;
	            return (false);
        }
	}
	return (true);
#else
	port = openPort(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	fd = port;
    if(port == -1)  {
        std::cerr << "Unable to open port.\n" << std::endl;
        return (false);
    }
    return (true);
#endif
}

bool SerialPort::prepare() {
#ifdef _WIN32
	DCB toptions;
    if(!GetCommState(port, &toptions)){
        std::cerr << "Couldn't get COM state.\n" << std::endl;
        return (false);
    }

    toptions.BaudRate = SerialPort::getBaudRateInOS(baudRate);
    toptions.ByteSize = 8;
    toptions.StopBits = ONESTOPBIT;
    toptions.Parity = NOPARITY; 
    toptions.fOutxCtsFlow = FALSE;
    toptions.fOutxDsrFlow = FALSE;
    toptions.fOutX = FALSE;
    toptions.fInX = FALSE;
    toptions.fRtsControl = RTS_CONTROL_DISABLE;
    toptions.fTXContinueOnXoff = TRUE;
    toptions.fNull = FALSE;

    if(!SetCommState(port, &toptions)) {
        std::cerr << " Couldn't set COM state.\n" << std::endl;
        return (false);
    }
    return (true);
#else
    struct termios toptions;
    if (tcgetattr(port, &toptions) < 0) {
        std::cerr << "Couldn't get term attributes.\n" << std::endl;
        return false;
    }

    cfsetispeed(&toptions, SerialPort::getBaudRateInOS(baudRate));
    cfsetospeed(&toptions, SerialPort::getBaudRateInOS(baudRate));

    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw

    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    
    if(tcsetattr(port, TCSANOW, &toptions) < 0) {
        std::cerr << "Couldn't set term attributes.\n" << std::endl;
        return (false);
    }
    return (true);
#endif
}

bool SerialPort::close() {
	if(port < 0) {
        return (false);
    }
#ifdef _WIN32
    CloseHandle(port);
#else /* Linux, Mac */
    closePort(port);
#endif
    return (true);
}

SP_BAUDRATE SerialPort::getBaudRateInOS(BaudRate baudRate) {
#ifdef _WIN32
	switch(baudRate) {
		case BR_300: return CBR_300;
		case BR_1200: return CBR_1200;
		case BR_2400: return CBR_2400;
		case BR_4800: return CBR_4800;
		case BR_9600: return CBR_9600;
		case BR_19200: return CBR_19200;
		case BR_38400: return CBR_38400;
		case BR_57600: return CBR_57600;
		case BR_115200: return CBR_115200;
		default: return CBR_9600;
	}
#else
	switch(baudRate) {
		case BR_300: return B300;
		case BR_1200: return B1200;
		case BR_2400: return B2400;
		case BR_4800: return B4800;
		case BR_9600: return B9600;
		case BR_19200: return B19200;
		case BR_38400: return B38400;
		case BR_57600: return B57600;
		case BR_115200: return B115200;
		default: return B9600;
	}
#endif
}

bool SerialPort::write(char b) {
#ifdef _WIN32
	DWORD errors;
    COMSTAT status;
    DWORD n;
    if(!WriteFile((HANDLE) port, (void *)(&b), 1, &n, NULL)){
        ClearCommError((HANDLE) port, &errors, &status);
        std::cerr << "Error writing to COM port.\n" << std::endl;
        return (false);
    }
    return (true);
#else
    int n = writeData(port, &b, 1);
    if(n != 1) {
    	return (false);
    }
    return (true);
#endif
}

bool SerialPort::write(const char * str) {
	return (writeString(std::string(str)));
}

bool SerialPort::writeString(const std::string str) {
#ifdef _WIN32
	DWORD n;
    DWORD errors;
    COMSTAT status;
    if(!WriteFile((HANDLE) port, (void *) str.c_str(), str.size(), &n, 0)){
        ClearCommError((HANDLE) port, &errors, &status);
        std::cerr << "Error writing to COM port.\n" << std::endl;
        return (false);
    }
    return (true);
#else
    int n = writeData(port, str.c_str(), str.size());
    if(n != str.size()) {
    	return (false);
    }
    return (true);
#endif
}

bool SerialPort::read(char * byte) {
#ifdef _WIN32
    DWORD n;
    if(ReadFile(port, byte, 1, &n, NULL)){
        return (false);
    }
    return (true);
#else /* Linux, Mac */
    int n = readData(port, byte, 1);  // readAngles a char at a time
    if(n == 1) {
    	return (false);
    }
    return (true);
#endif
}

bool SerialPort::read(int * byte) {
#ifdef _WIN32
    DWORD n;
    if(ReadFile(port, byte, sizeof(int), &n, NULL)){
        return (false);
    }
    return (true);
#else /* Linux, Mac */
    int n = readData(port, (char *)byte, 1);  // readAngles a char at a time
    if(n == 1) {
    	return (false);
    }
    return (true);
#endif
}

bool SerialPort::readUntil(std::string& str, char until) {
	str = "";
#ifdef _WIN32 
	char b;
	do {
		DWORD n;
        if(!ReadFile(port, &b, 1, &n, NULL)) { /* READFILE returns TRUE on success */
            return (false);
        }
        if(n == -1) {
        	return (false); 
        } else if(n == 0) {
//            usleep(10); // wait 10 msec try again
            continue;
        } else {
            str += b;
        }
	} while(b != until);
	return (true);
#else
	char b;
	do {
		int n = readData(port, &b, 1);  // readAngles a char at a time
        if(n == -1) {
        	continue;
        } else {
            str += b;
        }
	} while(b != until);
	return (true);
#endif
}

bool SerialPort::readString(std::string& str) {
	return (readUntil(str, '\0'));
}
