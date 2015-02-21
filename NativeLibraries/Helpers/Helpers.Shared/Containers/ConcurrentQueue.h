#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<class T>
class ConcurrentQueue{
public:
	ConcurrentQueue()
		: waitStopped(false){
	}

	void Push(const T &v){
		std::unique_lock<std::mutex> lk(this->qMtx);

		this->q.push(v);
		this->qCv.notify_one();
	}

	void Push(T &&v){
		std::unique_lock<std::mutex> lk(this->qMtx);

		this->q.push(std::move(v));
		this->qCv.notify_one();
	}

	bool Pop(T &v, bool wait){
		bool haveValue = false;
		std::unique_lock<std::mutex> lk(this->qMtx);

		if (wait){
			while (this->q.empty() && !this->waitStopped){
				this->qCv.wait(lk);
			}
		}

		if (!this->q.empty()){
			haveValue = true;
			v = std::move(this->q.front());
			this->q.pop();
		}

		return haveValue;
	}

	bool Empty(){
		std::unique_lock<std::mutex> lk(this->qMtx);
		return this->q.empty();
	}


	/*
	Wake-up all threads and disable waiting.
	By default waiting is enabled.
	*/
	void StopWait(){
		std::unique_lock<std::mutex> lk(this->qMtx);
		this->waitStopped = true;
		this->qCv.notify_all();
	}

	/*
	Enable waiting.
	By default waiting is enabled.
	*/
	void ContinueWait(){
		std::unique_lock<std::mutex> lk(this->qMtx);
		this->waitStopped = false;
	}
private:
	std::queue<T> q;
	std::mutex qMtx;
	std::condition_variable qCv;
	bool waitStopped;
};