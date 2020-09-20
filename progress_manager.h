/*
*	created by liyun
*   function 进度管理器，用于打印执行进度(线程安全)
*/
#pragma once

#include <string>
#include <map>
#include <fstream>
#include <mutex>

namespace Common {

class ProressManager {
public:
	static ProressManager* GetInstance() {
		return &progress_manager_;
	}

	bool Init(const std::string& file) {
		os_.open(file.c_str());
		return os_.is_open();
	}

	void Register(int type, const std::string& name, int max_num = 100) {
		if (os_.is_open()) return;
		std::lock_guard<std::mutex> lg(mutex);
		type_name_dict_[type] = name;
		os_ << "REGISTER##" << name << "#" << max_num;
	}

	void SetNum(int type, int value, const std::string& details = "") {
		if (os_.is_open()) return;
		std::lock_guard<std::mutex> lg(mutex);
		auto it = type_name_dict_.find(type);
		if (it == type_name_dict_.end()) return;
		os_ << "ADD##" << it->second << "##" << value << "##" << details;
	}

private:
	ProressManager() {
	};
	~ProressManager() {
		if (os_.is_open()) {
			os_.flush();
			os_.close();
		}
	};

	void Print(const std::string& name, const std::string& value, const std::string& details) {
		os_ << "ADD##" << name << "##" << value << "##" << details;
	}

private:
	static ProressManager progress_manager_;
	static std::ofstream os_;
	std::map<int, std::string> type_name_dict_;
	std::mutex mutex;
};
}