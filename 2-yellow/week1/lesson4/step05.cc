#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>

// enum class TaskStatus
// {
//     NEW = 0,         // новая
//     IN_PROGRESS = 1, // в разработке
//     TESTING = 2,     // на тестировании
//     DONE = 3         // завершена
// };

// using TasksInfo = std::map<TaskStatus, int>;

inline TaskStatus Next(const TaskStatus status) noexcept
{
    return (status == TaskStatus::DONE)
               ? status
               : static_cast<TaskStatus>(static_cast<int>(status) + 1);
}

class TeamTasks
{
public:
    const TasksInfo &GetPersonTasksInfo(const std::string &person) const
    {
        return team_tasks_.at(person);
    }

    void AddNewTask(const std::string &person)
    {
        team_tasks_[person][TaskStatus::NEW]++;
    }

    std::tuple<TasksInfo, TasksInfo> PerformPersonTasks(
        const std::string &person, int task_count)
    {
        const auto tasks_info_it = team_tasks_.find(person);

        if (tasks_info_it == team_tasks_.end())
        {
            return {};
        }

        auto &tasks = tasks_info_it->second;
        TasksInfo untouched = tasks;
        TasksInfo updated;

        for (const auto status : {TaskStatus::NEW, TaskStatus::IN_PROGRESS, TaskStatus::TESTING})
        {
            const auto tasks_it = tasks.find(status);
            const auto untouched_it = untouched.find(status);
            const int updated_count = untouched_it != untouched.end()
                                          ? std::min(untouched_it->second, task_count)
                                          : 0;

            if (updated_count == 0)
            {
                continue;
            }

            tasks_it->second -= updated_count;
            untouched_it->second -= updated_count;
            tasks[Next(status)] += updated_count;
            updated[Next(status)] += updated_count;
            task_count -= updated_count;

            if (tasks_it->second == 0)
            {
                tasks.erase(tasks_it);
            }

            if (untouched_it->second == 0)
            {
                untouched.erase(untouched_it);
            }
        }

        untouched.erase(TaskStatus::DONE);
        return {std::move(updated), std::move(untouched)};
    }

private:
    std::unordered_map<std::string, TasksInfo> team_tasks_;
};

// void PrintTasksInfo(TasksInfo tasks_info)
// {
//     std::cout << tasks_info[TaskStatus::NEW] << " new tasks" << ", "
//               << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" << ", "
//               << tasks_info[TaskStatus::TESTING] << " tasks are being tested" << ", "
//               << tasks_info[TaskStatus::DONE] << " tasks are done" << std::endl;
// }

// int main()
// {
//     TeamTasks tasks;
//     tasks.AddNewTask("Ilia");
//     for (int i = 0; i < 3; ++i)
//     {
//         tasks.AddNewTask("Ivan");
//     }
//     std::cout << "Ilia's tasks: ";
//     PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
//     std::cout << "Ivan's tasks: ";
//     PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

//     TasksInfo updated_tasks, untouched_tasks;

//     std::tie(updated_tasks, untouched_tasks) =
//         tasks.PerformPersonTasks("Ivan", 2);
//     std::cout << "Updated Ivan's tasks: ";
//     PrintTasksInfo(updated_tasks);
//     std::cout << "Untouched Ivan's tasks: ";
//     PrintTasksInfo(untouched_tasks);

//     std::tie(updated_tasks, untouched_tasks) =
//         tasks.PerformPersonTasks("Ivan", 2);
//     std::cout << "Updated Ivan's tasks: ";
//     PrintTasksInfo(updated_tasks);
//     std::cout << "Untouched Ivan's tasks: ";
//     PrintTasksInfo(untouched_tasks);

//     return 0;
// }
