#pragma once
#include "ThreadTask.h"
#include "..\Containers\ConcurrentQueue.h"

#include <memory>
#include <thread>
#include <mutex>
#include <list>

class ThreadPool{
	class Worker;
public:
	~ThreadPool(){
		{
			std::lock_guard<std::mutex> lk(this->workersMtx);

			for (auto &i : this->workers){
				i->SetExit(true);
			}
		}

		this->taskQueue.StopWait();
	}

	void Initialize(){
		auto hwThreads = std::thread::hardware_concurrency();
		std::lock_guard<std::mutex> lk(this->workersMtx);

		for (uint32_t i = 0; i < hwThreads; i++){
			std::unique_ptr<Worker> worker(new Worker(this));
			worker->Run();
			this->workers.push_back(std::move(worker));
		}
	}

	void AddTask(std::unique_ptr<ThreadTask> &&task){
		this->taskQueue.Push(std::move(task));
	}
private:
	ConcurrentQueue<std::unique_ptr<ThreadTask>> taskQueue;
	std::mutex workersMtx;
	std::list<std::unique_ptr<Worker>> workers;

	class Worker{
	public:
		Worker(ThreadPool *parent)
			: parent(parent), exit(false){
		}

		~Worker(){
			if (this->thread.joinable()){
				this->thread.join();
			}
		}

		void SetExit(bool v){
			this->exit = v;
		}

		void Run(){
			this->thread = std::thread([=](){
				while (!this->exit){
					std::unique_ptr<ThreadTask> task;

					if (this->parent->taskQueue.Pop(task, true)){
						task->Run();
					}
				}
			});
		}
	private:
		bool exit;
		ThreadPool *parent;
		std::thread thread;

		static void RunImpl(Worker *_this){

		}
	};
};