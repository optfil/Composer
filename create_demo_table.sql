CREATE TABLE tasks (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE,
    text TEXT NOT NULL
);

INSERT INTO tasks (name, text)
VALUES
    ('_Демонстрационная задача', '_Демонстрационный текст'),
    ('_Еще задача', '_С текстом'),
    ('_Задача', '_Без тэгов')
;

CREATE TABLE tags (
    id INTEGER PRIMARY KEY,
    name TEXT NOT NULL UNIQUE
);

INSERT INTO tags (name)
VALUES
    ('Механика'),
    ('Термодинамика'),
    ('7 класс')
;

CREATE TABLE task_tag (
    id INTEGER PRIMARY KEY,
    task_id INTEGER,
    tag_id INTEGER
);

INSERT INTO task_tag (task_id, tag_id)
VALUES
    (1, 1),
    (1, 3),
    (2, 2)
;
