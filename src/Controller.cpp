/*
 * Controller.cpp
 *
 *  Created on: Jan 1, 2019
 *      Author: forsaken
 */

#include "Controller.h"

namespace Rgb {

Controller::Controller(string portName): ControllerConfig(portName), readStream(),
		welcomeCheck("RGBController v([0-9]+)\\.([0-9]+)\\.([0-9]+)\\n.*"),
		portEffect("Port ([0-9]+) Effect: \\[([0-9]+)\\] [^,]+, Duration: ([0-9]+)ms, Active color: ([0-9]+)"),
		portColor("Port ([0-9]+) Color ([0-9]+): #([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})")
{
	// Initialize members
	this->portName = portName;
	this->portHandle = NULL;
	this->portWaitEvent = NULL;
	this->portOpen = false;
	this->version.major = 0;
	this->version.minor = 0;
	this->version.patch = 0;
	// Obtain port handle by name
	sp_return result = sp_get_port_by_name(portName.c_str(), &this->portHandle);
	if (result != SP_OK) {
		throw Exceptions::PortException(Exceptions::ERR_NOT_FOUND, portName, result);
	}
	// Prepare event to wait until the port is ready
	sp_new_event_set(&this->portWaitEvent);
	sp_add_port_events(this->portWaitEvent, this->portHandle, (sp_event)(SP_EVENT_RX_READY | SP_EVENT_TX_READY));
}

Controller::~Controller() {
	this->close();
	this->freeHandle();
}

void Controller::freeHandle() {
	if (this->portWaitEvent != NULL) {
		sp_free_event_set(this->portWaitEvent);
	}
	if (this->portHandle != NULL) {
		this->close();
		sp_free_port(this->portHandle);
		this->portHandle = NULL;
	}
}

void Controller::close() {
	if (this->portOpen) {
		sp_return result = sp_close(this->portHandle);
		if (result != SP_OK) {
			throw Exceptions::PortException(Exceptions::ERR_CLOSE_FAILED, this->portName, result);
		}
		this->portOpen = false;
		this->outputText("Serial port closed");
	}
}

void Controller::open() {
	if (!this->portOpen) {
		// Open serial port
		sp_return result = sp_open(this->portHandle, SP_MODE_READ_WRITE);
		if (result != SP_OK) {
			throw Exceptions::PortException(Exceptions::ERR_OPEN_FAILED, this->portName, result);
		}
		this->portOpen = true;
		this->outputText("Serial port opened");
		// Set baudrate
		result = sp_set_baudrate(this->portHandle, 9600);
		if (result != SP_OK) {
			throw Exceptions::PortException(Exceptions::ERR_BAUDRATE_FAILED, this->portName, result);
		}
		this->outputText("Set baudrate to 9600");
		// Wait until the port is ready
		result = sp_wait(this->portWaitEvent, 100);
		if (result != SP_OK) {
			throw Exceptions::PortException(Exceptions::ERR_WAIT_FAILED, this->portName, result);
		}
		// Flush buffers
		sp_flush(this->portHandle, SP_BUF_BOTH);
		if (result != SP_OK) {
			throw Exceptions::PortException(Exceptions::ERR_FLUSH_FAILED, this->portName, result);
		}
		// Check for welcome message and version information
		// e.g.: "RGBController v0.0.1\n"
		timer::cpu_timer welcomeTimer;
		timer::nanosecond_type welcomeTimeout( 2000 * 1000000LL ); // 2000ms
        int bytesWaiting = sp_input_waiting(this->portHandle);
        while ((bytesWaiting < 21) && (welcomeTimer.elapsed().wall < welcomeTimeout)) {
        	this_thread::sleep(posix_time::milliseconds(100));
        	bytesWaiting = sp_input_waiting(this->portHandle);
        }
        if (bytesWaiting < 21) {
        	throw Exceptions::ControllerNotFound(this->portName);
        }
        char readBuffer[24];
        int bytesRead = sp_nonblocking_read(this->portHandle, readBuffer, 21);
		if (bytesRead != 21) {
			throw Exceptions::PortException(Exceptions::ERR_READ_FAILED, this->portName, result);
		}
		cmatch welcomeMatches;
		if (!regex_match(readBuffer, welcomeMatches, this->welcomeCheck)) {
			throw Exceptions::ControllerNotFound(this->portName);
		}
		this->version.major = stoi( welcomeMatches[1].str() );
		this->version.minor = stoi( welcomeMatches[2].str() );
		this->version.patch = stoi( welcomeMatches[3].str() );
		this->outputText("RGB controller detected!");
		this->outputVersion();
	}
}

void Controller::update() {
	// Wait until the port is ready
	//sp_return result = sp_wait(this->portWaitEvent, 100);
	//if (result != SP_OK) {
	//	throw Exceptions::PortException(Exceptions::ERR_WAIT_FAILED, this->portName, result);
	//}
	// Check if there is data available for reading
    int bytesWaiting = sp_input_waiting(this->portHandle);
    if (bytesWaiting > 0) {
        char readBuffer[256];
        while (bytesWaiting > 0) {
			if (bytesWaiting > 255) {
				// Prevent overflowing the buffer
				bytesWaiting = 255;
			}
			// Read from serial
			int bytesRead = sp_nonblocking_read(this->portHandle, readBuffer, bytesWaiting);
			if (bytesRead != bytesWaiting) {
				throw Exceptions::PortException(Exceptions::ERR_READ_FAILED, this->portName, (sp_return)bytesRead);
			}
			readBuffer[bytesRead] = 0;
			this->readStream << readBuffer;
			this->outputBuffer();
			// Check remaining readable bytes
			bytesWaiting = sp_input_waiting(this->portHandle);
        }
    }
}

void Controller::receivedReply(string reply) {
	cmatch matches;
	if (regex_match(reply.c_str(), matches, this->portEffect)) {
		// Port ([0-9]+) Effect: \\[([0-9]+)\\] [^,]+, Duration: ([0-9]+)ms, Active color: ([0-9]+)
		int portIndex = stoi( matches[1].str() );
		PortConfig config;
		if (this->controllerPorts.count(portIndex) > 0) {
			config = this->controllerPorts[portIndex];
		}
		config.setEffectType( (PortConfigType)stoi(matches[2].str()) );
		config.setEffectDuration( stoi(matches[3].str()) );
		config.setEffectColorIndex( stoi(matches[4].str()) );
		this->controllerPorts[portIndex] = config;
	} else if (regex_match(reply.c_str(), matches, this->portColor)) {
		// Port ([0-9]+) Color ([0-9]+): #([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})
		int portIndex = stoi( matches[1].str() );
		int colorIndex = stoi( matches[2].str() );
		PortConfig config;
		if (this->controllerPorts.count(portIndex) > 0) {
			config = this->controllerPorts[portIndex];
		}
		PortConfigColor color = config.getColor(colorIndex);
		color.red = stoi(matches[3].str(), 0, 16);
		color.green = stoi(matches[4].str(), 0, 16);
		color.blue = stoi(matches[5].str(), 0, 16);
		config.setColor(colorIndex, color);
		this->controllerPorts[portIndex] = config;
	}
	this->outputText(reply);
}

void Controller::writeCommand(string command) {
	// Wait until the port is ready
	sp_return result = sp_wait(this->portWaitEvent, 100);
	if (result != SP_OK) {
		throw Exceptions::PortException(Exceptions::ERR_WAIT_FAILED, this->portName, result);
	}
	// Send the command
    cout << this->getPortName() << ": => " << command << "\n";
    cout.flush();
	uint32_t bytesWritten = sp_nonblocking_write(this->portHandle, command.c_str(), command.length());
	if (bytesWritten != command.length()) {
		throw Exceptions::PortException(Exceptions::ERR_WRITE_FAILED, this->portName, (sp_return)bytesWritten);
	}
}

void Controller::outputText(string text) {
    cout << this->getPortName() << ": " << text << "\n";
    cout.flush();
}

void Controller::outputVersion() {
    cout << this->getPortName() << ": Version " << this->getVersionString() << "\n";
    cout.flush();
}

void Controller::outputBuffer() {
	stringstream output;
	int offset = 0;
	int length = this->readStream.tellp();
	int next = 0;
	while (offset < length) {
		this->readStream.seekg(offset++);
		next = this->readStream.peek();
		if (next == '\n') {
			this->receivedReply( output.str() );
			output.str(string());
		} else {
			output << (char)next;
		}
	}
	this->readStream.str(string());
	this->readStream << output.str();
}

void Controller::setEffect(int portIndex, PortConfigType effectType, int effectDuration) {
	Interface::setEffect(portIndex, effectType, effectDuration);
	ControllerConfig::setEffect(portIndex, effectType, effectDuration);
}

void Controller::setColor(int portIndex, int colorIndex, uint8_t red, uint8_t green, uint8_t blue) {
	Interface::setColor(portIndex, colorIndex, red, green, blue);
	ControllerConfig::setColor(portIndex, colorIndex, red, green, blue);
}

} /* namespace Rgb */
