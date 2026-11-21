# 16 · Todo Task Manager (Cisco onsite, Easy) — Python class, CRUD + status check
# The report asks for Python. Tasks have an id, a title and a status in
# {new, pending, resolved}.
#   create(title, status="new") -> new task id (ids start at 1, never reused)
#   get(task_id)                -> the Task
#   update(task_id, title=None, status=None) -> changes the given fields
#   delete(task_id)
#   list_tasks(status=None)     -> tasks in id order, optionally one status
# Invalid status -> ValueError; unknown id -> KeyError (assumption: raise, don't return
# None, so a caller can't miss the error).
# Assumption: any status may change to any other; ordered transitions are a follow-up.

VALID_STATUSES = ("new", "pending", "resolved")


class Task:
    def __init__(self, task_id, title, status):
        self.task_id = task_id
        self.title = title
        self.status = status


class TodoManager:
    # tasks[task_id] = Task; dicts keep insertion order, which is id order here
    def __init__(self):
        self.tasks = {}
        self.next_id = 1

    def _check_status(self, status):
        if status not in VALID_STATUSES:
            raise ValueError("invalid status: " + str(status))

    def _find(self, task_id):
        if task_id not in self.tasks:
            raise KeyError(task_id)
        return self.tasks[task_id]

    # T: O(1) for create / get / update / delete, O(n) for list_tasks
    def create(self, title, status="new"):
        self._check_status(status)
        task_id = self.next_id
        self.next_id += 1
        self.tasks[task_id] = Task(task_id, title, status)
        return task_id

    def get(self, task_id):
        return self._find(task_id)

    def update(self, task_id, title=None, status=None):
        task = self._find(task_id)
        # validate before changing anything, so a bad call leaves the task as it was
        if status is not None:
            self._check_status(status)
        if title is not None:
            task.title = title
        if status is not None:
            task.status = status
        return task

    def delete(self, task_id):
        self._find(task_id)
        del self.tasks[task_id]

    def list_tasks(self, status=None):
        if status is not None:
            self._check_status(status)
        result = []
        for task in self.tasks.values():
            if status is None or task.status == status:
                result.append(task)
        return result


def check(label, ok):
    print(label, "PASS" if ok else "FAIL")


def raises(error, func, *args):
    try:
        func(*args)
    except error:
        return True
    return False


def main():
    todo = TodoManager()
    first = todo.create("write tests")
    second = todo.create("fix flaky test", "pending")
    check("create-default-new", todo.get(first).status == "new")

    todo.update(first, status="resolved")
    check("update-status", todo.get(first).status == "resolved")
    check("bad-status-rejected", raises(ValueError, todo.update, second, None, "done"))
    check("bad-update-no-change", todo.get(second).status == "pending")

    check("filter-by-status", [t.task_id for t in todo.list_tasks("pending")] == [second])
    todo.delete(first)
    check("deleted-gone", raises(KeyError, todo.get, first))
    check("unknown-id", raises(KeyError, todo.delete, 99))
    check("ids-not-reused", todo.create("next") == 3)


main()

# follow-up:
# allowed transitions only (new -> pending -> resolved) -> dict status -> next statuses,
#   check it in update() before assigning
# status as an Enum -> class Status(Enum); parse the string once at the API boundary
# persistence -> save tasks as JSON (id, title, status) on each change, or SQLite
# thread safety -> one threading.Lock held around every method body
# pytest version -> a fixture that returns a fresh TodoManager; parametrize bad statuses
