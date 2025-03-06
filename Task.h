//
// Created by EmsiaetKadosh on 25-3-4.
//
#pragma once

#include "utils.h"

class Task;
class TaskScheduler;

class Task final : public AnywhereEditable<Task> {
	friend class TaskScheduler;

public:
	Function<void(Task& self)> func;

	explicit Task(const Function<void(Task& self)>& func) : func(func) {}
	explicit Task(Function<void(Task& self)>&& func) : func(std::move(func)) {}
	void schedulePop(const bool pop) noexcept { reserved[0] = pop; }
	bool scheduledPop() const noexcept { return reserved[0]; }

	int pop() noexcept override {
		schedulePop(true);
		Success();
	}
};

class TaskScheduler {
public:
	AnywhereEditableList<Task> tasks;

	void runAll() {
		for (Task& task : tasks) {
			task.func(task);
			if (task.scheduledPop()) tasks.pop(&task);
		}
	}

	void pushCopy(Task& task) { tasks.pushCopy(&task); }
	/* 必须是new Task，交付托管 */
	void pushNewed(Task* task) { tasks.pushNewed(task); }
	void pushThis(Task& task) { tasks.pushThis(&task); }
};
