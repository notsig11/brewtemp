#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<regex>
#include<thread>
#include<chrono>
#include<mutex>
#include<condition_variable>

const std::string sensor_path = "/sys/devices/w1_bus_master1/28-000008aac4f7/w1_slave";
const std::chrono::seconds read_frequency(3);

std::mutex m;
std::condition_variable cv;
int temp = 0;

// Looks like  ~900ms to read the device...
int read_sensor(const std::string& path) {
	std::ifstream sensor(path);
	if(!sensor.is_open()) {
		std::cout << "Can't open sensor device.\n";
		exit(-1);
	}

	std::string line;
	std::regex temp_regex("t=([0-9]*)$");
	std::smatch temp_match;
	while(std::getline(sensor, line)) {
		// TODO: Check CRC instead of discarding the first line.
		if(std::regex_search(line, temp_match, temp_regex)) {
			return std::stoi(temp_match[1]);
		}
	}

	std::cout << "Failed to read temperature.\n";
	return -1;
}

void poll_sensor() {
	while(1) {
		std::unique_lock<std::mutex> lock(m);

		auto start = std::chrono::high_resolution_clock::now();
		temp = read_sensor(sensor_path);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		std::cout << "Device read took " << elapsed.count() << "ms.  Adjusting sleep time.\n";

		if(temp == -1) {
			std::cout << "Error reading sensor.\n";	
			exit(-1);
		}

		m.unlock();
		cv.notify_one();
		std::this_thread::sleep_for(read_frequency - elapsed);
	}
}

int main(int argc, char* argv[]) {
	std::thread poll(poll_sensor);
	int current_temp = 0;

	while(1) {
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock);
		{
			current_temp = temp;
		}
		std::cout << "read temp: " << (current_temp / 1000.0) << "\n";
	}
}
