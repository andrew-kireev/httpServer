//
// Created by Andrew Kireev on 25.09.2021.
//

#ifndef HTTPSERVER_THREAD_POOL_H
#define HTTPSERVER_THREAD_POOL_H

#include <thread>
#include <vector>
#include <future>
#include <queue>
#include <functional>

class ThreadPool {
public:
	ThreadPool() = default;

	explicit ThreadPool(size_t thread_amount) {
		for (size_t i = 0; i < thread_amount; ++i) {
			auto worker = [this]() {

				std::cout << "запустили" << std::endl;

				while (true) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(this->queue_mutex_);
						this->condition_.wait(lock,
											  [this] { return !this->queue_.empty(); });
						if (this->queue_.empty())
							return;

						task = std::move(this->queue_.front());
						this->queue_.pop();

					}
					task();
				}

			};

			threads_.emplace_back(worker);
		}
	}

	template<class F>
	void add_task(F &&f) {
		auto task = std::make_shared<std::packaged_task<void()>>(
				std::bind(f)
		);

		{
			std::unique_lock<std::mutex> lock(queue_mutex_);
			queue_.emplace([task]() { (*task)(); });
		}
		condition_.notify_one();
	}

	~ThreadPool() {
		for (auto &worker : threads_) {
			worker.join();
		}
	}

private:
	std::vector<std::thread> threads_;
	std::queue<std::function<void()>> queue_;
	std::mutex queue_mutex_;
	std::condition_variable condition_;
};

#endif //HTTPSERVER_THREAD_POOL_H
