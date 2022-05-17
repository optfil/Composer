import pathlib
import sqlite3
from dataclasses import dataclass
import PyQt5

db_file_name = 'tasks.db'  # ':memory:' for in-memory db
reset_sql_script = 'create_demo_table.sql'


@dataclass
class Task:
    id: int
    name: str
    text: str


@dataclass
class Tag:
    id: int
    name: str


class DBManager:
    def __init__(self, file_name, reset_script=None):
        self.db_file_name = file_name
        if reset_script and not file_name == ':memory:':
            pathlib.Path(file_name).unlink(missing_ok=True)
            self.connection = sqlite3.connect(file_name)
            self.cursor = self.connection.cursor()
            with open(reset_script, encoding='utf-8') as f:
                sqlite_query = f.read()
                self.cursor.executescript(sqlite_query)
        else:
            self.connection = sqlite3.connect(file_name)
            self.cursor = self.connection.cursor()
        self.tasks_name = 'tasks'
        self.tags_name = 'tags'
        self.task_tag_name = 'task_tag'
        self.cursor.execute('''SELECT * FROM {};'''.format(self.tasks_name))
        self.tasks = [Task(*t) for t in self.cursor.fetchall()]
        self.cursor.execute('''SELECT * FROM {};'''.format(self.tags_name))
        self.tags = [Tag(*t) for t in self.cursor.fetchall()]

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.cursor.close()
        self.connection.close()

    def tables(self):
        #self.cursor.execute('''SELECT name FROM PRAGMA_TABLE_LIST WHERE type='table';''')  # may work in SQLite >= 3.37.0
        self.cursor.execute('''SELECT name FROM sqlite_master WHERE type='table';''')
        return [p[0] for p in self.cursor.fetchall()]

    def columns(self, table_name):
        self.cursor.execute('''SELECT name FROM PRAGMA_TABLE_INFO(?);''', (table_name,))
        return [p[0] for p in self.cursor.fetchall()]

    def task_tags(self, task):
        self.cursor.execute(f'''SELECT tags.name FROM {self.tasks_name} as tasks \
                                    INNER JOIN {self.task_tag_name} as task_tag 
                                    ON tasks.id == task_tag.task_id
                                    INNER JOIN {self.tags_name} as tags
                                    ON task_tag.tag_id == tags.id
                                    WHERE tasks.id == {task.id};''')
        res = self.cursor.fetchall()
        return [p[0] for p in res]


if __name__ == '__main__':
    with DBManager(db_file_name, reset_script=reset_sql_script) as dbm:
        for task in dbm.tasks:
            print(task.name, task.text, dbm.task_tags(task))
