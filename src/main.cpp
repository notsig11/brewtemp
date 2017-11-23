#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<regex>
#include<thread>
#include<chrono>
#include<mutex>
#include<condition_variable>

const std::string sensor_path = "/sys/bus/w1/devices/28-000008aac4f7/w1_slave";

std::mutex m;
std::condition_variable cv;
int temp = 0;

int read_sensor(const std::string& path) {
	std::ifstream sensor(path);
	if(!sensor.is_open()) {
		std::cout << "Can't open sensor file.\n";
		exit(-1);
	}

	std::string line;
	std::regex temp_regex("t=([0-9]*)$");
	std::smatch temp_match;
	while(std::getline(sensor, line)) {
		if(std::regex_search(line, temp_match, temp_regex)) {
			return std::stoi(temp_match[1]);
			break;
		}	
	}	

	std::cout << "Failed to read temperature.\n";
	return -1;
}

void poll_sensor() {
	while(1) {
		std::unique_lock<std::mutex> lock(m);
		temp = read_sensor(sensor_path);
		cv.notify_one();
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

int main(int argc, char* argv[]) {
	std::thread poll(poll_sensor);
	int current_temp = 0;

	for(;;) {
		std::unique_lock<std::mutex> lock(m);
		cv.wait(lock);
		{	
			current_temp = temp;
		}
		std::cout << "read temp: " << current_temp << "\n";
	}	
}
