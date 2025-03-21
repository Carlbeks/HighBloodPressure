//
// Created by EmsiaetKadosh on 25-3-4.
//
#pragma once

#include "utils.h"

class Task;
class TaskScheduler;

class Task final : public AnywhereEditable<Task> {
	friend class TaskScheduler;
#ifndef __CARLBEKS_DEBUG__
	QWORD triggerInterval = 0, nextExecuteTime = 0, lastExecuteTime = 0;
#endif

public:
	Function<void(Task& self)> func;

	Task(const Function<void(Task& self)>& func) : func(func) {}
	Task(Function<void(Task& self)>&& func) : func(std::move(func)) {}
	void schedulePop(const bool pop) noexcept { reserved[0] = pop; }
	[[nodiscard]] bool scheduledPop() const noexcept { return reserved[0]; }

	int pop() noexcept override {
		schedulePop(true);
		Success();
	}
#ifndef __CARLBEKS_DEBUG__
	Task& every(QWORD tick);
	Task& after(QWORD tick);
	Task& until(QWORD tick);
	Task& forever();
#endif
};

class TaskScheduler {
public:
	AnywhereEditableList<Task> tasks;

	void runAll() {
		for (Task& task : tasks) {
			task.func(task);
			if (task.scheduledPop()) {
				task.schedulePop(false);
				tasks.pop(&task); // pop后删除了内存，迭代器的current失效
			}
		}
	}

	void pushCopy(Task& task) { tasks.pushCopy(&task); }
	/* 必须是new Task，交付托管 */
	void pushNewed(Task* task) { tasks.pushNewed(task); }
	void pushThis(Task& task) { tasks.pushThis(&task); }
};
