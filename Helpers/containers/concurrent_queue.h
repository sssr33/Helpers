#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class concurrent_queue{
public:
	concurrent_queue()
		: waitStopped(false){
	}

	void push(const T &v){
		std::unique_lock<std::mutex> lk(this->qMtx);

		this->q.push(v);
		this->qCv.notify_all();
	}

	void push(T &&v){
		std::unique_lock<std::mutex> lk(this->qMtx);

		this->q.push(std::move(v));
		this->qCv.notify_all();
	}

	bool pop(T &v, bool wait){
		bool haveValue = false;
		std::unique_lock<std::mutex> lk(this->qMtx);

		if (wait){
			while (this->q.empty() && !this->waitStopped){
				this->qCv.wait(lk);
			}

			this->waitStopped = false;
		}

		if (!this->q.empty()){
			haveValue = true;
			v = std::move(this->q.front());
			this->q.pop();
		}

		return haveValue;
	}

	bool empty(){
		std::unique_lock<std::mutex> lk(this->qMtx);
		return this->q.empty();
	}

	void stop_wait(){
		std::unique_lock<std::mutex> lk(this->qMtx);
		this->waitStopped = true;
		this->qCv.notify_all();
	}
private:
	std::queue<T> q;
	std::mutex qMtx;
	std::condition_variable qCv;
	bool waitStopped;
};